//
//  RBNetConfiger.h
//  Pods
//
//  Created by baxiang on 2017/9/29.
//

#import <Foundation/Foundation.h>

@interface RBNetConfiger : NSObject
///-----------------------------------------------
/// @name Properties to Assign Values for RBCenter
///-----------------------------------------------

/**
 The general server address to assign for RBCenter.
 */
@property (nonatomic, copy, nullable) NSString *generalServer;

/**
 The general parameters to assign for RBCenter.
 */
@property (nonatomic, strong, nullable) NSDictionary<NSString *, id> *generalParameters;

/**
 The general headers to assign for RBCenter.
 */
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *generalHeaders;

/**
 The general user info to assign for RBCenter.
 */
@property (nonatomic, strong, nullable) NSDictionary *generalUserInfo;

/**
 The dispatch callback queue to assign for RBCenter.
 */
@property (nonatomic, strong, nullable) dispatch_queue_t callbackQueue;

/**
 The global requests engine to assign for RBCenter.
 */
@property (nonatomic, strong, nullable) RBNetEngine *engine;

/**
 The console log BOOL value to assign for RBCenter.
 */
@property (nonatomic, assign) BOOL consoleLog;
@end
