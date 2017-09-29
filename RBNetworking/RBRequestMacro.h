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
    RBRequestNormal    = 0,
    RBRequestUpload    = 1,
    RBRequestDownload  = 2,
};

typedef NS_ENUM(NSInteger, RBHTTPMethodType) {
    RBHTTPMethodGET    = 0,    //!< GET
    RBHTTPMethodPOST   = 1,    //!< POST
    RBHTTPMethodHEAD   = 2,    //!< HEAD
    RBHTTPMethodDELETE = 3,    //!< DELETE
    RBHTTPMethodPUT    = 4,    //!< PUT
    RBHTTPMethodPATCH  = 5,    //!< PATCH
};

typedef NS_ENUM(NSInteger,RBRequestSerializerType) {
    RBRequestSerializerRAW     = 0,
    RBRequestSerializerJSON    = 1,
    RBRequestSerializerPlist   = 2,
};


typedef NS_ENUM(NSInteger, RBResponseSerializerType) {
    RBResponseSerializerRAW    = 0,
    RBResponseSerializerJSON   = 1,
    RBResponseSerializerPlist  = 2,
    RBResponseSerializerXML    = 3,
};

typedef void (^RBRequestConfigBlock)(RBNetRequest *request);
typedef void (^RBBatchRequestConfigBlock)(RBBatchRequest *batchRequest);
typedef void (^RBChainRequestConfigBlock)(RBChainRequest *chainRequest);
typedef void (^RBProgressBlock)(NSProgress *progress);
typedef void (^RBSuccessBlock)(id _Nullable responseObject);
typedef void (^RBFailureBlock)(NSError * _Nullable error);
typedef void (^RBFinishedBlock)(id _Nullable responseObject, NSError * _Nullable error);
typedef void (^RBCancelBlock)(id _Nullable request);
typedef void (^RBBCSuccessBlock)(NSArray *responseObjects);
typedef void (^RBBCFailureBlock)(NSArray *errors);
typedef void (^RBBCFinishedBlock)(NSArray * _Nullable responseObjects, NSArray * _Nullable errors);
typedef void (^RBBCNextBlock)(RBNetRequest *request, id _Nullable responseObject, BOOL *isSent);
typedef void (^RBCenterRequestProcessBlock)(RBNetRequest *request);
typedef void (^RBCenterResponseProcessBlock)(RBNetRequest *request, id _Nullable responseObject, NSError * _Nullable __autoreleasing *error);

NS_ASSUME_NONNULL_END
