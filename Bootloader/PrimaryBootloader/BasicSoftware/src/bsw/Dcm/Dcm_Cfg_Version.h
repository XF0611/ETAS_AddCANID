

#ifndef DCM_VERSION_H
#define DCM_VERSION_H






/**
 * @ingroup DCM_VERSION_H
 *
 * Vendor Id refers to a unique ID assigned by Autosar to every Member of Autosar Consortium, used to indicate the vendor during the call of Det_ReportError()\n
 */
#define     DCM_VENDOR_ID                              6

/**
 * @ingroup DCM_VERSION_H
 *
 * Module Id refers to a unique ID assigned by Autosar to every Module/Component within the Autosar BSW, used to indicate that module which is reporting the error during the call of Det_ReportError()\n
 */
#define     DCM_MODULE_ID                              0x35

/**
 * @ingroup DCM_VERSION_H
 * 
 * Instance Id refers to an ID assigned by Autosar to identify every unique instance of one Module/Component, used to indicate that module instance which is reporting the error during the call of Det_ReportError()\n
 */
#define     DCM_INSTANCE_ID                            0x00u

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare the Major Release Number of Autosar Main Baseline under which this Component Dcm is delivered
 */
#define     DCM_AR_RELEASE_MAJOR_VERSION               4

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare the Minor Release Number of Autosar Main Baseline under which this Component Dcm is delivered
 */
#define     DCM_AR_RELEASE_MINOR_VERSION               2

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare the Revision Release Number of Autosar Main Baseline under which this Component Dcm is delivered
 */
#define     DCM_AR_RELEASE_REVISION_VERSION            2

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare Software Major Version number of Component Dcm, used during the call of Dcm_GetVersionInfo()
 */
#define     DCM_SW_MAJOR_VERSION                       9

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare Software Minor Version number of Component Dcm, used during the call of Dcm_GetVersionInfo()
 */
#define     DCM_SW_MINOR_VERSION                       0

/**
 * @ingroup DCM_VERSION_H
 *
 * Used to declare Software Patch Version number of Component Dcm, used during the call of Dcm_GetVersionInfo()
 */
#define     DCM_SW_PATCH_VERSION                       0

#endif
