

#ifndef DEM_PRV_DET_H
#define DEM_PRV_DET_H

#include "Dem_Cfg_Main.h"
#include "Dem_Types.h"

extern Dem_EventIdType Dem_EventIdCausingLastDetError;

#if (DEM_CFG_BUILDTARGET == DEM_CFG_BUILDTARGET_DEMTESTSUITE)
    #include "rba_SimCUnit.h"
    #include "DemTest_Det.h"
    #define DEM_DET(APIID,ERRORID,EVENTID)   do {Dem_EventIdCausingLastDetError = EVENTID; DemTestDET::checkDetError(APIID,ERRORID);}while(0)
#else
    #if (DEM_CFG_DEVERRORDETECT == DEM_CFG_DEVERRORDETECT_ON)
    #include "Det.h"
    #define DEM_DET(APIID,ERRORID,EVENTID)    do {Dem_EventIdCausingLastDetError = EVENTID; Det_ReportError(DEM_MODULE_ID,DEM_INSTANCE_ID,APIID,ERRORID);}while(0)
    #else
    #define DEM_DET(APIID,ERRORID,EVENTID)   do { } while(0)
    #endif
#endif

#ifdef QAC
    #define DEM_ASSERT(C,APIID,ERRORID)  (void)(C)
#elif (DEM_CFG_BUILDTARGET == DEM_CFG_BUILDTARGET_DEMTESTSUITE)
    #define DEM_ASSERT(C,APIID,ERRORID)  assertTrue(C)
#else
    #define DEM_ASSERT(C,APIID,ERRORID)   do { if (!(C)) { DEM_DET(APIID,ERRORID,0); } } while(0)
#endif


/**********************************************************************************************************************
 * Development Errors
 **********************************************************************************************************************/
#define DEM_E_WRONG_CONFIGURATION   0x10
#define DEM_E_PARAM_POINTER         0x11
#define DEM_E_PARAM_DATA            0x12
#define DEM_E_PARAM_LENGTH          0x13
#define DEM_E_UNINIT                0x20
#define DEM_E_NODATAAVAILABLE       0x30
#define DEM_E_WRONG_CONDITION       0x40
#define DEM_E_INTERNAL              0x50
#define DEM_E_OUTOFTIME             0x60


/**********************************************************************************************************************
 * AUTOSAR Service IDs
 **********************************************************************************************************************/
#define DEM_DET_APIID_GETVERSIONINFO                            0x00
#define DEM_DET_APIID_PREINIT                                   0x01
#define DEM_DET_APIID_INIT                                      0x02
#define DEM_DET_APIID_SHUTDOWN                                  0x03
#define DEM_DET_APIID_SETEVENTSTATUS                            0x04
#define DEM_DET_APIID_RESETEVENTSTATUS                          0x05
#define DEM_DET_APIID_PRESTOREFREEZEFRAME                       0x06
#define DEM_DET_APIID_DEM_CLEARPRESTOREDFREEZEFRAME             0x07
#define DEM_DET_APIID_DEM_SETOPERATIONCYCLESTATE                0x08
#define DEM_DET_APIID_DEM_RESETEVENTDEBOUNCESTATUS              0x09
#define DEM_DET_APIID_DEM_GETEVENTSTATUS                        0x0A
#define DEM_DET_APIID_DEM_GETEVENTFAILED                        0x0B
#define DEM_DET_APIID_DEM_GETEVENTTESTED                        0x0C
#define DEM_DET_APIID_DEM_GETDTCOFEVENT                         0x0D
#define DEM_DET_APIID_DEM_GETSEVERITYOFDTC                      0x0E
#define DEM_DET_APIID_REPORTERRORSTATUS                         0x0F
#define DEM_DET_APIID_SETDTCFILTER                              0x13
#define DEM_DET_APIID_DEM_GETSTATUSOFDTC                        0x15
#define DEM_DET_APIID_GETDTCSTATUSAVAILABILITYMASK              0x16
#define DEM_DET_APIID_GETNUMBEROFFILTEREDDTC                    0x17
#define DEM_DET_APIID_GETNEXTFILTEREDDTC                        0x18
#define DEM_DET_APIID_GETDTCBYOCCURRENCETIME                    0x19
#define DEM_DET_APIID_DISABLEDTCRECORDUPDATE                    0x1A
#define DEM_DET_APIID_ENABLEDTCRECORDUPDATE                     0x1B
#define DEM_DET_APIID_GETFREEZEFRAMEDATABYRECORD                0x1C
#define DEM_DET_APIID_GETNEXTFREEZEFRAMEDATA                    0x1D
#define DEM_DET_APIID_GETSIZEOFFREEZEFRAMESELECTION             0x1F
#define DEM_DET_APIID_GETNEXTEXTENDEDDATARECORD                 0x20
#define DEM_DET_APIID_GETSIZEOFEXTENDEDDATARECORDSELECTION      0x21
#define DEM_DET_APIID_CLEARDTC                                  0x23
#define DEM_DET_APIID_DISABLEDTCSETTING                         0x24
#define DEM_DET_APIID_ENABLEDTCSETTING                          0x25
#define DEM_DET_APIID_DEM_GETINDICATORSTATUS                    0x29
#define DEM_DET_APIID_DEM_GETCOMPONENTFAILED                    0x2A
#define DEM_DET_APIID_DEM_SETCOMPONENTAVAILABLE                 0x2B
#define DEM_DET_APIID_GETEVENTEXTENDEDDATARECORD                0x30
#define DEM_DET_APIID_GETEVENTFREEZEFRAMEDATA                   0x31
#define DEM_DET_APIID_GETEVENTMEMORYOVERFLOW                    0x32
#define DEM_DET_APIID_DEM_SETDTCSUPPRESSION                     0x33
#define DEM_DET_APIID_DEM_GETFUNCTIONAlUNITOFDTC                0x34
#define DEM_DET_APIID_DEM_EVMEMGETNUMBEROFEVENTENTRIES          0x35
#define DEM_DET_APIID_DEM_SETEVENTAVAILABLE                     0x37
#define DEM_DET_APIID_SETSTORAGECONDITION                       0x38
#define DEM_DET_APIID_SETENABLECONDITION                        0x39
#define DEM_DET_APIID_GETNEXTFILTEREDRECORD                     0x3A
#define DEM_DET_APIID_GETNEXTFILTEREDDTCANDFDC                  0x3B
#define DEM_DET_APIID_GETTRANSLATIONTYPE                        0x3C
#define DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY             0x3D
#define DEM_DET_APIID_GETFAULTDETECTIONCOUNTER                  0x3E
#define DEM_DET_APIID_SETFREEZEFRAMERECORDFILTER                0x3F
#define DEM_DET_APIID_SETCYCLEQUALIFIED                         0x56
#define DEM_DET_APIID_DEM_GETEVENTEXTENDEDDATARECORDEX          0x6D
#define DEM_DET_APIID_DEM_GETEVENTFREEZEFRAMEDATAEX             0x6E
#define DEM_DET_APIID_DEM_SETWIRSTATUS                          0x7A
#define DEM_DET_APIID_DEM_J1939DcmClearDTC                      0x95
#define DEM_DET_APIID_DEM_J1939DCMSETFREEZEFRAMEFILTER          0x96
#define DEM_DET_APIID_DEM_J1939DCMGETNEXTFREEZEFRAME            0x97
#define DEM_DET_APIID_DEM_GETOPERATIONCYCLESTATE                0x9E
#define DEM_DET_APIID_DEM_GETDEBOUNCINGOFEVENT                  0x9F
#define DEM_DET_APIID_GETCYCLEQUALIFIED                         0xAB
#define DEM_DET_APIID_GETDTCSEVERITYAVAILABILITYMASK            0xB2
#define DEM_DET_APIID_DEM_SELECTDTC                             0xB7
#define DEM_DET_APIID_DEM_GETDTCSELECTIONRESULT                 0xB8
#define DEM_DET_APIID_SELECTFREEZEFRAMEDATA                     0xB9
#define DEM_DET_APIID_SELECTEXTENDEDDATARECORD                  0xBA
#define DEM_DET_APIID_DEM_GETDTCSELECTIONRESULTFORCLEAR         0xBB
#define DEM_DET_APIID_DEM_GETDTCSUPPRESSION                     0xBC

/**********************************************************************************************************************
 * Internal Service IDs 0xA0 - 0xFF
 **********************************************************************************************************************/
#define DEM_DET_APIID_DEBCALLFILTER                             0xA0
#define DEM_DET_APIID_DEBMAINFUNCTION                           0xA1
#define DEM_DET_APIID_ENVDARETRIEVE                             0xA2
#define DEM_DET_APIID_ENVGETSIZEOFEDR                           0xA3
#define DEM_DET_APIID_ENVGETSIZEOFEDR_ALLOROBDSTORED            0xA4
#define DEM_DET_APIID_ENVRETRIEVEEDR                            0xA5
#define DEM_DET_APIID_ENVRETRIEVEFF                             0xA6
#define DEM_DET_APIID_ENVDACAPTURE                              0xA7
#define DEM_DET_APIID_ENVDASKIP                                 0xA8
#define DEM_DET_APIID_ENVDAUPDATE                               0xA9
#define DEM_DET_APIID_ENVEDCOPYRAW                              0xAA
#define DEM_DET_APIID_ENVCAPTUREED                              0xAB
#define DEM_DET_APIID_ENVCAPTUREFF                              0xAC
#define DEM_DET_APIID_ENVCOPYRAWFF                              0xAD
#define DEM_DET_APIID_ENVFFCOPYRAW                              0xAE
#define DEM_DET_APIID_ENVUPDATERAWED                            0xAF
#define DEM_DET_APIID_EVTSRESTOREFAILUREFROMPREVIOUSIC          0xB0
#define DEM_DET_APIID_EVMEMERASEEVENTMEMORY                     0xB1
#define DEM_DET_APIID_EVMEMSETEVENTFAILED                       0xB5
#define DEM_DET_APIID_EVMEMGETEVENTMEMORYLOCIDOFEVENT           0xB6
/* 0xB7, 0xB8, 0xB9, 0xBA and 0xBB shall not be used since it is used by AUTOSAR interface */
#define DEM_DET_APIID_EVMEMNVMREADEVENTMEMORYINIT               0xBC
#define DEM_DET_APIID_EVENTIDLISTITERATOR                       0xBD
#define DEM_DET_APIID_EVMEMCOPYTOMIRRORMEMORY                   0xBE
#define DEM_DET_APIID_EVMEMCLEARSHADOWMEMORY                    0xBF
#define DEM_DET_APIID_EVMEMGETSHADOWMEMORYLOCIDOFDTC            0xC0
#define DEM_DET_APIID_EVMEMGETEVENTMEMORYSTATUSOFEVENT          0xC1
#define DEM_DET_APIID_DEM_EVMEMGETEVENTMEMID                    0xC2
/* FC_VariationPoint_START */
#define DEM_DET_APIID_OBDENVCAPTUREFF                           0xC3
#define DEM_DET_APIID_OBDENVFFCOPYRAW                           0xC4
/* FC_VariationPoint_END */
#define DEM_DET_APIID_DTCGROUPIDIDLISTITERATOR                  0xC5
#define DEM_DET_APIID_EVENTDEPENDENCY                           0xC6
/* FC_VariationPoint_START */
#define DEM_DET_APIID_BFM                                       0xC7
#define DEM_DET_APIID_BFM_BUFFER                                0xC8
#define DEM_DET_APIID_BFM_RECORD                                0xC9
/* FC_VariationPoint_END */
#define DEM_DET_APIID_DISTMEMORY                                0xCA
#define DEM_DET_APIID_ENVGETINDEXFROMFFRECNUM                   0xCB
#define DEM_DET_APIID_ENVGETFFRECNUMFROMINDEX                   0xCC
#define DEM_DET_APIID_ENVGETINDEXOFCONFFFREC                    0xCD
#define DEM_DET_APIID_GETHISTORYSTATUS                          0xCE
#define DEM_DET_APIID_SERIALIZATION                             0xCF
#define DEM_DET_APIID_EVMEMSETEVENTUNROBUST                     0xD0
/* FC_VariationPoint_START */
#define DEM_DET_APIID_BFM_EXT_RECORD                            0xD1
/* FC_VariationPoint_END */
#define DEM_DET_APIID_REPORERRORSTATUSQUEUE                     0xD2
#define DEM_DET_APIID_NVMSTATEMACHINE                           0xD3
#define DEM_DET_APIID_EVENTDEPENDENCIES_ISCAUSAL                0xE1
#define DEM_DET_APIID_EVENTDEPENDENCIES                         0xE2
#define DEM_DET_APIID_EVMEMCLEAREVENT                           0xE3
#define DEM_DET_APIID_J1939DCMNODEIDLISTITERATOR                0xE4
#define DEM_DET_APIID_EVMEMSETEVENTPASSED                       0xE5
#define DEM_DET_APIID_J1939DCMSETDTCFILTER                      0xE6
#define DEM_DET_APIID_J1939DCMGETNUMBEROFFILTEREDDTC            0xE7
#define DEM_DET_APIID_J1939DCMGETNEXTFILTEREDDTC                0xE8
#define DEM_DET_APIID_J1939ENVRETRIEVEFF                        0xE9
#define DEM_DET_APIID_J1939ENVRETRIEVEEXPFF                     0xEA
#define DEM_DET_APIID_J1939ENVCOPYRAWFF                         0xEB
#define DEM_DET_APIID_J1939ENVCOPYRAWEXPFF                      0xEC
#define DEM_DET_APIID_J1939ENVFFCOPYRAW                         0xED
#define DEM_DET_APIID_J1939ENVCAPTUREFF                         0xEE
#define DEM_DET_APIID_J1939ENVCAPTUREEXPFF                      0xEF
#define DEM_DET_APIID_J1939DCMGETNEXTDTCWITHLAMPSTATUS          0xF0
#define DEM_DET_APIID_CLIENT_OPERATION                          0xF1
#define DEM_DET_APIID_EVMEMSTARTOPERATIONCYCLE                  0xF2
#define DEM_DET_APIID_EVMEMGETNVMIDFROMLOCID                    0xF3
#define DEM_DET_APIID_EVMEMSTARTAGINGCYCLE                      0xF4
#define DEM_DET_APIID_EVMEMGETEVENTMEMORYSTATUSOFDTC            0xF5
#define DEM_DET_APIID_GETENABLECONDITION                        0xF6
#define DEM_DET_APIID_RESETCYCLEQUALIFIED                       0xF7
#define DEM_DET_APIID_REPORTUNROBUSTQUEUEOVERFLOW               0xF8
#define DEM_DET_APIID_EVENTDATANOTCAPTURED                      0xF9
#define DEM_DET_APIID_CLIENTIDITERATOR                          0xFA
#endif /* DEM_PRV_DET_H */
