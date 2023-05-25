#include "Det.h"

uint8 errId;
uint32 cnt = 0;
Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    (void)ModuleId;             /* PRQA S 3112 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
    (void)InstanceId;           /* PRQA S 3112 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
    (void)ApiId;                /* PRQA S 3112 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
    (void)ErrorId;              /* PRQA S 3112 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
    errId = ErrorId;

    return E_OK;
}
FUNC(Std_ReturnType, DET_CODE) Det_ReportRuntimeError(VAR(  uint16, AUTOMATIC) ModuleId,                /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
                                                            VAR(uint8, AUTOMATIC) InstanceId,           /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
                                                            VAR(uint8, AUTOMATIC) ApiId,                /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
                                                            VAR(uint8, AUTOMATIC) ErrorId)              /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
{
	Std_ReturnType retVal = E_OK;
    volatile uint8 debug_exit = 0;

    while (debug_exit == 1u) {}

    return retVal;
}
