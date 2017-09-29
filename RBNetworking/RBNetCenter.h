//
//  RBNetCenter.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>
#import "RBRequestMacro.h"
@class RBRequestConfiger,RBNetEngine,RBRequestConfiger;

NS_ASSUME_NONNULL_BEGIN
@interface RBNetCenter : NSObject

+ (instancetype)center;
+ (instancetype)defaultCenter;

@property (nonatomic, copy, nullable) NSString *generalServer;
@property (nonatomic, strong, nullable, readonly) NSMutableDictionary<NSString *, id> *generalParameters;
@property (nonatomic, strong, nullable, readonly) NSMutableDictionary<NSString *, NSString *> *generalHeaders;
@property (nonatomic, strong, nullable) NSDictionary *generalUserInfo;
@property (nonatomic, strong, nullable) dispatch_queue_t callbackQueue;
@property (nonatomic, strong) RBNetEngine *engine;
@property (nonatomic, assign) BOOL consoleLog;


- (void)setupConfig:(void(^)(RBRequestConfiger *config))block;

- (void)setRequestProcessBlock:(RBCenterRequestProcessBlock)block;

- (void)setResponseProcessBlock:(RBCenterResponseProcessBlock)block;

- (void)setGeneralHeaderValue:(nullable NSString *)value forField:(NSString *)field;

- (void)setGeneralParameterValue:(nullable id)value forKey:(NSString *)key;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onProgress:(nullable RBProgressBlock)progressBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

- (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onProgress:(nullable RBProgressBlock)progressBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

- (nullable NSString *)sendBatchRequest:(RBBatchRequestConfigBlock)configBlock
                              onSuccess:(nullable RBBCSuccessBlock)successBlock
                              onFailure:(nullable RBBCFailureBlock)failureBlock
                             onFinished:(nullable RBBCFinishedBlock)finishedBlock;

- (nullable NSString *)sendChainRequest:(RBChainRequestConfigBlock)configBlock
                              onSuccess:(nullable RBBCSuccessBlock)successBlock
                              onFailure:(nullable RBBCFailureBlock)failureBlock
                             onFinished:(nullable RBBCFinishedBlock)finishedBlock;

- (void)cancelRequest:(NSString *)identifier;

- (void)cancelRequest:(NSString *)identifier
             onCancel:(nullable RBCancelBlock)cancelBlock;

- (nullable id)getRequest:(NSString *)identifier;

- (BOOL)isNetworkReachable;
+ (void)setupConfig:(void(^)(RBRequestConfiger *config))block;
+ (void)setRequestProcessBlock:(RBCenterRequestProcessBlock)block;
+ (void)setResponseProcessBlock:(RBCenterResponseProcessBlock)block;
+ (void)setGeneralHeaderValue:(nullable NSString *)value forField:(NSString *)field;
+ (void)setGeneralParameterValue:(nullable id)value forKey:(NSString *)key;


+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onProgress:(nullable RBProgressBlock)progressBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock;

+ (nullable NSString *)sendRequest:(RBRequestConfigBlock)configBlock
                        onProgress:(nullable RBProgressBlock)progressBlock
                         onSuccess:(nullable RBSuccessBlock)successBlock
                         onFailure:(nullable RBFailureBlock)failureBlock
                        onFinished:(nullable RBFinishedBlock)finishedBlock;

+ (nullable NSString *)sendBatchRequest:(RBBatchRequestConfigBlock)configBlock
                              onSuccess:(nullable RBBCSuccessBlock)successBlock
                              onFailure:(nullable RBBCFailureBlock)failureBlock
                             onFinished:(nullable RBBCFinishedBlock)finishedBlock;

+ (nullable NSString *)sendChainRequest:(RBChainRequestConfigBlock)configBlock
                              onSuccess:(nullable RBBCSuccessBlock)successBlock
                              onFailure:(nullable RBBCFailureBlock)failureBlock
                             onFinished:(nullable RBBCFinishedBlock)finishedBlock;

#pragma mark -

+ (void)cancelRequest:(NSString *)identifier;

+ (void)cancelRequest:(NSString *)identifier
             onCancel:(nullable RBCancelBlock)cancelBlock;

+ (nullable id)getRequest:(NSString *)identifier;

+ (BOOL)isNetworkReachable;

#pragma mark -

+ (void)addSSLPinningURL:(NSString *)url;
+ (void)addSSLPinningCert:(NSData *)cert;
+ (void)addTwowayAuthenticationPKCS12:(NSData *)p12 keyPassword:(NSString *)password;

@end


NS_ASSUME_NONNULL_END
