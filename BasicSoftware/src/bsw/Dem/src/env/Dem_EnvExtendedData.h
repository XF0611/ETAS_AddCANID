

#ifndef DEM_ENVEXTENDEDDATA_H
#define DEM_ENVEXTENDEDDATA_H

#include "Dem_Types.h"
#include "Dem_EnvExtendedDataRec.h"
#include "Dem_Cfg_EnvExtendedData.h"
#include "Dem_Cfg_Main.h"
#include "Dem_InternalEnvData.h"

typedef struct
{
	uint16 extDataRecIndex;
	uint16 rawByteSize;
} Dem_EnvExtData;
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
extern const uint8 Dem_Cfg_EnvExtData2ExtDataRec[];
extern const Dem_EnvExtData Dem_Cfg_EnvExtData[DEM_CFG_ENVEXTDATA_ARRAYLENGTH];
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

DEM_INLINE void Dem_EnvEDCapture(uint8 extDataId, uint8* buffer, uint16 size, const Dem_InternalEnvData* internalEnvData)
{
	uint16_least i;
	const uint8* end = &buffer[size];

	for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
	{
		Dem_EnvEDRCapture (Dem_Cfg_EnvExtData2ExtDataRec[i], &buffer, end, internalEnvData);
	}

#if (DEM_CFG_BUILDTARGET==DEM_CFG_BUILDTARGET_DEMTESTSUITE)
	DEM_MEMSET(&buffer,0xFF,end-buffer);
#endif
}

DEM_INLINE uint16 Dem_EnvEDGetRawByteSize(uint8 extDataId)
{
	return Dem_Cfg_EnvExtData[extDataId].rawByteSize;
}

DEM_INLINE void Dem_EnvEDCopyRaw(uint8 extDataId, uint8* dest, uint16 bufsize, const uint8* src, Dem_EnvTriggerParamType* triggerParam)
{
	uint16_least i;
	const uint8* end = dest + bufsize;

	for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
	{
		Dem_EnvEDRCopyRaw (Dem_Cfg_EnvExtData2ExtDataRec[i], &dest, end, &src, triggerParam);
	}
}

DEM_INLINE Std_ReturnType Dem_EnvEDRetrieveExtendedData(uint8 extDataId,
		uint8* dest,
		uint16* bufsize,
		const uint8* src,
		const Dem_InternalEnvData* internalEnvData)
{
	uint8* writepos = dest;
	uint8* end = dest + *bufsize;
	uint16_least i;

	for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
	{
		if (!Dem_EnvEDRRetrieve (Dem_Cfg_EnvExtData2ExtDataRec[i], &writepos, end, &src, internalEnvData))
		{
			return DEM_BUFFER_TOO_SMALL;
		}
	}

	*bufsize = (uint16)(writepos - dest); /* current writing position - starting writing position */
	return E_OK;
}

DEM_INLINE Dem_boolean_least Dem_EnvEDIsRecordNumberValid(uint8 extDataId, uint8 RecordNumber, Dem_TriggerType* trigger, uint16* EDRIndex)
{
	uint16 i;

	if(extDataId != 0)
    {
        for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
        {
            if (Dem_EnvEDRGetRecordNumber(Dem_Cfg_EnvExtData2ExtDataRec[i]) == RecordNumber)
            {
                *trigger = Dem_EnvEDRGetRecordTrigger(Dem_Cfg_EnvExtData2ExtDataRec[i]);
                *EDRIndex = i;
                return TRUE;
            }
        }
    }
	return FALSE;
}

DEM_INLINE Std_ReturnType Dem_EnvEDRetrieveRawOrFormattedExtendedDataRecord(uint8 extDataId,
        uint8 RecordNumber,
        uint8* dest,
        uint16* bufsize,
        const uint8* src,
        const Dem_InternalEnvData* internalEnvData,
        boolean Raw)
{
    uint8* writepos = dest;
    uint8* end = dest + *bufsize;
    uint16_least i;

    if(! Raw)
    {
        if (*bufsize < 1)
        {
            return DEM_BUFFER_TOO_SMALL;
        }
        *writepos = RecordNumber; //Fill the first byte with Record Number.
        writepos++;
    }

    for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
    {
        if (Dem_EnvEDRGetRecordNumber (Dem_Cfg_EnvExtData2ExtDataRec[i]) == RecordNumber)
        {
            if (!Dem_EnvEDRRetrieve (Dem_Cfg_EnvExtData2ExtDataRec[i], &writepos, end, &src, internalEnvData))
            {
                return DEM_BUFFER_TOO_SMALL;
            }
            *bufsize = (uint16)(writepos - dest); /* current writing position - starting writing position */
            return E_OK;
        }
        else
        {
            Dem_EnvEDRSkipSrc (Dem_Cfg_EnvExtData2ExtDataRec[i], &src);
        }
    }
    return DEM_NO_SUCH_ELEMENT;
}

DEM_INLINE Std_ReturnType Dem_EnvEDGetSizeOfEDR(uint8 extDataId,
		uint8 RecordNumber,
		uint16* size)
{
	uint16_least i;

	for (i = Dem_Cfg_EnvExtData[extDataId - 1].extDataRecIndex; i < Dem_Cfg_EnvExtData[extDataId].extDataRecIndex; i++)
	{
		if (Dem_EnvEDRGetRecordNumber (Dem_Cfg_EnvExtData2ExtDataRec[i]) == RecordNumber)
		{
			*size = (uint16)(Dem_EnvEDRGetSize (Dem_Cfg_EnvExtData2ExtDataRec[i]) + 1);
			return E_OK;
		}
	}

	return DEM_NO_SUCH_ELEMENT;
}

#endif

