#ifndef STUB_H
#define STUB_H

#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "arc_exception.h"
//#include "dw_timer.h"
//#include "timer_hal.h"
#include "arc_timer.h"
#include "arc.h"
#include "arc_builtin.h"
//#include "arc_wdg.h"


#ifdef OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

//#include "Gpt.h"
//#include "Gpt_Alps.h"

#define ISR_INSTALL_ISR2(_name, _entry, _vector, _priority)\
        do {\
			int_handler_install( _vector, (void *) _entry);\
		}while(0);
		

void NotifCh0 (void);
void NotifCh1 (void);
void NotifCh2 (void);
void installChannelInterrupt(uint8_t Chnl);

#endif


