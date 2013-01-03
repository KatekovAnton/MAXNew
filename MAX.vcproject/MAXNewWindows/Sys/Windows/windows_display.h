#pragma once
#include "display.h"
#include <windows.h>
class windows_display :
	public Display
{

	float w;
	float h;
	HDC			hDC;
public:
	
	windows_display(float _w, float _h, HDC _hDC);
	windows_display(void);
	~windows_display(void);

	static void SetSharedDisplay(windows_display* display);
	static windows_display* GetSharedDisplay();

	virtual void InitGLSurface(unsigned int depthFormat, unsigned int pixelFormat);
    
    virtual GLfloat GetDisplayHeight();
    virtual GLfloat GetDisplayWidth();
    virtual GLfloat GetDisplayScale();
    
    virtual void    SwapOGLBuffers();
    
    virtual bool    isOpenGLReady();
    virtual bool    setContentScaleFactor(float contentScaleFactor);
    
    // keep compatible
    virtual void    end();
    virtual void    swapBuffers();
    
    virtual void setIMEKeyboardState(bool bOpen);
    virtual void SetPinchDelegate(DisplayPinchDelegate* _delegate);

	const CCSize& getDesignResolutionSize() const;
	virtual void setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy);
};

