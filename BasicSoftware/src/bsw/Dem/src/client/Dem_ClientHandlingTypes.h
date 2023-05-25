

#ifndef DEM_CLIENTHANDLINGTYPES_H
#define DEM_CLIENTHANDLINGTYPES_H

#include "Dem_Types.h"

/******************************************************************************************/
/****************  Client Type  *********************************************************/
#define DEM_CLIENTTYPE_STANDARD      0x00u
#define DEM_CLIENTTYPE_J1939         0x01u
#define DEM_CLIENTTYPE_INVALID       0xFFu

/******************************************************************************************/
/****************  Client States  *********************************************************/
#define DEM_CLIENT_STATE_INIT                   0x00u
#define DEM_CLIENT_STATE_PARAMETERS_SET         0x01u
#define DEM_CLIENT_STATE_REQUESTED_OPERATION    0x02u

/******************************************************************************************/
/****************  Dem_ClientSelectionType  ***********************************************/

#define DEM_CLIENT_SELECTION_TYPE_INVALID                   0X00u
#define DEM_CLIENT_SELECTION_TYPE_ALL_DTCS                  0X01u
#define DEM_CLIENT_SELECTION_TYPE_GROUP_DTC                 0X02u
#define DEM_CLIENT_SELECTION_TYPE_SINGLE_DTC                0X03u
#define DEM_CLIENT_SELECTION_TYPE_EMISSION_RELATED_DTCS     0X04u

/******************************************************************************************/
/****************  Dem_ClientRequestType  ************************************************/

#define DEM_CLIENT_REQUEST_NONE                             0x00u
#define DEM_CLIENT_REQUEST_CLEAR                            0x02u
#define DEM_CLIENT_REQUEST_CANCEL_CLEAR                     0x03u
#define DEM_CLIENT_REQUEST_GETSTATUS                        0x04u
#define DEM_CLIENT_REQUEST_CANCEL_GETSTATUS                 0x05u
#define DEM_CLIENT_REQUEST_TRIGGERSELECTIONRESULT           0x06u
#define DEM_CLIENT_REQUEST_CANCEL_TRIGGERSELECTIONRESULT    0x07u
#define DEM_CLIENT_REQUEST_DISABLERECORDUPDATE              0x08u
#define DEM_CLIENT_REQUEST_CANCEL_DISABLERECORDUPDATE       0x09u
#define DEM_CLIENT_REQUEST_ENABLERECORDUPDATE               0x0Au
#define DEM_CLIENT_REQUEST_CANCEL_ENABLERECORDUPDATE        0x0Bu
#define DEM_CLIENT_REQUEST_GETSELECTIONRESULT4CLEAR         0x0Cu
#define DEM_CLIENT_REQUEST_CANCEL_GETSELECTIONRESULT4CLEAR  0x0Du
#define DEM_CLIENT_REQUEST_SELECT_ED_FF_DATA                0x0Eu
#define DEM_CLIENT_REQUEST_CANCEL_SELECT_ED_FF_DATA         0x0Fu
#define DEM_CLIENT_REQUEST_ENABLE_SUPPRESSION               0x10u
#define DEM_CLIENT_REQUEST_CANCEL_ENABLE_SUPPRESSION    	0x11u
#define DEM_CLIENT_REQUEST_DISABLE_SUPPRESSION          	0x12u
#define DEM_CLIENT_REQUEST_CANCEL_DISABLE_SUPPRESSION   	0x13u
#define DEM_CLIENT_REQUEST_GET_SUPPRESSION           		0x14u
#define DEM_CLIENT_REQUEST_CANCEL_GET_SUPPRESSION           0x15u


typedef uint16 Dem_ClientRequestType;
typedef uint16 Dem_ClientResultType;
typedef uint32 Dem_ClientSelectionType;

typedef uint8 Dem_ClientPriorityType;

/** Additional internal return values for the client functions */
#define DEM_WRONG_CLIENTID      E_NOT_OK
/* Internal Return value for functions Dem_GetNextExtendedDataRecord and Dem_GetNextFreezeFrameData */
#define DEM_INVALID_RECORDNUMBER          25u

#endif
