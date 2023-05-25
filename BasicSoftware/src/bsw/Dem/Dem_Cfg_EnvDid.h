/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVDID_H
#define DEM_CFG_ENVDID_H

#define DEM_CFG_ENVDID2DATAELEMENT \
{ \
   DEM_DATAELEM_SR_FF_267_YEAR,   DEM_DATAELEM_SR_FF_267_MONTH,   DEM_DATAELEM_SR_FF_267_DAY,   DEM_DATAELEM_SR_FF_267_HOUR,   DEM_DATAELEM_SR_FF_267_MINUTE,   DEM_DATAELEM_SR_FF_267_SECOND,                                   /* DidClass_DID_0x010B */    \
   DEM_DATAELEM_SR_FF_274_NEW_DATA_OBJECT,                                   /* DidClass_DID_0x0112 */    \
   DEM_DATAELEM_SR_FF_53249_NEW_DATA_OBJECT,                                   /* DidClass_DID_0xD001 */    \
   DEM_DATAELEM_SR_FF_55553_NEW_DATA_OBJECT,                                   /* DidClass_DID_0xD901 */    \
   DEM_DATAELEM_SR_FF_57601_NEW_DATA_OBJECT,                                   /* DidClass_DID_0xE101 */    \
   0 \
}

#define DEM_DID_DIDCLASS_DID_0X010B         1
#define DEM_DID_DIDCLASS_DID_0X0112         2
#define DEM_DID_DIDCLASS_DID_0XD001         3
#define DEM_DID_DIDCLASS_DID_0XD901         4
#define DEM_DID_DIDCLASS_DID_0XE101         5

#define DEM_CFG_ENVDID \
{ \
   { 0, 0 } \
   ,{ 6, 267 }      /* DEM_DID_DIDCLASS_DID_0X010B */ \
   ,{ 7, 274 }      /* DEM_DID_DIDCLASS_DID_0X0112 */ \
   ,{ 8, 53249 }      /* DEM_DID_DIDCLASS_DID_0XD001 */ \
   ,{ 9, 55553 }      /* DEM_DID_DIDCLASS_DID_0XD901 */ \
   ,{ 10, 57601 }      /* DEM_DID_DIDCLASS_DID_0XE101 */ \
}

#define DEM_CFG_ENVDID_ARRAYLENGTH  (5+1)

#endif

