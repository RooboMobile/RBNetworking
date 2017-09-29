//
//  RBNetEngine.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBNetRequest;


typedef void (^RBCompletionHandler) (id _Nullable responseObject, NSError * _Nullable error);


@interface RBNetEngine : NSObject


+ (instancetype)engine;


+ (instancetype)sharedEngine;



- (void)sendRequest:(RBNetRequest *)request completionHandler:(nullable RBCompletionHandler)completionHandler;


- (nullable RBNetRequest *)cancelRequestByIdentifier:(NSString *)identifier;


- (nullable RBNetRequest *)getRequestByIdentifier:(NSString *)identifier;


- (NSInteger)reachabilityStatus;


- (void)addSSLPinningURL:(NSString *)url;


- (void)addSSLPinningCert:(NSData *)cert;

- (void)addTwowayAuthenticationPKCS12:(NSData *)p12 keyPassword:(NSString *)password;

@end

NS_ASSUME_NONNULL_END
