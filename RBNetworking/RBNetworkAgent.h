//
//  RBNetworkAgent.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
@class RBBaseRequest;

@interface RBNetworkAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

+ (RBNetworkAgent *)sharedAgent;

- (void)addRequest:(RBBaseRequest *)request;

- (void)cancelRequest:(RBBaseRequest *)request;

- (void)cancelAllRequests;

- (NSString *)buildRequestUrl:(RBBaseRequest *)request;

@end

NS_ASSUME_NONNULL_END
