//
//  RBRequestMacro.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>
#define RB_SAFE_BLOCK(BlockName, ...) ({ !BlockName ? nil : BlockName(__VA_ARGS__); })
#define RBLock() dispatch_semaphore_wait(self->_lock, DISPATCH_TIME_FOREVER)
#define RBUnlock() dispatch_semaphore_signal(self->_lock)
NS_ASSUME_NONNULL_BEGIN

@class RBNetRequest, RBBatchRequest, RBChainRequest;


typedef NS_ENUM(NSInteger, RBRequestType) {
    RBRequestNormal    = 0,    //!< Normal HTTP request type, such as GET, POST, ...
    RBRequestUpload    = 1,    //!< Upload request type
    RBRequestDownload  = 2,    //!< Download request type
};

/**
 HTTP methods enum for XMRequest.
 */
typedef NS_ENUM(NSInteger, RBHTTPMethodType) {
    RBHTTPMethodGET    = 0,    //!< GET
    RBHTTPMethodPOST   = 1,    //!< POST
    RBHTTPMethodHEAD   = 2,    //!< HEAD
    RBHTTPMethodDELETE = 3,    //!< DELETE
    RBHTTPMethodPUT    = 4,    //!< PUT
    RBHTTPMethodPATCH  = 5,    //!< PATCH
};

/**
 Resquest parameter serialization type enum for XMRequest, see `AFURLRequestSerialization.h` for details.
 */
typedef NS_ENUM(NSInteger,RBRequestSerializerType) {
    RBRequestSerializerRAW     = 0,    //!< Encodes parameters to a query string and put it into HTTP body, setting the `Content-Type` of the encoded request to default value `application/x-www-form-urlencoded`.
    RBRequestSerializerJSON    = 1,    //!< Encodes parameters as JSON using `NSJSONSerialization`, setting the `Content-Type` of the encoded request to `application/json`.
    RBRequestSerializerPlist   = 2,    //!< Encodes parameters as Property List using `NSPropertyListSerialization`, setting the `Content-Type` of the encoded request to `application/x-plist`.
};

/**
 Response data serialization type enum for XMRequest, see `AFURLResponseSerialization.h` for details.
 */
typedef NS_ENUM(NSInteger, RBResponseSerializerType) {
    RBResponseSerializerRAW    = 0,    //!< Validates the response status code and content type, and returns the default response data.
    RBResponseSerializerJSON   = 1,    //!< Validates and decodes JSON responses using `NSJSONSerialization`, and returns a NSDictionary/NSArray/... JSON object.
    RBResponseSerializerPlist  = 2,    //!< Validates and decodes Property List responses using `NSPropertyListSerialization`, and returns a property list object.
    RBResponseSerializerXML    = 3,    //!< Validates and decodes XML responses as an `NSXMLParser` objects.
};

///------------------------------
/// @name XMRequest Config Blocks
///------------------------------

typedef void (^RBRequestConfigBlock)(RBNetRequest *request);
typedef void (^RBBatchRequestConfigBlock)(RBBatchRequest *batchRequest);
typedef void (^RBChainRequestConfigBlock)(RBChainRequest *chainRequest);

///--------------------------------
/// @name XMRequest Callback Blocks
///--------------------------------

typedef void (^RBProgressBlock)(NSProgress *progress);
typedef void (^RBSuccessBlock)(id _Nullable responseObject);
typedef void (^RBFailureBlock)(NSError * _Nullable error);
typedef void (^RBFinishedBlock)(id _Nullable responseObject, NSError * _Nullable error);
typedef void (^RBCancelBlock)(id _Nullable request); // The `request` might be a XMRequest/XMBatchRequest/XMChainRequest object.

///-------------------------------------------------
/// @name Callback Blocks for Batch or Chain Request
///-------------------------------------------------

typedef void (^RBBCSuccessBlock)(NSArray *responseObjects);
typedef void (^RBBCFailureBlock)(NSArray *errors);
typedef void (^RBBCFinishedBlock)(NSArray * _Nullable responseObjects, NSArray * _Nullable errors);
typedef void (^RBBCNextBlock)(RBNetRequest *request, id _Nullable responseObject, BOOL *isSent);

///------------------------------
/// @name XMCenter Process Blocks
///------------------------------

/**
 The custom request pre-process block for all XMRequests invoked by XMCenter.
 
 @param request The current XMRequest object.
 */
typedef void (^RBCenterRequestProcessBlock)(RBNetRequest *request);
/**
 The custom response process block for all XMRequests invoked by XMCenter.
 
 @param request The current XMRequest object.
 @param responseObject The response data return from server.
 @param error The error that occurred while the response data don't conforms to your own business logic.
 */
typedef void (^RBCenterResponseProcessBlock)(RBNetRequest *request, id _Nullable responseObject, NSError * _Nullable __autoreleasing *error);

NS_ASSUME_NONNULL_END
