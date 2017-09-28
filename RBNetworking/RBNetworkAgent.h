//
//  RBNetworkAgent.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBBaseRequest;

///  YTKNetworkAgent is the underlying class that handles actual request generation,
///  serialization and response handling.
@interface RBNetworkAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared agent.
+ (RBNetworkAgent *)sharedAgent;

///  Add request to session and start it.
- (void)addRequest:(RBBaseRequest *)request;

///  Cancel a request that was previously added.
- (void)cancelRequest:(RBBaseRequest *)request;

///  Cancel all requests that were previously added.
- (void)cancelAllRequests;

///  Return the constructed URL of request.
///
///  @param request The request to parse. Should not be nil.
///
///  @return The result URL.
- (NSString *)buildRequestUrl:(RBBaseRequest *)request;

@end

NS_ASSUME_NONNULL_END
