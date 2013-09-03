//
//  ViewController.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
//
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "ios_display.h"
#import "DisplayPinchDelegate.h"
#import <CoreMotion/CoreMotion.h>

struct __TouchDelegateAdapter {
    iosDisplay    *_touchDelegate;
};

typedef struct __TouchDelegateAdapter TouchDelagteAdapter;

@class GLSurface;

@interface ViewController : UIViewController<UIGestureRecognizerDelegate> {
@private
    GLSurface               *_glView;
    TouchDelagteAdapter     _touchApdater;

    id _pan;
    id _pinch;
    id _tap;
    
    
    DisplayPinchDelegate* _pinchDelegate;
    
    NSSet* _touches;
    UIEvent* _event;
    
    BOOL _canhandleTap;
    
    NSTimeInterval lastTapTime;
    CMMotionManager *_motionManager;
}
@property (nonatomic, assign) GLuint depthFormat;
@property (nonatomic, assign) GLuint pixelFormat;

+ (ViewController *)instance;

- (void)setPinchDelegate:(DisplayPinchDelegate*)delegate;
- (void)setTouchDelegate:(TouchDelagteAdapter)delegate;

@end
