//
//  ViewController.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "ios_display.h"
#import "DisplayPinchDelegate.h"

struct __TouchDelegateAdapter {
    iosDisplay    *_touchDelegate;
};

typedef struct __TouchDelegateAdapter TouchDelagteAdapter;

@class GLSurface;

@interface ViewController : UIViewController<UIGestureRecognizerDelegate> {
@private
    GLSurface               *_glView;
    TouchDelagteAdapter     _touchApdater;

    DisplayPinchDelegate* _pinchDelegate;
}
@property (nonatomic, assign) GLuint depthFormat;
@property (nonatomic, assign) GLuint pixelFormat;

+ (ViewController *)instance;

- (void)setPinchDelegate:(DisplayPinchDelegate*)delegate;
- (void)setTouchDelegate:(TouchDelagteAdapter)delegate;

@end
