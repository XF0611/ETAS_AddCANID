/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVEXTENDEDDATA_H
#define DEM_CFG_ENVEXTENDEDDATA_H

#define DEM_CFG_ENVEXTDATA2EXTDATAREC \
{                                     \
   DEM_EXTDATAREC_RECORD_EXTENDEDDATARECORD0X01,  /* ExtendedDataClass_DTC_0x000001_Properties */ \
   DEM_EXTDATAREC_RECORD_EXTENDEDDATARECORD0X01,  /* ExtendedDataClass_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Properties */ \
   DEM_EXTDATAREC_RECORD_EXTENDEDDATARECORD0X01,  /* ExtendedDataClass_DTC_0xc22a87_Properties */ \
   0 \
}

#define DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES         1
#define DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_PROPERTIES         2
#define DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES         3

#define DEM_CFG_ENVEXTDATA                 \
{                                          \
   { 0, 0 }                                \
,{1,4}   /* DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES */     \
,{2,4}   /* DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_PROPERTIES */     \
,{3,4}   /* DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES */     \
}

#define DEM_CFG_ENVEXTDATA_ARRAYLENGTH  (3+1)

#endif

