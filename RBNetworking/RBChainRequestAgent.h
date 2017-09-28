//
//  RBChainRequestAgent.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBChainRequest;


@interface RBChainRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;


+ (RBChainRequestAgent *)sharedAgent;

- (void)addChainRequest:(RBChainRequest *)request;

- (void)removeChainRequest:(RBChainRequest *)request;

@end

NS_ASSUME_NONNULL_END
