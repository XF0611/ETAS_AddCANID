#include "Gpt.h"
#include "Gpt_Alps.h"
#include "stub.h"

#include <stdint.h>


void NotifCh0 (void)
{

}

void NotifCh1 (void)
{

}

void NotifCh2 (void)
{

}

/* ETAS BIP Porting
void installChannelInterrupt(uint8_t Chnl)
{
    switch (Chnl) {
        case GPT_HW_TIMER2:
            //Install ISR. IRQ ID#36 Timer 1 Interrupt 1
            ISR_INSTALL_ISR2("Gpt", Gpt1_Isr, (IRQ_TIMER1),GPT_ISR_PRIORITY);
            break;

        case GPT_HW_TIMER3:
            //Install ISR. IRQ ID#37 Timer 2 Interrupt 2
            ISR_INSTALL_ISR2("Gpt", Gpt2_Isr, (IRQ_TIMER2),GPT_ISR_PRIORITY);
            break;

        case GPT_HW_TIMER4:
            //Install ISR. IRQ ID#38 Timer 3 Interrupt 3
            ISR_INSTALL_ISR2("Gpt", Gpt3_Isr, (IRQ_TIMER3),GPT_ISR_PRIORITY);
            break;
    }
}
*/
