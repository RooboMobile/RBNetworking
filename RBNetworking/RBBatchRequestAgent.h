//
//  RBBatchRequestAgent.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBBatchRequest;

///  YTKBatchRequestAgent handles batch request management. It keeps track of all
///  the batch requests.
@interface RBBatchRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared batch request agent.
+ (RBBatchRequestAgent *)sharedAgent;

///  Add a batch request.
- (void)addBatchRequest:(RBBatchRequest *)request;

///  Remove a previously added batch request.
- (void)removeBatchRequest:(RBBatchRequest *)request;

@end

NS_ASSUME_NONNULL_END
