
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.10.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/

#ifndef CANNM_PBCFG_H
#define CANNM_PBCFG_H

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)


/* Number of PN's relevent: Useful for EIRA and ERA calculation */
#define CANNM_NUMBER_OF_PNS    0U



#else
/* Number of PN's relevent: Useful for EIRA and ERA calculation */
#define CANNM_NUMBER_OF_PNS                                                0U
#endif

/*
 *******************************************************************************
 * Extern declarations
 *******************************************************************************
 */
#if (CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_SELECTABLE)

extern const CanNm_GlobalConfigType CanNm_GlobalConfigData_cs;

extern const CanNm_ChannelConfigType CanNm_ChannelConfigData_cs[];

extern const CanNm_ConfigType CanNmGlobalConfig;

#endif

#endif /* CANNM_PBCFG_H */

