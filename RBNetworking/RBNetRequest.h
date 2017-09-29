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

/**
 Finished block for request, called when current request is finished, the block will execute in `callbackQueue` of XMCenter.
 */
@property (nonatomic, copy, readonly, nullable) RBFinishedBlock finishedBlock;

/**
 Progress block for upload/download request, called when the upload/download progress is updated,
 NOTE: This block is called on the session queue, not the `callbackQueue` of XMCenter !!!
 */
@property (nonatomic, copy, readonly, nullable) RBProgressBlock progressBlock;

/**
 Nil out all callback blocks when a request is finished to break the potential retain cycle.
 */
- (void)cleanCallbackBlocks;

/**
 Upload files form data for upload request, `nil` by default, see `XMUploadFormData` class and `AFMultipartFormData` protocol for details.
 NOTE: This property is effective only when `requestType` is assigned to `kXMRequestUpload`.
 */
@property (nonatomic, strong, nullable) NSMutableArray<RBUploadFormData *> *uploadFormDatas;

/**
 Local save path for downloaded file, `nil` by default.
 NOTE: This property is effective only when `requestType` is assigned to `kXMRequestDownload`.
 */
@property (nonatomic, copy, nullable) NSString *downloadSavePath;

///----------------------------------------------------
/// @name Quickly Methods For Add Upload File Form Data
///----------------------------------------------------

- (void)addFormDataWithName:(NSString *)name fileData:(NSData *)fileData;
- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData;
- (void)addFormDataWithName:(NSString *)name fileURL:(NSURL *)fileURL;
- (void)addFormDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL;

@end

#pragma mark - XMBatchRequest

///------------------------------------------------------
/// @name XMBatchRequest Class for sending batch requests
///------------------------------------------------------

@interface RBBatchRequest : NSObject

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) NSMutableArray *requestArray;
@property (nonatomic, strong, readonly) NSMutableArray *responseArray;

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(nullable id)responseObject error:(nullable NSError *)error;

@end

#pragma mark - XMChainRequest

///------------------------------------------------------
/// @name XMChainRequest Class for sending chain requests
///------------------------------------------------------

@interface RBChainRequest : NSObject

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) RBNetRequest *runningRequest;

- (RBChainRequest *)onFirst:(RBRequestConfigBlock)firstBlock;
- (RBChainRequest *)onNext:(RBBCNextBlock)nextBlock;

- (BOOL)onFinishedOneRequest:(RBNetRequest *)request response:(nullable id)responseObject error:(nullable NSError *)error;

@end

#pragma mark - XMUploadFormData

/**
 `XMUploadFormData` is the class for describing and carring the upload file data, see `AFMultipartFormData` protocol for details.
 */
@interface RBUploadFormData : NSObject

/**
 The name to be associated with the specified data. This property must not be `nil`.
 */
@property (nonatomic, copy) NSString *name;

/**
 The file name to be used in the `Content-Disposition` header. This property is not recommended be `nil`.
 */
@property (nonatomic, copy, nullable) NSString *fileName;

/**
 The declared MIME type of the file data. This property is not recommended be `nil`.
 */
@property (nonatomic, copy, nullable) NSString *mimeType;

/**
 The data to be encoded and appended to the form data, and it is prior than `fileURL`.
 */
@property (nonatomic, strong, nullable) NSData *fileData;

/**
 The URL corresponding to the file whose content will be appended to the form, BUT, when the `fileData` is assigned，the `fileURL` will be ignored.
 */
@property (nonatomic, strong, nullable) NSURL *fileURL;

// NOTE: Either of the `fileData` and `fileURL` should not be `nil`, and the `fileName` and `mimeType` must both be `nil` or assigned at the same time,

///-----------------------------------------------------
/// @name Quickly Class Methods For Creates A New Object
///-----------------------------------------------------

+ (instancetype)formDataWithName:(NSString *)name fileData:(NSData *)fileData;
+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileData:(NSData *)fileData;
+ (instancetype)formDataWithName:(NSString *)name fileURL:(NSURL *)fileURL;
+ (instancetype)formDataWithName:(NSString *)name fileName:(NSString *)fileName mimeType:(NSString *)mimeType fileURL:(NSURL *)fileURL;

@end

NS_ASSUME_NONNULL_END
