/*******************************************************************************
   Copyright (C) Marvell International Ltd. and its affiliates

   This software file (the "File") is owned and distributed by Marvell
   International Ltd. and/or its affiliates ("Marvell") under the following
   alternative licensing terms.  Once you have made an election to distribute the
   File under one of the following license alternatives, please (i) delete this
   introductory statement regarding license alternatives, (ii) delete the two
   license alternatives that you have not elected to use and (iii) preserve the
   Marvell copyright notice above.

********************************************************************************
   Marvell Commercial License Option

   If you received this File from Marvell and you have entered into a commercial
   license agreement (a "Commercial License") with Marvell, the File is licensed
   to you under the terms of the applicable Commercial License.

********************************************************************************
   Marvell GPL License Option

   If you received this File from Marvell, you may opt to use, redistribute and/or
   modify this File in accordance with the terms and conditions of the General
   Public License Version 2, June 1991 (the "GPL License"), a copy of which is
   available along with the File in the license.txt file or by writing to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
   on the worldwide web at http://www.gnu.org/licenses/gpl.txt.

   THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
   WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
   DISCLAIMED.  The GPL License provides additional details about this warranty
   disclaimer.
********************************************************************************
   Marvell BSD License Option

   If you received this File from Marvell, you may opt to use, redistribute and/or
   modify this File under the following licensing terms.
   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

*   Redistributions of source code must retain the above copyright notice,
            this list of conditions and the following disclaimer.

*   Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

*   Neither the name of Marvell nor the names of its contributors may be
        used to endorse or promote products derived from this software without
        specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/
#include "mvCommon.h"
#include "mvBoardEnvLib.h"
#include "mvBoardEnvSpec.h"
#include "twsi/mvTwsi.h"
#include "pex/mvPexRegs.h"

/***************************************** Customer Boards ****************************************/
/*******************************************************************************
	Avanata LP Customer board - Based on RD-88F6660
*******************************************************************************/
MV_BOARD_TWSI_INFO avanta_lp_customer_board_0_InfoBoardTwsiDev[] = {
	/* {{MV_BOARD_DEV_CLASS devClass, MV_U8 devClassId,  MV_U8 twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{ BOARD_DEV_TWSI_IO_EXPANDER,	0,	0x20,	ADDR7_BIT },
};

/* When Switch Port 4 is connected to external PHY through RGMII-0,
 * this external PHY is managed through MAC-0 SMI lines.
 * The 'boardEthSmiAddr' variable is used only for PHY init. */
MV_BOARD_MAC_INFO avanta_lp_customer_board_0_InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED boardMacSpeed, MV_32 boardEthSmiAddr , MV_32 boardEthSmiAddr0;}} */
	{ BOARD_MAC_SPEED_2000M, -1,  -1 },
	{ BOARD_MAC_SPEED_AUTO,  0x1, 0x1},
	{ BOARD_MAC_SPEED_1000M, -1,  -1 },
	{ BOARD_MAC_SPEED_1000M, -1,  -1 },
};
MV_BOARD_MPP_TYPE_INFO avanta_lp_customer_board_0_InfoBoardModTypeInfo[] = {
	{
		.boardMppSlic = MV_BOARD_SLIC_TDMMC_ID,
		.ethSataComplexOpt = MV_ETHCOMP_SW_P0_2_GE_PHY_P0 |
				     MV_ETHCOMP_SW_P1_2_GE_PHY_P1 |
				     MV_ETHCOMP_SW_P2_2_GE_PHY_P2 |
				     MV_ETHCOMP_SW_P3_2_GE_PHY_P3 |
				     MV_ETHCOMP_P2P_MAC0_2_SW_SPEED_2G |
				     MV_ETHCOMP_SW_P4_2_RGMII0   |
				     MV_ETHCOMP_GE_MAC0_2_SW_P6,
		.ethPortsMode = 0x0
	}
};

MV_DEV_CS_INFO avanta_lp_customer_board_0_InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{ SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8 } /* SPI DEV */
#endif
};

MV_BOARD_TDM_INFO avanta_lp_customer_board_0_Tdm886[] = { {0}, {1} };

MV_BOARD_TDM_SPI_INFO avanta_lp_customer_board_0_TdmSpiInfo[] = { {1} };

MV_BOARD_MPP_INFO avanta_lp_customer_board_0_InfoBoardMppConfigValue[] = {
	{ {
		AVANTA_LP_CUSTOMER_MPP0_7,
		AVANTA_LP_CUSTOMER_MPP8_15,
		AVANTA_LP_CUSTOMER_MPP16_23,
		AVANTA_LP_CUSTOMER_MPP24_31,
		AVANTA_LP_CUSTOMER_MPP32_39,
		AVANTA_LP_CUSTOMER_MPP40_47,
		AVANTA_LP_CUSTOMER_MPP48_55,
		AVANTA_LP_CUSTOMER_MPP56_63,
		AVANTA_LP_CUSTOMER_MPP64_67,
	} }
};

MV_BOARD_INFO avanta_lp_customer_board_0_info = {
	.boardName			= "AVANTA-LP-Customer-Board-0",
	.numBoardMppTypeValue		= ARRSZ(avanta_lp_customer_board_0_InfoBoardModTypeInfo),
	.pBoardModTypeValue		= avanta_lp_customer_board_0_InfoBoardModTypeInfo,
	.pBoardMppConfigValue		= avanta_lp_customer_board_0_InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(avanta_lp_customer_board_0_InfoBoardDeCsInfo),
	.pDevCsInfo			= avanta_lp_customer_board_0_InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(avanta_lp_customer_board_0_InfoBoardTwsiDev),
	.pBoardTwsiDev			= avanta_lp_customer_board_0_InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(avanta_lp_customer_board_0_InfoBoardMacInfo),
	.pBoardMacInfo			= avanta_lp_customer_board_0_InfoBoardMacInfo,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= 0,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 80000,

	/* GPP values */
	.gppOutEnValLow			= AVANTA_LP_CUSTOMER_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= AVANTA_LP_CUSTOMER_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= AVANTA_LP_CUSTOMER_GPP_OUT_ENA_HIGH,
	.gppOutValLow			= AVANTA_LP_CUSTOMER_GPP_OUT_VAL_LOW,
	.gppOutValMid			= AVANTA_LP_CUSTOMER_GPP_OUT_VAL_MID,
	.gppOutValHigh			= AVANTA_LP_CUSTOMER_GPP_OUT_VAL_HIGH,
	.gppPolarityValLow		= AVANTA_LP_CUSTOMER_GPP_POL_LOW,
	.gppPolarityValMid		= AVANTA_LP_CUSTOMER_GPP_POL_MID,
	.gppPolarityValHigh		= AVANTA_LP_CUSTOMER_GPP_POL_HIGH,

	/* TDM */
	.numBoardTdmInfo		= {2},
	.pBoardTdmInt2CsInfo		= {avanta_lp_customer_board_0_Tdm886},
	.boardTdmInfoIndex		= 0,

	.pBoardSpecInit			= NULL,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,

	.pBoardTdmSpiInfo		= avanta_lp_customer_board_0_TdmSpiInfo,

	/* NOR init params */
	.norFlashReadParams		= 0,
	.norFlashWriteParams		= 0,
	/* Enable modules auto-detection. */
	.configAutoDetect		= MV_FALSE
};

MV_BOARD_INFO *customerBoardInfoTbl[] = {
	&avanta_lp_customer_board_0_info,
	&avanta_lp_customer_board_0_info,
};

/***************************************** Marvell Boards *****************************************/
/*******************************************************************************
 * AvantaLP DB-88F6660 board */
/*******************************************************************************/

MV_BOARD_TWSI_INFO db88f6660InfoBoardTwsiDev[] = {
	/* {{MV_BOARD_DEV_CLASS devClass, MV_U8 devClassId,  MV_U8 twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{ BOARD_DEV_TWSI_SATR,		0,	0x4C,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_SATR,		1,	0x4D,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_EEPROM,	0,	0x55,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_IO_EXPANDER,	0,	0x21,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_IO_EXPANDER,	1,	0x22,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_IO_EXPANDER,	2,	0x24,	   ADDR7_BIT	},
};
MV_BOARD_MAC_INFO db88f6660InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED boardMacSpeed, MV_32 boardEthSmiAddr , MV_32 boardEthSmiAddr0;}} */
	{ BOARD_MAC_SPEED_1000M, -1, -1 },
	{ BOARD_MAC_SPEED_AUTO, 0x1, 0x1},
	{ BOARD_MAC_SPEED_1000M, -1, -1 },
	{ BOARD_MAC_SPEED_1000M, -1, -1 },
};
MV_BOARD_MPP_TYPE_INFO db88f6660InfoBoardModTypeInfo[] = {
	{
		.boardMppSlic = MV_BOARD_SLIC_DISABLED,
		.ethSataComplexOpt = MV_ETHCOMP_SW_P0_2_GE_PHY_P0 |
				     MV_ETHCOMP_SW_P1_2_GE_PHY_P1 |
				     MV_ETHCOMP_SW_P2_2_GE_PHY_P2 |
				     MV_ETHCOMP_SW_P3_2_GE_PHY_P3 |
				     MV_ETHCOMP_P2P_MAC0_2_SW_SPEED_2G |
				     MV_ETHCOMP_GE_MAC0_2_SW_P6 |
				     MV_ETHCOMP_GE_MAC1_2_RGMII1,
		.ethPortsMode = 0x0
	}
};

MV_DEV_CS_INFO db88f6660InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{ SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8 } /* SPI DEV */
#endif
};

MV_BOARD_MPP_INFO db88f6660InfoBoardMppConfigValue[] = {
	{ {
		  DB_88F6660_MPP0_7,
		  DB_88F6660_MPP8_15,
		  DB_88F6660_MPP16_23,
		  DB_88F6660_MPP24_31,
		  DB_88F6660_MPP32_39,
		  DB_88F6660_MPP40_47,
		  DB_88F6660_MPP48_55,
		  DB_88F6660_MPP56_63,
		  DB_88F6660_MPP64_67,
	 } }
};
MV_BOARD_TDM_INFO db88f6660TdmZSI[] = { {2} };

MV_BOARD_TDM_INFO db88f6660TdmISI[] = { {2} };

MV_BOARD_TDM_INFO db88f6660Tdm2c[] = { {0} };

MV_BOARD_TDM_INFO db88f6660Tdmmc[] = { {0}, {1}, {2}, {3} };

MV_BOARD_TDM_SPI_INFO db88f6660TdmSpiInfo[] = { {-1}, {-1}, {1}, {1}, {1}, {1} };

MV_BOARD_INFO db88f6660_board_info = {
	.boardName			= "DB-88F6660",
	.numBoardMppTypeValue		= ARRSZ(db88f6660InfoBoardModTypeInfo),
	.pBoardModTypeValue		= db88f6660InfoBoardModTypeInfo,
	.pBoardMppConfigValue		= db88f6660InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(db88f6660InfoBoardDeCsInfo),
	.pDevCsInfo			= db88f6660InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(db88f6660InfoBoardTwsiDev),
	.pBoardTwsiDev			= db88f6660InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(db88f6660InfoBoardMacInfo),
	.pBoardMacInfo			= db88f6660InfoBoardMacInfo,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= 0,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 80000,

	/* GPP values */
	.gppOutEnValLow			= DB_88F6660_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= DB_88F6660_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= DB_88F6660_GPP_OUT_ENA_HIGH,
	.gppOutValLow			= DB_88F6660_GPP_OUT_VAL_LOW,
	.gppOutValMid			= DB_88F6660_GPP_OUT_VAL_MID,
	.gppOutValHigh			= DB_88F6660_GPP_OUT_VAL_HIGH,
	.gppPolarityValLow		= DB_88F6660_GPP_POL_LOW,
	.gppPolarityValMid		= DB_88F6660_GPP_POL_MID,
	.gppPolarityValHigh		= DB_88F6660_GPP_POL_HIGH,

	/* TDM */
	.numBoardTdmInfo		= {-1, -1, -1, 1, 1, 4},
	.pBoardTdmInt2CsInfo		= {NULL, NULL, db88f6660TdmISI, db88f6660TdmZSI, db88f6660Tdm2c,
						db88f6660Tdmmc},
	.boardTdmInfoIndex		= 0,

	.pBoardSpecInit			= NULL,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,
	.pBoardTdmSpiInfo		= db88f6660TdmSpiInfo,

	/* NOR init params */
	.norFlashReadParams		= 0,
	.norFlashWriteParams		= 0,
	/* Enable modules auto-detection. */
	.configAutoDetect		= MV_TRUE
};

/*******************************************************************************
 * AvantaLP DB-88F6650 board */
/*******************************************************************************/

MV_BOARD_TWSI_INFO db88f6650InfoBoardTwsiDev[] = {
	/* {{MV_BOARD_DEV_CLASS devClass, MV_U8 devClassId,  MV_U8 twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{ BOARD_DEV_TWSI_SATR,		0,	0x4C,	ADDR7_BIT },
	{ BOARD_DEV_TWSI_SATR,		1,	0x4E,	ADDR7_BIT },
	{ BOARD_DEV_TWSI_EEPROM,	0,	0x55,	ADDR7_BIT },
	{ BOARD_DEV_TWSI_IO_EXPANDER,	0,	0x21,	ADDR7_BIT },
	{ BOARD_DEV_TWSI_IO_EXPANDER,	2,	0x24,	ADDR7_BIT },
};

MV_BOARD_MAC_INFO db88f6650InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED boardMacSpeed, MV_32 boardEthSmiAddr , MV_32 boardEthSmiAddr0;}} */
	{ BOARD_MAC_SPEED_AUTO, 0x0, 0x0},
	{ BOARD_MAC_SPEED_AUTO, 0x3, 0x3},
	{ BOARD_MAC_SPEED_1000M, -1, -1 },
	{ BOARD_MAC_SPEED_1000M, -1, -1 },
};

MV_BOARD_MPP_TYPE_INFO db88f6650InfoBoardModTypeInfo[] = {
	{
		.boardMppSlic = MV_BOARD_SLIC_DISABLED,
		.ethSataComplexOpt = MV_ETHCOMP_SW_P0_2_GE_PHY_P0 |
				     MV_ETHCOMP_SW_P1_2_GE_PHY_P1 |
				     MV_ETHCOMP_SW_P2_2_GE_PHY_P2 |
				     MV_ETHCOMP_SW_P3_2_GE_PHY_P3 |
				     MV_ETHCOMP_P2P_MAC0_2_SW_SPEED_2G |
				     MV_ETHCOMP_GE_MAC0_2_SW_P6 |
				     MV_ETHCOMP_GE_MAC1_2_RGMII1,
		.ethPortsMode = 0x0
	}
};

MV_DEV_CS_INFO db88f6650InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{ SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8 } /* SPI DEV */
#endif
};

MV_BOARD_MPP_INFO db88f6650InfoBoardMppConfigValue[] = {
	{ {
		DB_88F6650_MPP0_7,
		DB_88F6650_MPP8_15,
		DB_88F6650_MPP16_23,
		DB_88F6650_MPP24_31,
		DB_88F6650_MPP32_39,
		DB_88F6650_MPP40_47,
		DB_88F6650_MPP48_55,
		DB_88F6650_MPP56_63,
		DB_88F6650_MPP64_67,
	 } }
};

MV_BOARD_TDM_INFO db88f6650Tdm880[] = { {0} };

MV_BOARD_TDM_SPI_INFO db88f6650TdmSpiInfo[] = { {1} };

MV_BOARD_INFO db88f6650_board_info = {
	.boardName			= "DB-88F6650",
	.numBoardMppTypeValue		= ARRSZ(db88f6650InfoBoardModTypeInfo),
	.pBoardModTypeValue		= db88f6650InfoBoardModTypeInfo,
	.pBoardMppConfigValue		= db88f6650InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(db88f6650InfoBoardDeCsInfo),
	.pDevCsInfo			= db88f6650InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(db88f6650InfoBoardTwsiDev),
	.pBoardTwsiDev			= db88f6650InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(db88f6650InfoBoardMacInfo),
	.pBoardMacInfo			= db88f6650InfoBoardMacInfo,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= 0,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 80000,

	/* GPP values */
	.gppOutEnValLow			= DB_88F6650_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= DB_88F6650_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= DB_88F6650_GPP_OUT_ENA_HIGH,
	.gppOutValLow			= DB_88F6650_GPP_OUT_VAL_LOW,
	.gppOutValMid			= DB_88F6650_GPP_OUT_VAL_MID,
	.gppOutValHigh			= DB_88F6650_GPP_OUT_VAL_HIGH,
	.gppPolarityValLow		= DB_88F6650_GPP_POL_LOW,
	.gppPolarityValMid		= DB_88F6650_GPP_POL_MID,
	.gppPolarityValHigh		= DB_88F6650_GPP_POL_HIGH,

	/* TDM */
	.numBoardTdmInfo		= {1},
	.pBoardTdmInt2CsInfo		= {db88f6650Tdm880},
	.boardTdmInfoIndex		= 0,

	.pBoardSpecInit			= NULL,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,
	.pBoardTdmSpiInfo		= db88f6650TdmSpiInfo,

	/* NOR init params */
	.norFlashReadParams		= 0,
	.norFlashWriteParams		= 0,
	/* Enable modules auto-detection. */
	.configAutoDetect		= MV_TRUE
};

/*******************************************************************************
 * AvantaLP RD-88F6650 board */
/*******************************************************************************/

MV_BOARD_TWSI_INFO rd88f6650InfoBoardTwsiDev[] = {
	/* {{MV_BOARD_DEV_CLASS devClass, MV_U8 devClassId,  MV_U8 twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{ BOARD_DEV_TWSI_SATR,		0,	0x4C,	   ADDR7_BIT	},
	{ BOARD_DEV_TWSI_SATR,		1,	0x4E,	   ADDR7_BIT	},
};

/* When Switch Port 4 is connected to external PHY through RGMII-0,
 * this external PHY is managed through MAC-0 SMI lines.
 * The 'boardEthSmiAddr' variable is used only for PHY init. */
MV_BOARD_MAC_INFO rd88f6650InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED boardMacSpeed, MV_32 boardEthSmiAddr , MV_32 boardEthSmiAddr0;}} */
	{ BOARD_MAC_SPEED_2000M, 0x1, 0x1},
	{ BOARD_MAC_SPEED_AUTO,  0x1, 0x1},
	{ BOARD_MAC_SPEED_1000M,  -1, -1 },
	{ BOARD_MAC_SPEED_1000M,  -1, -1 },
};
MV_BOARD_MPP_TYPE_INFO rd88f6650InfoBoardModTypeInfo[] = {
	{
		.boardMppSlic = MV_BOARD_SLIC_TDM2C_ID,
		.ethSataComplexOpt = MV_ETHCOMP_SW_P0_2_GE_PHY_P0 |
				     MV_ETHCOMP_SW_P1_2_GE_PHY_P1 |
				     MV_ETHCOMP_SW_P2_2_GE_PHY_P2 |
				     MV_ETHCOMP_SW_P3_2_GE_PHY_P3 |
				     MV_ETHCOMP_P2P_MAC0_2_SW_SPEED_2G |
				     MV_ETHCOMP_SW_P4_2_RGMII0   |
				     MV_ETHCOMP_SW_P4_2_RGMII0_EXT_PHY |
				     MV_ETHCOMP_GE_MAC0_2_SW_P6,
		.ethPortsMode = 0x0
	}
};


MV_DEV_CS_INFO rd88f6650InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{ SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8 } /* SPI DEV */
#endif
};

MV_BOARD_MPP_INFO rd88f6650InfoBoardMppConfigValue[] = {
	{ {
		RD_88F6650_MPP0_7,
		RD_88F6650_MPP8_15,
		RD_88F6650_MPP16_23,
		RD_88F6650_MPP24_31,
		RD_88F6650_MPP32_39,
		RD_88F6650_MPP40_47,
		RD_88F6650_MPP48_55,
		RD_88F6650_MPP56_63,
		RD_88F6650_MPP64_67,
	} }
};

MV_BOARD_INFO rd88f6650_board_info = {
	.boardName			= "RD-88F6650",
	.numBoardMppTypeValue		= ARRSZ(rd88f6650InfoBoardModTypeInfo),
	.pBoardModTypeValue		= rd88f6650InfoBoardModTypeInfo,
	.pBoardMppConfigValue		= rd88f6650InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(rd88f6650InfoBoardDeCsInfo),
	.pDevCsInfo			= rd88f6650InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(rd88f6650InfoBoardTwsiDev),
	.pBoardTwsiDev			= rd88f6650InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(rd88f6650InfoBoardMacInfo),
	.pBoardMacInfo			= rd88f6650InfoBoardMacInfo,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= 0,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 80000,

	/* GPP values */
	.gppOutEnValLow			= RD_88F6650_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= RD_88F6650_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= RD_88F6650_GPP_OUT_ENA_HIGH,
	.gppOutValLow			= RD_88F6650_GPP_OUT_VAL_LOW,
	.gppOutValMid			= RD_88F6650_GPP_OUT_VAL_MID,
	.gppOutValHigh			= RD_88F6650_GPP_OUT_VAL_HIGH,
	.gppPolarityValLow		= RD_88F6650_GPP_POL_LOW,
	.gppPolarityValMid		= RD_88F6650_GPP_POL_MID,
	.gppPolarityValHigh		= RD_88F6650_GPP_POL_HIGH,

	/* TDM */
	.numBoardTdmInfo		= {},
	.pBoardTdmInt2CsInfo		= {},
	.boardTdmInfoIndex		= -1,

	.pBoardSpecInit			= NULL,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,
	/* NOR init params */
	.norFlashReadParams		= 0,
	.norFlashWriteParams		= 0,
	/* Enable modules auto-detection. */
	.configAutoDetect		= MV_FALSE
};

/*******************************************************************************
 * AvantaLP RD-88F6660 board */
/*******************************************************************************/
MV_BOARD_TWSI_INFO rd88f6660InfoBoardTwsiDev[] = {
	/* {{MV_BOARD_DEV_CLASS devClass, MV_U8 devClassId,  MV_U8 twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{ BOARD_DEV_TWSI_IO_EXPANDER,	0,	0x20,	ADDR7_BIT },
};

/* When Switch Port 4 is connected to external PHY through RGMII-0,
 * this external PHY is managed through MAC-0 SMI lines.
 * The 'boardEthSmiAddr' variable is used only for PHY init. */
MV_BOARD_MAC_INFO rd88f6660InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED boardMacSpeed, MV_32 boardEthSmiAddr , MV_32 boardEthSmiAddr0;}} */
	{ BOARD_MAC_SPEED_2000M,  -1, -1 },
	{ BOARD_MAC_SPEED_AUTO,  0x1, 0x1},
	{ BOARD_MAC_SPEED_1000M,  -1, -1 },
	{ BOARD_MAC_SPEED_1000M,  -1, -1 },
};
MV_BOARD_MPP_TYPE_INFO rd88f6660InfoBoardModTypeInfo[] = {
	{
		.boardMppSlic = MV_BOARD_SLIC_TDMMC_ID,
		.ethSataComplexOpt = MV_ETHCOMP_SW_P0_2_GE_PHY_P0 |
				     MV_ETHCOMP_SW_P1_2_GE_PHY_P1 |
				     MV_ETHCOMP_SW_P2_2_GE_PHY_P2 |
				     MV_ETHCOMP_SW_P3_2_GE_PHY_P3 |
				     MV_ETHCOMP_P2P_MAC0_2_SW_SPEED_2G |
				     MV_ETHCOMP_SW_P4_2_RGMII0   |
				     MV_ETHCOMP_P2P_MAC_2_PON_ETH_SERDES |
				     MV_ETHCOMP_GE_MAC0_2_SW_P6,
		.ethPortsMode = 0x0
	}
};

MV_DEV_CS_INFO rd88f6660InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{ SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8 } /* SPI DEV */
#endif
};

MV_BOARD_TDM_INFO rd88f6660Tdm886[] = { {0}, {1} };

MV_BOARD_TDM_SPI_INFO rd88f6660TdmSpiInfo[] = { {1} };

MV_BOARD_MPP_INFO rd88f6660InfoBoardMppConfigValue[] = {
	{ {
		RD_88F6660_MPP0_7,
		RD_88F6660_MPP8_15,
		RD_88F6660_MPP16_23,
		RD_88F6660_MPP24_31,
		RD_88F6660_MPP32_39,
		RD_88F6660_MPP40_47,
		RD_88F6660_MPP48_55,
		RD_88F6660_MPP56_63,
		RD_88F6660_MPP64_67,
	} }
};

MV_BOARD_INFO rd88f6660_board_info = {
	.boardName			= "RD-88F6660",
	.numBoardMppTypeValue		= ARRSZ(rd88f6660InfoBoardModTypeInfo),
	.pBoardModTypeValue		= rd88f6660InfoBoardModTypeInfo,
	.pBoardMppConfigValue		= rd88f6660InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(rd88f6660InfoBoardDeCsInfo),
	.pDevCsInfo			= rd88f6660InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(rd88f6660InfoBoardTwsiDev),
	.pBoardTwsiDev			= rd88f6660InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(rd88f6660InfoBoardMacInfo),
	.pBoardMacInfo			= rd88f6660InfoBoardMacInfo,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= 0,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 80000,

	/* GPP values */
	.gppOutEnValLow			= RD_88F6660_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= RD_88F6660_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= RD_88F6660_GPP_OUT_ENA_HIGH,
	.gppOutValLow			= RD_88F6660_GPP_OUT_VAL_LOW,
	.gppOutValMid			= RD_88F6660_GPP_OUT_VAL_MID,
	.gppOutValHigh			= RD_88F6660_GPP_OUT_VAL_HIGH,
	.gppPolarityValLow		= RD_88F6660_GPP_POL_LOW,
	.gppPolarityValMid		= RD_88F6660_GPP_POL_MID,
	.gppPolarityValHigh		= RD_88F6660_GPP_POL_HIGH,

	/* TDM */
	.numBoardTdmInfo		= {2},
	.pBoardTdmInt2CsInfo		= {rd88f6660Tdm886},
	.boardTdmInfoIndex		= 0,

	.pBoardSpecInit			= NULL,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,

	.pBoardTdmSpiInfo		= rd88f6660TdmSpiInfo,

	/* NOR init params */
	.norFlashReadParams		= 0,
	.norFlashWriteParams		= 0,
	/* Enable modules auto-detection. */
	.configAutoDetect		= MV_FALSE
};

/*******************************************************************************
 * AvantaLP FPGA board
*******************************************************************************/
MV_BOARD_INFO avanta_lp_fpga_board_info = {
	.boardName			= "CortexA9-FPGA",
	.numBoardMppTypeValue		= 0,
	.pBoardModTypeValue		= NULL,
	.pBoardMppConfigValue		= NULL,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= 0,
	.pDevCsInfo			= NULL,
	.numBoardTwsiDev		= 0,
	.pBoardTwsiDev			= NULL,
	.numBoardMacInfo		= 0,
	.pBoardMacInfo			= NULL,
	.numBoardGppInfo		= 0,
	.pBoardGppInfo			= NULL,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* PMU Power */
	.pmuPwrUpPolarity		= 0,
	.pmuPwrUpDelay			= 0,

	/* GPP values */
	.gppOutEnValLow			= 0,
	.gppOutEnValMid			= 0,
	.gppOutEnValHigh		= 0,
	.gppOutValLow			= 0,
	.gppOutValMid			= 0,
	.gppOutValHigh			= 0,
	.gppPolarityValLow		= 0,
	.gppPolarityValMid		= 0,
	.gppPolarityValHigh		= 0,

	/* TDM */
	.numBoardTdmInfo		= {},
	.pBoardTdmInt2CsInfo		= {},
	.boardTdmInfoIndex		= -1,

	/* NAND init params */
	.nandFlashReadParams		= 0,
	.nandFlashWriteParams		= 0,
	.nandFlashControl		= 0,
	.nandIfMode             = NAND_IF_NFC,
};

/*
 * All supported avanta boards
 */
MV_BOARD_INFO *marvellBoardInfoTbl[] = {
	&rd88f6650_board_info,
	&db88f6650_board_info,
	&rd88f6660_board_info,
	&db88f6660_board_info,
	&avanta_lp_fpga_board_info,
};

