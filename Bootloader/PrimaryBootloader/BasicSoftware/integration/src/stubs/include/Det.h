#ifndef DET_H
#define DET_H

#include "Std_Types.h"


#define DET_AR_RELEASE_MAJOR_VERSION    4
#define DET_AR_RELEASE_MINOR_VERSION    2
/**
 * @ingroup API
 *
 * Det_ReportError      Service to report development errors.
 *
 * @param ModuleId      Module ID of calling module.
 * @param InstanceId    The identifier of the index based instance of a module, starting from 0.
 *                      If the module is a single instance module it shall pass 0 as the InstanceId.
 * @param ApiId         ID of API service in which error is detected (defined in SWS of calling module).
 * @param ErrorId       ID of detected development error (defined in SWS of calling module).
 * @return              returns always E_OK (is required for services).
 */
extern Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);
/**
 * @brief
 *
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportRuntimeError(  VAR(uint16, AUTOMATIC) ModuleId,
                                                        VAR(uint8, AUTOMATIC) InstanceId,
                                                        VAR(uint8, AUTOMATIC) ApiId,
                                                        VAR(uint8, AUTOMATIC) ErrorId);
#endif
