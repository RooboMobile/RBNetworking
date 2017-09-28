//
//  RBBaseRequest.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
FOUNDATION_EXPORT NSString *const RBRequestValidationErrorDomain;
FOUNDATION_EXPORT NSString *const RBRequestCacheErrorDomain;
typedef NS_ENUM(NSInteger,RBequestValidationError) {
    RBRequestValidationErrorInvalidStatusCode = -8,
    RBRequestValidationErrorInvalidJSONFormat = -9,
    };
typedef NS_ENUM(NSInteger, RBRequestMethod) {
    RBRequestMethodGET = 0,
    RBRequestMethodPOST,
    RBRequestMethodHEAD,
    RBRequestMethodPUT,
    RBRequestMethodDELETE,
    RBRequestMethodPATCH,
    };
typedef NS_ENUM(NSInteger, RBRequestSerializerType) {
    RBRequestSerializerTypeHTTP = 0,
    RBRequestSerializerTypeJSON,
};
typedef NS_ENUM(NSInteger, RBResponseSerializerType) {
    RBResponseSerializerTypeHTTP,
    RBResponseSerializerTypeJSON,
    RBResponseSerializerTypeXMLParser,
};
    
typedef NS_ENUM(NSInteger, RBRequestPriority) {
    RBRequestPriorityLow = -4L,
    RBRequestPriorityDefault = 0,
    RBRequestPriorityHigh = 4,
};

typedef NS_ENUM(NSInteger,RBRequestCacheError) {
    RBRequestCacheErrorExpired = -1,
    RBRequestCacheErrorVersionMismatch = -2,
    RBRequestCacheErrorSensitiveDataMismatch = -3,
    RBRequestCacheErrorAppVersionMismatch = -4,
    RBRequestCacheErrorInvalidCacheTime = -5,
    RBRequestCacheErrorInvalidMetadata = -6,
    RBRequestCacheErrorInvalidCacheData = -7,
};

@protocol AFMultipartFormData;

typedef void (^AFConstructingBlock)(id<AFMultipartFormData> formData);
typedef void (^AFURLSessionTaskProgressBlock)(NSProgress *);
    
@class RBBaseRequest;
typedef void(^RBRequestCompletionBlock)(__kindof RBBaseRequest *request);
    
@protocol RBRequestDelegate <NSObject>
    
@optional

- (void)requestFinished:(__kindof RBBaseRequest *)request;
    
- (void)requestFailed:(__kindof RBBaseRequest *)request;
    
@end
    
@protocol RBRequestAccessory <NSObject>
    
@optional
    
- (void)requestWillStart:(id)request;
    
- (void)requestWillStop:(id)request;
    
- (void)requestDidStop:(id)request;
@end
    
  
@interface RBBaseRequest : NSObject

#pragma mark - Request and Response Information

@property (nonatomic, strong, readonly) NSURLSessionTask *requestTask;

@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;


@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;


@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;


@property (nonatomic, readonly) NSInteger responseStatusCode;


@property (nonatomic, strong, readonly, nullable) NSDictionary *responseHeaders;


@property (nonatomic, strong, readonly, nullable) NSData *responseData;


@property (nonatomic, strong, readonly, nullable) NSString *responseString;


@property (nonatomic, strong, readonly, nullable) id responseObject;


@property (nonatomic, strong, readonly, nullable) id responseJSONObject;


@property (nonatomic, strong, readonly, nullable) NSError *error;


@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;


@property (nonatomic, readonly, getter=isExecuting) BOOL executing;

@property (nonatomic) BOOL ignoreCache;
#pragma mark - Request Configuration

@property (nonatomic) NSInteger tag;

@property (nonatomic, strong, nullable) NSDictionary *userInfo;

@property (nonatomic, weak, nullable) id<RBRequestDelegate> delegate;

@property (nonatomic, copy, nullable) RBRequestCompletionBlock successCompletionBlock;

@property (nonatomic, copy, nullable) RBRequestCompletionBlock failureCompletionBlock;

@property (nonatomic, strong, nullable) NSMutableArray<id<RBRequestAccessory>> *requestAccessories;

@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

@property (nonatomic, strong, nullable) NSString *resumableDownloadPath;

@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock resumableDownloadProgressBlock;


@property (nonatomic) RBRequestPriority requestPriority;


- (void)setCompletionBlockWithSuccess:(nullable RBRequestCompletionBlock)success
                              failure:(nullable RBRequestCompletionBlock)failure;


- (void)clearCompletionBlock;


- (void)addAccessory:(id<RBRequestAccessory>)accessory;

#pragma mark - Request Action

- (void)start;


- (void)stop;


- (void)startWithCompletionBlockWithSuccess:(nullable RBRequestCompletionBlock)success
                                    failure:(nullable RBRequestCompletionBlock)failure;


#pragma mark - Subclass Override

- (void)requestCompletePreprocessor;


- (void)requestCompleteFilter;


- (void)requestFailedPreprocessor;


- (void)requestFailedFilter;

- (NSString *)baseUrl;

- (NSString *)requestUrl;


- (NSString *)cdnUrl;


- (NSTimeInterval)requestTimeoutInterval;


- (nullable id)requestArgument;


- (id)cacheFileNameFilterForRequestArgument:(id)argument;


- (RBRequestMethod)requestMethod;


- (RBRequestSerializerType)requestSerializerType;


- (RBResponseSerializerType)responseSerializerType;


- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;


- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;


- (nullable NSURLRequest *)buildCustomUrlRequest;


- (BOOL)useCDN;


- (BOOL)allowsCellularAccess;


- (nullable id)jsonValidator;

- (BOOL)statusCodeValidator;


- (BOOL)isDataFromCache;


- (BOOL)loadCacheWithError:(NSError * __autoreleasing *)error;


- (void)startWithoutCache;


- (void)saveResponseDataToCacheFile:(NSData *)data;

#pragma mark - Subclass Override

- (NSInteger)cacheTimeInSeconds;


- (long long)cacheVersion;


- (nullable id)cacheSensitiveData;


- (BOOL)writeCacheAsynchronously;

@end
NS_ASSUME_NONNULL_END

