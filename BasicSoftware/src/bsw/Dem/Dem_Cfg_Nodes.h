/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_NODES_H
#define DEM_CFG_NODES_H

/* ---------------------------------------- */
/* DEM_CFG_DEPRECOVERYLIMIT                 */
/* ---------------------------------------- */
#define DEM_CFG_DEPRECOVERYLIMIT_OFF  STD_OFF
#define DEM_CFG_DEPRECOVERYLIMIT_ON   STD_ON
#define DEM_CFG_DEPRECOVERYLIMIT  DEM_CFG_DEPRECOVERYLIMIT_OFF

#define DEM_CFG_DEPENDENCY_PENDING_ON             FALSE

#define DEM_CFG_FAILUREDEPENDENCY_RECHECK_LIMIT  80u

/*                  ALLOWEDRECOVERIES             IGNORESPRIO    FAILEDCALLBACK_IDX                       */

#define DEM_CFG_NODEPARAMS \
{ \
    DEM_NODES_INIT ((DEM_NODE_INFINITE_RECOVERIES),(0u),         (0)                                     ) \
}

#define  DEM_CFG_NODEFAILEDCALLBACK_COUNT  0
#define  DEM_CFG_NODEFAILEDCALLBACK_ARRAYLENGTH  (DEM_CFG_NODEFAILEDCALLBACK_COUNT+1)

#define DEM_CFG_NODEFAILEDCALLBACKS \
{ \
	NULL_PTR \
}

#endif

