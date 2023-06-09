#ifndef FUSA_ALPS_CAN_H
#define FUSA_ALPS_CAN_H

#include "Platform_Types.h"

#define REG_CAN_VERSION_OFFSET					(0x0000)
#define REG_CAN_MODE_CTRL_OFFSET				(0x0004)
#define REG_CAN_PROTOCOL_CTRL_OFFSET			(0x0008)
#define REG_CAN_TIMESTAMP_COUNTER_CFG_OFFSET	(0x000C)
#define REG_CAN_TIMESTAMP_COUNTER_VAL_OFFSET	(0x0010)
#define REG_CAN_DATA_BIT_TIME_CTRL_OFFSET		(0x0014)
#define REG_CAN_NML_BIT_TIME_CTRL_OFFSET	(0x0018)
#define REG_CAN_ERROR_COUNTER_OFFSET			(0x001C)
#define REG_CAN_PROTOCOL_STATUS_OFFSET			(0x0020)
#define REG_CAN_ECC_ERROR_STATUS_OFFSET			(0x0024)
#define REG_CAN_TRANSMIT_DELAY_COM_OFFSET		(0x0028)
#define REG_CAN_TIMEOUT_COUNTER_OFFSET			(0x002C)
#define REG_CAN_TIMEOUT_COUNTER_VAL_OFFSET		(0x0030)
#define REG_CAN_ISR_OFFSET			        	(0x0034)
#define REG_CAN_ISR_ENABLE_OFFSET			    (0x0038)
#define REG_CAN_ISR_LINE_SELECT0_OFFSET	        (0x003C)
#define REG_CAN_ISR_LINE_SELECT1_OFFSET	        (0x0040)
#define REG_CAN_ISR_LINE_ENABLE_OFFSET	        (0x0044)
#define REG_CAN_ID_FILTER_CTRL_OFFSET			(0x0048)
#define REG_CAN_EXTEND_ID_AND_MASK_OFFSET		(0x004C)
#define REG_CAN_RX_EL_SIZE_CFG_OFFSET		    (0x0050)
#define REG_CAN_RX_BUFFER_STATUS0_OFFSET		(0x0054)
#define REG_CAN_RX_BUFFER_STATUS1_OFFSET		(0x0058)
#define REG_CAN_RX_FIFO_CFG_OFFSET				(0x005C)
#define REG_CAN_RX_FIFO_STATUS_OFFSET			(0x0060)
#define REG_CAN_TX_EL_SIZE_CFG_OFFSET		    (0x0064)
#define REG_CAN_TX_BUF_REQ_PENDING0_OFFSET		(0x0068)
#define REG_CAN_TX_BUF_REQ_PENDING1_OFFSET		(0x006C)
#define REG_CAN_TX_BUF_ADD_REQ0_OFFSET			(0x0070)
#define REG_CAN_TX_BUF_ADD_REQ1_OFFSET			(0x0074)
#define REG_CAN_TX_BUF_CANCEL_REQ0_OFFSET		(0x0078)
#define REG_CAN_TX_BUF_CANCEL_REQ1_OFFSET		(0x007C)
#define REG_CAN_TX_BUF_TRANS_OCCURRED0_OFFSET	(0x0080)
#define REG_CAN_TX_BUF_TRANS_OCCURRED1_OFFSET	(0x0084)
#define REG_CAN_TX_BUF_CANCEL_FINISHED0_OFFSET	(0x0088)
#define REG_CAN_TX_BUF_CANCEL_FINISHED1_OFFSET	(0x008C)
#define REG_CAN_TX_BUF_TRANS_INT_EN0_OFFSET		(0x0090)
#define REG_CAN_TX_BUF_TRANS_INT_EN1_OFFSET		(0x0094)
#define REG_CAN_TX_BUF_CANCELED_INT_EN0_OFFSET	(0x0098)
#define REG_CAN_TX_BUF_CANCELED_INT_EN1_OFFSET	(0x009C)
#define REG_CAN_TX_FIFO_CFG_OFFSET				(0x00A0)
#define REG_CAN_TX_FIFO_STATUS_OFFSET			(0x00A4)
#define REG_CAN_TX_EVENT_FIFO_CFG_OFFSET		(0x00A8)
#define REG_CAN_TX_EVENT_FIFO_STATUS_OFFSET		(0x00AC)
#define REG_CAN_RX_FIFO_EL_OFFSET(id)	    	(0x00B0 + ((id) << 2))
#define REG_CAN_TX_FIFO_EL_OFFSET(id)		    (0x00F8 + ((id) << 2))
#define REG_CAN_TX_EV_FIFO_EL_OFFSET(id)	    (0x0140 + ((id) << 2))

#define REG_CAN_TX_BUFFER_OFFSET				(0x1000)
#define REG_CAN_RX_BUFFER_OFFSET				(0x1C00)
#define REG_CAN_STD_ID_FILTER_OFFSET			(0x2400)
#define REG_CAN_EXT_ID_FILTER_OFFSET			(0x2800)
#define REG_CAN_EXT_ID_FILTER_EL_F1_OFFSET	    (0x4)
/* mode control. */
#define BIT_MODE_CTRL_TDENA						(1 << 18)
#define BIT_MODE_CTRL_RDENA						(1 << 17)
#define BIT_MODE_CTRL_ECCENA					(1 << 16)
#define BIT_MODE_CTRL_TESTRX					(1 << 10)
#define BITS_MODE_CTRL_TESTRX_SHIFT				(8)
#define BITS_MODE_CTRL_TESTRX_MASK				(0x3)
#define BIT_MODE_CTRL_TEST						(1 << 7)
#define BIT_MODE_CTRL_SLEEP						(1 << 6)
#define BIT_MODE_CTRL_SPACK						(1 << 5)
#define BIT_MODE_CTRL_ROPT						(1 << 4)
#define BIT_MODE_CTRL_MON						(1 << 3)
#define BIT_MODE_CTRL_LBACK						(1 << 2)
#define BIT_MODE_CTRL_CFG						(1 << 1)
#define BIT_MODE_CTRL_RESET						(1 << 0)

/* protocol control. */
#define BIT_PROTOCOL_CTRL_FDD					(1 << 31)
#define BIT_PROTOCOL_CTRL_FDO					(1 << 3)
#define BIT_PROTOCOL_CTRL_BRS					(1 << 2)
#define BIT_PROTOCOL_CTRL_TDC					(1 << 1)
#define BIT_PROTOCOL_CTRL_ART					(1 << 0)

/* timestamp counter configure. */
#define BITS_TIMESTAMP_CFG_TSCP_SHIFT			(16)
#define BITS_TIMESTAMP_CFG_TSCP_MASK			(0xF)
#define BITS_TIMESTAMP_CFG_TSCM_SHIFT			(0)
#define BITS_TIMESTAMP_CFG_TSCM_MASK			(0x3)

/* data bit time control. */
#define BITS_DATA_BIT_TIME_CTRL_DSJW_SHIFT		(24)
#define BITS_DATA_BIT_TIME_CTRL_DSJW_MASK		(0xF)
#define BITS_DATA_BIT_TIME_CTRL_DBRP_SHIFT		(16)
#define BITS_DATA_BIT_TIME_CTRL_DBRP_MASK		(0x1F)
#define BITS_DATA_BIT_TIME_CTRL_DTSG1_SHIFT	(8)
#define BITS_DATA_BIT_TIME_CTRL_DTSG1_MASK		(0x1F)
#define BITS_DATA_BIT_TIME_CTRL_DTSG2_SHIFT	(0)
#define BITS_DATA_BIT_TIME_CTRL_DTSG2_MASK		(0xF)

/* nominal bit time control. */
#define BITS_NML_BIT_TIME_CTRL_NSJW_SHIFT	(25)
#define BITS_NML_BIT_TIME_CTRL_NSJW_MASK	(0x7F)
#define BITS_NML_BIT_TIME_CTRL_NBRP_SHIFT	(16)
#define BITS_NML_BIT_TIME_CTRL_NBRP_MASK	(0x1FF)
#define BITS_NML_BIT_TIME_CTRL_NTSEG1_SHIFT	(8)
#define BITS_NML_BIT_TIME_CTRL_NTSEG1_MASK	(0xFF)
#define BITS_NML_BIT_TIME_CTRL_NTSEG2_SHIFT	(0)
#define BITS_NML_BIT_TIME_CTRL_NTSEG2_MASK	(0x7F)

/* error counter. */
#define BITS_ERROR_CTRL_CEL_SHIFT		(16)
#define BITS_ERROR_CTRL_CEL_MASK		(0xFF)
#define BITS_ERROR_CTRL_REC_SHIFT		(8)
#define BITS_ERROR_CTRL_REC_MASK		(0x7F)
#define BITS_ERROR_CTRL_TEC_SHIFT		(0)
#define BITS_ERROR_CTRL_TEC_MASK		(0xFF)

/* protocol status. */
#define BIT_PROTOCOL_STATUS_BO				(1 << 27)
#define BIT_PROTOCOL_STATUS_EW				(1 << 26)
#define BIT_PROTOCOL_STATUS_EP				(1 << 25)
#define BITS_PROTOCOL_STATUS_ACT_SHIFT		(23)
#define BITS_PROTOCOL_STATUS_ACT_MASK		(0x3)
#define BIT_PROTOCOL_STATUS_PXE				(1 << 6)
#define BIT_PROTOCOL_STATUS_RFDF			(1 << 5)
#define BIT_PROTOCOL_STATUS_RBRS			(1 << 4)
#define BIT_PROTOCOL_STATUS_RESI			(1 << 3)
#define BITS_PROTOCOL_STATUS_LEC_SHIFT		(0)
#define BITS_PROTOCOL_STATUS_LEC_MASK		(0x7)

/* transmitter delay compensation. */
#define BITS_TRANSMIT_DELAY_COM_TDCV_SHIFT	(16)
#define BITS_TRANSMIT_DELAY_COM_TDCV_MASK	(0x7F)
#define BITS_TRANSMIT_DELAY_COM_TDCO_SHIFT	(8)
#define BITS_TRANSMIT_DELAY_COM_TDCO_MASK	(0x7F)
#define BITS_TRANSMIT_DELAY_COM_TDCF_SHIFT	(0)
#define BITS_TRANSMIT_DELAY_COM_TDCF_MASK	(0x7F)

/* timeour counter. */
#define BITS_TIMEOUT_COUNTER_TOP_SHIFT		(16)
#define BITS_TIMEOUT_COUNTER_TOP_MASK		(0xFFFF)
#define BITS_TIMEOUT_COUNTER_TOS_SHIFT		(1)
#define BITS_TIMEOUT_COUNTER_TOS_MASK		(0x3)
#define BIT_TIMEOUT_COUNTER_TOE				(1 << 0)

/* interrupt. */
#define BIT_INTERRUPT_TOO			(1 << 25)
#define BIT_INTERRUPT_APR			(1 << 24)
#define BIT_INTERRUPT_PED			(1 << 23)
#define BIT_INTERRUPT_BO			(1 << 22)
#define BIT_INTERRUPT_EW			(1 << 21)
#define BIT_INTERRUPT_EP			(1 << 20)
#define BIT_INTERRUPT_ELO			(1 << 19)
#define BIT_INTERRUPT_BEU			(1 << 18)
#define BIT_INTERRUPT_BEC			(1 << 17)
#define BIT_INTERRUPT_MRAF			(1 << 16)
#define BIT_INTERRUPT_TSW			(1 << 15)
#define BIT_INTERRUPT_RXBN			(1 << 14)
#define BIT_INTERRUPT_RXFL			(1 << 13)
#define BIT_INTERRUPT_RXFF			(1 << 12)
#define BIT_INTERRUPT_RXFW			(1 << 11)
#define BIT_INTERRUPT_RXFE			(1 << 10)
#define BIT_INTERRUPT_TXBC			(1 << 9)
#define BIT_INTERRUPT_TXBT			(1 << 8)
#define BIT_INTERRUPT_TXFL			(1 << 7)
#define BIT_INTERRUPT_TXFF			(1 << 6)
#define BIT_INTERRUPT_TXFW			(1 << 5)
#define BIT_INTERRUPT_TXFE			(1 << 4)
#define BIT_INTERRUPT_TEFL			(1 << 3)
#define BIT_INTERRUPT_TEFF			(1 << 2)
#define BIT_INTERRUPT_TEFW			(1 << 1)
#define BIT_INTERRUPT_TEFE			(1 << 0)

#if 0
/* interrupt enable. */
#define BIT_ISR_TOOE			(1 << 25)
#define BIT_ISR_APRE			(1 << 24)
#define BIT_ISR_PEDE			(1 << 23)
#define BIT_ISR_BOE			(1 << 22)
#define BIT_ISR_EWE			(1 << 21)
#define BIT_ISR_EPE			(1 << 20)
#define BIT_ISR_ELOE			(1 << 19)
#define BIT_ISR_BEUE			(1 << 18)
#define BIT_ISR_BECE			(1 << 17)
#define BIT_ISR_MRAFE			(1 << 16)
#define BIT_ISR_TSWE			(1 << 15)
#define BIT_ISR_RXBNE			(1 << 14)
#define BIT_ISR_RXFLE			(1 << 13)
#define BIT_ISR_RXFFE			(1 << 12)
#define BIT_ISR_RXFWEE			(1 << 11)
#define BIT_ISR_RXFEE			(1 << 10)
#define BIT_ISR_TXBCE			(1 << 9)
#define BIT_ISR_TXBTE			(1 << 8)
#define BIT_ISR_TXFLE			(1 << 7)
#define BIT_ISR_TXFFE			(1 << 6)
#define BIT_ISR_TXFWE			(1 << 5)
#define BIT_ISR_TXFEE			(1 << 4)
#define BIT_ISR_TEFLE			(1 << 3)
#define BIT_ISR_TEFFE			(1 << 2)
#define BIT_ISR_TEFWE			(1 << 1)
#define BIT_ISR_TEFEE			(1 << 0)
#endif

/* interrupt line select. */
#define BITS_ISR_LINE_SELECT_TOOLS_SHIFT	(20)
#define BITS_ISR_LINE_SELECT_TOOLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_APRLS_SHIFT	(18)
#define BITS_ISR_LINE_SELECT_APRLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_PEDLS_SHIFT	(16)
#define BITS_ISR_LINE_SELECT_PEDLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_BOLS_SHIFT	(14)
#define BITS_ISR_LINE_SELECT_BOLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_EWLS_SHIFT	(12)
#define BITS_ISR_LINE_SELECT_EWLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_EPLS_SHIFT	(10)
#define BITS_ISR_LINE_SELECT_EPLS_MASK	(0x3)
//#define BITS_ISR_LINE_SELECT_EPLS_SHIFT	(10)
//#define BITS_ISR_LINE_SELECT_EPLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_ELOLS_SHIFT	(8)
#define BITS_ISR_LINE_SELECT_ELOLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_BEULS_SHIFT	(6)
#define BITS_ISR_LINE_SELECT_BEULS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_BECLS_SHIFT	(4)
#define BITS_ISR_LINE_SELECT_BECLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_MRAFLS_SHIFT	(2)
#define BITS_ISR_LINE_SELECT_MRAFLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TSWLS_SHIFT	(0)
#define BITS_ISR_LINE_SELECT_TSWLS_MASK	(0x3)

#define BITS_ISR_LINE_SELECT_RXBNLS_SHIFT	(28)
#define BITS_ISR_LINE_SELECT_RXBNLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_RXFLLS_SHIFT	(26)
#define BITS_ISR_LINE_SELECT_RXFLLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_RXFFLS_SHIFT	(24)
#define BITS_ISR_LINE_SELECT_RXFFLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_RXFWLS_SHIFT	(22)
#define BITS_ISR_LINE_SELECT_RXFWLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_RXFELS_SHIFT	(20)
#define BITS_ISR_LINE_SELECT_RXFELS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXBCLS_SHIFT	(18)
#define BITS_ISR_LINE_SELECT_TXBCLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXBTLS_SHIFT	(16)
#define BITS_ISR_LINE_SELECT_TXBTLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXFLLS_SHIFT	(14)
#define BITS_ISR_LINE_SELECT_TXFLLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXFFLS_SHIFT	(12)
#define BITS_ISR_LINE_SELECT_TXFFLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXFWLS_SHIFT	(10)
#define BITS_ISR_LINE_SELECT_TXFWLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TXFELS_SHIFT	(8)
#define BITS_ISR_LINE_SELECT_TXFELS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TEFLLS_SHIFT	(6)
#define BITS_ISR_LINE_SELECT_TEFLLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TEFFLS_SHIFT	(4)
#define BITS_ISR_LINE_SELECT_TEFFLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TEFWLS_SHIFT	(2)
#define BITS_ISR_LINE_SELECT_TEFWLS_MASK	(0x3)
#define BITS_ISR_LINE_SELECT_TEFELS_SHIFT	(0)
#define BITS_ISR_LINE_SELECT_TEFELS_MASK	(0x3)

/* interrupt line enable. */
#define BIT_ISR_LINE_ENABLE_EINT3		(1 << 3)
#define BIT_ISR_LINE_ENABLE_EINT2		(1 << 2)
#define BIT_ISR_LINE_ENABLE_EINT1		(1 << 1)
#define BIT_ISR_LINE_ENABLE_EINT0		(1 << 0)

/* id filter control. */
#define BIT_ID_FILTER_CTRL_RNMFE			(1 << 25)
#define BIT_ID_FILTER_CTRL_RRFE				(1 << 24)
#define BITS_ID_FILTER_CTRL_XIDFS_SHIFT		(16)
#define BITS_ID_FILTER_CTRL_XIDFS_MASK		(0xFF)
#define BIT_ID_FILTER_CTRL_RNMFS			(1 << 9)
#define BIT_ID_FILTER_CTRL_RRFS				(1 << 8)
#define BITS_ID_FILTER_CTRL_SIDFS_SHIFT		(0)
#define BITS_ID_FILTER_CTRL_SIDFS_MASK		(0xFF)

#define BITS_STD_ID_FILTER_EL_SFT_SHIFT		(30)
#define BITS_STD_ID_FILTER_EL_SFT_MSASK		(0x3)
#define BITS_STD_ID_FILTER_EL_SFEC_SHIT		(27)
#define BITS_STD_ID_FILTER_EL_SFEC_MASK		(0x7)
#define BITS_STD_ID_FILTER_EL_SFID0_SHIFT		(16)
#define BITS_STD_ID_FILTER_EL_SFID0_MASK		(0x7FF)
#define BITS_STD_ID_FILTER_EL_SFID1_SHIFT		(0)
#define BITS_STD_ID_FILTER_EL_SFID1_MASK		(0x7FF)
#define BITS_EXT_ID_FILTER_EL_EFT_SHIFT		(30)
#define BITS_EXT_ID_FILTER_EL_EFT_MASK			(0x3)
#define BITS_EXT_ID_FILTER_EL_EFID0_SHIFT		(0)
#define BITS_EXT_ID_FILTER_EL_EFID0_MASK		(0x1FFFFFFF)
#define BITS_EXT_ID_FILTER_EL_EFEC_SHIFT		(29)
#define BITS_EXT_ID_FILTER_EL_EFEC_MASK		(0x7)
#define BITS_EXT_ID_FILTER_EL_EFID1_SHIFT		(0)
#define BITS_EXT_ID_FILTER_EL_EFID1_MASK		(0x1FFFFFFF)
/* rx fifo or BUffer element size configure. */
#define BITS_RX_EL_SIZE_CFG_RXFDS_SHIFT	(4)
#define BITS_RX_EL_SIZE_CFG_RXFDS_MASK		(0x7)
#define BITS_RX_EL_SIZE_CFG_RXBDS_SHIFT	(0)
#define BITS_RX_EL_SIZE_CFG_RXBDS_MASK		(0x7)

/* RX fifo configure. */
#define BITS_RX_FIFO_CFG_RXFWM_SHIFT		(24)
#define BITS_RX_FIFO_CFG_RXFWM_MASK			(0xFF)
#define BITS_RX_FIFO_CFG_RXFSZ_SHIFT		(16)
#define BITS_RX_FIFO_CFG_RXFSZ_MASK			(0xFF)
#define BIT_RX_FIFO_CFG_RXFOPM				(1 << 0)

#if 0
/* rx fifo status. */
#define BITS_RX_FIFO_STATUS_RXFPI_SHIFT		(24)
#define BITS_RX_FIFO_STATUS_RXFPI_MASK		(0xFF)
#define BITS_RX_FIFO_STATUS_RXFGI_SHIFT		(16)
#define BITS_RX_FIFO_STATUS_RXFGI_MASK		(0xFF)
#define BITS_RX_FIFO_STATUS_RXFLV_SHIFT		(8)
#define BITS_RX_FIFO_STATUS_RXFLV_MASK		(0xFF)
#define BIT_RX_FIFO_STATUS_RXFML		(1 << 7)
#define BIT_RX_FIFO_STATUS_RXFF			(1 << 6)
#define BIT_RX_FIFO_STATUS_RXFE			(1 << 5)
#define BIT_RX_FIFO_STATUS_RXFACK		(1 << 0)
#endif

/* tx fifo or BUffer element size configure. */
#define BITS_TX_EL_SIZE_CFG_TXFDS_SHIFT	(4)
#define BITS_TX_EL_SIZE_CFG_TXFDS_MASK		(0x7)
#define BITS_TX_EL_SIZE_CFG_TXBDS_SHIFT	(0)
#define BITS_TX_EL_SIZE_CFG_TXBDS_MASK		(0x7)

/* TX fifo configure. */
#define BITS_TX_FIFO_CFG_TXFWM_SHIFT		(24)
#define BITS_TX_FIFO_CFG_TXFWM_MASK			(0xFF)
#define BITS_TX_FIFO_CFG_TXFSZ_SHIFT		(16)
#define BITS_TX_FIFO_CFG_TXFSZ_MASK			(0xFF)
#define BIT_TX_FIFO_CFG_TXFOPM				(1 << 0)

#if 0
/* tx fifo status. */
#define BITS_TX_FIFO_STATUS_TXFPI_SHIFT		(24)
#define BITS_TX_FIFO_STATUS_TXFPI_MASK		(0xFF)
#define BITS_TX_FIFO_STATUS_TXFGI_SHIFT		(16)
#define BITS_TX_FIFO_STATUS_TXFGI_MASK		(0xFF)
#define BITS_TX_FIFO_STATUS_TXFLV_SHIFT		(8)
#define BITS_TX_FIFO_STATUS_TXFLV_MASK		(0xFF)
#define BIT_TX_FIFO_STATUS_TXFML		(1 << 7)
#define BIT_TX_FIFO_STATUS_TXFF			(1 << 6)
#define BIT_TX_FIFO_STATUS_TXFE			(1 << 5)
#define BIT_TX_FIFO_STATUS_TXFACK		(1 << 0)
#endif

/* TX event fifo configure. */
#define BITS_TX_EVENT_FIFO_CFG_TEFWM_SHIFT	(24)
#define BITS_TX_EVENT_FIFO_CFG_TEFWM_MASK	(0xFF)
#define BITS_TX_EVENT_FIFO_CFG_TEFSZ_SHIFT	(16)
#define BITS_TX_EVENT_FIFO_CFG_TEFSZ_MASK	(0xFF)
#define BIT_TX_EVENT_FIFO_CFG_TEFOPM		(1 << 0)

#if 0
/* tx event fifo status. */
#define BITS_TX_EVENT_FIFO_STATUS_TEFPI_SHIFT	(24)
#define BITS_TX_EVENT_FIFO_STATUS_TEFPI_MASK	(0xFF)
#define BITS_TX_EVENT_FIFO_STATUS_TEFGI_SHIFT	(16)
#define BITS_TX_EVENT_FIFO_STATUS_TEFGI_MASK	(0xFF)
#define BITS_TX_EVENT_FIFO_STATUS_TEFLV_SHIFT	(8)
#define BITS_TX_EVENT_FIFO_STATUS_TEFLV_MASK	(0xFF)
#define BIT_TX_EVENT_FIFO_STATUS_TEFML		(1 << 7)
#define BIT_TX_EVENT_FIFO_STATUS_TEFF		(1 << 6)
#define BIT_TX_EVENT_FIFO_STATUS_TEFE		(1 << 5)
#define BIT_TX_EVENT_FIFO_STATUS_TEFACK		(1 << 0)
#endif


#define CAN_FIFO_STATUS_FPI_SHIFT		(24)
#define CAN_FIFO_STATUS_FPI_MASK		(0xFF)
#define CAN_FIFO_STATUS_FGI_SHIFT		(16)
#define CAN_FIFO_STATUS_FGI_MASK		(0xFF)
#define CAN_FIFO_STATUS_FLV_SHIFT		(8)
#define CAN_FIFO_STATUS_FLV_MASK		(0xFF)
#define CAN_FIFO_STATUS_FML				(1 << 7)
#define CAN_FIFO_STATUS_FF				(1 << 6)
#define CAN_FIFO_STATUS_FE				(1 << 5)
#define CAN_FIFO_STATUS_FACK			(1 << 0)

#define CAN_FIFO_CFG_FWM_SHIFT			(24)
#define CAN_FIFO_CFG_FWM_MASK			(0xFF)
#define CAN_FIFO_CFG_FSZ_SHIFT			(16)
#define CAN_FIFO_CFG_FSZ_MASK			(0xFF)
#define CAN_FIFO_CFG_FOPM				(1 << 0)

void fusa_can_clk_en(uint32 can_id, uint32 state);
void fusa_can_ecc_en(uint32 id, uint32 state);
void fusa_can_ecc_err_inject(uint32 can_id, uint32 state);
#endif