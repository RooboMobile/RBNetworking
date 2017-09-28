//
//  RBChainRequestAgent.m
//  AFNetworking
//
//  Created by baxiang on 2017/9/28.
//

#import "RBChainRequestAgent.h"
#import "RBChainRequest.h"

@interface RBChainRequestAgent()

@property (strong, nonatomic) NSMutableArray<RBChainRequest *> *requestArray;

@end

@implementation RBChainRequestAgent

+ (RBChainRequestAgent *)sharedAgent {
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

- (void)addChainRequest:(RBChainRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeChainRequest:(RBChainRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}

@end
