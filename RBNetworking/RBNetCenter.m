//
//  RBNetCenter.m
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import "RBNetCenter.h"
#import "RBNetRequest.h"
#import "RBNetEngine.h"
#import "RBRequestConfiger.h"
@interface RBNetCenter () {
    dispatch_semaphore_t _lock;
}
@property (nonatomic, assign) NSUInteger autoIncrement;
@property (nonatomic, strong) NSMutableDictionary<NSString *, id> *runningBatchAndChainPool;
@property (nonatomic, strong, readwrite) NSMutableDictionary<NSString *, id> *generalParameters;
@property (nonatomic, strong, readwrite) NSMutableDictionary<NSString *, NSString *> *generalHeaders;
@property (nonatomic, copy) RBCenterResponseProcessBlock responseProcessHandler;
@property (nonatomic, copy) RBCenterRequestProcessBlock requestProcessHandler;
@end

@implementation RBNetCenter

+ (instancetype)center {
    return [[[self class] alloc] init];
}

+ (instancetype)defaultCenter {
    static id sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [self center];
    });
    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    _autoIncrement = 0;
    _lock = dispatch_semaphore_create(1);
    _engine = [RBNetEngine sharedEngine];
    return self;
}

#pragma mark - Public Instance Methods for RBCenter

- (void)setupConfig:(void(^)(RBRequestConfiger *config))block {
    RBRequestConfiger *config = [[RBRequestConfiger alloc] init];
    RB_SAFE_BLOCK(block, config);
    
    if (config.generalServer) {
        self.generalServer = config.generalServer;
    }
    if (config.generalParameters.count > 0) {
        [self.generalParameters addEntriesFromDictionary:config.generalParameters];
    }
    if (config.generalHeaders.count > 0) {
        [self.generalHeaders addEntriesFromDictionary:config.generalHeaders];
    }
    if (config.callbackQueue != NULL) {
        self.callbackQueue = config.callbackQueue;
    }
    if (config.generalUserInfo) {
        self.generalUserInfo = config.generalUserInfo;
    }
    if (config.engine) {
        self.engine = config.engine;
    }
}

- (void)setRequestProcessBlock:(RBCenterRequestProcessBlock)block {
    self.requestProcessHandler = block;
}

- (void)setResponseProcessBlock:(RBCenterResponseProcessBlock)block {
    self.responseProcessHandler = block;
}

- (void)setGeneralHeaderValue:(NSString *)value forField:(NSString *)field {
    [self.generalHeaders setValue:value forKey:field];
}

- (void)setGeneralParameterValue:(id)value forKey:(NSString *)key {
    [self.generalParameters setValue:value forKey:key];
}

#pragma mark -

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:nil onFinished:nil];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:nil onFinished:nil];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:failureBlock onFinished:nil];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:nil onFinished:finishedBlock];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:failureBlock onFinished:nil];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    return [self sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onProgress:(nullable RBProgressBlock)progressBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [self sendRequest:configBlock onProgress:progressBlock onSuccess:successBlock onFailure:failureBlock onFinished:nil];
}

- (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onProgress:(nullable RBProgressBlock)progressBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    RBNetRequest *request = [RBNetRequest request];
    RB_SAFE_BLOCK(self.requestProcessHandler, request);
    RB_SAFE_BLOCK(configBlock, request);
    
    [self RB_processRequest:request onProgress:progressBlock onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
    [self RB_sendRequest:request];
    
    return request.identifier;
}

- (NSString *)sendBatchRequest:(RBBatchRequestConfigBlock)configBlock
                     onSuccess:(nullable RBBCSuccessBlock)successBlock
                     onFailure:(nullable RBBCFailureBlock)failureBlock
                    onFinished:(nullable RBBCFinishedBlock)finishedBlock {
    RBBatchRequest *batchRequest = [[RBBatchRequest alloc] init];
    RB_SAFE_BLOCK(configBlock, batchRequest);
    
    if (batchRequest.requestArray.count > 0) {
        if (successBlock) {
            [batchRequest setValue:successBlock forKey:@"_batchSuccessBlock"];
        }
        if (failureBlock) {
            [batchRequest setValue:failureBlock forKey:@"_batchFailureBlock"];
        }
        if (finishedBlock) {
            [batchRequest setValue:finishedBlock forKey:@"_batchFinishedBlock"];
        }
        [batchRequest.responseArray removeAllObjects];
        for (RBNetRequest *request in batchRequest.requestArray) {
            [batchRequest.responseArray addObject:[NSNull null]];
            __weak __typeof(self)weakSelf = self;
            [self RB_processRequest:request
                         onProgress:nil
                          onSuccess:nil
                          onFailure:nil
                         onFinished:^(id responseObject, NSError *error) {
                             if ([batchRequest onFinishedOneRequest:request response:responseObject error:error]) {
                                 __strong __typeof(weakSelf)strongSelf = weakSelf;
                                 dispatch_semaphore_wait(strongSelf->_lock, DISPATCH_TIME_FOREVER);
                                 [strongSelf.runningBatchAndChainPool removeObjectForKey:batchRequest.identifier];
                                 dispatch_semaphore_signal(strongSelf->_lock);
                             }
                         }];
            [self RB_sendRequest:request];
        }
        
        NSString *identifier = [self RB_identifierForBatchAndChainRequest];
        [batchRequest setValue:identifier forKey:@"_identifier"];
        RBLock();
        [self.runningBatchAndChainPool setValue:batchRequest forKey:identifier];
        RBUnlock();
        
        return identifier;
    } else {
        return nil;
    }
}

- (NSString *)sendChainRequest:(RBChainRequestConfigBlock)configBlock
                     onSuccess:(nullable RBBCSuccessBlock)successBlock
                     onFailure:(nullable RBBCFailureBlock)failureBlock
                    onFinished:(nullable RBBCFinishedBlock)finishedBlock {
    RBChainRequest *chainRequest = [[RBChainRequest alloc] init];
    RB_SAFE_BLOCK(configBlock, chainRequest);
    
    if (chainRequest.runningRequest) {
        if (successBlock) {
            [chainRequest setValue:successBlock forKey:@"_chainSuccessBlock"];
        }
        if (failureBlock) {
            [chainRequest setValue:failureBlock forKey:@"_chainFailureBlock"];
        }
        if (finishedBlock) {
            [chainRequest setValue:finishedBlock forKey:@"_chainFinishedBlock"];
        }
        
        [self RB_sendChainRequest:chainRequest];
        
        NSString *identifier = [self RB_identifierForBatchAndChainRequest];
        [chainRequest setValue:identifier forKey:@"_identifier"];
        RBLock();
        [self.runningBatchAndChainPool setValue:chainRequest forKey:identifier];
        RBUnlock();
        
        return identifier;
    } else {
        return nil;
    }
}

#pragma mark -

- (void)cancelRequest:(NSString *)identifier {
    [self cancelRequest:identifier onCancel:nil];
}

- (void)cancelRequest:(NSString *)identifier
             onCancel:(nullable RBCancelBlock)cancelBlock {
    id request = nil;
    if ([identifier hasPrefix:@"BC"]) {
        RBLock();
        request = [self.runningBatchAndChainPool objectForKey:identifier];
        [self.runningBatchAndChainPool removeObjectForKey:identifier];
        RBUnlock();
        if ([request isKindOfClass:[RBBatchRequest class]]) {
            RBBatchRequest *batchRequest = request;
            if (batchRequest.requestArray.count > 0) {
                for (RBNetRequest *rq in batchRequest.requestArray) {
                    if (rq.identifier.length > 0) {
                        [self.engine cancelRequestByIdentifier:rq.identifier];
                    }
                }
            }
        } else if ([request isKindOfClass:[RBChainRequest class]]) {
            RBChainRequest *chainRequest = request;
            if (chainRequest.runningRequest && chainRequest.runningRequest.identifier.length > 0) {
                [self.engine cancelRequestByIdentifier:chainRequest.runningRequest.identifier];
            }
        }
    } else if (identifier.length > 0) {
        request = [self.engine cancelRequestByIdentifier:identifier];
    }
    RB_SAFE_BLOCK(cancelBlock, request);
}

- (id)getRequest:(NSString *)identifier {
    if (identifier == nil) {
        return nil;
    } else if ([identifier hasPrefix:@"BC"]) {
        RBLock();
        id request = [self.runningBatchAndChainPool objectForKey:identifier];
        RBUnlock();
        return request;
    } else {
        return [self.engine getRequestByIdentifier:identifier];
    }
}

- (BOOL)isNetworkReachable {
    return self.engine.reachabilityStatus != 0;
}

#pragma mark - Public Class Methods for RBCenter

+ (void)setupConfig:(void(^)(RBRequestConfiger *config))block {
    [[RBNetCenter defaultCenter] setupConfig:block];
}

+ (void)setRequestProcessBlock:(RBCenterRequestProcessBlock)block {
    [[RBNetCenter defaultCenter] setRequestProcessBlock:block];
}

+ (void)setResponseProcessBlock:(RBCenterResponseProcessBlock)block {
    [[RBNetCenter defaultCenter] setResponseProcessBlock:block];
}

+ (void)setGeneralHeaderValue:(NSString *)value forField:(NSString *)field {
    [[RBNetCenter defaultCenter].generalHeaders setValue:value forKey:field];
}

+ (void)setGeneralParameterValue:(id)value forKey:(NSString *)key {
    [[RBNetCenter defaultCenter].generalParameters setValue:value forKey:key];
}

#pragma mark -

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:nil onFinished:nil];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:nil onFinished:nil];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:failureBlock onFinished:nil];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:nil onFailure:nil onFinished:finishedBlock];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:failureBlock onFinished:nil];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:nil onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onProgress:(nullable RBProgressBlock)progressBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:progressBlock onSuccess:successBlock onFailure:failureBlock onFinished:nil];
}

+ (NSString *)sendRequest:(RBRequestConfigBlock)configBlock
               onProgress:(nullable RBProgressBlock)progressBlock
                onSuccess:(nullable RBSuccessBlock)successBlock
                onFailure:(nullable RBFailureBlock)failureBlock
               onFinished:(nullable RBFinishedBlock)finishedBlock {
    return [[RBNetCenter defaultCenter] sendRequest:configBlock onProgress:progressBlock onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
}

+ (NSString *)sendBatchRequest:(RBBatchRequestConfigBlock)configBlock
                     onSuccess:(nullable RBBCSuccessBlock)successBlock
                     onFailure:(nullable RBBCFailureBlock)failureBlock
                    onFinished:(nullable RBBCFinishedBlock)finishedBlock {
    return [[RBNetCenter defaultCenter] sendBatchRequest:configBlock onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
}

+ (NSString *)sendChainRequest:(RBChainRequestConfigBlock)configBlock
                     onSuccess:(nullable RBBCSuccessBlock)successBlock
                     onFailure:(nullable RBBCFailureBlock)failureBlock
                    onFinished:(nullable RBBCFinishedBlock)finishedBlock {
    return [[RBNetCenter defaultCenter] sendChainRequest:configBlock onSuccess:successBlock onFailure:failureBlock onFinished:finishedBlock];
}

#pragma mark -

+ (void)cancelRequest:(NSString *)identifier {
    [[RBNetCenter defaultCenter] cancelRequest:identifier onCancel:nil];
}

+ (void)cancelRequest:(NSString *)identifier
             onCancel:(nullable RBCancelBlock)cancelBlock {
    [[RBNetCenter defaultCenter] cancelRequest:identifier onCancel:cancelBlock];
}

+ (nullable id)getRequest:(NSString *)identifier {
    return [[RBNetCenter defaultCenter] getRequest:identifier];
}

+ (BOOL)isNetworkReachable {
    return [[RBNetCenter defaultCenter] isNetworkReachable];
}

#pragma mark -

+ (void)addSSLPinningURL:(NSString *)url {
    [[RBNetCenter defaultCenter].engine addSSLPinningURL:url];
}

+ (void)addSSLPinningCert:(NSData *)cert {
    [[RBNetCenter defaultCenter].engine addSSLPinningCert:cert];
}

+ (void)addTwowayAuthenticationPKCS12:(NSData *)p12 keyPassword:(NSString *)password {
    [[RBNetCenter defaultCenter].engine addTwowayAuthenticationPKCS12:p12 keyPassword:password];
}

#pragma mark - Private Methods for RBCenter

- (void)RB_sendChainRequest:(RBChainRequest *)chainRequest {
    if (chainRequest.runningRequest != nil) {
        __weak __typeof(self)weakSelf = self;
        [self RB_processRequest:chainRequest.runningRequest
                     onProgress:nil
                      onSuccess:nil
                      onFailure:nil
                     onFinished:^(id responseObject, NSError *error) {
                         __strong __typeof(weakSelf)strongSelf = weakSelf;
                         if ([chainRequest onFinishedOneRequest:chainRequest.runningRequest response:responseObject error:error]) {
                             dispatch_semaphore_wait(strongSelf->_lock, DISPATCH_TIME_FOREVER);
                             [strongSelf.runningBatchAndChainPool removeObjectForKey:chainRequest.identifier];
                             dispatch_semaphore_signal(strongSelf->_lock);
                         } else {
                             if (chainRequest.runningRequest != nil) {
                                 [strongSelf RB_sendChainRequest:chainRequest];
                             }
                         }
                     }];
        
        [self RB_sendRequest:chainRequest.runningRequest];
    }
}

- (void)RB_processRequest:(RBNetRequest *)request
               onProgress:(RBProgressBlock)progressBlock
                onSuccess:(RBSuccessBlock)successBlock
                onFailure:(RBFailureBlock)failureBlock
               onFinished:(RBFinishedBlock)finishedBlock {
    
    // set callback blocks for the request object.
    if (successBlock) {
        [request setValue:successBlock forKey:@"_successBlock"];
    }
    if (failureBlock) {
        [request setValue:failureBlock forKey:@"_failureBlock"];
    }
    if (finishedBlock) {
        [request setValue:finishedBlock forKey:@"_finishedBlock"];
    }
    if (progressBlock && request.requestType != RBRequestNormal) {
        [request setValue:progressBlock forKey:@"_progressBlock"];
    }
    
    // add general user info to the request object.
    if (!request.userInfo && self.generalUserInfo) {
        request.userInfo = self.generalUserInfo;
    }
    
    // add general parameters to the request object.
    if (request.useGeneralParameters && self.generalParameters.count > 0) {
        NSMutableDictionary *parameters = [NSMutableDictionary dictionary];
        [parameters addEntriesFromDictionary:self.generalParameters];
        if (request.parameters.count > 0) {
            [parameters addEntriesFromDictionary:request.parameters];
        }
        request.parameters = parameters;
    }
    
    // add general headers to the request object.
    if (request.useGeneralHeaders && self.generalHeaders.count > 0) {
        NSMutableDictionary *headers = [NSMutableDictionary dictionary];
        [headers addEntriesFromDictionary:self.generalHeaders];
        if (request.headers) {
            [headers addEntriesFromDictionary:request.headers];
        }
        request.headers = headers;
    }
    
    // process url for the request object.
    if (request.url.length == 0) {
        if (request.server.length == 0 && request.useGeneralServer && self.generalServer.length > 0) {
            request.server = self.generalServer;
        }
        if (request.api.length > 0) {
            NSURL *baseURL = [NSURL URLWithString:request.server];
            // ensure terminal slash for baseURL path, so that NSURL +URLWithString:relativeToURL: works as expected.
            if ([[baseURL path] length] > 0 && ![[baseURL absoluteString] hasSuffix:@"/"]) {
                baseURL = [baseURL URLByAppendingPathComponent:@""];
            }
            request.url = [[NSURL URLWithString:request.api relativeToURL:baseURL] absoluteString];
        } else {
            request.url = request.server;
        }
    }
    NSAssert(request.url.length > 0, @"The request url can't be null.");
}

- (void)RB_sendRequest:(RBNetRequest *)request {
    
   
    
    // send the request through RBEngine.
    [self.engine sendRequest:request completionHandler:^(id responseObject, NSError *error) {
        // the completionHandler will be execured in a private concurrent dispatch queue.
        if (error) {
            [self RB_failureWithError:error forRequest:request];
        } else {
            [self RB_successWithResponse:responseObject forRequest:request];
        }
    }];
}

- (void)RB_successWithResponse:(id)responseObject forRequest:(RBNetRequest *)request {
    
    NSError *processError = nil;
    // custom processing the response data.
    RB_SAFE_BLOCK(self.responseProcessHandler, request, responseObject, &processError);
    if (processError) {
        [self RB_failureWithError:processError forRequest:request];
        return;
    }
    
  
    
    if (self.callbackQueue) {
        __weak __typeof(self)weakSelf = self;
        dispatch_async(self.callbackQueue, ^{
            __strong __typeof(weakSelf)strongSelf = weakSelf;
            [strongSelf RB_execureSuccessBlockWithResponse:responseObject forRequest:request];
        });
    } else {
        // execure success block on a private concurrent dispatch queue.
        [self RB_execureSuccessBlockWithResponse:responseObject forRequest:request];
    }
}

- (void)RB_execureSuccessBlockWithResponse:(id)responseObject forRequest:(RBNetRequest *)request {
    RB_SAFE_BLOCK(request.successBlock, responseObject);
    RB_SAFE_BLOCK(request.finishedBlock, responseObject, nil);
    [request cleanCallbackBlocks];
}

- (void)RB_failureWithError:(NSError *)error forRequest:(RBNetRequest *)request {
    
   
    
    if (request.retryCount > 0) {
        request.retryCount --;
        // retry current request after 2 seconds.
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [self RB_sendRequest:request];
        });
        return;
    }
    
    if (self.callbackQueue) {
        __weak __typeof(self)weakSelf = self;
        dispatch_async(self.callbackQueue, ^{
            __strong __typeof(weakSelf)strongSelf = weakSelf;
            [strongSelf RB_execureFailureBlockWithError:error forRequest:request];
        });
    } else {
        // execure failure block in a private concurrent dispatch queue.
        [self RB_execureFailureBlockWithError:error forRequest:request];
    }
}

- (void)RB_execureFailureBlockWithError:(NSError *)error forRequest:(RBNetRequest *)request {
    RB_SAFE_BLOCK(request.failureBlock, error);
    RB_SAFE_BLOCK(request.finishedBlock, nil, error);
    [request cleanCallbackBlocks];
}

- (NSString *)RB_identifierForBatchAndChainRequest {
    NSString *identifier = nil;
    RBLock();
    self.autoIncrement++;
    identifier = [NSString stringWithFormat:@"BC%lu", (unsigned long)self.autoIncrement];
    RBUnlock();
    return identifier;
}

#pragma mark - Accessor

- (NSMutableDictionary<NSString *, id> *)runningBatchAndChainPool {
    if (!_runningBatchAndChainPool) {
        _runningBatchAndChainPool = [NSMutableDictionary dictionary];
    }
    return _runningBatchAndChainPool;
}

- (NSMutableDictionary<NSString *, id> *)generalParameters {
    if (!_generalParameters) {
        _generalParameters = [NSMutableDictionary dictionary];
    }
    return _generalParameters;
}

- (NSMutableDictionary<NSString *, NSString *> *)generalHeaders {
    if (!_generalHeaders) {
        _generalHeaders = [NSMutableDictionary dictionary];
    }
    return _generalHeaders;
}

@end



