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

#define ARRSZ(x)	(sizeof(x)/sizeof(x[0]))

/**********************/
/* BOBCAT2-DB BOARD */
/**********************/
#define DB_98DX4251_BOARD_NAND_READ_PARAMS	0x000C0282
#define DB_98DX4251_BOARD_NAND_WRITE_PARAMS	0x00010305
/*NAND care support for small page chips*/
#define DB_98DX4251_BOARD_NAND_CONTROL		0x01c00543

#define DB_98DX4251_BOARD_NOR_READ_PARAMS	0x403E07CF
#define DB_98DX4251_BOARD_NOR_WRITE_PARAMS	0x000F0F0F

MV_BOARD_TWSI_INFO	db98DX4251InfoBoardTwsiDev[] = {
/* {{MV_BOARD_DEV_CLASS	devClass, MV_U8	twsiDevAddr, MV_U8 twsiDevAddrType}} */
	{BOARD_DEV_TWSI_PLD, 0x30, ADDR7_BIT},		/* Access to control PLD reg file */
	{BOARD_DEV_TWSI_ZARLINK, 0x18, ADDR7_BIT},		/* Access to Zarlink 	*/
	{BOARD_DEV_TWSI_SATR, 0x4C, ADDR7_BIT},         /* SatR bios 0		*/
	{BOARD_DEV_TWSI_SATR, 0x4D, ADDR7_BIT},         /* SatR bios 1		*/
	{BOARD_DEV_TWSI_SATR, 0x4E, ADDR7_BIT},          /* SatR bios 2		*/
	{BOARD_DEV_TWSI_SATR, 0x4F, ADDR7_BIT},          /* SatR bios 3		*/
	{BOARD_DEV_TWSI_INIT_EPROM, 0x50, ADDR7_BIT},          /* Serial Ini EPROM	*/
	{BOARD_DEV_TWSI_PCA9555_IO_EXPANDER, 0x20, ADDR7_BIT},          /* Qsgmii/sfp mux control PCA9555 IO expander */
	{BOARD_DEV_TWSI_PCA9548_IO_MUX, 0x70, ADDR7_BIT},          /* PCA9548 I2C mux 0	*/
	{BOARD_DEV_TWSI_PCA9548_IO_MUX, 0x71, ADDR7_BIT},          /* PCA9548 I2C mux 1	*/
	{BOARD_DEV_TWSI_PCA9548_IO_MUX, 0x75, ADDR7_BIT}          /* PCA9548 I2C mux 2	*/
};

MV_BOARD_MAC_INFO db98DX4251InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED	boardMacSpeed, MV_U8 boardEthSmiAddr}} */
	{BOARD_MAC_SPEED_AUTO, 0x0,0x0 	  , 0x0 },
	{BOARD_MAC_SPEED_AUTO, 0x1,0x0	  , 0x1 },
};

MV_BOARD_MODULE_TYPE_INFO db98DX4251InfoBoardModTypeInfo[] = {
	{
		.boardMppMod		= MV_BOARD_AUTO,
	}
};

MV_BOARD_GPP_INFO db98DX4251InfoBoardGppInfo[] = {
	/* {{MV_BOARD_GPP_CLASS	devClass, MV_U8	gppPinNum}} */
	{BOARD_GPP_USB_VBUS,    24} /* from MPP map */
	/*{BOARD_GPP_RESET,       47},*/
};

MV_DEV_CS_INFO db98DX4251InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth, busWidth }*/
#if defined(MV_INCLUDE_SPI)
	{SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8, 8}, /* SPI DEV */
#endif
#if defined(MV_INCLUDE_NOR)
	{DEV_BOOCS, N_A, BOARD_DEV_NOR_FLASH, 16, 16} /* NOR DEV */
#endif
};

MV_BOARD_MPP_INFO db98DX4251InfoBoardMppConfigValue[] = {
	{ {
	DB_98DX4251_MPP0_7,
	DB_98DX4251_MPP8_15,
	DB_98DX4251_MPP16_23,
	DB_98DX4251_MPP24_31,
	DB_98DX4251_MPP32_39,
	} },
};

MV_BOARD_INFO db98DX4251Info = {
	.boardName			= "DB-98DX4251-BP",
	.numBoardMppTypeValue		= ARRSZ(db98DX4251InfoBoardModTypeInfo),
	.pBoardModTypeValue		= db98DX4251InfoBoardModTypeInfo,
	.numBoardMppConfigValue		= ARRSZ(db98DX4251InfoBoardMppConfigValue),
	.pBoardMppConfigValue		= db98DX4251InfoBoardMppConfigValue,
	.intsGppMaskLow			= 0,
	.intsGppMaskMid			= 0,
	.intsGppMaskHigh		= 0,
	.numBoardDeviceIf		= ARRSZ(db98DX4251InfoBoardDeCsInfo),
	.pDevCsInfo			= db98DX4251InfoBoardDeCsInfo,
	.numBoardTwsiDev		= ARRSZ(db98DX4251InfoBoardTwsiDev),
	.pBoardTwsiDev			= db98DX4251InfoBoardTwsiDev,
	.numBoardMacInfo		= ARRSZ(db98DX4251InfoBoardMacInfo),
	.pBoardMacInfo			= db98DX4251InfoBoardMacInfo,
	.numBoardGppInfo		= ARRSZ(db98DX4251InfoBoardGppInfo),
	.pBoardGppInfo			= db98DX4251InfoBoardGppInfo,
	.activeLedsNumber		= 0,
	.pLedGppPin			= NULL,
	.ledsPolarity			= 0,

	/* GPP values */
	.gppOutEnValLow			= DB_98DX4251_GPP_OUT_ENA_LOW,
	.gppOutEnValMid			= DB_98DX4251_GPP_OUT_ENA_MID,
	.gppOutEnValHigh		= 0,
	.gppOutValLow			= DB_98DX4251_GPP_OUT_VAL_LOW,
	.gppOutValMid			= DB_98DX4251_GPP_OUT_VAL_MID,
	.gppOutValHigh			= 0,
	.gppPolarityValLow		= DB_98DX4251_GPP_POL_LOW,
	.gppPolarityValMid		= DB_98DX4251_GPP_POL_MID,
	.gppPolarityValHigh		= 0,

	/* External Switch Configuration */
	.pSwitchInfo = NULL,
	.switchInfoNum = 0,


	/* NAND init params */
	.nandFlashReadParams		= DB_98DX4251_BOARD_NAND_READ_PARAMS,
	.nandFlashWriteParams		= DB_98DX4251_BOARD_NAND_WRITE_PARAMS,
	.nandFlashControl		= DB_98DX4251_BOARD_NAND_CONTROL,
	/* NOR init params */
	.norFlashReadParams		= DB_98DX4251_BOARD_NOR_READ_PARAMS,
	.norFlashWriteParams		= DB_98DX4251_BOARD_NOR_WRITE_PARAMS
};

/*****************************/
/* BobCat2 RD 98DX4051 BOARD */
/*****************************/
#define RD_98DX4051_BOARD_NAND_READ_PARAMS		0x000C0282
#define RD_98DX4051_BOARD_NAND_WRITE_PARAMS	0x00010305
/*NAND care support for small page chips*/
#define RD_98DX4051_BOARD_NAND_CONTROL			0x01c00543

MV_BOARD_MAC_INFO rd98DX4051InfoBoardMacInfo[] = {
	/* {{MV_BOARD_MAC_SPEED	boardMacSpeed, MV_U8 boardEthSmiAddr}} */
	{BOARD_MAC_SPEED_1000M, 0x0, 0x0, 0x0},
	{BOARD_MAC_SPEED_1000M, 0x1, 0x0, 0x1},
};

MV_BOARD_MODULE_TYPE_INFO rd98DX4051InfoBoardModTypeInfo[] = {
	{
		.boardMppMod		= MV_BOARD_AUTO,
	}
};

MV_DEV_CS_INFO rd98DX4051InfoBoardDeCsInfo[] = {
	/*{deviceCS, params, devType, devWidth}*/
#if defined(MV_INCLUDE_SPI)
	{SPI_CS0, N_A, BOARD_DEV_SPI_FLASH, 8} /* SPI DEV */
#endif
};

MV_BOARD_MPP_INFO rd98DX4051InfoBoardMppConfigValue[] = {
	{ {
		RD_98DX4051_MPP0_7,
		RD_98DX4051_MPP8_15,
		RD_98DX4051_MPP16_23,
		RD_98DX4051_MPP24_31,
		RD_98DX4051_MPP32_39,
	} }
};
MV_U8	rd98DX4051InfoBoardDebugLedIf[] = {17, 18, 19};

MV_BOARD_INFO rd98DX4051Info = {
 .boardName				= "RD-98DX4051-SERVER",
 .numBoardMppTypeValue			= ARRSZ(rd98DX4051InfoBoardModTypeInfo),
 .pBoardModTypeValue			= rd98DX4051InfoBoardModTypeInfo,
 .numBoardMppConfigValue		= ARRSZ(rd98DX4051InfoBoardMppConfigValue),
 .pBoardMppConfigValue			= rd98DX4051InfoBoardMppConfigValue,
 .intsGppMaskLow			= 0,
 .intsGppMaskMid			= 0,
 .intsGppMaskHigh			= 0,
 .numBoardDeviceIf			= ARRSZ(rd98DX4051InfoBoardDeCsInfo),
 .pDevCsInfo				= rd98DX4051InfoBoardDeCsInfo,
 .numBoardTwsiDev			= 0,
 .pBoardTwsiDev				= NULL,
 .numBoardMacInfo			= ARRSZ(rd98DX4051InfoBoardMacInfo),
 .pBoardMacInfo				= rd98DX4051InfoBoardMacInfo,
 .numBoardGppInfo			= 0,
 .pBoardGppInfo				= NULL,
	.activeLedsNumber		= ARRSZ(rd98DX4051InfoBoardDebugLedIf),
 .pLedGppPin				= rd98DX4051InfoBoardDebugLedIf,
 .ledsPolarity				= 0,

 /* GPP values */
 .gppOutEnValLow			= RD_98DX4051_GPP_OUT_ENA_LOW,
 .gppOutEnValMid			= RD_98DX4051_GPP_OUT_ENA_MID,
 .gppOutEnValHigh			= 0,
 .gppOutValLow				= RD_98DX4051_GPP_OUT_VAL_LOW,
 .gppOutValMid				= RD_98DX4051_GPP_OUT_VAL_MID,
 .gppOutValHigh				= 0,
 .gppPolarityValLow			= RD_98DX4051_GPP_POL_LOW,
 .gppPolarityValMid			= RD_98DX4051_GPP_POL_MID,
 .gppPolarityValHigh			= 0,

	/* External Switch Configuration */
	.pSwitchInfo = NULL,
	.switchInfoNum = 0,

 /* NAND init params */
 .nandFlashReadParams			= RD_98DX4051_BOARD_NAND_READ_PARAMS,
 .nandFlashWriteParams			= RD_98DX4051_BOARD_NAND_WRITE_PARAMS,
 .nandFlashControl			= RD_98DX4051_BOARD_NAND_CONTROL
};


/*********************************************************************************/

MV_BOARD_INFO *boardInfoTbl[] = {
	&db98DX4251Info,
	&rd98DX4051Info
};