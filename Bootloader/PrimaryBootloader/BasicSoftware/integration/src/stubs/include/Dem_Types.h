
#ifndef  _DEM_TYPES_H
#define  _DEM_TYPES_H

/*******************************************************************************
**                      Includes
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define DEM_EVENT_STATUS_PASSED ((Dem_EventStatusType)0x00)
#define DEM_EVENT_STATUS_FAILED ((Dem_EventStatusType)0x01)

#define DEM_CONTROL_DTC_SETTING_OK      0x00
#define DEM_CONTROL_DTC_SETTING_N_OK    0x01
#define DEM_CONTROL_DTC_WRONG_DTCGROUP  0x02

#define DEM_DTC_GROUP_EMISSION_REL_DTCS     0x00000000
#define DEM_DTC_GROUP_ALL_DTCS              0xFFFFFFFFu
#define DEM_DTC_KIND_ALL_DTCS                  0x01
#define DEM_DTC_KIND_EMISSION_REL_DTCS         0x02

/* component specific extension to the Std_ReturnType */
#define DEM_PENDING                  4u
#define DEM_CLEAR_BUSY               5u
#define DEM_CLEAR_MEMORY_ERROR       6u
#define DEM_CLEAR_FAILED             7u
#define DEM_WRONG_DTC                8u
#define DEM_WRONG_DTCORIGIN          9u
#define DEM_E_NO_DTC_AVAILABLE      10u
#define DEM_E_NO_FDC_AVAILABLE      14u
#define DEM_BUFFER_TOO_SMALL        21u
#define DEM_BUSY                    22u
#define DEM_NO_SUCH_ELEMENT         48u
/******************************************************/

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
typedef uint16 Dem_EventIdType;

typedef uint8 Dem_EventStatusType;  /**< @brief dem event status type */
typedef uint8 Dem_ReturnControlDTCSettingType;
typedef uint8 Dem_InitMonitorReasonType;

typedef uint32 Dem_DTCGroupType;

typedef uint8  Dem_DTCKindType;
/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif
