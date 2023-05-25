

#include "Dem_Internal.h"
#include "Rte_Dem.h"

#include "Dem_EvMemGen.h"

#include "Dem_Main.h"
#include "Dem_Client.h"
#include "Dem_Obd.h"

/* -------------------------------------------------
   service Dem_GetDTCByOccurrenceTime
   -------------------------------------------------
 */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
#if ( DEM_CFG_EVMEMGENERIC_SUPPORTED != FALSE )
Std_ReturnType Dem_GetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC)
{
    Dem_DtcIdType DtcId;
    Std_ReturnType retVal = E_NOT_OK;

    /* is DEM Initialized ?*/
    if (!Dem_OpMoIsInitialized())
    {
        DEM_DET(DEM_DET_APIID_GETDTCBYOCCURRENCETIME, DEM_E_UNINIT,0u);
    }

    else if (!Dem_isClientIdValid(ClientId))
    {
        DEM_DET(DEM_DET_APIID_GETDTCBYOCCURRENCETIME, DEM_E_WRONG_CONFIGURATION,0u);
    }

    /* parameter check: DTCRequest */
    else if ((DTCRequest != DEM_FIRST_FAILED_DTC) &&
            (DTCRequest != DEM_MOST_RECENT_FAILED_DTC) &&
            (DTCRequest != DEM_FIRST_DET_CONFIRMED_DTC) &&
            (DTCRequest != DEM_MOST_REC_DET_CONFIRMED_DTC)
#if (DEM_CFG_OBD == DEM_CFG_OBD_ON)
            && (DTCRequest != DEM_FIRST_FAILED_OBD_DTC)
            && (DTCRequest != DEM_MOST_RECENT_FAILED_OBD_DTC)
#endif
            )
    {
        DEM_DET(DEM_DET_APIID_GETDTCBYOCCURRENCETIME, DEM_E_WRONG_CONFIGURATION,0u);
    }

    else
    {
        /* get the stored DtcId matching the DTCRequest */
        DtcId = Dem_EvMemGenGetDtcIdByOccIndex(DTCRequest);
        /* Is DtcId valid ?*/
        if (!Dem_isDtcIdValid(DtcId))
        { /* No DTC is stored */
            retVal = DEM_NO_SUCH_ELEMENT;
        }
        else
        {
#if (DEM_CFG_OBD == DEM_CFG_OBD_ON)
            /* get the OBD DTC code */
            if ((DTCRequest == DEM_FIRST_FAILED_OBD_DTC) || (DTCRequest == DEM_MOST_RECENT_FAILED_OBD_DTC))
            {
                *DTC = (uint32) rba_DemObdBasic_Dtc_GetCode(DtcId);
                retVal = E_OK;
            }
            else
#endif
            {
                /* get the DTC code */
                *DTC = (uint32) Dem_GetDtcCode(DtcId);
                retVal = E_OK;
            }
        }
    }

    return retVal;
}
#endif

/* -------------------------------------------------
   service Dem_GetEventMemoryOverflow
   -------------------------------------------------
 */
Std_ReturnType Dem_GetEventMemoryOverflow(uint8 ClientId, Dem_DTCOriginType DTCOrigin, boolean* OverflowIndication)
{
   if (Dem_LibGetParamBool(DEM_CFG_EVMEMGENERIC_SUPPORTED))
   {
       /*   Entry Condition Check   */
       DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED(DEM_DET_APIID_GETEVENTMEMORYOVERFLOW,E_NOT_OK);
       DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(OverflowIndication,DEM_DET_APIID_GETEVENTMEMORYOVERFLOW,E_NOT_OK);

       /* Currently only one EventMemorySet is supported, so ClientId is not used to get the EventMemorySet referred by the Client */
       if (!Dem_isClientIdValid(ClientId))
       {
           DEM_DET(DEM_DET_APIID_GETEVENTMEMORYOVERFLOW, DEM_E_WRONG_CONFIGURATION,0u);
           return E_NOT_OK;
       }

      if (!Dem_EvMemIsDtcOriginValid (&DTCOrigin))
      {
         DEM_DET(DEM_DET_APIID_GETEVENTMEMORYOVERFLOW, DEM_E_WRONG_CONFIGURATION,0u);
         return E_NOT_OK;
      }

      /* get the stored overflow indicator matching the DTCOrigin */
      *OverflowIndication = Dem_EvMemGenIsOverflow(DTCOrigin);
      return E_OK;
   }
   else
   {
	   DEM_DET(DEM_DET_APIID_GETEVENTMEMORYOVERFLOW, DEM_E_WRONG_CONDITION,0u);
	   return E_NOT_OK;
   }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

