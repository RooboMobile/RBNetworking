//
//  RBNetworkConfig.h
//  Pods
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBBaseRequest;
@class AFSecurityPolicy;

///  YTKUrlFilterProtocol can be used to append common parameters to requests before sending them.
@protocol RBUrlFilterProtocol <NSObject>
///  Preprocess request URL before actually sending them.
///
///  @param originUrl request's origin URL, which is returned by `requestUrl`
///  @param request   request itself
///
///  @return A new url which will be used as a new `requestUrl`
- (NSString *)filterUrl:(NSString *)originUrl withRequest:(RBBaseRequest *)request;
@end

///  YTKCacheDirPathFilterProtocol can be used to append common path components when caching response results
@protocol RBCacheDirPathFilterProtocol <NSObject>
///  Preprocess cache path before actually saving them.
///
///  @param originPath original base cache path, which is generated in `YTKRequest` class.
///  @param request    request itself
///
///  @return A new path which will be used as base path when caching.
- (NSString *)filterCacheDirPath:(NSString *)originPath withRequest:(RBBaseRequest *)request;
@end

///  YTKNetworkConfig stored global network-related configurations, which will be used in `YTKNetworkAgent`
///  to form and filter requests, as well as caching response.
@interface RBNetworkConfig : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Return a shared config object.
+ (RBNetworkConfig *)sharedConfig;

///  Request base URL, such as "http://www.yuantiku.com". Default is empty string.
@property (nonatomic, strong) NSString *baseUrl;
///  Request CDN URL. Default is empty string.
@property (nonatomic, strong) NSString *cdnUrl;
///  URL filters. See also `YTKUrlFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<RBUrlFilterProtocol>> *urlFilters;
///  Cache path filters. See also `YTKCacheDirPathFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<RBCacheDirPathFilterProtocol>> *cacheDirPathFilters;
///  Security policy will be used by AFNetworking. See also `AFSecurityPolicy`.
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;
///  Whether to log debug info. Default is NO;
@property (nonatomic) BOOL debugLogEnabled;
///  SessionConfiguration will be used to initialize AFHTTPSessionManager. Default is nil.
@property (nonatomic, strong) NSURLSessionConfiguration* sessionConfiguration;

///  Add a new URL filter.
- (void)addUrlFilter:(id<RBUrlFilterProtocol>)filter;
///  Remove all URL filters.
- (void)clearUrlFilter;
///  Add a new cache path filter
- (void)addCacheDirPathFilter:(id<RBCacheDirPathFilterProtocol>)filter;
///  Clear all cache path filters.
- (void)clearCacheDirPathFilter;

@end

NS_ASSUME_NONNULL_END
