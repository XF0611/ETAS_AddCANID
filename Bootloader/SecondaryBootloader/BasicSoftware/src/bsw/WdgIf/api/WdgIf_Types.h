 

 #ifndef WDGIF_TYPES_H
 #define WDGIF_TYPES_H
 /*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "Std_Types.h"
/*
#if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || (STD_TYPES_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(STD_TYPES_AR_RELEASE_MINOR_VERSION) || (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/
 /*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

 /*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

 /*TRACE[WDGIF016] Definations of WdgIf Mode Type*/
 typedef enum
 {
        WDGIF_OFF_MODE,
        WDGIF_SLOW_MODE,
        WDGIF_FAST_MODE
 }WdgIf_ModeType;

 /* This enum is used to check(read) the status of switches */
 typedef enum
 {
     WDGIF_RB_ALLSWTCLSD_E = 0,                  /* All switches closed (switches MM2ERR and ERR2WDA are closed) */
     WDGIF_RB_MM2ERRSWTOPN_E,        /* MM2ERR switch open, remaining (ERR2WDA) close */
     WDGIF_RB_ERR2WDASWTOPN_E,     /* ERR2WDA switch open, remaining (MM2ERR) close */
     WDGIF_RB_ALLSWTOPN_E                     /* All switches opened (switches MM2ERR and ERR2WDA are opened) */
 } WdgIf_Rb_SwtPosn_ten;

 /* This enum is used to set and clear the status of Voltage Monitoring bits  */
 typedef enum
 {
     WdgIf_Rb_stNoLatch_e,      /* Disable VDD5 overvoltage latch */
     WdgIf_Rb_stLatch_e,        /* Activate VDD5 overvoltage latch */
     WdgIf_Rb_stLoLimActv_e,    /* Simulate VDD5 undervoltage */
     WdgIf_Rb_stUpLimActv_e     /* Simulate VDD5 overvoltage */
 }WdgIf_Rb_OperMode_ten;

/* This enum is used to read all voltage monitoring status bits */
 typedef enum
 {
     WdgIf_Rb_stStrictAllUMon_e,   /* Enum member to read all voltage monitoring status bits, "overvoltage/undervoltage" inactive is dominant. strict does not stop the evaluation at 1st occurance of overvoltage or undervoltage active */
     WdgIf_Rb_stAllUMon_e          /* Enum member to read all voltage monitoring status bits, "overvoltage/undervoltage" active is dominant, evaluation is stopped at 1st occurance of overvoltage or undervoltage active */
 }WdgIf_Rb_UMonScope_ten;

  /*This struct is used to get SPI status*/
   typedef struct
  {
      uint32 tiResp_u32;          /* current response time in microsec */
      uint32 tiStampLastTx_u32;   /* timestamp of last SPI transmission in timer ticks */
      uint8 xCurrReq_u8;         /* current request of monitoring module (0..16) */
      uint8 cntrErr_u8;          /* value of error counter (0..7) */
      uint8 stMm_u8;             /* status bits of the monitoring module (bit coded) */
  } WdgIf_Rb_RequestType_tst;

  /*This struct is used to get address of configured external watchdog module api's*/
  typedef struct
  {
      Std_ReturnType (*IoExt_Rb_RequestQuery_pfct)(void);
      Std_ReturnType (*IoExt_Rb_SetResponseTime_pfct)(uint32 tiResponseTime_u32);
      Std_ReturnType (*IoExt_Rb_GetReq_pfct) (WdgIf_Rb_RequestType_tst *xRequest_pst);
      Std_ReturnType (*IoExt_Rb_SetResponse_pfct) (uint32 ResponseValue);
      Std_ReturnType (*IoExt_Rb_Transmit_pfct) (void);
      void (*IoExt_Rb_WatchDogHandle_pfct) (void);
      uint32 (*IoExt_Rb_WatchDogAnswer_pfct) (uint8 xQuery_u8);
      Std_ReturnType (*IoExt_Rb_GetRegCanTdi_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_GetRegInitWdr_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_GetRegWda_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_GetWda_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_SetRegCanTdi_pfct) (boolean isActive_b);
      Std_ReturnType (*IoExt_Rb_SetRegInitWdr_pfct) (boolean isActive_b);
      Std_ReturnType (*IoExt_Rb_GetReg_Tst0_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_SetReg_Tst0_pfct) (boolean isActive_b);
      Std_ReturnType (*IoExt_Rb_GetReg_Tst1_pfct) (boolean *isActive_b);
      Std_ReturnType (*IoExt_Rb_SetReg_Tst1_pfct) (boolean isActive_b);
      Std_ReturnType (*IoExt_Rb_SetSwtPosn_pfct) (WdgIf_Rb_SwtPosn_ten posnSwt_en);
      Std_ReturnType (*IoExt_Rb_GetSwtPosn_pfct) (WdgIf_Rb_SwtPosn_ten* posnSwt_pen);
      Std_ReturnType (*IoExt_Rb_SetWdgThd_pfct) (uint8 threshold);
      Std_ReturnType (*IoExt_Rb_GetWdgThd_pfct) (uint8 *threshold);
      Std_ReturnType (*IoExt_Rb_SetWdgTiWin_pfct) (uint32 timeWin);
      Std_ReturnType (*IoExt_Rb_GetWdgTiWin_pfct) (uint32 *timeWin);
      Std_ReturnType (*IoExt_Rb_SetOperMode_pfct) (WdgIf_Rb_OperMode_ten stOperMode_en);
      Std_ReturnType (*IoExt_Rb_ReqUMonStatus_pfct) (void);
      Std_ReturnType (*IoExt_Rb_GetUMonStatus_pfct) (WdgIf_Rb_UMonScope_ten stScope_en, boolean *isActive_pb);
      Std_ReturnType (*IoExt_Rb_SetRegVdd5OvWda_pfct) (boolean isActive_b);
      Std_ReturnType (*IoExt_Rb_GetRegVdd5OvWda_pfct) (boolean *isActive_pb);
      Std_ReturnType (*IoExt_Rb_ReqWda_pfct) (void);
      Std_ReturnType (*IoExt_Rb_StartWdaMonMode_pfct) (void);
      Std_ReturnType (*IoExt_Rb_StopWdaMonMode_pfct) (void);
  }WdgIf_Rb_ExtWdgdriver_tst;


#endif  /* WDGIF_TYPES_H */
