//
//  Sys.c
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/15/12.
//
//

#include <stdio.h>
#include "stdarg.h"
#ifdef TARGET_OS_IPHONE
#include <strings.h>
#endif
#include "Sys.h"
#include "GlobalConstants.h"


#define TE_VERSION          0.1


void SysInit(void) {
#ifdef TARGET_OS_IPHONE
    lcl_configure_by_name("Sys_*", SysLogLevelDebug);
    
    SysLogInfo("ENGINE_VERSION: terminal-extraction-%f", TE_VERSION);
    SysLogInfo("GL_VENDOR:     %s", glGetString(GL_VENDOR));
    SysLogInfo("GL_RENDERER:   %s", glGetString(GL_RENDERER));
    SysLogInfo("GL_VERSION:    %s", glGetString(GL_VERSION));
    
    SysLogInfo("Engine initialized ...");
#endif
}

