//
//  RBChainRequest.h
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class RBChainRequest;
@class RBBaseRequest;
@protocol RBRequestAccessory;

///  The YTKChainRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue. Note the delegate methods will be called when all the requests
///  of chain request finishes.
@protocol RBChainRequestDelegate <NSObject>

@optional
///  Tell the delegate that the chain request has finished successfully.
///
///  @param chainRequest The corresponding chain request.
- (void)chainRequestFinished:(RBChainRequest *)chainRequest;

///  Tell the delegate that the chain request has failed.
///
///  @param chainRequest The corresponding chain request.
///  @param request      First failed request that causes the whole request to fail.
- (void)chainRequestFailed:(RBChainRequest *)chainRequest failedBaseRequest:(RBBaseRequest*)request;

@end

typedef void (^RBChainCallback)(RBChainRequest *chainRequest, RBBaseRequest *baseRequest);

///  YTKBatchRequest can be used to chain several YTKRequest so that one will only starts after another finishes.
///  Note that when used inside YTKChainRequest, a single YTKRequest will have its own callback and delegate
///  cleared, in favor of the batch request callback.
@interface RBChainRequest : NSObject

///  All the requests are stored in this array.
- (NSArray<RBBaseRequest *> *)requestArray;

///  The delegate object of the chain request. Default is nil.
@property (nonatomic, weak, nullable) id<RBChainRequestDelegate> delegate;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<RBRequestAccessory>> *requestAccessories;

///  Convenience method to add request accessory. See also `requestAccessories`.
- (void)addAccessory:(id<RBRequestAccessory>)accessory;

///  Start the chain request, adding first request in the chain to request queue.
- (void)start;

///  Stop the chain request. Remaining request in chain will be cancelled.
- (void)stop;

///  Add request to request chain.
///
///  @param request  The request to be chained.
///  @param callback The finish callback
- (void)addRequest:(RBBaseRequest *)request callback:(nullable RBChainCallback)callback;

@end
NS_ASSUME_NONNULL_END

