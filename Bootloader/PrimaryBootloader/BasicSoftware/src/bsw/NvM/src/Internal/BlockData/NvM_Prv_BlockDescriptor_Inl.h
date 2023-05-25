
#ifndef NVM_PRV_BLOCKDESCRIPTOR_INL_H
#define NVM_PRV_BLOCKDESCRIPTOR_INL_H

/*
**********************************************************************************************************************
* Inline functions declarations
**********************************************************************************************************************
*/
LOCAL_INLINE boolean NvM_Prv_IsBlockSelected(NvM_BlockIdType idBlock_uo,
                                             NvM_Prv_BlockConfiguration_ten SelectionMask_en);
LOCAL_INLINE boolean NvM_Prv_IsDefaultDataAvailable(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE boolean NvM_Prv_IsBlockLengthValid(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE boolean NvM_Prv_HasBlockImmediateJobPriority(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_GetBlockSize(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetBlockSizeStored(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE NvM_BlockManagementType NvM_Prv_GetBlockType(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint8 NvM_Prv_GetNrNonVolatileBlocks(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_GetNrDataIndexes(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE void * NvM_Prv_GetPRamBlockAddress(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE void const* NvM_Prv_GetRomBlockAddress(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_GetIdBlockMemIf(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint8 NvM_Prv_GetIdxDevice(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE NvM_Prv_Crc_Type_ten NvM_Prv_GetCrcType(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_GetIdxRamBlockCrc(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_GetIdPersistent(uint16 idxPersistentId_u16);
LOCAL_INLINE NvM_BlockIdType NvM_Prv_GetIdBlock(uint16 idxPersistentId_u16);
LOCAL_INLINE NvM_Prv_ExplicitSync_Copy_tpfct NvM_Prv_GetCopyFctForRead(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE NvM_Prv_ExplicitSync_Copy_tpfct NvM_Prv_GetCopyFctForWrite(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE Std_ReturnType NvM_Prv_InvokeInitBlockCallback(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE void NvM_Prv_InvokeSingleBlockStartCallback(NvM_BlockIdType idBlock_uo,
                                                         NvM_Prv_idService_tuo idService_uo);
LOCAL_INLINE void NvM_Prv_InvokeSingleBlockCallback(NvM_BlockIdType idBlock_uo,
                                                    NvM_Prv_idService_tuo idService_uo,
                                                    NvM_RequestResultType Result_uo);
LOCAL_INLINE void NvM_Prv_InvokeObserverCallback(NvM_BlockIdType idBlock_uo,
                                                 NvM_Prv_idService_tuo idService_uo,
                                                 NvM_RequestResultType Result_uo);
LOCAL_INLINE void NvM_Prv_InvokeMultiStartCallback(NvM_Prv_idService_tuo idService_uo);
LOCAL_INLINE void NvM_Prv_InvokeMultiCallback(NvM_Prv_idService_tuo idService_uo,
                                              NvM_RequestResultType Result_uo);

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetPersistantId(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsm(NvM_BlockIdType idBlock_uo,
                                                    NvM_Prv_Crypto_idService_ten idServiceCrypto_en);
LOCAL_INLINE uint32 NvM_Prv_BlkDesc_GetIdJobCsm(NvM_BlockIdType idBlock_uo,
                                                NvM_Prv_Crypto_idService_ten idServiceCrypto_en);
LOCAL_INLINE uint8 const* NvM_Prv_BlkDesc_GetDataJobCsmAssociated(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsmAssociated(NvM_BlockIdType idBlock_uo);
LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsmTag(NvM_BlockIdType idBlock_uo);

#endif

/*
**********************************************************************************************************************
* Inline functions
**********************************************************************************************************************
*/
/**
 * \brief
 * This NvM private function provides information whether a block is configured for the given feature.
 *
 * \param idBlock_uo
 * ID of the block for which the configuration information will be provided.
 * \param SelectionMask_en
 * Bit mask for the configuration information to be provide.
 *
 * \return
 * - TRUE = block is configured for the given feature
 * - FALSE = block is not configured for the given feature
 */
LOCAL_INLINE boolean NvM_Prv_IsBlockSelected(NvM_BlockIdType idBlock_uo,
                                             NvM_Prv_BlockConfiguration_ten SelectionMask_en)
{
    return ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
            ((NvM_Prv_BlockDescriptors_acst[idBlock_uo].stFlags_uo & ((uint32)SelectionMask_en)) != 0u));
}

/**
 * \brief
 * This NvM private function provides the information whether default data is available for the given block.
 *
 * \param idBlock_uo
 * ID of the block to provide the availability of the default data.
 *
 * \return
 * - TRUE = default data is available
 * - FALSE = default data is not available
 */
LOCAL_INLINE boolean NvM_Prv_IsDefaultDataAvailable(NvM_BlockIdType idBlock_uo)
{
    return ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
            ((NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].adrRomBlock_pcv) ||
             (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].InitBlockCallback_pfct)));
}

/**
 * \brief
 * This NvM private function checks for the given block whether the configured block length is valid.
 *
 * \param idBlock
 * ID of the block for which the block length will be checked.
 *
 * \return
 * - TRUE = block length is valid
 * - FALSE = block length is invalid
 */
LOCAL_INLINE boolean NvM_Prv_IsBlockLengthValid(NvM_BlockIdType idBlock_uo)
{
    return ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
            (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytes_pu16) &&
            (0u != *NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytes_pu16));
}

/**
 * \brief
 * This NvM private function checks whether the given block is configured with immediate priority.
 *
 * \param idBlock_uo
 * ID of the block for which the priority will be checked.
 *
 * \return
 * - TRUE = block is configured with immediate priority
 * - FALSE = block is configured with standard priority
 */
LOCAL_INLINE boolean NvM_Prv_HasBlockImmediateJobPriority(NvM_BlockIdType idBlock_uo)
{
    boolean HasBlockImmediateJobPriority_b = FALSE;

#if (NVM_JOB_PRIORITIZATION == STD_ON)
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NVM_PRV_JOB_PRIORITY_IMMEDIATE == NvM_Prv_BlockDescriptors_acst[idBlock_uo].JobPriority_u8))
    {
        HasBlockImmediateJobPriority_b = TRUE;
    }
#else
    (void)idBlock_uo;
#endif
    return HasBlockImmediateJobPriority_b;
}

/**
 * \brief
 * This NvM private function returns the configured block size for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured block size will be returned.
 *
 * \return
 * Configured block size
 */
LOCAL_INLINE uint16 NvM_Prv_GetBlockSize(NvM_BlockIdType idBlock_uo)
{
    uint16 BlockSize_u16 = 0u;

    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytes_pu16))
    {
        BlockSize_u16 = *NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytes_pu16;
    }

    return BlockSize_u16;
}

/**
 * \brief
 * This NvM private function returns the configured block size stored on the medium for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured block size stored on the medium will be returned.
 *
 * \return
 * Configured block size stored on the medium
 */
LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetBlockSizeStored(NvM_BlockIdType idBlock_uo)
{
    uint16 BlockSizeStored_u16 = 0u;

    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
#if (defined(NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG) && (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON))
        BlockSizeStored_u16 = *NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytes_pu16;
#else
        BlockSizeStored_u16 = NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrBlockBytesStored_u16;
#endif
    }

    return BlockSizeStored_u16;
}

/**
 * \brief
 * This NvM private function returns the configured block management type for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured block management type will be returned.
 *
 * \return
 * Configured block management type
 */
LOCAL_INLINE NvM_BlockManagementType NvM_Prv_GetBlockType(NvM_BlockIdType idBlock_uo)
{
    NvM_BlockManagementType BlockType = NVM_BLOCK_NATIVE;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        BlockType = NvM_Prv_BlockDescriptors_acst[idBlock_uo].BlockManagementType_en;
    }
    return BlockType;
}

/**
 * \brief
 * This NvM private function returns the configured number of non-volatile data sets for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured number of non-volatile data sets will be returned.
 *
 * \return
 * Configured number of non-volatile data sets
 */
LOCAL_INLINE uint8 NvM_Prv_GetNrNonVolatileBlocks(NvM_BlockIdType idBlock_uo)
{
    uint8 nrNvBlocks_u8 = 0u;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        nrNvBlocks_u8 = NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrNvBlocks_u8;
    }
    return nrNvBlocks_u8;
}

/**
 * \brief
 * This NvM private function returns the configured overall number of data sets for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured overall number of data sets will be returned.
 *
 * \return
 * Configured number of data sets
 */
LOCAL_INLINE uint16 NvM_Prv_GetNrDataIndexes(NvM_BlockIdType idBlock_uo)
{
    uint8 nrDataIndexes = 0u;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        nrDataIndexes = (NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrNvBlocks_u8 +
                         NvM_Prv_BlockDescriptors_acst[idBlock_uo].nrRomBlocks_u8);
    }
    return nrDataIndexes;
}

/**
 * \brief
 * This NvM private function returns the pointer to the configured permanent RAM block for the given block.
 *
 * \details
 * If no permanent RAM block is configured for a block then this function returns a NULL pointer.
 *
 * \param idBlock_uo
 * ID of the block for which the pointer to the configured permanent RAM block will be returned.
 *
 * \return
 * Pointer to the configured permanent RAM block
 */
LOCAL_INLINE void * NvM_Prv_GetPRamBlockAddress(NvM_BlockIdType idBlock_uo)
{
    void * PRamBlockAddress_pv = NULL_PTR;
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].adrRamBlock_ppv))
    {
        PRamBlockAddress_pv = *NvM_Prv_BlockDescriptors_acst[idBlock_uo].adrRamBlock_ppv;
    }

    return PRamBlockAddress_pv;
}

/**
 * \brief
 * This NvM private function returns the pointer to the configured ROM block for the given block.
 *
 * \details
 * If no ROM block is configured for a block then this function returns a NULL pointer.
 *
 * \param idBlock_uo
 * ID of the block for which the pointer to the configured ROM block will be returned.
 *
 * \return
 * Pointer to the configured ROM block
 */
LOCAL_INLINE void const* NvM_Prv_GetRomBlockAddress(NvM_BlockIdType idBlock_uo)
{
    void const* adrRomBlock_pcv = NULL_PTR;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        adrRomBlock_pcv = NvM_Prv_BlockDescriptors_acst[idBlock_uo].adrRomBlock_pcv;
    }
    return adrRomBlock_pcv;
}

/**
 * \brief
 * This NvM private function returns the configured block ID used by the mem interface for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the block ID used by the mem interface will be returned.
 *
 * \return
 * Configured block ID used by the mem interface
 */
LOCAL_INLINE uint16 NvM_Prv_GetIdBlockMemIf(NvM_BlockIdType idBlock_uo)
{
    uint16 idBlockMemIf_u16 = 0u;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        idBlockMemIf_u16 = NvM_Prv_BlockDescriptors_acst[idBlock_uo].idBlockMemIf_u16;
    }
    return idBlockMemIf_u16;
}

/**
 * \brief
 * This NvM private function returns the configured device index where the given block is located (Fee / Ea).
 *
 * \param idBlock_uo
 * ID of the block for which the configured device index will be returned.
 *
 * \return
 * Configured device index where the given block is located
 */
LOCAL_INLINE uint8 NvM_Prv_GetIdxDevice(NvM_BlockIdType idBlock_uo)
{
    uint8 idxDevice_u8 = 0u;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        idxDevice_u8 = NvM_Prv_BlockDescriptors_acst[idBlock_uo].idxDevice_u8;
    }
    return idxDevice_u8;
}

/**
 * \brief
 * This NvM private function returns the configured CRC type for the given block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured device index will be returned.
 *
 * \return
 * Configured CRC type
 */
LOCAL_INLINE NvM_Prv_Crc_Type_ten NvM_Prv_GetCrcType(NvM_BlockIdType idBlock_uo)
{
    NvM_Prv_Crc_Type_ten TypeCrc_en = NvM_Prv_Crc_Type_NoCrc_e;
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        TypeCrc_en = NvM_Prv_BlockDescriptors_acst[idBlock_uo].TypeCrc_en;
    }
#else
    (void)idBlock_uo;
#endif
    return TypeCrc_en;
}

/**
 * \brief
 * This NvM private function returns the configured index of the RAM block CRC for the given block.
 *
 * If CRC is generally disabled by configuration then this function always returns 0.
 *
 * \attention
 * The user shall make sure that return value of this function is used only if RAM block CRC is configured.
 *
 * \param[in] idBlock_uo
 * ID of the block for which the configured index of the RAM block CRC will be returned.
 *
 * \return
 * Configured index of the RAM block CRC
 */
LOCAL_INLINE uint16 NvM_Prv_GetIdxRamBlockCrc(NvM_BlockIdType idBlock_uo)
{
    uint16 idxRamBlockCrc_u16 = 0u;
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        idxRamBlockCrc_u16 = NvM_Prv_BlockDescriptors_acst[idBlock_uo].idxRamBlockCrc_u16;
    }
#else
    (void)idBlock_uo;
#endif
    return idxRamBlockCrc_u16;
}

/**
 * \brief
 * This NvM private function returns the configured persistent ID of a block identified by index for persistent IDs.
 *
 * \param idxPersistentId_u16
 * Index of the required persistent ID.
 *
 * \return
 * Configured persistent ID of a block identified by index for persistent IDs
 */
LOCAL_INLINE uint16 NvM_Prv_GetIdPersistent(uint16 idxPersistentId_u16)
{
    uint16 PersistentId_u16 = 0u;
    if (idxPersistentId_u16 < NVM_PRV_NR_PERSISTENT_IDS)
    {
        PersistentId_u16 = NvM_Prv_PersId_BlockId_acst[idxPersistentId_u16].PersistentId_u16;
    }
    return PersistentId_u16;
}

/**
 * \brief
 * This NvM private function returns the configured NvM ID of a block identified by index for persistent IDs.
 *
 * \param idxPersistentId_u16
 * Index of the required persistent ID.
 *
 * \return
 * Configured NvM ID of a block identified by index for persistent IDs
 */
LOCAL_INLINE NvM_BlockIdType NvM_Prv_GetIdBlock(uint16 idxPersistentId_u16)
{
    uint16 BlockId_u16 = 0u;
    if (idxPersistentId_u16 < NVM_PRV_NR_PERSISTENT_IDS)
    {
        BlockId_u16 = NvM_Prv_PersId_BlockId_acst[idxPersistentId_u16].BlockId_u16;
    }
    return BlockId_u16;
}

/**
 * \brief
 * This NvM private function returns the configured pointer to the block specific callback function
 * to copy data from the NvM mirror to the application's RAM block.
 *
 * \param idBlock_uo
 * ID of the block for which the configured copy function will be returned.
 *
 * \return
 * Configured pointer to the copy function
 */
LOCAL_INLINE NvM_Prv_ExplicitSync_Copy_tpfct NvM_Prv_GetCopyFctForRead(NvM_BlockIdType idBlock_uo)
{
    NvM_Prv_ExplicitSync_Copy_tpfct ReadRamBlockFromNvm_pfct = NULL_PTR;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        ReadRamBlockFromNvm_pfct = NvM_Prv_BlockDescriptors_acst[idBlock_uo].ReadRamBlockFromNvm_pfct;
    }
    return ReadRamBlockFromNvm_pfct;
}

/**
 * \brief
 * This NvM private function returns the configured pointer to the block specific callback function
 * to copy data from the application's RAM block to the NvM mirror.
 *
 * \param idBlock_uo
 * ID of the block for which the configured copy function will be returned.
 *
 * \return
 * Configured pointer to the copy function
 */
LOCAL_INLINE NvM_Prv_ExplicitSync_Copy_tpfct NvM_Prv_GetCopyFctForWrite(NvM_BlockIdType idBlock_uo)
{
    NvM_Prv_ExplicitSync_Copy_tpfct WriteRamBlockToNvm_pfct = NULL_PTR;
    if (idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
        WriteRamBlockToNvm_pfct = NvM_Prv_BlockDescriptors_acst[idBlock_uo].WriteRamBlockToNvm_pfct;
    }
    return WriteRamBlockToNvm_pfct;
}

/**
 * \brief
 * This NvM private function returns the configured block specific initializaton callback function.
 * \details
 * If no initializaton callback function is configured for the passed block then this function does nothing and
 * returns E_NOT_OK.
 *
 * \param idBlock_uo
 * ID of the block for which the configured initialization callback function will be invoked.
 *
 * \return
 * E_OK = no initialization callback function is configured or has returned E_OK
 * E_NOT_OK = initialization callback function is configured and has returned E_NOT_OK
 */
LOCAL_INLINE Std_ReturnType NvM_Prv_InvokeInitBlockCallback(NvM_BlockIdType idBlock_uo)
{
    Std_ReturnType RetValue = E_OK;
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].InitBlockCallback_pfct))
    {
        RetValue = NvM_Prv_BlockDescriptors_acst[idBlock_uo].InitBlockCallback_pfct();
    }
    return RetValue;
}

/**
 * \brief
 * This NvM private function invokes the configured block specific single request start callback function.
 * \details
 * If no single request start callback function is configured for the passed block then this function does nothing.
 *
 * \param idBlock_uo
 * ID of the block for which the configured single request start callback function will be invoked.
 * \param idService_uo
 * Id of the request for which the start callback function will be invoked.
 */
LOCAL_INLINE void NvM_Prv_InvokeSingleBlockStartCallback(NvM_BlockIdType idBlock_uo,
                                                         NvM_Prv_idService_tuo idService_uo)
{
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].SingleBlockStartCallback_pfct))
    {
        // Start callback function returns allways E_OK so return value can be dropped safely
        (void)(NvM_Prv_BlockDescriptors_acst[idBlock_uo].SingleBlockStartCallback_pfct)(idService_uo);
    }
}

/**
 * \brief
 * This NvM private function invokes the configured block specific single request termination callback function.
 * \details
 * If no single request termination callback function is configured for the passed block then this function does nothing.
 *
 * \param idBlock_uo
 * ID of the block for which the configured termination callback function will be invoked.
 * \param idService_uo
 * Id of the request for which the termination callback function will be invoked.
 * \param Result_uo
 * Result of the terminated request.
 */
LOCAL_INLINE void NvM_Prv_InvokeSingleBlockCallback(NvM_BlockIdType idBlock_uo,
                                                    NvM_Prv_idService_tuo idService_uo,
                                                    NvM_RequestResultType Result_uo)
{
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
        (NULL_PTR != NvM_Prv_BlockDescriptors_acst[idBlock_uo].SingleBlockCallback_pfct))
    {
        // Termination callback function returns allways E_OK so return value can be dropped safely
        (void)(NvM_Prv_BlockDescriptors_acst[idBlock_uo].SingleBlockCallback_pfct)(idService_uo, Result_uo);
    }
}

/**
 * \brief
 * This NvM private function invokes the configured observer callback function.
 * \details
 * If no observer callback function is configured for the NvM then this function does nothing.
 *
 * \param idBlock_uo
 * ID of the block to be passed to the configured observer callback function.
 * \param idService_uo
 * Id of the request to be passed to the configured observer callback function.
 * \param Result_uo
 * Result of the request to be passed to the configured observer callback function.
 */
LOCAL_INLINE void NvM_Prv_InvokeObserverCallback(NvM_BlockIdType idBlock_uo,
                                                 NvM_Prv_idService_tuo idService_uo,
                                                 NvM_RequestResultType Result_uo)
{
    if (NULL_PTR != NvM_Prv_Common_cst.ObserverCallback_pfct)
    {
        // Observer callback function returns allways E_OK so return value can be dropped safely
        (void)(NvM_Prv_Common_cst.ObserverCallback_pfct)(idBlock_uo, idService_uo, Result_uo);
    }
}

/**
 * \brief
 * This NvM private function invokes the configured multi-block request start callback function.
 * \details
 * If no multi-block request start callback function is configured then this function does nothing.
 *
 * \param idService_uo
 * Id of the request for which the start callback function will be invoked.
 */
LOCAL_INLINE void NvM_Prv_InvokeMultiStartCallback(NvM_Prv_idService_tuo idService_uo)
{
    if (NULL_PTR != NvM_Prv_Common_cst.RbMultiBlockStartCallback_pfct)
    {
        NvM_Prv_Common_cst.RbMultiBlockStartCallback_pfct(idService_uo);
    }
}

/**
 * \brief
 * This NvM private function invokes the configured multi-block request termination callback function.
 * \details
 * If no multi-block request termination callback function is configured then this function does nothing.
 *
 * \param idService_uo
 * Id of the request to be passed to the configured multi-block request termination callback function.
 * \param Result_uo
 * Result of the request to be passed to the configured multi-block request termination callback function.
 */
LOCAL_INLINE void NvM_Prv_InvokeMultiCallback(NvM_Prv_idService_tuo idService_uo,
                                              NvM_RequestResultType Result_uo)
{
    if (NULL_PTR != NvM_Prv_Common_cst.MultiBlockCallback_pfct)
    {
        NvM_Prv_Common_cst.MultiBlockCallback_pfct(idService_uo, Result_uo);
    }
}

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetPersistantId(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].PersistentId_u16;
}

LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsm(NvM_BlockIdType idBlock_uo,
                                                    NvM_Prv_Crypto_idService_ten idServiceCrypto_en)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].CryptoConfig_st.LengthJobCsm_auo[idServiceCrypto_en];
}

LOCAL_INLINE uint32 NvM_Prv_BlkDesc_GetIdJobCsm(NvM_BlockIdType idBlock_uo,
                                                NvM_Prv_Crypto_idService_ten idServiceCrypto_en)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].CryptoConfig_st.idJobCsm_auo[idServiceCrypto_en];
}

LOCAL_INLINE uint8 const* NvM_Prv_BlkDesc_GetDataJobCsmAssociated(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].CryptoConfig_st.dataJobCsmAssociated_pcu8;
}

LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsmAssociated(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].CryptoConfig_st.LengthJobCsmAssociated_uo;
}

LOCAL_INLINE uint16 NvM_Prv_BlkDesc_GetLengthJobCsmTag(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_BlockDescriptors_acst[idBlock_uo].CryptoConfig_st.LengthJobCsmTag_uo;
}

#endif

/* NVM_PRV_BLOCKDESCRIPTOR_INL_H */
#endif
