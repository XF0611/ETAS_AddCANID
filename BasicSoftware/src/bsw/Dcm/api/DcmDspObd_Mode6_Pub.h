

#ifndef DCMDSPOBD_MODE6_PUB_H
#define DCMDSPOBD_MODE6_PUB_H

/* BSW-8793 */
#if(DCM_DSP_OBDMID_DTR_RB_SUPPORT == DCM_CFG_ON)
/* END BSW-8793 */

#ifndef DCM_DTRSTATUS_VISIBLE
#define DCM_DTRSTATUS_VISIBLE     0
#endif
#ifndef DCM_DTRSTATUS_INVISIBLE
#define DCM_DTRSTATUS_INVISIBLE   1
#endif

typedef union
{
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Function pointer for the  function GetDTRvalue with OpStatus
 */
    Std_ReturnType (*GetDTRvalue_pf1) (Dcm_OpStatusType OpStatus,
                                       uint16 * Testval,
                                       uint16 * Minlimit,
                                       uint16 * Maxlimit,
                                       uint8 * Status);

/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Function pointer for the  function GetDTRvalue without OpStatus
 */
    Std_ReturnType (*GetDTRvalue_pf2) (uint16 * Testval,
                                       uint16 * Minlimit,
                                       uint16 * Maxlimit,
                                       uint8 * Status);
}un_mode6;

#if(DCM_CFG_DSP_OBDMODE6_ENABLED != DCM_CFG_OFF)
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Dcm_Dsp_Mode6BitMask_Type:Structure contains,\n
 * uint32 BitMask_u32;        Bit mask for the special OBDMID\n
 * uint8  StartIndex_u8;     Starting index of OBDMID array for this special OBDMID\n
 * uint8  NumMids_u8;       Number of OBDMIDs configured for this special OBDMID \n
 */
/* Bitmask array */
typedef struct
{
    uint32 BitMask_u32;       /* Bit mask for the special OBDMID */
    uint8  StartIndex_u8;     /* Starting index of OBDMID array for this special OBDMID */
    uint8  NumMids_u8;        /* Number of OBDMIDs configured for this special OBDMID */
}Dcm_Dsp_Mode6BitMask_Type;

/* OBDMID array */
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Dcm_Dsp_ObdMidArray_Type:Structure contains,
 * uint16 Tid_ArrayIndex_u16;  Index to the data source in TID array \n
 * uint8 Mid_Id_u8;            Identifier of the OBDMID \n
 * uint8 Num_Tids_u8;          Number of data source (TIDs) exists for this OBDMID
 */
typedef struct
{
    uint16 Tid_ArrayIndex_u16; /* Index to the data source in TID array */
    uint8 Mid_Id_u8;           /* Identifier of the OBDMID */
    uint8 Num_Tids_u8;         /* Number of data source (TIDs) exists for this OBDMID */
}Dcm_Dsp_ObdMidArray_Type;

/* TID array */
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Dcm_Dsp_TidArray_Type:Structure contains,
 * void* const GetDTRvalue_pf;       Function to get TID data\n
 * uint8 UnitScale_u8;               Unit and scaling value\n
 * uint8 Tid_Id_u8;                  Tid Identifier\n
 * boolean Is_UsePort_Enabled_b   ;  Is use port enabled or not \n
 */

typedef struct
{
    un_mode6 GetDTRvalue_pf;      /* Function to get TID data */
    uint8 UnitScale_u8;              /* Unit and scaling value */
    uint8 Tid_Id_u8;                 /* Tid Identifier */
    boolean Is_UsePort_Enabled_b   ; /* Is use port enabled or not */
}Dcm_Dsp_TidArray_Type;

/***************************************************************************************************/
/*                      Externs                                                                    */
/***************************************************************************************************/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Bitmask array for OBD mode6
 * */
extern const Dcm_Dsp_Mode6BitMask_Type Dcm_Dsp_Mode6Bitmask_acs[8];
/**
 * @ingroup DCMDSP_OBD
 * OBDMID array  for OBD mode6
 * */
extern const Dcm_Dsp_ObdMidArray_Type Dcm_Dsp_ObdMidArray_acs[DCM_CFG_DSP_OBDMIDARRAYSIZE];
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * TID array  for OBD mode6
 * */
extern const Dcm_Dsp_TidArray_Type Dcm_Dsp_TidArray_acs[];

#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#endif /* For switch DCM_CFG_DSP_OBDMODE6_ENABLED*/
#include "Dcm_MemMap.h"
/* BSW-8793 */
#endif
/* END BSW-8793 */
#endif /* For end of file */

