

#ifndef DCMDSPOBD_MODE1_PUB_H
#define DCMDSPOBD_MODE1_PUB_H

/**
 ***************************************************************************************************
            OBD Mode 01
 ***************************************************************************************************
 */
#if(DCM_CFG_DSP_OBDMODE1_ENABLED != DCM_CFG_OFF)

/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Read Data Function Pointer Types\n
 * When UsePort is USE_DATA_SYNC_FNC or DATA_SYNC_CLIENT_SERVER or USE_DATA_SENDER_RECEIVER \n
 */

typedef union
{
  Std_ReturnType (*GetPIDvalue1_pf) (uint8 * Data);
  Std_ReturnType (*GetPIDvalue2_pf) (uint16 * Data);
  Std_ReturnType (*GetPIDvalue3_pf) (uint32 * Data);
  Std_ReturnType (*GetPIDvalue4_pf) (sint8 * Data);
  Std_ReturnType (*GetPIDvalue5_pf) (sint16 * Data);
  Std_ReturnType (*GetPIDvalue6_pf) (sint32 * Data);
  Std_ReturnType (*GetPIDvalue7_pf) (boolean * Data);
}un_mode1;

/**
 * @ingroup DCMDSP_OBD_AUTOSAR
 * DcmDspPidDataUsePort:This can have the values\n
 * OBD_USE_DATA_SENDER_RECEIVER,
 * OBD_USE_DATA_SYNCH_CLIENT_SERVER,
 * OBD_USE_DATA_SYNCH_FNC
 */
typedef enum
{
	OBD_USE_DATA_SENDER_RECEIVER,
	OBD_USE_DATA_SYNCH_CLIENT_SERVER,
	OBD_USE_DATA_SYNCH_FNC
}DcmDspPidDataUsePort;
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * supported PID configuration for OBD MODE 01 service\n
 * uint32 BitMask_u32;                  Bit mask for the special PID\n
 * uint8  StartIndex_u8;                Starting index of PID array (Dcm_Dsp_PidArray_acs) for this special PID \n
 * uint8  NumPids_u8;                   Number of PID configured for this special PID\n
 */
typedef struct
{
    uint32 BitMask_u32;                  /* Bit mask for the special PID */
    uint8  StartIndex_u8;                /* Starting index of PID array (Dcm_Dsp_PidArray_acs) for this special PID */
    uint8  NumPids_u8;                   /* Number of PID configured for this special PID */
}Dcm_Dsp_Mode1BitMask_Type;
/**
 * @ingroup DCMDSP_OBD_EXTENDED
* PID Configuration (PID which reports data value) for OBD MODE 01 service\n
* uint8  Pid_Id_u8;                     PID number\n
* uint8  Num_DataSourcePids_u8;         Number of data bytes for each PID \n
* uint16 DataSourcePid_ArrayIndex_u16;  Array index to data associated with each PID \n
* Include the config_mask only if DCM_CFG_POSTBUILD_SUPPORT is enabled\n
* uint8 Config_Mask;                   Config mask indicating PID is active which configuration\n
* uint16 Pid_Size_u8;                  Total size of the PIDs
*/
typedef struct
{
    uint8 Pid_Id_u8;                     /* PID number      */
    uint8 Num_DataSourcePids_u8;         /* Number of data bytes for each PID */
    uint16 DataSourcePid_ArrayIndex_u16; /* Array index to data associated with each PID */
	/* Include the config_mask only if DCM_CFG_POSTBUILD_SUPPORT is enabled*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
    uint8 Config_Mask;                   /*Config mask indicating PID is active which configuration*/
#endif
    uint16 Pid_Size_u8;                  /* Total size of the PIDs */
}Dcm_Dsp_Mode1Pid_Type;

/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Dcm_Dsp_Mode1SupportInfo_Type has member elements,\n
 * uint8 SupportInfoLen_u8;             Length of Support Information in bytes \n
 * uint8 SupportInfoPos_u8;            Position of Support Information in bytes\n
 * */
typedef struct
{
	uint8 SupportInfoLen_u8;             /* Length of Support Information in bytes */
	uint8 SupportInfoPos_u8;             /* Position of Support Information in bytes */
}Dcm_Dsp_Mode1SupportInfo_Type;


/**
 * @ingroup DCMDSP_OBD_EXTENDED
 *   void* const GetPIDvalue_pf;           Function to get PID data \n
 *   const Dcm_Dsp_Mode1SupportInfo_Type * ptr_supportinfo_pcs;  Reference to support info table \n
 *   DcmDspPidDataUsePort PidUsePort;      Use port type \n
 *   uint16 Length_data_u16;               Length of the data in bits \n
 *   uint16 Pos_data_u16;                  Position of data in bits \n
 *   uint8 dataType_u8;                    DataType of the signal \n
 *   uint8 dataEndianness_u8;             Data endianness\n
 *   uint8 SupportInfoBit_u8;             Support Info bit\n
 * */



typedef struct
{
    un_mode1 GetPIDvalue_pf;          /* Function to get PID data */
    const Dcm_Dsp_Mode1SupportInfo_Type * ptr_supportinfo_pcs;  /* Reference to support info table */
    DcmDspPidDataUsePort PidUsePort;     /* Use port type */
    uint16 Length_data_u16;              /* Length of the data in bits */
    uint16 Pos_data_u16;                 /* Position of data in bits */
    uint8 dataType_u8;                   /* DataType of the signal */
    uint8 dataEndianness_u8;             /*Data endianness*/
    uint8 SupportInfoBit_u8;             /* Support Info bit */
}Dcm_Dsp_Mode1DataSourcePid_Type;
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Bitmask array for OBDmode1
 */
extern const Dcm_Dsp_Mode1BitMask_Type Dcm_Dsp_Mode1Bitmask_acs[8];
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * PID array  for OBD mode1
 */
extern const Dcm_Dsp_Mode1Pid_Type Dcm_Dsp_Mode1PidArray_acs[DCM_CFG_DSP_NUMPIDSUPP_MODE1];
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * data elements array configured for the PIDs for OBD MODE $01
 */
extern const Dcm_Dsp_Mode1DataSourcePid_Type Dcm_Dsp_Mode1DataSourcePid_acs[DCM_CFG_DSP_NUMPIDDATA_MODE1];
#if (DCM_CFG_DSP_NUMSUPPINFO_MODE1 != 0)
/**
 * @ingroup DCMDSP_OBD_EXTENDED
 * Array for support information for the PIDs for MODE $01
 *
 * */
extern const Dcm_Dsp_Mode1SupportInfo_Type Dcm_Dsp_Mode1SupportInfo_acs[DCM_CFG_DSP_NUMSUPPINFO_MODE1];

#endif
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif  /* End of DCM_CFG_DSP_OBDMODE1_ENABLED */
#endif /* _DCMDSPOBD_MODE1_PUB_H  */
