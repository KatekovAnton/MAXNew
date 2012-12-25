//
//  Sys.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/15/12.
//
//

#ifndef TerminalExtraction_Sys_h
#define TerminalExtraction_Sys_h

#include <lcl.h>

#undef lcl_log
#define lcl_log  _lcl_logger

#define SysLogComponent lcl_cSys_

#define SysLogCritical(...)                                                              \
lcl_log(SysLogComponent, lcl_vCritical, __VA_ARGS__)

#define SysLogError(...)                                                                 \
lcl_log(SysLogComponent, lcl_vError, __VA_ARGS__)

#define SysLogWarning(...)                                                               \
lcl_log(SysLogComponent, lcl_vWarning, __VA_ARGS__)

#define SysLogInfo(...)                                                                  \
lcl_log(SysLogComponent, lcl_vInfo, __VA_ARGS__)

#define SysLogDebug(...)                                                                 \
lcl_log(SysLogComponent, lcl_vDebug, __VA_ARGS__)

#define SysLogTrace(...)                                                                 \
lcl_log(SysLogComponent, lcl_vTrace, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif


void SysInit();
    
#ifdef __cplusplus
}
#endif

#define SysLogLevelOff       lcl_vOff
#define SysLogLevelCritical  lcl_vCritical
#define SysLogLevelError     lcl_vError
#define SysLogLevelWarning   lcl_vWarning
#define SysLogLevelInfo      lcl_vInfo
#define SysLogLevelDebug     lcl_vDebug
#define SysLogLevelTrace     lcl_vTrace

#define SysLogConfigureByName(name, level)                                               \
lcl_configure_by_name(name, level);

#endif
