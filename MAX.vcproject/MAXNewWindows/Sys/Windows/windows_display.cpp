#include "windows_display.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "glext.h"

static windows_display* shared_display_instance = NULL;

void windows_display::SetSharedDisplay(windows_display* _display)
{
	shared_display_instance = _display;
}

windows_display* windows_display::GetSharedDisplay()
{
	return shared_display_instance;
}

windows_display::windows_display(void)
{
}

void windows_display::setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy)
{
	this->setFrameSize(width, height);
	Display::setDesignResolutionSize(width, height, resolutionPolicy);
}

windows_display::~windows_display(void)
{
}

windows_display::windows_display(float _w, float _h, HDC _hDC)
{
    w = _w;
	h = _h;

	hDC = _hDC;
}

void windows_display::InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat) {
    
   
}

GLfloat windows_display::GetDisplayHeight() {
    float result = h;
    return result;
}

GLfloat windows_display::GetDisplayWidth() {
    float result = w;
    return result;
}

GLfloat windows_display::GetDisplayScale()
{
    return 1.0;
}

void swapOglBuffers(HDC _p)
{
	SwapBuffers(_p);
}

void windows_display::SwapBuffers() {
	swapOglBuffers(hDC);
}

void windows_display::swapBuffers() {
    //SwapBuffers();
}

bool windows_display::isOpenGLReady() {
    return true;
}

bool windows_display::setContentScaleFactor(float contentScaleFactor) {
    return true;
}

void windows_display::end() {
    // do nothing;
}

void windows_display::setIMEKeyboardState(bool bOpen) {
    
}

void windows_display::SetPinchDelegate(DisplayPinchDelegate* _delegate) {
   
}