//
//  AppDelegate.m
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/16/12.
//
//

#import "AppDelegate.h"
#include "MAXEngine.h"
#include "MAXGame.h"
#include "Utils.h"
#include "PivotObject.h"
#include "cocos2d.h"
#import <QuartzCore/QuartzCore.h>
#include "DebugStackTrace.h"

@interface AppDelegate ()

- (void)frameCallback:(CADisplayLink *)link;

@end

@implementation AppDelegate

- (void)initEnviroment {
    DEBUG_FUNCTION_MESSAGE;
    CADisplayLink *link = [CADisplayLink displayLinkWithTarget:self selector:@selector(frameCallback:)];
    [link addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
 
    
    lastTime = [[NSDate dateWithTimeIntervalSinceNow:0] timeIntervalSince1970];
    engine->Init();
    game->Init();
    DEBUG_FUNCTION_EXIT;
}

- (void)frameCallback:(CADisplayLink *)link {
    DEBUG_FUNCTION_MESSAGE;
    UIApplicationState s = [UIApplication sharedApplication].applicationState;
    if (s != UIApplicationStateActive) {
        DEBUG_FUNCTION_EXIT;
        return;
    }
    NSTimeInterval time = [[NSDate dateWithTimeIntervalSinceNow:0] timeIntervalSince1970];
    NSTimeInterval delta = time - lastTime;
    lastTime = time;
    engine->RunLoop(delta);
    DEBUG_FUNCTION_EXIT;
}

void uncaughtExceptionHandler(NSException *exception) {
   
    NSString* str = [NSString stringWithCString:DEBUG_UTILS->fullStackTrace().c_str() encoding:[NSString defaultCStringEncoding]];
    
    
    NSLog(@"Stack Trace: \n%@", str);
    // Internal error reporting
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
//    {
//        
//        cout << sizeof(shared_ptr<PivotObject>);
//    
//        UContainer<MyClass> container = UContainer<MyClass>(10);
//        {
//            shared_ptr<MyClass> outptr;
//            {
//                shared_ptr<MyClass> ptr(new MyClass());
//                ptr->a = 10;
//                outptr = ptr;
//            }
//            container.addObject(outptr);
//        }
//        
//        
//        shared_ptr<MyClass> outptr = container.objectAtIndex(0);
//        cout <<"count=" <<container.GetCount() <<" item=" << outptr->a <<endl;
//        
//    }
    
    NSSetUncaughtExceptionHandler(&uncaughtExceptionHandler);
    srand([[NSDate date] timeIntervalSince1970]);
    [self initEnviroment];
    
    return YES;
}

int main(int argc, char *argv[])
{
    @autoreleasepool {
        
        int result = UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
        return result;
    }
}



@end
