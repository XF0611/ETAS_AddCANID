#ifndef  COMMON_ALPS_H
#define  COMMON_ALPS_H

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef signed char	int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;

#include "arc_builtin.h"
#include <stdint.h>

typedef void (*exc_entry)(void);

#define PNULL   (0)

#ifdef __GNU__
#define arc_lr_reg(aux)         __builtin_arc_lr(aux)
#define arc_sr_reg(aux, val)    asm volatile ("sr %0, [%1]\n": : "ir"(val), "r"(aux))
#else
#define arc_lr_reg(aux)         _lr(aux)
#define arc_sr_reg(aux, val)    _sr(val, aux)
#endif

//static inline void raw_writel(uint32_t addr, uint32_t val)
//{
//    *(volatile uint32_t *)(addr) = val;
//}
//
//static inline uint32_t raw_readl(uint32_t addr)
//{
//    return *(volatile uint32_t *)(addr);
//}

static inline void raw_clear(uint32_t addr, uint32_t bits)
{
    uint32_t reg = raw_readl(addr);
    reg &= ~bits;
    raw_writel(addr, reg);
}

static inline void raw_set(uint32_t addr, uint32_t bits)
{
    uint32_t reg = raw_readl(addr);
    reg |= bits;
    raw_writel(addr, reg);
}

static inline void raw_or(uint32_t addr, uint32_t bits, uint32_t mask)
{
    uint32_t reg = raw_readl(addr);
    reg &= ~mask;
    reg |= bits;
    raw_writel(addr, reg);
}

static inline void raw_and(uint32_t addr, uint32_t bits)
{
    uint32_t reg = raw_readl(addr);
    reg &= bits;
    raw_writel(addr, reg);
}

/*
 * MCU frequency is 400MHz,
 *
 * label: add r0, #1
 * 		  cmp r0, r1
 * 		  b.lt label
 * a loop approximates to 4 machine cycles, about 10ns.
 * */
static inline void __loop_delay(uint32_t us)
{
    uint32_t i;
    for (i = 0; i < us * 120; i++);
}

#endif /* COMMON_ALPS_H */
