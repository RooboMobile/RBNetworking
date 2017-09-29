//
//  RBNetConfiger.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>
@class RBNetEngine;

@interface RBRequestConfiger : NSObject

@property (nonatomic, copy, nullable) NSString *generalServer;
@property (nonatomic, strong, nullable) NSDictionary<NSString *, id> *generalParameters;
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *generalHeaders;
@property (nonatomic, strong, nullable) NSDictionary *generalUserInfo;
@property (nonatomic, strong, nullable) dispatch_queue_t callbackQueue;
@property (nonatomic, strong, nullable) RBNetEngine *engine;
@property (nonatomic, assign) BOOL consoleLog;
@end
