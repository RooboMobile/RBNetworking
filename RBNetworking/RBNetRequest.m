//
//  RBNetRequest.m
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import "RBNetRequest.h"

@interface RBNetRequest ()

@end

@implementation RBNetRequest

+ (instancetype)request {
    return [[[self class] alloc] init];
}

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    _requestType = RBRequestNormal;
    _httpMethod = RBHTTPMethodPOST;
    _requestSerializerType = RBRequestSerializerRAW;
    _responseSerializerType = RBResponseSerializerJSON;
    _timeoutInterval = 60.0;
    
    _useGeneralServer = YES;
    _useGeneralHeaders = YES;
    _useGeneralParameters = YES;
    
    _retryCount = 0;
    
#ifdef XMMEMORYLOG
    NSLog(@"%@: %s", self, __FUNCTION__);
#endif
    
    return self;
}

- (void)cleanCallbackBlocks {
    _successBlock = nil;
    _failureBlock = nil;
    _finishedBlock = nil;
    _progressBlock = nil;
}

- (NSMutableArray<RBUploadFormData *> *)uploadFormDatas {
    if (!_uploadFormDatas) {
        _uploadFormDatas = [NSMutableArray array];
    }
    return _uploadFormDatas;
}

- (void)addFormDataWithName:(NSString *)name fileData:(NSData *)fileData {
    RBUploadFormData *formData = [RBUploadFormData formDataWithName:name fileData:fileData];
    [self.uploadFormDatas addObject:formData];
}

- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData {
    RBUploadFormData *formData = [RBUploadFormData formDataWithName:name fileName:fileName mimeType:mimeType fileData:fileData];
    [self.uploadFormDatas addObject:formData];
}

- (void)addFormDataWithName:(NSString *)name fileURL:(NSURL *)fileURL {
    RBUploadFormData *formData = [RBUploadFormData formDataWithName:name fileURL:fileURL];
    [self.uploadFormDatas addObject:formData];
}

- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL {
    RBUploadFormData *formData = [RBUploadFormData formDataWithName:name fileName:fileName mimeType:mimeType fileURL:fileURL];
    [self.uploadFormDatas addObject:formData];
}

#ifdef XMMEMORYLOG
- (void)dealloc {
    NSLog(@"%@: %s", self, __FUNCTION__);
}
#endif

@end



@interface RBBatchRequest () {
    dispatch_semaphore_t _lock;
    NSUInteger _finishedCount;
    BOOL _failed;
}

@property (nonatomic, copy) RBBCSuccessBlock batchSuccessBlock;
@property (nonatomic, copy) RBBCFailureBlock batchFailureBlock;
@property (nonatomic, copy) RBBCFinishedBlock batchFinishedBlock;

@end

@implementation RBBatchRequest

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _failed = NO;
    _finishedCount = 0;
    _lock = dispatch_semaphore_create(1);
    
    _requestArray = [NSMutableArray array];
    _responseArray = [NSMutableArray array];
    
#ifdef XMMEMORYLOG
    NSLog(@"%@: %s", self, __FUNCTION__);
#endif
    
    return self;
}

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(id)responseObject error:(NSError *)error {
    BOOL isFinished = NO;
    RBLock();
    NSUInteger index = [_requestArray indexOfObject:request];
    if (responseObject) {
        [_responseArray replaceObjectAtIndex:index withObject:responseObject];
    } else {
        _failed = YES;
        if (error) {
            [_responseArray replaceObjectAtIndex:index withObject:error];
        }
    }
    
    _finishedCount++;
    if (_finishedCount == _requestArray.count) {
        if (!_failed) {
            RB_SAFE_BLOCK(_batchSuccessBlock, _responseArray);
            RB_SAFE_BLOCK(_batchFinishedBlock, _responseArray, nil);
        } else {
            RB_SAFE_BLOCK(_batchFailureBlock, _responseArray);
            RB_SAFE_BLOCK(_batchFinishedBlock, nil, _responseArray);
        }
        [self cleanCallbackBlocks];
        isFinished = YES;
    }
    RBUnlock();
    return isFinished;
}

- (void)cleanCallbackBlocks {
    _batchSuccessBlock = nil;
    _batchFailureBlock = nil;
    _batchFinishedBlock = nil;
}

#ifdef XMMEMORYLOG
- (void)dealloc {
    NSLog(@"%@: %s", self, __FUNCTION__);
}
#endif

@end

#pragma mark - XMChainRequest

@interface RBChainRequest () {
    NSUInteger _chainIndex;
}

@property (nonatomic, strong, readwrite) RBNetRequest *runningRequest;

@property (nonatomic, strong) NSMutableArray<RBBCNextBlock> *nextBlockArray;
@property (nonatomic, strong) NSMutableArray *responseArray;

@property (nonatomic, copy) RBBCSuccessBlock chainSuccessBlock;
@property (nonatomic, copy) RBBCFailureBlock chainFailureBlock;
@property (nonatomic, copy) RBBCFinishedBlock chainFinishedBlock;

@end

@implementation RBChainRequest : NSObject

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _chainIndex = 0;
    _responseArray = [NSMutableArray array];
    _nextBlockArray = [NSMutableArray array];
    
#ifdef XMMEMORYLOG
    NSLog(@"%@: %s", self, __FUNCTION__);
#endif
    
    return self;
}

- (RBChainRequest *)onFirst:(RBRequestConfigBlock)firstBlock {
    NSAssert(firstBlock != nil, @"The first block for chain requests can't be nil.");
    NSAssert(_nextBlockArray.count == 0, @"The `-onFirst:` method must called befault `-onNext:` method");
    _runningRequest = [RBNetRequest request];
    firstBlock(_runningRequest);
    [_responseArray addObject:[NSNull null]];
    return self;
}

- (RBChainRequest *)onNext:(RBBCNextBlock)nextBlock {
    NSAssert(nextBlock != nil, @"The next block for chain requests can't be nil.");
    [_nextBlockArray addObject:nextBlock];
    [_responseArray addObject:[NSNull null]];
    return self;
}

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(id)responseObject error:(NSError *)error {
    BOOL isFinished = NO;
    if (responseObject) {
        [_responseArray replaceObjectAtIndex:_chainIndex withObject:responseObject];
        if (_chainIndex < _nextBlockArray.count) {
            _runningRequest = [RBNetRequest request];
            RBBCNextBlock nextBlock = _nextBlockArray[_chainIndex];
            BOOL isSent = YES;
            nextBlock(_runningRequest, responseObject, &isSent);
            if (!isSent) {
                RB_SAFE_BLOCK(_chainFailureBlock, _responseArray);
                RB_SAFE_BLOCK(_chainFinishedBlock, nil, _responseArray);
                [self cleanCallbackBlocks];
                isFinished = YES;
            }
        } else {
            RB_SAFE_BLOCK(_chainSuccessBlock, _responseArray);
            RB_SAFE_BLOCK(_chainFinishedBlock, _responseArray, nil);
            [self cleanCallbackBlocks];
            isFinished = YES;
        }
    } else {
        if (error) {
            [_responseArray replaceObjectAtIndex:_chainIndex withObject:error];
        }
        RB_SAFE_BLOCK(_chainFailureBlock, _responseArray);
        RB_SAFE_BLOCK(_chainFinishedBlock, nil, _responseArray);
        [self cleanCallbackBlocks];
        isFinished = YES;
    }
    _chainIndex++;
    return isFinished;
}

- (void)cleanCallbackBlocks {
    _runningRequest = nil;
    _chainSuccessBlock = nil;
    _chainFailureBlock = nil;
    _chainFinishedBlock = nil;
    [_nextBlockArray removeAllObjects];
}

#ifdef XMMEMORYLOG
- (void)dealloc {
    NSLog(@"%@: %s", self, __FUNCTION__);
}
#endif

@end

#pragma mark - XMUploadFormData

@implementation RBUploadFormData

+ (instancetype)formDataWithName:(NSString *)name fileData:(NSData *)fileData {
    RBUploadFormData *formData = [[RBUploadFormData alloc] init];
    formData.name = name;
    formData.fileData = fileData;
    return formData;
}

+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData {
    RBUploadFormData *formData = [[RBUploadFormData alloc] init];
    formData.name = name;
    formData.fileName = fileName;
    formData.mimeType = mimeType;
    formData.fileData = fileData;
    return formData;
}

+ (instancetype)formDataWithName:(NSString *)name fileURL:(NSURL *)fileURL {
    RBUploadFormData *formData = [[RBUploadFormData alloc] init];
    formData.name = name;
    formData.fileURL = fileURL;
    return formData;
}

+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL {
    RBUploadFormData *formData = [[RBUploadFormData alloc] init];
    formData.name = name;
    formData.fileName = fileName;
    formData.mimeType = mimeType;
    formData.fileURL = fileURL;
    return formData;
}

@end
