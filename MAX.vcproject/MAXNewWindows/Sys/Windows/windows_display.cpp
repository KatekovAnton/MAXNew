#include "windows_display.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "glext.h"
#include <windows.h>		// Header File For Windows

static windows_display* shared_display_instance = NULL;
void GetWindowPos(HWND hWnd, int *x, int *y)
{
    HWND hWndParent = GetParent(hWnd);

    RECT parentScreenRect;
    RECT itemScreenRect;
    GetWindowRect(hWndParent, &parentScreenRect);
    GetWindowRect(hWnd, &itemScreenRect);

    (*x) = itemScreenRect.left - parentScreenRect.left;
    (*y) = itemScreenRect.top - parentScreenRect.top;
}

windows_display::windows_display(float _w, float _h, HDC _hDC, HWND _hWND, DisplayPinchDelegate **_actionDelegate)
	:_mouseDown(false), _mouseDownToCocos(false)
{
    w = _w;
	h = _h;

	hDC = _hDC;
	hWND= _hWND;
	this->_delegate = _actionDelegate;
}

windows_display::~windows_display(void)
{
}

void windows_display::SetSharedDisplay(windows_display* _display)
{
	shared_display_instance = _display;
}

windows_display* windows_display::GetSharedDisplay()
{
	return shared_display_instance;
}

windows_display::windows_display()
{
}

void windows_display::setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy)
{
	this->setFrameSize(width, height);
	Display::setDesignResolutionSize(width, height, resolutionPolicy);
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

void windows_display::SetPinchDelegate(DisplayPinchDelegate* _newDelegate) {
	*this->_delegate = _newDelegate;
}

void windows_display::ProceedMessage(MSG msg)
{
	POINT position;
	
	GetCursorPos(&position);
	ScreenToClient(hWND, &position);

	DisplayPinchDelegate* _delegateC = *_delegate;
	bool lastMouseFlag = _mouseDown;
	if (msg.message == WM_KEYDOWN)
	{
		int a = 0;
		a++;
		if (msg.wParam == 107)
			_delegateC->ProceedPinch(1.1);
		if (msg.wParam == 109)
			_delegateC->ProceedPinch(0.9);


		
		if (msg.wParam == 37)
			_delegateC->ProceedPan(20, 0);
		if (msg.wParam == 38)
			_delegateC->ProceedPan(0, 20);
		if (msg.wParam == 39)
			_delegateC->ProceedPan(-20, 0);
		if (msg.wParam == 40)
			_delegateC->ProceedPan(0, -20);
		
	}
	else if (msg.message == WM_LBUTTONDOWN && !_mouseDown)
	{
		SetCapture(hWND);
		if (_delegateC->CanStartPinch(position.x,position.y))
		{
			_mouseDown = true;
			_mouseDownPoint = position;
		}
		else
		{
			_mouseDownToCocos = true;
			//to cocos
			int ids[1] = {0};
			float xs[1] = {position.x};
			float ys[1] = {position.y};
			this->handleTouchesBegin(1, ids, xs, ys); 
		}
	}
	if (msg.message == WM_LBUTTONUP)
	{
		if (_mouseDown)
		{
			_mouseDown = false;
			if (_mouseDownPoint.x == position.x && _mouseDownPoint.y == position.y)
				_delegateC->ProceedTap(position.x, position.y);
		}
		else if (_mouseDownToCocos)
		{
			_mouseDownToCocos = false;
			//to cocos
			int ids[1] = {0};
			float xs[1] = {position.x};
			float ys[1] = {position.y};
			this->handleTouchesEnd(1, ids, xs, ys); 
		}
		ReleaseCapture();
	}
	if (msg.message == WM_MOUSEMOVE )
	{
		if (_mouseDown)
		{
			POINT delta;
			delta.x = 0;
			delta.y = 0;
			if (lastMouseFlag)
			{
				delta.x = _mousePointLast.x - position.x;
				delta.y = _mousePointLast.y - position.y;
			}
			_delegateC->ProceedPan(-delta.x, -delta.y);
		}
		else if (_mouseDownToCocos)
		{
			//to cocos
			int ids[1] = {0};
			float xs[1] = {position.x};
			float ys[1] = {position.y};
			this->handleTouchesMove(1, ids, xs, ys); 
		}
		_delegateC->onDeviceMoved(-(position.x / m_obScreenSize.width - 0.5) * 0.7f, (position.y / m_obScreenSize.height - 0.5) * 0.7f);
	}
	if (msg.message == WM_RBUTTONUP && _delegateC->CanStartPinch(position.x,position.y))
	{
		_delegateC->ProceedLongTap(position.x, position.y);
	}
	if (msg.message == 522 && _delegateC->CanStartPinch(position.x,position.y))
	{
		short t = GET_WHEEL_DELTA_WPARAM(msg.wParam);
		if (_delegate)
			_delegateC->ProceedPinch(t>0?1.1:0.9);
	}
	_mousePointLast = position;
}