//
//  RBBaseRequest.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString *const YTKRequestValidationErrorDomain;

NS_ENUM(NSInteger) {
    YTKRequestValidationErrorInvalidStatusCode = -8,
    YTKRequestValidationErrorInvalidJSONFormat = -9,
    };
    
    typedef NS_ENUM(NSInteger, YTKRequestMethod) {
        YTKRequestMethodGET = 0,
        YTKRequestMethodPOST,
        YTKRequestMethodHEAD,
        YTKRequestMethodPUT,
        YTKRequestMethodDELETE,
        YTKRequestMethodPATCH,
    };
    
    ///  Request serializer type.
    typedef NS_ENUM(NSInteger, YTKRequestSerializerType) {
        YTKRequestSerializerTypeHTTP = 0,
        YTKRequestSerializerTypeJSON,
    };
    
    typedef NS_ENUM(NSInteger, YTKResponseSerializerType) {
        
        YTKResponseSerializerTypeHTTP,
       
        YTKResponseSerializerTypeJSON,
       
        YTKResponseSerializerTypeXMLParser,
    };
    
    ///  Request priority
    typedef NS_ENUM(NSInteger, YTKRequestPriority) {
        YTKRequestPriorityLow = -4L,
        YTKRequestPriorityDefault = 0,
        YTKRequestPriorityHigh = 4,
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

///  Shortcut for `requestTask.currentRequest`.
@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;

///  Shortcut for `requestTask.originalRequest`.
@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;

///  Shortcut for `requestTask.response`.
@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;

///  The response status code.
@property (nonatomic, readonly) NSInteger responseStatusCode;

///  The response header fields.
@property (nonatomic, strong, readonly, nullable) NSDictionary *responseHeaders;

///  The raw data representation of response. Note this value can be nil if request failed.
@property (nonatomic, strong, readonly, nullable) NSData *responseData;

///  The string representation of response. Note this value can be nil if request failed.
@property (nonatomic, strong, readonly, nullable) NSString *responseString;

///  This serialized response object. The actual type of this object is determined by
///  `YTKResponseSerializerType`. Note this value can be nil if request failed.
///
///  @discussion If `resumableDownloadPath` and DownloadTask is using, this value will
///              be the path to which file is successfully saved (NSURL), or nil if request failed.
@property (nonatomic, strong, readonly, nullable) id responseObject;

///  If you use `YTKResponseSerializerTypeJSON`, this is a convenience (and sematic) getter
///  for the response object. Otherwise this value is nil.
@property (nonatomic, strong, readonly, nullable) id responseJSONObject;

///  This error can be either serialization error or network error. If nothing wrong happens
///  this value will be nil.
@property (nonatomic, strong, readonly, nullable) NSError *error;

///  Return cancelled state of request task.
@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;

///  Executing state of request task.
@property (nonatomic, readonly, getter=isExecuting) BOOL executing;


#pragma mark - Request Configuration


///  Tag can be used to identify request. Default value is 0.
@property (nonatomic) NSInteger tag;

///  The userInfo can be used to store additional info about the request. Default is nil.
@property (nonatomic, strong, nullable) NSDictionary *userInfo;

///  The delegate object of the request. If you choose block style callback you can ignore this.
///  Default is nil.
@property (nonatomic, weak, nullable) id<RBRequestDelegate> delegate;

///  The success callback. Note if this value is not nil and `requestFinished` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) RBRequestCompletionBlock successCompletionBlock;

///  The failure callback. Note if this value is not nil and `requestFailed` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) RBRequestCompletionBlock failureCompletionBlock;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<RBRequestAccessory>> *requestAccessories;

///  This can be use to construct HTTP body when needed in POST request. Default is nil.
@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

@property (nonatomic, strong, nullable) NSString *resumableDownloadPath;

///  You can use this block to track the download progress. See also `resumableDownloadPath`.
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock resumableDownloadProgressBlock;

///  The priority of the request. Effective only on iOS 8+. Default is `YTKRequestPriorityDefault`.
@property (nonatomic) YTKRequestPriority requestPriority;

///  Set completion callbacks
- (void)setCompletionBlockWithSuccess:(nullable RBRequestCompletionBlock)success
                              failure:(nullable RBRequestCompletionBlock)failure;

///  Nil out both success and failure callback blocks.
- (void)clearCompletionBlock;

///  Convenience method to add request accessory. See also `requestAccessories`.
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


- (YTKRequestMethod)requestMethod;


- (YTKRequestSerializerType)requestSerializerType;


- (YTKResponseSerializerType)responseSerializerType;

///  Username and password used for HTTP authorization. Should be formed as @[@"Username", @"Password"].
- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;

///  Additional HTTP request header field.
- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

///  Use this to build custom request. If this method return non-nil value, `requestUrl`, `requestTimeoutInterval`,
///  `requestArgument`, `allowsCellularAccess`, `requestMethod` and `requestSerializerType` will all be ignored.
- (nullable NSURLRequest *)buildCustomUrlRequest;

///  Should use CDN when sending request.
- (BOOL)useCDN;

///  Whether the request is allowed to use the cellular radio (if present). Default is YES.
- (BOOL)allowsCellularAccess;

///  The validator will be used to test if `responseJSONObject` is correctly formed.
- (nullable id)jsonValidator;

///  This validator will be used to test if `responseStatusCode` is valid.
- (BOOL)statusCodeValidator;

@end
    
    NS_ASSUME_NONNULL_END

