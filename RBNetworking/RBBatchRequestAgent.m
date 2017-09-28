//
//  RBBatchRequestAgent.m
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import "RBBatchRequestAgent.h"
#import "RBBatchRequest.h"

@interface RBBatchRequestAgent()

@property (strong, nonatomic) NSMutableArray<RBBatchRequest *> *requestArray;

@end

@implementation RBBatchRequestAgent

+ (RBBatchRequestAgent *)sharedAgent {
    static id sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _requestArray = [NSMutableArray array];
    }
    return self;
}

- (void)addBatchRequest:(RBBatchRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeBatchRequest:(RBBatchRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}

@end
