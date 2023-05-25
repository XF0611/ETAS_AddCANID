

#ifndef MFL_CFG_H
#define MFL_CFG_H

/*
 **********************************************************************************************************************
 * GetVersionInfo
 **********************************************************************************************************************
 */

/* Availability and activity of GetVersionInfo function */
#define MFL_VERSIONINFOAPI           STD_OFF

/*
 **********************************************************************************************************************
 * Memory Configuration
 **********************************************************************************************************************
 */

/* Macro for code location           */
/* Do not change these macros        */
#define MFL_CFG_LOCATION_NORMAL      1
#define MFL_CFG_LOCATION_FAST        2
#define MFL_CFG_LOCATION_SLOW        3
#define MFL_CFG_LOCATION_INLINE      4

/*
 **********************************************************************************************************************
 * Switches for Optimizations 
 **********************************************************************************************************************
 */

/* Defines for optimization switches */
/* Do not change these macros        */
#define MFL_CFG_NOOPTIMIZATION       0
#define MFL_CFG_TCCOMMON             1
#define MFL_CFG_TC27XX               2
#define MFL_CFG_MPCCOMMON            3
#define MFL_CFG_MPCCUT2              4

/* Activation of specific optimization */
#define MFL_CFG_OPTIMIZATION          MFL_CFG_NOOPTIMIZATION
/*
 **********************************************************************************************************************
 * For usage of HW Rounding Mode
 **********************************************************************************************************************
 */
 
#define MFL_CFG_OFF     0 
#define MFL_CFG_ON     1

/* Choosing HW Rounding Mode */
#define MFL_CFG_HW_ROUNDING          MFL_CFG_OFF

/*********************************************************************************************************************/
/* Modification of location of services                              */
/* values:     MFL_CFG_LOCATION_NORMAL                               */
/*                                 -  Location in normal flash       */
/*             MFL_CFG_LOCATION_FAST                                 */
/*                                 -  Location in fast flash         */
/*             MFL_CFG_LOCATION_SLOW                                 */
/*                                 -  Location in slow flash         */
/*             MFL_CFG_LOCATION_INLINE                               */
/*                                 -  Service is inlined             */

#define MFL_CFG_ABS_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ARCCOS_F32_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ARCSIN_F32_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ARCTAN_F32_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ARCTAN2_F32_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ARRAYAVERAGE_F32_F32_LOCATION              MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_AVERAGE_F32_F32_LOCATION                   MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CALCTEQ_F32_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CALCTEQAPP_F32_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CEIL_F32_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_COS_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CTRLSETLIMIT_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_F32_S16_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_F32_S32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_F32_U16_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_F32_U32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_S16_F32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_S32_F32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_U16_F32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_CVRT_U32_F32_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DEADTIME_F32_F32_LOCATION                  MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DEBOUNCE_U8_U8_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DEBOUNCEINIT_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DEBOUNCESETPARAM_LOCATION                  MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP1CALC_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP1OUT_F32_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP1SETSTATE_LOCATION                   MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP2CALC_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP2OUT_F32_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_DT1TYP2SETSTATE_LOCATION                   MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_EXP_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_S16_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_S32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_S8_LOCATION             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_U16_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOATTOINTCVRT_F32_U8_LOCATION             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_FLOOR_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_HYPOT_F32F32_F32_LOCATION                  MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_HYSTCENTERHALFDELTA_F32_U8_LOCATION        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_HYSTDELTARIGHT_F32_U8_LOCATION             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION              MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_HYSTLEFTRIGHT_F32_U8_LOCATION              MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ICALC_LOCATION                             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ILIMCALC_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_S16_F32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_S8_F32_LOCATION             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_U16_F32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_U32_F32_LOCATION            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_IOUT_F32_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ISETSTATE_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_LIMIT_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_LOG_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_MAX_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_MEDIANSORT_F32_F32_LOCATION                MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_MIN_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PCALC_LOCATION                             MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PDCALC_LOCATION                            MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PDOUT_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PDSETPARAM_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PDSETSTATE_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDOUT_F32_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDSETPARAM_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDSETSTATE_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDTYP1CALC_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDTYP1LIMCALC_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDTYP2CALC_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIDTYP2LIMCALC_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PIOUT_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PISETPARAM_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PISETSTATE_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PITYP1CALC_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PITYP1LIMCALC_LOCATION                     MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PITYP2CALC_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PITYP2LIMCALC_LOCATION                     MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_POUT_F32_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_POW_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PT1CALC_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PT1OUT_F32_LOCATION                        MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_PT1SETSTATE_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPCALC_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPCALCJUMP_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPCALCSWITCH_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPCHECKACTIVITY_LOCATION                 MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPGETSWITCHPOS_LOCATION                  MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPINITSTATE_LOCATION                     MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPOUT_F32_LOCATION                       MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RAMPSETPARAM_LOCATION                      MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_RATELIMITER_F32_LOCATION                   MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_ROUND_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_SIGN_F32_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_SIN_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_SORTASCEND_F32_LOCATION                    MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_SORTDESCEND_F32_LOCATION                   MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_SQRT_F32_LOCATION                          MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_TAN_F32_LOCATION                           MFL_CFG_LOCATION_NORMAL
#define MFL_CFG_TRUNC_F32_LOCATION                         MFL_CFG_LOCATION_NORMAL

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

 /* Patch to speed up the run of QAC tool */
#if (!defined(__QACDF__) || (defined(__QACDF__) && defined(SRVLIBS)))

/* Normal code section ***********************************************************************************************/

#define MFL_START_SEC_CODE
#include "Mfl_MemMap.h"
    extern float32 Mfl_Abs_f32(float32 ValValue);
    extern float32 Mfl_ArcCos_f32(float32 value);
    extern float32 Mfl_ArcSin_f32(float32 value);
    extern float32 Mfl_ArcTan_f32(float32 value);
    extern float32 Mfl_ArcTan2_f32(float32 x, float32 y);
    extern float32 Mfl_ArrayAverage_f32_f32(const float32* Array, uint32 Count);
    extern float32 Mfl_Average_f32_f32(float32 value1, float32 value2);
    extern float32 Mfl_CalcTeQ_f32(float32 T1rec_f32, float32 dT_f32);
    extern float32 Mfl_CalcTeQApp_f32(float32 T1rec_f32, float32 dT_f32);
    extern float32 Mfl_Ceil_f32(float32 ValValue);
    extern float32 Mfl_Cos_f32(float32 value);
    extern void Mfl_CtrlSetLimit(Mfl_Limits_Type* Limit_cpst, float32 Min_f32, float32 Max_f32);
    extern sint16 Mfl_Cvrt_f32_s16(float32 ValFloat, sint16 ValFixedExponent);
    extern sint32 Mfl_Cvrt_f32_s32(float32 ValFloat, sint16 ValFixedExponent);
    extern uint16 Mfl_Cvrt_f32_u16(float32 ValFloat, sint16 ValFixedExponent);
    extern uint32 Mfl_Cvrt_f32_u32(float32 ValFloat, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_s16_f32(sint16 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_s32_f32(sint32 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_u16_f32(uint16 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_u32_f32(uint32 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_DeadTime_f32_f32(float32 X, float32 DelayTime, float32 StepTime, Mfl_DeadTimeParam_Type* Param);
    extern boolean Mfl_Debounce_u8_u8(boolean X, Mfl_DebounceState_Type* State, Mfl_DebounceParam_Type* Param, float32 dT);
    extern void Mfl_DebounceInit(Mfl_DebounceState_Type* State, boolean X);
    extern void Mfl_DebounceSetParam(Mfl_DebounceParam_Type* Param, float32 THighLow, float32 TLowHigh);
    extern void Mfl_DT1Typ1Calc(float32 X_f32, Mfl_StateDT1Typ1_Type* State_cpst, float32 K_f32, float32 TeQ_f32, float32 dT_f32);
    extern float32 Mfl_DT1Typ1Out_f32(const Mfl_StateDT1Typ1_Type* State_cpst);
    extern void Mfl_DT1Typ1SetState(Mfl_StateDT1Typ1_Type* State_cpst, float32 X1_f32, float32 X2_f32, float32 Y1_f32);
    extern void Mfl_DT1Typ2Calc(float32 X_f32, Mfl_StateDT1Typ2_Type* State_cpst, float32 K_f32, float32 TeQ_f32, float32 dT_f32);
    extern float32 Mfl_DT1Typ2Out_f32(const Mfl_StateDT1Typ2_Type* State_cpst);
    extern void Mfl_DT1Typ2SetState(Mfl_StateDT1Typ2_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_Exp_f32(float32 ValValue);
    extern sint16 Mfl_FloatToIntCvrt_f32_s16(float32 ValFloat);
    extern sint32 Mfl_FloatToIntCvrt_f32_s32(float32 ValFloat);
    extern sint8 Mfl_FloatToIntCvrt_f32_s8(float32 ValFloat);
    extern uint16 Mfl_FloatToIntCvrt_f32_u16(float32 ValFloat);
    extern uint32 Mfl_FloatToIntCvrt_f32_u32(float32 ValFloat);
    extern uint8 Mfl_FloatToIntCvrt_f32_u8(float32 ValFloat);
    extern float32 Mfl_Floor_f32(float32 ValValue);
    extern float32 Mfl_Hypot_f32f32_f32(float32 x_value, float32 y_value);
    extern boolean Mfl_HystCenterHalfDelta_f32_u8(float32 X, float32 center, float32 halfDelta, uint8* State);
    extern boolean Mfl_HystDeltaRight_f32_u8(float32 X, float32 Delta, float32 Rsp, uint8* State);
    extern boolean Mfl_HystLeftDelta_f32_u8(float32 X, float32 Lsp, float32 Delta, uint8* State);
    extern boolean Mfl_HystLeftRight_f32_u8(float32 X, float32 Lsp, float32 Rsp, uint8* State);
    extern void Mfl_ICalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, float32 dT_f32);
    extern void Mfl_ILimCalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_IntToFloatCvrt_s16_f32(sint16 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_s32_f32(sint32 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_s8_f32(sint8 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u16_f32(uint16 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u32_f32(uint32 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u8_f32(uint8 ValInteger);
    extern float32 Mfl_IOut_f32(const Mfl_StateI_Type* State_cpst);
    extern void Mfl_ISetState(Mfl_StateI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_Limit_f32(float32 val, float32 lowLim, float32 upLim);
    extern float32 Mfl_Log_f32(float32 ValValue);
    extern float32 Mfl_Max_f32(float32 Value1, float32 Value2);
    extern float32 Mfl_MedianSort_f32_f32(float32* Array, uint8 N);
    extern float32 Mfl_Min_f32(float32 Value1, float32 Value2);
    extern void Mfl_PCalc(float32 X_f32, float32* P_pf32, float32 K_f32);
    extern void Mfl_PDCalc(float32 X_f32, Mfl_StatePD_Type* State_cpst, const Mfl_ParamPD_Type* Param_cpst, float32 dT_f32);
    extern float32 Mfl_PDOut_f32(const Mfl_StatePD_Type* State_cpst);
    extern void Mfl_PDSetParam(Mfl_ParamPD_Type* Param_cpst, float32 K_f32, float32 Tv_f32);
    extern void Mfl_PDSetState(Mfl_StatePD_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_PIDOut_f32(const Mfl_StatePID_Type* State_cpst);
    extern void Mfl_PIDSetParam(Mfl_ParamPID_Type* Param_cpst, float32 K_f32, float32 Tv_f32, float32 Tnrec_f32);
    extern void Mfl_PIDSetState(Mfl_StatePID_Type* State_cpst, float32 X1_f32, float32 X2_f32, float32 Y1_f32);
    extern void Mfl_PIDTyp1Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp1LimCalc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp2Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp2LimCalc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_PIOut_f32(const Mfl_StatePI_Type* State_cpst);
    extern void Mfl_PISetParam(Mfl_ParamPI_Type* Param_cpst, float32 K_f32, float32 Tnrec);
    extern void Mfl_PISetState(Mfl_StatePI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern void Mfl_PITyp1Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PITyp1LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern void Mfl_PITyp2Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PITyp2LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_POut_f32(const float32* P_pf32);
    extern float32 Mfl_Pow_f32(float32 ValBase, float32 ValExp);
    extern void Mfl_PT1Calc(float32 X_f32, Mfl_StatePT1_Type* State_cpst, float32 K_f32, float32 TeQ_f32);
    extern float32 Mfl_PT1Out_f32(const Mfl_StatePT1_Type* State_cpst);
    extern void Mfl_PT1SetState(Mfl_StatePT1_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern void Mfl_RampCalc(float32 X_f32, Mfl_StateRamp_Type* State_cpst, const Mfl_ParamRamp_Type* Param_cpcst, float32 dT_f32);
    extern void Mfl_RampCalcJump(float32 X_f32, Mfl_StateRamp_Type* State_cpst);
    extern float32 Mfl_RampCalcSwitch(float32 Xa_f32, float32 Xb_f32, boolean Switch_B, Mfl_StateRamp_Type* State_cpst);
    extern boolean Mfl_RampCheckActivity(Mfl_StateRamp_Type* State_cpst);
    extern boolean Mfl_RampGetSwitchPos(Mfl_StateRamp_Type* State_cpst);
    extern void Mfl_RampInitState(Mfl_StateRamp_Type* State_cpst, float32 Val_f32);
    extern float32 Mfl_RampOut_f32(const Mfl_StateRamp_Type* State_cpcst);
    extern void Mfl_RampSetParam(Mfl_ParamRamp_Type* Param_cpst, float32 SlopePosVal_f32, float32 SlopeNegVal_f32);
    extern float32 Mfl_RateLimiter_f32(float32 newval, float32 oldval, float32 maxdif);
    extern float32 Mfl_Round_f32(float32 ValValue);
    extern sint8 Mfl_Sign_f32(float32 ValValue);
    extern float32 Mfl_Sin_f32(float32 value);
    extern void Mfl_SortAscend_f32(float32* Array, uint16 Num);
    extern void Mfl_SortDescend_f32(float32* Array, uint16 Num);
    extern float32 Mfl_Sqrt_f32(float32 ValValue);
    extern float32 Mfl_Tan_f32(float32 value);
    extern float32 Mfl_Trunc_f32(float32 ValValue);
#define MFL_STOP_SEC_CODE
#include "Mfl_MemMap.h"

#else
    extern float32 Mfl_Abs_f32(float32 ValValue);
    extern float32 Mfl_ArcCos_f32(float32 value);
    extern float32 Mfl_ArcSin_f32(float32 value);
    extern float32 Mfl_ArcTan_f32(float32 value);
    extern float32 Mfl_ArcTan2_f32(float32 x, float32 y);
    extern float32 Mfl_ArrayAverage_f32_f32(const float32* Array, uint32 Count);
    extern float32 Mfl_Average_f32_f32(float32 value1, float32 value2);
    extern float32 Mfl_CalcTeQ_f32(float32 T1rec_f32, float32 dT_f32);
    extern float32 Mfl_CalcTeQApp_f32(float32 T1rec_f32, float32 dT_f32);
    extern float32 Mfl_Ceil_f32(float32 ValValue);
    extern float32 Mfl_Cos_f32(float32 value);
    extern void Mfl_CtrlSetLimit(Mfl_Limits_Type* Limit_cpst, float32 Min_f32, float32 Max_f32);
    extern sint16 Mfl_Cvrt_f32_s16(float32 ValFloat, sint16 ValFixedExponent);
    extern sint32 Mfl_Cvrt_f32_s32(float32 ValFloat, sint16 ValFixedExponent);
    extern uint16 Mfl_Cvrt_f32_u16(float32 ValFloat, sint16 ValFixedExponent);
    extern uint32 Mfl_Cvrt_f32_u32(float32 ValFloat, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_s16_f32(sint16 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_s32_f32(sint32 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_u16_f32(uint16 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_Cvrt_u32_f32(uint32 ValFixedInteger, sint16 ValFixedExponent);
    extern float32 Mfl_DeadTime_f32_f32(float32 X, float32 DelayTime, float32 StepTime, Mfl_DeadTimeParam_Type* Param);
    extern boolean Mfl_Debounce_u8_u8(boolean X, Mfl_DebounceState_Type* State, Mfl_DebounceParam_Type* Param, float32 dT);
    extern void Mfl_DebounceInit(Mfl_DebounceState_Type* State, boolean X);
    extern void Mfl_DebounceSetParam(Mfl_DebounceParam_Type* Param, float32 THighLow, float32 TLowHigh);
    extern void Mfl_DT1Typ1Calc(float32 X_f32, Mfl_StateDT1Typ1_Type* State_cpst, float32 K_f32, float32 TeQ_f32, float32 dT_f32);
    extern float32 Mfl_DT1Typ1Out_f32(const Mfl_StateDT1Typ1_Type* State_cpst);
    extern void Mfl_DT1Typ1SetState(Mfl_StateDT1Typ1_Type* State_cpst, float32 X1_f32, float32 X2_f32, float32 Y1_f32);
    extern void Mfl_DT1Typ2Calc(float32 X_f32, Mfl_StateDT1Typ2_Type* State_cpst, float32 K_f32, float32 TeQ_f32, float32 dT_f32);
    extern float32 Mfl_DT1Typ2Out_f32(const Mfl_StateDT1Typ2_Type* State_cpst);
    extern void Mfl_DT1Typ2SetState(Mfl_StateDT1Typ2_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_Exp_f32(float32 ValValue);
    extern sint16 Mfl_FloatToIntCvrt_f32_s16(float32 ValFloat);
    extern sint32 Mfl_FloatToIntCvrt_f32_s32(float32 ValFloat);
    extern sint8 Mfl_FloatToIntCvrt_f32_s8(float32 ValFloat);
    extern uint16 Mfl_FloatToIntCvrt_f32_u16(float32 ValFloat);
    extern uint32 Mfl_FloatToIntCvrt_f32_u32(float32 ValFloat);
    extern uint8 Mfl_FloatToIntCvrt_f32_u8(float32 ValFloat);
    extern float32 Mfl_Floor_f32(float32 ValValue);
    extern float32 Mfl_Hypot_f32f32_f32(float32 x_value, float32 y_value);
    extern boolean Mfl_HystCenterHalfDelta_f32_u8(float32 X, float32 center, float32 halfDelta, uint8* State);
    extern boolean Mfl_HystDeltaRight_f32_u8(float32 X, float32 Delta, float32 Rsp, uint8* State);
    extern boolean Mfl_HystLeftDelta_f32_u8(float32 X, float32 Lsp, float32 Delta, uint8* State);
    extern boolean Mfl_HystLeftRight_f32_u8(float32 X, float32 Lsp, float32 Rsp, uint8* State);
    extern void Mfl_ICalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, float32 dT_f32);
    extern void Mfl_ILimCalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_IntToFloatCvrt_s16_f32(sint16 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_s32_f32(sint32 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_s8_f32(sint8 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u16_f32(uint16 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u32_f32(uint32 ValInteger);
    extern float32 Mfl_IntToFloatCvrt_u8_f32(uint8 ValInteger);
    extern float32 Mfl_IOut_f32(const Mfl_StateI_Type* State_cpst);
    extern void Mfl_ISetState(Mfl_StateI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_Limit_f32(float32 val, float32 lowLim, float32 upLim);
    extern float32 Mfl_Log_f32(float32 ValValue);
    extern float32 Mfl_Max_f32(float32 Value1, float32 Value2);
    extern float32 Mfl_MedianSort_f32_f32(float32* Array, uint8 N);
    extern float32 Mfl_Min_f32(float32 Value1, float32 Value2);
    extern void Mfl_PCalc(float32 X_f32, float32* P_pf32, float32 K_f32);
    extern void Mfl_PDCalc(float32 X_f32, Mfl_StatePD_Type* State_cpst, const Mfl_ParamPD_Type* Param_cpst, float32 dT_f32);
    extern float32 Mfl_PDOut_f32(const Mfl_StatePD_Type* State_cpst);
    extern void Mfl_PDSetParam(Mfl_ParamPD_Type* Param_cpst, float32 K_f32, float32 Tv_f32);
    extern void Mfl_PDSetState(Mfl_StatePD_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern float32 Mfl_PIDOut_f32(const Mfl_StatePID_Type* State_cpst);
    extern void Mfl_PIDSetParam(Mfl_ParamPID_Type* Param_cpst, float32 K_f32, float32 Tv_f32, float32 Tnrec_f32);
    extern void Mfl_PIDSetState(Mfl_StatePID_Type* State_cpst, float32 X1_f32, float32 X2_f32, float32 Y1_f32);
    extern void Mfl_PIDTyp1Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp1LimCalc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp2Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PIDTyp2LimCalc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_PIOut_f32(const Mfl_StatePI_Type* State_cpst);
    extern void Mfl_PISetParam(Mfl_ParamPI_Type* Param_cpst, float32 K_f32, float32 Tnrec);
    extern void Mfl_PISetState(Mfl_StatePI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern void Mfl_PITyp1Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PITyp1LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern void Mfl_PITyp2Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32);
    extern void Mfl_PITyp2LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, const Mfl_Limits_Type* Limit_cpst, float32 dT_f32);
    extern float32 Mfl_POut_f32(const float32* P_pf32);
    extern float32 Mfl_Pow_f32(float32 ValBase, float32 ValExp);
    extern void Mfl_PT1Calc(float32 X_f32, Mfl_StatePT1_Type* State_cpst, float32 K_f32, float32 TeQ_f32);
    extern float32 Mfl_PT1Out_f32(const Mfl_StatePT1_Type* State_cpst);
    extern void Mfl_PT1SetState(Mfl_StatePT1_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
    extern void Mfl_RampCalc(float32 X_f32, Mfl_StateRamp_Type* State_cpst, const Mfl_ParamRamp_Type* Param_cpcst, float32 dT_f32);
    extern void Mfl_RampCalcJump(float32 X_f32, Mfl_StateRamp_Type* State_cpst);
    extern float32 Mfl_RampCalcSwitch(float32 Xa_f32, float32 Xb_f32, boolean Switch_B, Mfl_StateRamp_Type* State_cpst);
    extern boolean Mfl_RampCheckActivity(Mfl_StateRamp_Type* State_cpst);
    extern boolean Mfl_RampGetSwitchPos(Mfl_StateRamp_Type* State_cpst);
    extern void Mfl_RampInitState(Mfl_StateRamp_Type* State_cpst, float32 Val_f32);
    extern float32 Mfl_RampOut_f32(const Mfl_StateRamp_Type* State_cpcst);
    extern void Mfl_RampSetParam(Mfl_ParamRamp_Type* Param_cpst, float32 SlopePosVal_f32, float32 SlopeNegVal_f32);
    extern float32 Mfl_RateLimiter_f32(float32 newval, float32 oldval, float32 maxdif);
    extern float32 Mfl_Round_f32(float32 ValValue);
    extern sint8 Mfl_Sign_f32(float32 ValValue);
    extern float32 Mfl_Sin_f32(float32 value);
    extern void Mfl_SortAscend_f32(float32* Array, uint16 Num);
    extern void Mfl_SortDescend_f32(float32* Array, uint16 Num);
    extern float32 Mfl_Sqrt_f32(float32 ValValue);
    extern float32 Mfl_Tan_f32(float32 value);
    extern float32 Mfl_Trunc_f32(float32 ValValue);
#endif
/* MFL_CFG_H_ */
#endif
