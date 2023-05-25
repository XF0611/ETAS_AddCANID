#ifndef SCHM_SPI_H
#define SCHM_SPI_H

#include "Os.h"

#define SchM_Enter_Spi_EA_0() SuspendOSInterrupts()
#define SchM_Exit_Spi_EA_0() ResumeOSInterrupts()

#define SchM_Enter_Spi_SyncTransmit() SchM_Enter_Spi_EA_0()
#define SchM_Exit_Spi_SyncTransmit() SchM_Exit_Spi_EA_0()
#define SchM_Enter_Spi_AsyncTransmit() SchM_Enter_Spi_EA_0()
#define SchM_Exit_Spi_AsyncTransmit() SchM_Exit_Spi_EA_0()
#define SchM_Enter_Spi_MainFunction() SchM_Enter_Spi_EA_0()
#define SchM_Exit_Spi_MainFunction() SchM_Exit_Spi_EA_0()
#define SchM_Enter_Spi_WriteIB() SchM_Enter_Spi_EA_0()
#define SchM_Exit_Spi_WriteIB() SchM_Exit_Spi_EA_0()
#endif /* SCHM_SPI_H */
