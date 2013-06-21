//
//  ViewController.m
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/18/12.
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
    
    UIPinchGestureRecognizer* vc = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
    vc.delegate = self;
    [self.view addGestureRecognizer:vc];
    _pinch = vc;
    
    UIPanGestureRecognizer* vc1 = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
    vc1.delegate = self;
    [self.view addGestureRecognizer:vc1];
    _pan = vc1;
    
    UITapGestureRecognizer* vc2 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTap:)];
    vc2.delegate = self;
    vc2.numberOfTouchesRequired = 1;
    vc2.numberOfTapsRequired = 1;
    [self.view addGestureRecognizer:vc2];
    _tap = vc2;
    
    UILongPressGestureRecognizer* vc3 = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongTap:)];
    vc3.delegate = self;
    vc3.minimumPressDuration = 0.5;
    [self.view addGestureRecognizer:vc3];
    
    lastTapTime = 0;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if(!_pinchDelegate)
        return NO;
    BOOL result = _pinchDelegate->CanStartPinch([touch locationInView:self.view].x, [touch locationInView:self.view].y);
    return result;
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
    if(!_pinchDelegate)
        return NO;
    return _pinchDelegate->CanStartPinch([gestureRecognizer locationInView:self.view].x, [gestureRecognizer locationInView:self.view].y);
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    if ((gestureRecognizer == _tap && otherGestureRecognizer == _pan) ||
        (gestureRecognizer == _pan && otherGestureRecognizer == _tap)) {
        return NO;
    }
    return YES;
}

- (void)onPinch:(UIPinchGestureRecognizer*)gestureRecognizer
{
    if (!_pinchDelegate)
        return;
    
    _pinchDelegate->ProceedPinch(gestureRecognizer.scale);
    gestureRecognizer.scale = 1;
}

- (void)onPan:(UIPanGestureRecognizer*)gestureRecognizer
{
    if (!_pinchDelegate)
        return;
    
    CGPoint translation = [gestureRecognizer translationInView:self.view];
    
    [gestureRecognizer setTranslation:CGPointMake(0, 0) inView:self.view];
    _pinchDelegate->ProceedPan(translation.x, translation.y);
}

- (void)onTap:(UITapGestureRecognizer*)gestureRecognizer
{
    if (!_pinchDelegate)
        return;
    if (!_canhandleTap)
        return;
    _canhandleTap = NO;
    NSTimeInterval now = [[NSDate dateWithTimeIntervalSinceNow:0] timeIntervalSince1970];
    if (now == lastTapTime)
        return;
    
    lastTapTime = now;
    
    CGPoint translation = [gestureRecognizer locationInView:self.view];
    _pinchDelegate->ProceedTap(translation.x, translation.y);
  //  [self touchesEnded:_touches withEvent:_event];
}

- (void)onLongTap:(UIPanGestureRecognizer*)gestureRecognizer
{
    if (!_pinchDelegate)
        return;
    if (!_canhandleTap) 
        return;
    _canhandleTap = NO;
    CGPoint translation = [gestureRecognizer locationInView:self.view];
    _pinchDelegate->ProceedLongTap(translation.x, translation.y);
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)setPinchDelegate:(DisplayPinchDelegate*)delegate
{
    _pinchDelegate = delegate;
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return UIInterfaceOrientationIsLandscape(toInterfaceOrientation);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    _touches = touches;
    _event = event;
    _canhandleTap = YES;
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
