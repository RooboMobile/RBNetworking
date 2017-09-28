//
//  RBNetworking.h
//  Pods
//
//  Created by baxiang on 2017/9/28.
//

#import <Foundation/Foundation.h>

#if __has_include(<RBNetworking/RBNetworking.h>)
#import <RBNetworking/RBBaseRequest.h>
#import <RBNetworking/RBBatchRequest.h>
#import <RBNetworking/RBBatchRequestAgent.h>
#import <RBNetworking/RBChainRequest.h>
#import <RBNetworking/RBChainRequestAgent.h>
#import <RBNetworking/RBNetworkAgent.h>
#import <RBNetworking/RBNetworkConfig.h>

#else
#import "RBBaseRequest.h"
#import "RBBatchRequest.h"
#import "RBBatchRequestAgent.h"
#import "RBChainRequest.h"
#import "RBChainRequestAgent.h"
#import "RBNetworkAgent.h"
#import "RBNetworkConfig.h"
#endif



@interface RBNetworking : NSObject

@end
