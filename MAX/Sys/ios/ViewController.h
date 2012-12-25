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

struct __TouchDelegateAdapter {
    iosDisplay    *_touchDelegate;
};

typedef struct __TouchDelegateAdapter TouchDelagteAdapter;

@class GLSurface;

@interface ViewController : UIViewController {
@private
    GLSurface               *_glView;
    TouchDelagteAdapter     _touchApdater;
}
@property (nonatomic, assign) GLuint depthFormat;
@property (nonatomic, assign) GLuint pixelFormat;

+ (ViewController *)instance;

- (void)setTouchDelegate:(TouchDelagteAdapter)delegate;

@end
