


#ifndef DEM_DTCS_H
#define DEM_DTCS_H



#include "Dem_Types.h"
#include "Dem_Array.h"
#include "Dem_BitArray.h"
#include "rba_DiagLib_Bits8.h"
#include "Dem_Cfg_DtcId.h"
#include "Dem_Cfg_DTC_DataStructures.h"
#include "Dem_Cfg_Main.h"
#include "Dem_Mapping.h"
#include "Dem_ISO14229Byte.h"
#include "Dem_Events.h"
#include "Dem_DTCStatusByte.h"
#include "Dem_Cfg_Events_DataStructures.h"

#include "Dem_Helpers.h"
#include "Dem_EvMem.h"

#define DEM_DTC_SETTING_ENABLED     FALSE
#define DEM_DTC_SETTING_DISABLED    TRUE


typedef struct {
	Dem_DtcStateType state;
} Dem_DtcState;

typedef boolean DemControlDtcSettingType;
#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
extern DemControlDtcSettingType Dem_DtcSettingDisabledFlag;
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

/******************************************************************************/



#define DEM_DTCS_NVSTORAGE_BP    2u
#define DEM_DTCS_ALTERNATIVEDTC_FLAG_BP    3u

#define DEM_DTCS_SEVERITYMASK    0xE0u
#define DEM_DTCS_KINDMASK        0x03u
#define DEM_DTCS_NVSTORAGEMASK   ((uint8)(1u<<DEM_DTCS_NVSTORAGE_BP))
#define DEM_DTCS_ALTERNATIVEDTC_FLAG   ((uint8)(1u<<DEM_DTCS_ALTERNATIVEDTC_FLAG_BP))



#define DEM_DTCS_INIT8(KIND, SEVERITY, NVSTORAGE, FUNC_UNIT, INDEXOF_ALTERNATIVE_DTCID) \
{   ((KIND) & (DEM_DTCS_KINDMASK))|((SEVERITY) & (DEM_DTCS_SEVERITYMASK)) | ((uint8)(((NVSTORAGE) & (0x01u)) << DEM_DTCS_NVSTORAGE_BP)) | ((uint8)((DEM_BOOL2BIT(INDEXOF_ALTERNATIVE_DTCID) & (0x01u)) << DEM_DTCS_ALTERNATIVEDTC_FLAG_BP)) \
    ,(FUNC_UNIT) \
}

/* FC_VariationPoint_START */
#if (DEM_CFG_OBD == DEM_CFG_OBD_ON) && (DEM_CFG_OBD_DTC_CONFIG != DEM_CFG_OBD_DTC_CONFIG_OFF)
#define DEM_DTCS_INIT16(OBDCODE) \
{   (OBDCODE) \
}
#endif
/* FC_VariationPoint_END */

#define DEM_DTCS_INIT32(CODE) \
{   (CODE) \
}



typedef struct {
   uint8 kind_severity_storage;
   uint8 Functional_Unit;
} Dem_DtcParam8;

/* FC_VariationPoint_START */
#if (DEM_CFG_OBD == DEM_CFG_OBD_ON) && (DEM_CFG_OBD_DTC_CONFIG != DEM_CFG_OBD_DTC_CONFIG_OFF)
typedef struct {
   uint16 ObdCode;
} Dem_DtcParam16;
#endif
/* FC_VariationPoint_END */

typedef struct {
   Dem_DtcCodeType code;
} Dem_DtcParam32;

typedef struct {
    Dem_DtcCodeType AltDtccode;
    Dem_DtcCodeType Dtccode;
}Dem_DtcAltParam;

/******************************************************************************/
#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

    DEM_ARRAY_DECLARE(Dem_DtcState, Dem_AllDTCsState, DEM_DTCID_ARRAYLENGTH);

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"

    #if(DEM_CFG_ALTERNATIVEDTC == DEM_CFG_ALTERNATIVEDTC_ON)
    extern boolean Dem_AlternativeDTCEnabled;
    #endif

#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

    #if (DEM_CFG_ALTERNATIVEDTC == DEM_CFG_ALTERNATIVEDTC_ON)
    DEM_ARRAY_DECLARE_CONST(Dem_DtcAltParam, Dem_AllAlternativeDTCsParam, DEM_ALTERNATIVE_DTCCODE_ARRAYLENGTH);
    #endif

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

/********DTC SUPPRESSION************************************************************/
DEM_INLINE Dem_boolean_least Dem_DtcIsSuppressed (Dem_DtcIdType dtcId)
{
#if (DEM_CFG_SUPPRESSION != DEM_NO_SUPPRESSION)
    return DEM_DTCSTATE_ISBITSET (Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED);
#else
    DEM_UNUSED_PARAM(dtcId);
    return FALSE;
#endif
}

DEM_INLINE Dem_boolean_least Dem_DtcIsSuppressedDirectly (Dem_DtcIdType dtcId)
{
#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
    return DEM_DTCSTATE_ISBITSET (Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED_DIRECTLY);
#else
    DEM_UNUSED_PARAM(dtcId);
    return FALSE;
#endif
}

DEM_INLINE Dem_boolean_least Dem_DtcIsSupported (Dem_DtcIdType dtcID)
{
    return (Dem_isDtcIdValid(dtcID)
            && (!Dem_DtcIsSuppressed(dtcID))
    );
}

#if ((DEM_CFG_SUPPRESSION == DEM_EVENT_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
DEM_INLINE void Dem_DtcHandleEventSuppression (Dem_DtcIdType dtcId, Dem_boolean_least eventUnsuppressed)
{
    DEM_ASSERT_ISLOCKED();

    if (!Dem_DtcIsSuppressedDirectly(dtcId))
    {
        Dem_EventIdListIterator it;

        if (!eventUnsuppressed)
        {
            for (Dem_EventIdListIteratorNewFromDtcId(&it, dtcId);
                    Dem_EventIdListIteratorIsValid(&it);
                    Dem_EventIdListIteratorNext(&it))
            {
                if ( !Dem_EvtIsSuppressed(Dem_EventIdListIteratorCurrent(&it)) )
                {
                    eventUnsuppressed = TRUE;
                    break;
                }
            }
        }

        DEM_DTCSTATE_OVERWRITEBIT (&Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED, !eventUnsuppressed);
    }
}
#endif

Dem_DtcIdType Dem_DtcIdFromDtcCode (Dem_DtcCodeType dtcCode);

#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
DEM_INLINE void Dem_DtcSuppressionApply (Dem_DtcIdType dtcId, Dem_boolean_least setBit)
{
	DEM_ENTERLOCK_MON();
	DEM_DTCSTATE_OVERWRITEBIT (&Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED_DIRECTLY, setBit);

	if (setBit)
	{
	    DEM_DTCSTATE_SETBIT (&Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED);
	}
	else
	{
# if (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION)
	    Dem_DtcHandleEventSuppression (dtcId, FALSE);
# else
	    DEM_DTCSTATE_CLEARBIT (&Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_STATE_SUPPRESSED);
# endif
	}

	DEM_EXITLOCK_MON();
}
#endif

#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
DEM_INLINE Dem_boolean_least Dem_IsEventMemoryEntryExistForDTC (Dem_DtcIdType dtcId)
{
	uint8 DtcStatusByte = Dem_DtcStatusByteRetrieve(dtcId);

	/**
	 * Checking, whether TestFailed or TestFailedSLC bit is set
	 * (To make sure that corresponding DTC already processed and available in Event memory)
	 */
	if(Dem_ISO14229ByteIsTestFailed(DtcStatusByte) || Dem_ISO14229ByteIsTestFailedSLC(DtcStatusByte))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

/***********************************************************************************/


/*-- DTC CODE ----------------------------------------------------------------*/


DEM_INLINE Dem_DtcCodeType Dem_GetDtcCode (Dem_DtcIdType dtcId)
{
#if (DEM_CFG_ALTERNATIVEDTC == DEM_CFG_ALTERNATIVEDTC_ON)
    if(Dem_Cfg_Dtc_GetDtcCode_Is_Index(dtcId))
    {
        if (Dem_AlternativeDTCEnabled)
        {
            return Dem_AllAlternativeDTCsParam[Dem_Cfg_Dtc_GetDtcCode(dtcId)].AltDtccode;
        }
        else
        {
            return Dem_AllAlternativeDTCsParam[Dem_Cfg_Dtc_GetDtcCode(dtcId)].Dtccode;
        }
    }
    else
#endif
    {
        return Dem_Cfg_Dtc_GetDtcCode(dtcId);
    }
}




/* FC_VariationPoint_START */
#if (DEM_CFG_OBD == DEM_CFG_OBD_ON) && (DEM_CFG_OBD_DTC_CONFIG != DEM_CFG_OBD_DTC_CONFIG_OFF)


#endif
/* FC_VariationPoint_END */


#if (DEM_CFG_CHECKAPICONSISTENCY == TRUE)
/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetDebouncingOfEvent to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetDTCOfEvent_GeneralEvtInfo(Dem_EventIdType EventId, Dem_DTCFormatType DTCFormat, uint32* DTCOfEvent);
#endif /* DEM_CFG_CHECKAPICONSISTENCY */



DEM_INLINE Dem_boolean_least Dem_EventUsesOrigin (Dem_EventIdType eventId, Dem_DTCOriginType origin)
{
	if (   ((origin == DEM_DTC_ORIGIN_PRIMARY_MEMORY) && Dem_EvtParam_GetIsEventDestPrimary(eventId))
			|| ((origin == DEM_DTC_ORIGIN_SECONDARY_MEMORY) && Dem_EvtParam_GetIsEventDestSecondary(eventId))
			|| ((origin == DEM_DTC_ORIGIN_MIRROR_MEMORY) && Dem_EvtParam_GetIsEventDestMirror(eventId))
/* FC_VariationPoint_START */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
			|| ((origin == DEM_DTC_ORIGIN_PERMANENT_MEMORY) && (Dem_Cfg_Dtc_GetKind(Dem_DtcIdFromEventId(eventId)) == DEM_DTC_KIND_EMISSION_REL_DTCS))
#endif
/* FC_VariationPoint_END */
	)
	{
		return TRUE;
	}
	return FALSE;
}

DEM_INLINE Dem_boolean_least Dem_DtcUsesOrigin (Dem_DtcIdType dtcId, Dem_DTCOriginType origin)
{
    Dem_EventIdType eventId = Dem_DtcIdGetFirstEventId(dtcId);
    return Dem_EventUsesOrigin(eventId, origin);
}


/*************************Enable & DiableDTCGroup****************************************************/
void Dem_DtcSetDTCSetting (Dem_DtcIdType dtcId, Dem_boolean_least setBit);


DEM_INLINE Dem_boolean_least Dem_DtcIsDTCSettingEnabled(Dem_DtcIdType dtcId)
{
	return (!(DEM_DTCSTATE_ISBITSET (Dem_AllDTCsState[dtcId].state, DEM_DTC_BP_DTCSETTING_DISABLED)));
}

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*************************CheckDTCSettingStatus****************************************************/

Dem_boolean_least Dem_IsEventEnabledByDtcSetting(Dem_EventIdType EventId);

DEM_INLINE Dem_boolean_least Dem_IsEventReportingEnabledByDtcSetting(Dem_EventIdType EventId)
{
#if (DEM_CFG_DTCSETTINGBLOCKSREPORTING)
    return Dem_IsEventEnabledByDtcSetting(EventId);
#else
    DEM_UNUSED_PARAM(EventId);
    return TRUE;
#endif
}

DEM_INLINE Dem_boolean_least Dem_IsEventStorageEnabledByDtcSetting(Dem_EventIdType EventId)
{
#if (DEM_CFG_DTCSETTINGBLOCKSREPORTING)
    DEM_UNUSED_PARAM(EventId);
    return TRUE;
#else
    return Dem_IsEventEnabledByDtcSetting(EventId);
#endif
}


/****************************************************************************************************/

sint8 Dem_DtcFaultDetectionRetrieve (Dem_DtcIdType dtcId);

/*************************ControlDTCSettingStatus****************************************************/

DEM_INLINE void Dem_SetDtcSettingFlag(DemControlDtcSettingType DtcSettingDisabled)
{
    Dem_DtcSettingDisabledFlag = DtcSettingDisabled;
}

DEM_INLINE Dem_boolean_least Dem_IsDtcSettingDisabled(void)
{
    return Dem_DtcSettingDisabledFlag;
}
/****************************************************************************************************/

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
