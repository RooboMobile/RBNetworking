//
//  RBNetworkConfig.h
//  Pods
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBBaseRequest;
@class AFSecurityPolicy;


@protocol RBUrlFilterProtocol <NSObject>

- (NSString *)filterUrl:(NSString *)originUrl withRequest:(RBBaseRequest *)request;
@end

@protocol RBCacheDirPathFilterProtocol <NSObject>

- (NSString *)filterCacheDirPath:(NSString *)originPath withRequest:(RBBaseRequest *)request;
@end

@interface RBNetworkConfig : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

+ (RBNetworkConfig *)sharedConfig;

@property (nonatomic, strong) NSString *baseUrl;


@property (nonatomic, strong, readonly) NSArray<id<RBUrlFilterProtocol>> *urlFilters;

@property (nonatomic, strong, readonly) NSArray<id<RBCacheDirPathFilterProtocol>> *cacheDirPathFilters;

@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

@property (nonatomic) BOOL debugLogEnabled;

@property (nonatomic, strong) NSURLSessionConfiguration* sessionConfiguration;

- (void)addUrlFilter:(id<RBUrlFilterProtocol>)filter;

- (void)clearUrlFilter;

- (void)addCacheDirPathFilter:(id<RBCacheDirPathFilterProtocol>)filter;

- (void)clearCacheDirPathFilter;

@end

NS_ASSUME_NONNULL_END
