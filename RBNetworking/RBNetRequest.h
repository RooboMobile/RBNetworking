//
//  RBNetRequest.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>

#import "RBRequestMacro.h"

NS_ASSUME_NONNULL_BEGIN

@class RBUploadFormData;


@interface RBNetRequest : NSObject


+ (instancetype)request;


@property (nonatomic, copy, readonly) NSString *identifier;


@property (nonatomic, copy, nullable) NSString *server;


@property (nonatomic, copy, nullable) NSString *api;


@property (nonatomic, copy, nullable) NSString *url;


@property (nonatomic, strong, nullable) NSDictionary<NSString *, id> *parameters;


@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *headers;

@property (nonatomic, assign) BOOL useGeneralServer;

@property (nonatomic, assign) BOOL useGeneralHeaders;

@property (nonatomic, assign) BOOL useGeneralParameters;

@property (nonatomic, assign) RBRequestType requestType;

@property (nonatomic, assign) RBHTTPMethodType httpMethod;

@property (nonatomic, assign) RBRequestSerializerType requestSerializerType;

@property (nonatomic, assign) RBResponseSerializerType responseSerializerType;

@property (nonatomic, assign) NSTimeInterval timeoutInterval;

@property (nonatomic, assign) NSUInteger retryCount;

@property (nonatomic, strong, nullable) NSDictionary *userInfo;

@property (nonatomic, copy, readonly, nullable) RBSuccessBlock successBlock;


@property (nonatomic, copy, readonly, nullable) RBFailureBlock failureBlock;


@property (nonatomic, copy, readonly, nullable) RBFinishedBlock finishedBlock;


@property (nonatomic, copy, readonly, nullable) RBProgressBlock progressBlock;


- (void)cleanCallbackBlocks;

@property (nonatomic, strong, nullable) NSMutableArray<RBUploadFormData *> *uploadFormDatas;


@property (nonatomic, copy, nullable) NSString *downloadSavePath;


- (void)addFormDataWithName:(NSString *)name fileData:(NSData *)fileData;
- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData;
- (void)addFormDataWithName:(NSString *)name fileURL:(NSURL *)fileURL;
- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL;

@end

#pragma mark - XMBatchRequest

@interface RBBatchRequest : NSObject

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) NSMutableArray *requestArray;
@property (nonatomic, strong, readonly) NSMutableArray *responseArray;

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(nullable id)responseObject error:(nullable NSError *)error;

@end

#pragma mark - XMChainRequest

@interface RBChainRequest : NSObject

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) RBNetRequest *runningRequest;

- (RBChainRequest *)onFirst:(RBRequestConfigBlock)firstBlock;
- (RBChainRequest *)onNext:(RBBCNextBlock)nextBlock;

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(nullable id)responseObject error:(nullable NSError *)error;

@end

#pragma mark - XMUploadFormData

@interface RBUploadFormData : NSObject

@property (nonatomic, copy) NSString *name;

@property (nonatomic, copy, nullable) NSString *fileName;

@property (nonatomic, copy, nullable) NSString *mimeType;

@property (nonatomic, strong, nullable) NSData *fileData;

@property (nonatomic, strong, nullable) NSURL *fileURL;


+ (instancetype)formDataWithName:(NSString *)name fileData:(NSData *)fileData;

+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData;

+ (instancetype)formDataWithName:(NSString *)name fileURL:(NSURL *)fileURL;

+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL;

@end

NS_ASSUME_NONNULL_END
