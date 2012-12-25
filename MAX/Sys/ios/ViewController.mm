//
//  ViewController.m
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/18/12.
//
//

#import "ViewController.h"
#import "GLSurface.h"

ViewController      *_g_Controller = nil;

@interface ViewController ()

@end

static int const _kTEiOSMaxTouchesCount = 10;

@implementation ViewController

+ (ViewController *)instance {
    if (!_g_Controller) {
        _g_Controller = [[ViewController alloc] init];
    }
    return _g_Controller;
}

- (void)loadView {
    UIScreen *screen = [UIScreen mainScreen];
    
    self.view = [[GLSurface alloc] initWithFrame:screen.bounds andDepthFormat:_depthFormat andPixelFormat:_pixelFormat];
    _glView = (GLSurface *)self.view;
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)setTouchDelegate:(TouchDelagteAdapter)delegate {
    _touchApdater = delegate;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscape;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    int ids[_kTEiOSMaxTouchesCount] = {0};
    float xs[_kTEiOSMaxTouchesCount] = {0.0f};
    float ys[_kTEiOSMaxTouchesCount] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (int)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    _touchApdater._touchDelegate->handleTouchesBegin(i, ids, xs, ys);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    int ids[_kTEiOSMaxTouchesCount] = {0};
    float xs[_kTEiOSMaxTouchesCount] = {0.0f};
    float ys[_kTEiOSMaxTouchesCount] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (int)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    _touchApdater._touchDelegate->handleTouchesCancel(i, ids, xs, ys);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    int ids[_kTEiOSMaxTouchesCount] = {0};
    float xs[_kTEiOSMaxTouchesCount] = {0.0f};
    float ys[_kTEiOSMaxTouchesCount] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (int)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    _touchApdater._touchDelegate->handleTouchesEnd(i, ids, xs, ys);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    int ids[_kTEiOSMaxTouchesCount] = {0};
    float xs[_kTEiOSMaxTouchesCount] = {0.0f};
    float ys[_kTEiOSMaxTouchesCount] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (int)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    _touchApdater._touchDelegate->handleTouchesMove(i, ids, xs, ys);
}

@end
