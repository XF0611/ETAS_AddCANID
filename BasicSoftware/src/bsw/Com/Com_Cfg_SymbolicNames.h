


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef COM_CFG_SYMBOLICNAMES_H
#define COM_CFG_SYMBOLICNAMES_H


/* if COM_DontUseExternalSymbolicNames is defined while including this file, then the below symbolic names will not
   be visible in the including file */
#ifndef     COM_DontUseExternalSymbolicNames

/* ------------------------------------------------------------------------ */
/* Begin section for IPdu symbolic names */

/* Tx IPdus */
        #define ComConf_ComIPdu_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 0
        #define ComConf_ComIPdu_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 1
        #define ComConf_ComIPdu_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 2
        #define ComConf_ComIPdu_IP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx 3
    /* Rx IPdus */
        #define ComConf_ComIPdu_IP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx 0
        #define ComConf_ComIPdu_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 1
        #define ComConf_ComIPdu_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 2

/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Signal symbolic names */



/* Signal IDs*/
/* Tx Signal ID*/
    #define ComConf_ComSignal_S_ETAS03_Sig10_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 0
    #define ComConf_ComSignal_S_ETAS03_Sig11_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 1
    #define ComConf_ComSignal_S_ETAS03_Sig12_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 2
    #define ComConf_ComSignal_S_ETAS03_Sig13_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 3
    #define ComConf_ComSignal_S_ETAS03_Sig14_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 4
    #define ComConf_ComSignal_S_ETAS03_Sig15_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 5
    #define ComConf_ComSignal_S_ETAS03_Sig16_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 6
    #define ComConf_ComSignal_S_ETAS03_Sig17_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 7
    #define ComConf_ComSignal_S_ETAS03_Sig18_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 8
    #define ComConf_ComSignal_S_ETAS03_Sig19_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx 9
    #define ComConf_ComSignal_S_ETAS04_Sig20_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 10
    #define ComConf_ComSignal_S_ETAS04_Sig21_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 11
    #define ComConf_ComSignal_S_ETAS04_Sig22_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 12
    #define ComConf_ComSignal_S_ETAS04_Sig23_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 13
    #define ComConf_ComSignal_S_ETAS04_Sig24_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 14
    #define ComConf_ComSignal_S_ETAS04_Sig25_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx 15
    #define ComConf_ComSignal_S_ETAS05_Sig26_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 16
    #define ComConf_ComSignal_S_ETAS05_Sig27_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 17
    #define ComConf_ComSignal_S_ETAS05_Sig28_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 18
    #define ComConf_ComSignal_S_ETAS05_Sig29_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 19
    #define ComConf_ComSignal_S_ETAS05_Sig30_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 20
    #define ComConf_ComSignal_S_ETAS05_Sig31_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 21
    #define ComConf_ComSignal_S_ETAS05_Sig32_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 22
    #define ComConf_ComSignal_S_ETAS05_Sig33_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx 23
    #define ComConf_ComSignal_S_NM_Tx_Sig_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx 24

/* Rx Signal ID*/
    #define ComConf_ComSignal_S_ETAS02_Sig09_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx 0
    #define ComConf_ComSignal_S_ETAS06_Sig34_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 1
    #define ComConf_ComSignal_S_ETAS06_Sig35_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 2
    #define ComConf_ComSignal_S_ETAS06_Sig36_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 3
    #define ComConf_ComSignal_S_ETAS06_Sig37_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 4
    #define ComConf_ComSignal_S_ETAS06_Sig38_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 5
    #define ComConf_ComSignal_S_ETAS06_Sig39_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 6
    #define ComConf_ComSignal_S_ETAS06_Sig40_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 7
    #define ComConf_ComSignal_S_ETAS06_Sig41_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx 8
    #define ComConf_ComSignal_S_ETAS01_Sig00_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 9
    #define ComConf_ComSignal_S_ETAS01_Sig01_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 10
    #define ComConf_ComSignal_S_ETAS01_Sig02_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 11
    #define ComConf_ComSignal_S_ETAS01_Sig03_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 12
    #define ComConf_ComSignal_S_ETAS01_Sig04_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 13
    #define ComConf_ComSignal_S_ETAS01_Sig05_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 14
    #define ComConf_ComSignal_S_ETAS01_Sig06_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 15
    #define ComConf_ComSignal_S_ETAS01_Sig07_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 16
    #define ComConf_ComSignal_S_ETAS01_Sig08_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx 17



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Signal group symbolic names */



/* Tx SignalGroup ID*/

/* Rx SignalGroup ID*/





/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Group signal symbolic names */



/* Tx GroupSignal ID*/

/* Rx GroupSignal ID*/




/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for IPdu group symbolic names */


/* IPduGroup ID*/
    #define ComConf_ComIPduGroup_ComIPduGroup_Rx 0
    #define ComConf_ComIPduGroup_ComIPduGroup_Tx 1
/* ------------------------------------------------------------------------ */
/* End section */

#endif /* #ifndef     COM_DontUseExternalSymbolicNames */


#endif /* COM_CFG_SYMBOLICNAMES_H */


