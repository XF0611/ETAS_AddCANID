/*
 * @Descripttion: 
 * @version: 
 * @Author: calterah
 * @Date: 2022-03-20 16:11:17
 * @LastEditTime: 2022-04-21 11:51:41
 */
#ifndef CDD_FUSA_CFG_H
#define CDD_FUSA_CFG_H

//#define FUNC_SAFETY
//#ifndef FUNC_SAFETY
//#error "FUNC SAFETY Is Not ON!"
//#else
#define FEATURE_ON          1
#define FEATURE_OFF         0

#define FEATURE_DEFAULT           FEATURE_OFF
/* FUSA CLI switch*/
#define FUSA_CLI                  FEATURE_ON
#define FUSA_DEBUG                FEATURE_ON
/* FUSA items switch*/
#define FUSA_FEATURE_SM1          FEATURE_ON
#define FUSA_FEATURE_SM2          FEATURE_ON
#define FUSA_FEATURE_SM3          FEATURE_ON
#define FUSA_FEATURE_SM4          FEATURE_ON
#define FUSA_FEATURE_SM5          FEATURE_ON
#define FUSA_FEATURE_SM6          FEATURE_ON
#define FUSA_FEATURE_SM8          FEATURE_ON //depend on AGC,if opened, agc_mode must be 1 or 2
#define FUSA_FEATURE_SM11         FEATURE_ON
#define FUSA_FEATURE_SM12         FEATURE_ON
#define FUSA_FEATURE_SM13         FEATURE_ON
#define FUSA_FEATURE_SM14         FEATURE_ON
#define FUSA_FEATURE_SM101        FEATURE_ON
#define FUSA_FEATURE_SM102        FEATURE_ON
#define FUSA_FEATURE_SM103        FEATURE_ON
#define FUSA_FEATURE_SM104        FEATURE_ON //sm104 and sm105 combine
#define FUSA_FEATURE_SM106        FEATURE_ON
#define FUSA_FEATURE_SM107        FEATURE_ON
#define FUSA_FEATURE_SM108        FEATURE_ON
#define FUSA_FEATURE_SM109        FEATURE_ON
#define FUSA_FEATURE_SM120        FEATURE_DEFAULT //
#define FUSA_FEATURE_SM121        FEATURE_DEFAULT //
#define FUSA_FEATURE_SM126        FEATURE_DEFAULT //
#define FUSA_FEATURE_SM130        FEATURE_ON
#define FUSA_FEATURE_SM133        FEATURE_ON
#define FUSA_FEATURE_SM201        FEATURE_ON
#define FUSA_FEATURE_SM901        FEATURE_ON
#define FUSA_FEATURE_SM902        FEATURE_ON
#define FUSA_FEATURE_SM904        FEATURE_ON
#define FUSA_FEATURE_SM905        FEATURE_ON
#define FUSA_FEATURE_SM906        FEATURE_ON
#define FUSA_FEATURE_SM907        FEATURE_ON
#define FUSA_FEATURE_SM908        FEATURE_ON
#define FUSA_FEATURE_SM910        FEATURE_ON
#define FUSA_FEATURE_SM911        FEATURE_ON


/******************************************************************************************/
/******************************************************************************************/
#if (FUSA_FEATURE_SM109 == FEATURE_ON)
#define FUSA_FEATURE_CAN0         FEATURE_ON
#define FUSA_FEATURE_CAN1         FEATURE_ON
#endif
/******************************************************************************************/
/******************************************************************************************/
#define FUSA_MECHANISM_POWER_ON_CHECK_EN        ((FUSA_FEATURE_SM901 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM902 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM904 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM905 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM906 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM907 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM908 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM911 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM922 == FEATURE_ON))

#define FUSA_PERIODIC_RUN_ITEMS_EN              ((FUSA_FEATURE_SM1   == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM6   == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM11  == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM12  == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM13  == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM14  == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM101 == FEATURE_ON) || \
                                                 (FUSA_FEATURE_SM201 == FEATURE_ON))

//#endif

#endif /* CDD_FUSA_CFG_H */
