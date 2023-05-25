
#ifndef NVM_PRV_CRYPTO_SERVICETYPES_H
#define NVM_PRV_CRYPTO_SERVICETYPES_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

# include "Csm.h"
// TRACE[NVM089] Check version compatibility of included header files
# if (!defined(CSM_AR_RELEASE_MAJOR_VERSION) || (CSM_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(CSM_AR_RELEASE_MINOR_VERSION) || (CSM_AR_RELEASE_MINOR_VERSION < 3))
#  error "AUTOSAR minor version undefined or mismatched"
# endif

# include "NvM_Prv_Crypto_Types.h"

#endif

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

/**
 * Definition of the structure for the data used for cryptographic services provided by the NvM.
 * This structure enables the NvM to handle all cryptographic services in the same way.
 *
 * \attention
 * Not all cryptographic services use all elements of this structure. Unused elements shall be reset to safe values.
 */
typedef struct
{
    uint8 const* inputPrimary_pcu8;
    uint32 inputPrimaryLength_u32;
    uint8 const* inputSecondary_pcu8;
    uint32 inputSecondaryLength_u32;
    uint8 const* inputTertiary_pcu8;
    uint32 inputTertiaryLength_u32;

    uint8* outputPrimary_pu8;
    uint32* outputPrimaryLength_pu32;
    uint8* outputSecondary_pu8;
    uint32* outputSecondaryLength_pu32;

    Crypto_VerifyResultType* ResultVerification_puo;

} NvM_Prv_Crypto_ServiceData_tst;

typedef struct
{
    NvM_Prv_Crypto_idService_ten idServiceCrypto_en;

    uint32 idJobStarted_uo;
    uint32 idJobCallback_uo;

    uint32 outputPrimaryLength_u32;
    uint32 outputPrimaryLengthConfigured_u32;
    uint32 outputSecondaryLength_u32;
    uint32 outputSecondaryLengthConfigured_u32;

    Std_ReturnType Result_uo;

    Crypto_VerifyResultType ResultVerification_uo;

} NvM_Prv_Crypto_ServiceResult_tst;

#endif

/* NVM_PRV_CRYPTO_SERVICETYPES_H */
#endif

