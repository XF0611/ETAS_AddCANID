


#ifndef DEM_MAIN_H
#define DEM_MAIN_H


#include "Dem_Types.h"
#include "Dem_Cfg_Main.h"
#include "Dem_Cfg_Events.h"

/* Indicates Operational States of Dem */
typedef uint8 Dem_OpMoStateType;
#define  DEM_OPMO_STATE_NOTINITIALIZED                  0
#define  DEM_OPMO_STATE_PREINITIALIZED                  1
#define  DEM_OPMO_STATE_INITIALIZED                     2

#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
#define  DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO           3
#endif

/* Indicates the State of Fim */
typedef uint8 Dem_FimStateType;
#define DEM_FIM_STATE_NOTINITIALIZED                    0
#define DEM_FIM_STATE_IS_IN_INITIALIZATION              1
#define DEM_FIM_STATE_INITIALIZED                       2


#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"
extern Dem_OpMoStateType Dem_OpMoState;
extern Dem_FimStateType Dem_FimState;
extern Dem_boolean_least Dem_TestFailedStatusInitialized;

#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"

/* Dem States and Entry conditions */
#define DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED(APIID,RETURNVALUE)                    do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE)                    do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_OR_FIM_IS_IN_INIT(APIID,RETURNVALUE)   do{DEM_ASSERT_ISNOTLOCKED(); if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && (Dem_FimState != DEM_FIM_STATE_IS_IN_INITIALIZATION))  \
                                                                                            {DEM_DET(APIID, DEM_E_UNINIT,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_OR_FIM_IS_IN_INIT_WITHEVENTID(EVTID,APIID,RETURNVALUE)   do{DEM_ASSERT_ISNOTLOCKED(); if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && (Dem_FimState != DEM_FIM_STATE_IS_IN_INITIALIZATION))  \
                                                                                            {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_DEM_PREINITIALIZED(APIID,RETURNVALUE)                 do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState == DEM_OPMO_STATE_NOTINITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_DEM_PREINITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE)                 do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState == DEM_OPMO_STATE_NOTINITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_VOID_API_DEM_PREINITIALIZED(APIID)                    do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState == DEM_OPMO_STATE_NOTINITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,0u); return ; }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_VOID_API_DEM_PREINITIALIZED_WITHEVENTID(EVTID,APIID)                    do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_OpMoState == DEM_OPMO_STATE_NOTINITIALIZED) {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return ; }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID(EVTID,APIID,RETURNVALUE)       do {DEM_ASSERT_ISNOTLOCKED(); if(!Dem_isEventIdValid(EVTID)) {DEM_DET(APIID, DEM_E_WRONG_CONFIGURATION,EVTID); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_VOID_API_EVENT_ID_VALID(EVTID,APIID)       do {DEM_ASSERT_ISNOTLOCKED(); if(!Dem_isEventIdValid(EVTID)) {DEM_DET(APIID, DEM_E_WRONG_CONFIGURATION,EVTID); return ; }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_COMPONENT_ID_VALID(COMPID,APIID,RETURNVALUE)  do {DEM_ASSERT_ISNOTLOCKED(); if (!Dem_NodeIdIsValid(COMPID)) {DEM_DET(APIID, DEM_E_WRONG_CONFIGURATION,0u); return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_EVENT_AVAILABLE(EVTID,APIID,RETURNVALUE)      do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_EvtIsSuppressed(EVTID)) {return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_VOID_API_EVENT_AVAILABLE(EVTID,APIID)      do {DEM_ASSERT_ISNOTLOCKED(); if(Dem_EvtIsSuppressed(EVTID)) {return ; }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE) do {    DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID(EVTID,APIID,RETURNVALUE);   \
                                                                                            DEM_ENTRY_CONDITION_CHECK_EVENT_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(POINTER,APIID,RETURNVALUE)       do {DEM_ASSERT_ISNOTLOCKED(); if((POINTER)==NULL_PTR) { DEM_DET(APIID, DEM_E_PARAM_POINTER,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR_WITHEVENTID(EVTID,POINTER,APIID,RETURNVALUE)       do {DEM_ASSERT_ISNOTLOCKED(); if((POINTER)==NULL_PTR) { DEM_DET(APIID, DEM_E_PARAM_POINTER,EVTID); return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_INIT_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)   do { DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                 DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)


#define DEM_ENTRY_CONDITION_CHECK_PREINIT_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE) do { DEM_ENTRY_CONDITION_CHECK_DEM_PREINITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                  DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_VOID_API_PREINIT_EVTIDVALID_EVTAVAILABLE(EVTID,APIID) do {   DEM_ENTRY_CONDITION_CHECK_VOID_API_DEM_PREINITIALIZED_WITHEVENTID(EVTID,APIID);    \
                                                                                                  DEM_ENTRY_CONDITION_CHECK_VOID_API_EVENT_ID_VALID(EVTID,APIID);  \
                                                                                                  DEM_ENTRY_CONDITION_CHECK_VOID_API_EVENT_AVAILABLE(EVTID,APIID); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_INIT_EVTIDVALID(EVTID,APIID,RETURNVALUE)                do { DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                 DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID(EVTID,APIID,RETURNVALUE); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_PREINIT_EVTIDVALID(EVTID,APIID,RETURNVALUE)                do { DEM_ENTRY_CONDITION_CHECK_DEM_PREINITIALIZED_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                 DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID(EVTID,APIID,RETURNVALUE); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_OPMO_ALLFAILUREINFOLOCKED(APIID,RETURNVALUE)  do { DEM_ASSERT_ISNOTLOCKED(); if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && (Dem_OpMoState != DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO)) \
                                                                                                    {DEM_DET(APIID, DEM_E_UNINIT,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_OPMO_ALLFAILUREINFOLOCKED_WITHEVENTID(EVTID,APIID,RETURNVALUE)  do { DEM_ASSERT_ISNOTLOCKED(); if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && (Dem_OpMoState != DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO)) \
                                                                                                    {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return (RETURNVALUE); }} while(0)

#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED(APIID,RETURNVALUE)  do { DEM_ASSERT_ISNOTLOCKED(); \
                                                                                                            if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && \
                                                                                                                (Dem_OpMoState != DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO) &&  \
                                                                                                                (Dem_FimState != DEM_FIM_STATE_IS_IN_INITIALIZATION))   \
                                                                                                            {DEM_DET(APIID, DEM_E_UNINIT,0u); return (RETURNVALUE); }} while(0)
#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED_WITHEVENTID(EVTID,APIID,RETURNVALUE)  do { DEM_ASSERT_ISNOTLOCKED(); \
                                                                                                            if((Dem_OpMoState != DEM_OPMO_STATE_INITIALIZED) && \
                                                                                                                (Dem_OpMoState != DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO) &&  \
                                                                                                                (Dem_FimState != DEM_FIM_STATE_IS_IN_INITIALIZATION))   \
                                                                                                            {DEM_DET(APIID, DEM_E_UNINIT,EVTID); return (RETURNVALUE); }} while(0)
#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_OPMO_ALLFAILUREINFOLOCKED_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)   do { DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_OPMO_ALLFAILUREINFOLOCKED_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                                            DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)

#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)    do { DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED_WITHEVENTID(EVTID,APIID,RETURNVALUE);   \
                                                                                                                                             DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)
#else
#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_OPMO_ALLFAILUREINFOLOCKED_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)    DEM_ENTRY_CONDITION_CHECK_INIT_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)

#define DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED_EVTIDVALID_EVTAVAILABLE(EVTID,APIID,RETURNVALUE)  do { DEM_ENTRY_CONDITION_CHECK_DEM_INITIALIZED_OR_FIM_IS_IN_INIT_WITHEVENTID(EVTID,APIID,RETURNVALUE);    \
                                                                                                                                            DEM_ENTRY_CONDITION_CHECK_EVENT_ID_VALID_AVAILABLE(EVTID,APIID,RETURNVALUE); } while(0)
#endif

#define DEM_ENTRY_CONDITION_CHECK_EVENT_ID_SUPPORT_FF_PRESTORAGE(EVTID,APIID,RETURNVALUE)  do {DEM_ASSERT_ISNOTLOCKED(); if (Dem_EvtParam_GetIsFFPrestorageSupported(EVTID)) {DEM_DET(APIID, DEM_E_WRONG_CONFIGURATION,EVTID); return (RETURNVALUE); }} while(0)

DEM_INLINE Dem_boolean_least Dem_OpMoIsInitialized(void)
{
   return (Dem_OpMoState == DEM_OPMO_STATE_INITIALIZED);
}

DEM_INLINE Dem_boolean_least Dem_Is_Fim_Initialized(void)
{
    return (Dem_FimState == DEM_FIM_STATE_INITIALIZED);
}

#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
DEM_INLINE Dem_boolean_least Dem_OpMoIsAllFailureInfoLocked(void)
{
   return (Dem_OpMoState == DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO);
}
#endif

DEM_INLINE Dem_boolean_least Dem_GetTestFailedInitState(void)
{
    return Dem_TestFailedStatusInitialized;
}

#endif
