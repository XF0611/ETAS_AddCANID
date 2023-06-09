

#ifndef XCP_CBK_H
#define XCP_CBK_H

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"


/***************************************************************************************************
* Defines/Macros
***************************************************************************************************/


/***************************************************************************************************
* Type definitions
***************************************************************************************************/


/***************************************************************************************************
* Variables
***************************************************************************************************/


/***************************************************************************************************
* Function prototypes
***************************************************************************************************/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/* Checksum functions */
#if (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function triggers checksum calculation
\param[in]  XcpAddr           start address of memory block
\param[in]  BlockSize         size of memory block from which the checksum should be calculated
\param[in]  ProtocolLayerId   protocol layer id
\return     ErrorCode
\retval     XCP_NO_ERROR      calculation started
\retval     XCP_ERR_CMD_BUSY  another calculation in progress
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_BuildChecksumTrigger(const Xcp_AddrType_t XcpAddr, uint32 BlockSize, uint8 ProtocolLayerId);

/**
****************************************************************************************************
Function to process checksum calculation
\param[out] ChecksumPtr       pointer to where calculated checksum should be stored
\param[out] ChecksumType      type of the checksum calculated by the process
\param[in]  ProtocolLayerId   Protocol Layer Id
\return     ErorCode
\retval     XCP_NO_ERROR            calculation finished
\retval     XCP_ERR_ACCESS_DENIED   wrong calpage, addr, etc
\retval     XCP_ERR_CMD_BUSY        another calculation in progress
\retval     XCP_NO_RESPONSE         nothing to do
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_BuildChecksumMainFunction(uint32* ChecksumPtr, uint8* ChecksumType, uint8 ProtocolLayerId);
#endif

/* Page switching functions */
#if (XCP_CMD_SET_CAL_PAGE_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function sets active calibration page
\param[in]  Mode              mode given from the command (ECU, XCP, All)
\param[in]  SegNum            segment number
\param[in]  PageNum           calibration data page
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_PAGE_NOT_VALID      Page not valid
\retval     XCP_ERR_SEGMENT_NOT_VALID   Segment not valid
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_SetCalPage(uint8 Mode, uint8 SegNum, uint8 PageNum, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_GET_CAL_PAGE_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function gets active calibration page
\param[in]  Mode              mode given from the command (ECU, XCP, All)
\param[in]  SegNum            segment number
\param[out] PageNum           calibration data page
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_SEGMENT_NOT_VALID   Segment not valid
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_GetCalPage(uint8 Mode, uint8 SegNum, uint8* PageNum, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_COPY_CAL_PAGE_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function copies calibration page
\param[in]  SegNumSrc         Source Segment
\param[in]  PageNumSrc        Source Page
\param[in]  SegNumDst         Destination Segment
\param[in]  PageNumDst        Destination Page
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_PAGE_NOT_VALID      Page not valid
\retval     XCP_ERR_SEGMENT_NOT_VALID   Segment not valid
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_CopyCalPage(uint8 SegNumSrc, uint8 PageNumSrc, uint8 SegNumDst, uint8 PageNumDst, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_GET_PAG_PROCESSOR_INFO_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function to get the MAX_SEGMENT number as general information on paging.
\param[in]  ProtocolLayerId   Protocol Layer Id
\param[out] MaxSegment        Maximum segment number
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_CMD_UNKNOWN         Command not supported
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_GetPagProcessorInfo(uint8* MaxSegment, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_GET_SEGMENT_INFO_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function prepares response packet based on Mode for command GET_SEGMENT_INFO
\param[in]  XcpCmdPtr         Command pointer
\param[out] XcpResPtr         Response pointer
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_CMD_UNKNOWN         Command not supported
\retval     XCP_ERR_OUT_OF_RANGE        Mode or SegmentInfo or MappingIndex are out of range
\retval     XCP_ERR_SEGMENT_NOT_VALID   Segment not valid
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_GetSegmentInfo(const uint8* XcpCmdPtr, uint8* XcpResPtr, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_GET_PAGE_INFO_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function returns page information
\param[in]  SegmentNumber     Segment Number
\param[in]  PageNumber        Page Number
\param[out] PageProperties    Page Properties
\param[out] InitSegment       Init Segment
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR                success
\retval     XCP_ERR_PAGE_NOT_VALID      Page not valid
\retval     XCP_ERR_SEGMENT_NOT_VALID   Segment not valid
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_GetPageInfo(uint8 SegmentNumber, uint8 PageNumber, uint8* PageProperties, uint8* InitSegment, uint8 ProtocolLayerId);
#endif

/* Command received function */
#if (XCP_CMD_RECEIVED_NOTIFICATION == STD_ON)
/**
****************************************************************************************************
Function is called when a XCP CMD is received
\param[in]  XcpCmd            Command pointer
\param[in]  Length            Command Length
\param[in]  ProtocolLayerId   Protocol layer id
\return     -
***************************************************************************************************/
extern void XcpAppl_CmdReceived(const uint8* XcpCmd, uint8 Length, uint8 ProtocolLayerId);
#endif

/* Connection to transport layer function */
#if (XCP_CONNECT_TRANSPORT_LAYER_CALLOUT == STD_ON)
/**
****************************************************************************************************
Function is called to check the connection is allowed to TransportLayer or not
\attention  XcpTransportLayerId is used and not XcpProtocolLayerId!
\param[in]  XcpTransportLayerId   Transport Layer Id
\return     State if connection is allowed
\retval     E_OK      Connection is allowed
\retval     E_NOT_OK  Connection is not allowed
***************************************************************************************************/
extern Std_ReturnType XcpAppl_ConnectTransportLayer(uint8 XcpTransportLayerId);
#endif

/* GetId function */
#if (XCP_CMD_GET_ID_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function is called when identification type 0 to 3, 128 to 255 is requested by master.
\param[in]  IdType            Identification Type from CMD [0..3, 128..255]
\param[out] AddrPtr           Pointer to the address where identification shall be stored
\param[in]  ProtocolLayerId   Protocol layer id
\return     Length of identification
\retval     0  Id is not supported
***************************************************************************************************/
extern uint32 XcpAppl_GetIdSlave(uint8 IdType, uint8** AddrPtr, uint8 ProtocolLayerId);
#endif


#if (XCP_SCHEDULECALL_FLAG == STD_ON)
/* Function call-out to store current system time */
/**
****************************************************************************************************
This function is called at the start of the processing of a specific DAQ List. It shall perform the function
of storing the current system time into a counter variable. This counter will then be used by the
XcpAppl_TriggerSchedule function in order to calculate total execution time by the DAQ List.
This is done to ensure that in co-operative multitasking, high prioritized tasks will not be blocked for too long.
This function shall then reset the counters for the given event channel.

\param[in]  EventChannelNum   Current event channel number
\return     -
\retval     -
***************************************************************************************************/
extern void XcpAppl_EnterDaqList(uint16 EventChannelNum);


/* Function call-out to check if the executed runtime exceeds the limit and if Schedule call is required */
/**
****************************************************************************************************
This function is called at the beginning of the processing of each ODT. It shall perform the function
of run-time monitoring of each ODT and ensure that a call to the Scheduler is made if the processing time of the
current DAQ list exceeds a given time period. The time period shall be fixed by the product line which requires this feature.
This is done to ensure that in co-operative multitasking, high prioritized tasks will not be blocked for too long.
The function then calls the Scheduler if the above mentioned condition is met.

\param[in]  EventChannelNum   Current event channel number
\return     -
\retval     -
***************************************************************************************************/
extern void XcpAppl_TriggerSchedule(uint16 EventChannelNum);
#endif

/* Initialization function to initialize and cancel pending request */
#if (    (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON) \
     || ((XCP_CALIBRATION_CAL == STD_ON) && (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)) )
/**
****************************************************************************************************
Function to do specific initialization or cancel pending background process
\param[in]  XcpInitStatus     Xcp Initialization status: XCP_INIT or XCP_UNINIT
\param[in]  ProtocolLayerId   protocol layer id
\return     Was initialization successful
\retval     E_OK              Initialization is finished
\retval     E_NOT_OK          Initialization is not finished, this function will be called
                              in next Xcp_MainFunction() again until it returns E_OK
***************************************************************************************************/
extern Std_ReturnType XcpAppl_Init(uint8 XcpInitStatus, uint8 ProtocolLayerId);
#endif



/* Memory access protection functions */
#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
# if (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Check if the requested area is allowed to read for Build Checksum
\param[in]  XcpAddr       - Memory start address
\param[in]  Length        - Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
\retval     XCP_NO_ACCESS_HIDE      access denied, but correct response is sent with data content 0x00
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_ChecksumProtection(const Xcp_AddrType_t XcpAddr, uint32 Length);
# endif

/**
****************************************************************************************************
Check if is allowed to read from this area
\param[in]  XcpAddr       - Memory start address
\param[in]  Length        - Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
\retval     XCP_NO_ACCESS_HIDE      access denied, but correct response is sent with data content 0x00
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_MemReadProtection(const Xcp_AddrType_t XcpAddr, uint8 Length);
#endif

#if (XCP_MEMORY_WRITE_PROTECTION == STD_ON)
/**
****************************************************************************************************
Check if is allowed to write to this area
\param[in]  XcpAddr   Memory start address
\param[in]  Length    Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_MemWriteProtection(const Xcp_AddrType_t XcpAddr, uint8 Length);
#endif

/* Memory access functions */
#if (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)
/**
****************************************************************************************************
Copy data from ECU memory to the internal buffer
\param[in]  AddrPtrDst        destination address (Xcp-internal buffer)
\param[in]  XcpAddrSrc        source address (ECU memory)
\param[in]  Length            length of the data to process
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR              All Ok, process finished successfully
\retval     XCP_NO_RESPONSE           Process started, but not finished
\retval     XCP_ERR_MEMORY_OVERFLOW   Memory overflow
\retval     XCP_ERR_ACCESS_DENIED     Access denied
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_MemRead(uint8* AddrPtrDst, const Xcp_AddrType_t XcpAddrSrc, uint8 Length, uint8 ProtocolLayerId);

# if (XCP_CALIBRATION_CAL == STD_ON)
/**
****************************************************************************************************
Copy data from internal buffer to ECU memory
\param[in]  XcpAddrDst        destination address (ECU memory)
\param[in]  AddrPtrSrc        source address (Xcp-internal buffer)
\param[in]  Length            length of the data to process
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR              All Ok, process finished successfully
\retval     XCP_NO_RESPONSE           Process started, but not finished
\retval     XCP_ERR_MEMORY_OVERFLOW   Memory overflow
\retval     XCP_ERR_ACCESS_DENIED     Access denied
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_MemWrite(Xcp_AddrType_t XcpAddrDst, const uint8* AddrPtrSrc, uint8 Length, uint8 ProtocolLayerId);

/**
****************************************************************************************************
Copy data from internal buffer to ECU memory - background process
\param[in]  ProtocolLayerId   Protocol Layer Id
\retval     XCP_NO_ERROR              All Ok, process finished successfully
\retval     XCP_NO_RESPONSE           Process started, but not finished
\retval     XCP_ERR_MEMORY_OVERFLOW   Memory overflow
\retval     XCP_ERR_ACCESS_DENIED     Access denied
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_MemWriteMainFunction(uint8 ProtocolLayerId);
# endif
#endif

/* OdtEntry access functions */
#if (XCP_DAQ_STIM_MEMORY_ACCESS_BY_APPL == STD_ON)
/**
****************************************************************************************************
Copy the DAQ data from ECU memory to internal buffer
\param[in]  AddrPtrDst  destination address (Xcp-internal buffer)
\param[in]  XcpAddrSrc  source address (ECU memory)
\param[in]  Length      length of the data to read
\return     -
***************************************************************************************************/
extern void XcpAppl_OdtEntryRead(uint8* AddrPtrDst, const Xcp_AddrType_t XcpAddrSrc, uint8 Length);

# if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
/**
****************************************************************************************************
Copy the STIM data from internal buffer to ECU memory
\param[in]  XcpAddrDst  destination address (ECU memory)
\param[in]  AddrPtrSrc  source address (Xcp-internal buffer)
\param[in]  Length      length of the data to read
\return     -
***************************************************************************************************/
extern void XcpAppl_OdtEntryWrite(Xcp_AddrType_t XcpAddrDst, const uint8* AddrPtrSrc, uint8 Length);
# endif
#endif

#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
/**
****************************************************************************************************
Transforms the given address into another address (Mainly used for HBB)
\param[inout] XcpAddrPtr            Address pointer that initially holds the address that is to be translated
                                    and after successful transformation it holds the new (transformed) address
\param[in]    Length                Length of the data that are located on the given address
\param[in]    XcpEventChannelNumber Number of the Event-Channel this Address belongs to (XCP_EVENT_CHANNEL_NOT_USED if not DAQ/STIM)
\param[in]    contextType           Context from which this function is called (DAQ/STIM/UPLOAD/DOWNLOAD)
\return       Xcp_ErrorCode
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_StaticAddressTransformation(Xcp_AddrType_t* XcpAddrPtr, uint32 Length, uint16 XcpEventChannelNumber, Xcp_ContextType_t contextType);
#endif

/* Seed and Key functions */
#if (XCP_CMD_GET_SEED_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Template of function which returns seed for unlock command
\param[out] SeedAdrPtr        Address of pointer to the Seed
\param[in]  ResourceType      Type of the resource to be unlocked
\param[in]  ProtocolLayerId   Id of protocol layer which is requesting SeedAndKey procedure
\return     Length of the Seed
***************************************************************************************************/
extern uint8         XcpAppl_GetSeed(uint8** SeedAdrPtr, uint8 ResourceType, uint8 ProtocolLayerId);
#endif

#if (XCP_CMD_UNLOCK_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Template of function which checks received key if is matching to calculated one
\param[in]  KeyPtr                Pointer to unlock Key
\param[in]  KeyPartLength         Length of actually received part of the Ke
\param[in]  RemainingKeyLength    Remaining length of the unlock Key
\param[out] UnlockedResource      Pointer to UnlockedResource
\param[in]  ProtocolLayerId       Id of protocol layer which is requesting SeedAndKey procedure
\retval     XCP_ERR_ACCESS_LOCKED if key is invalid
\retval     XCP_ERR_OUT_OF_RANGE  if key is too long
\retval     XCP_ERR_SEQUENCE      if key is too short
\retval     XCP_NO_ERROR          if the full key was not received UnlockedResource = 0 otherwise it will be:
                                  - XCP_RESOURCE_CALPAG
                                  - XCP_RESOURCE_DAQ
                                  - XCP_RESOURCE_STIM
                                  - XCP_RESOURCE_PGM
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_Unlock(const uint8* KeyPtr, uint8 KeyPartLength, uint8 RemainingKeyLength, uint8* UnlockedResource, uint8 ProtocolLayerId);
#endif

/* Timestamp function */
#if (XCP_TIMESTAMP == STD_ON)
/**
****************************************************************************************************
Function returns the current value of the synchronous data transfer clock (timestamp). The data
transfer clock is a free running counter, which is never reset or modified and wraps around if an
overflow occurs. Behaviour of timestamp shall be according to configured "Timestamp Ticks" and
"Timestamp Unit".
\param[in]  -
\return     Timestamp
***************************************************************************************************/
extern uint32 XcpAppl_GetTimestamp(void);
#endif

/* User defined Command function*/
#if (XCP_CMD_USER_CMD_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function for user-defined command
\param[in]  XcpCmdPtr         Command pointer
\param[in]  CmdLength         Command Length
\param[out] XcpResPtr         Response pointer
\param[out] ResLength         Response Length
\param[in]  ProtocolLayerId   protocol layer id
\retval     XCP_NO_ERROR          success \n
\retval     XCP_ERR_CMD_UNKNOWN   Command not supported
\retval     XCP_ERR_CMD_SYNTAX    Command syntax invalid
\retval     XCP_ERR_OUT_OF_RANGE  Command parameter(s) out of range
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_UserCmd(const uint8* XcpCmdPtr, uint8 CmdLength, uint8* XcpResPtr, uint8* ResLength, uint8 ProtocolLayerId);
#endif

/* User defined Command function*/
#if (XCP_RESUME_MODE_FLASH_CHECK == STD_ON)
/**
****************************************************************************************************
Template of function which check if a new software is flahsed before the XCP initialization
The function is used in resume mode
\param[in]  -
\retval     TRUE          When a new software is flashed
\retval     FALSE         When no new software is flashed
***************************************************************************************************/
extern boolean XcpAppl_NewSwFlashed(void);
#endif


/* Resume mode functions */
#if (XCP_STORE_CAL_SUPPORTED == STD_ON)
/**
****************************************************************************************************
Function that shall implement the Resume mode - Store Calibration Request feature. Here the PL can store the
calibration data into memory that will retain the variable values even after power off.
\param[in]  -
\retval     XCP_NO_ERROR            calibration data stored successfully
\retval     XCP_ERR_ACCESS_DENIED   access denied
***************************************************************************************************/
extern Xcp_ErrorCode XcpAppl_StoreCalReq(void);
#endif



#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


#endif   /* #ifndef XCP_CBK_H */


