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

#ifndef __INCmvBoardEnvLibh
#define __INCmvBoardEnvLibh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */
/* The below constant macros defines the board I2C EEPROM data offsets */

#include "ctrlEnv/mvCtrlEnvLib.h"
#include "mvSysHwConfig.h"
#include "boardEnv/mvBoardEnvSpec.h"
#include "twsi/mvTwsi.h"
#include "nfc/mvNfc.h"

#define ARRSZ(x)                (sizeof(x) / sizeof(x[0]))
#define BOARD_ETH_SWITCH_PORT_NUM       7
#define BOARD_ETH_SWITCH_SMI_SCAN_MODE	1	/* Use manual scanning mode */
#define MV_BOARD_MAX_MPP                9       /* number of MPP conf registers */
#define MV_BOARD_MAX_MPP_GROUPS         9
#define MV_BOARD_MPP_GROUPS_MAX_TYPES   9
#define MV_BOARD_NAME_LEN               0x20

typedef enum _devBoardSlicType {
	MV_BOARD_SLIC_DISABLED,
	MV_BOARD_SLIC_SSI_ID, /* Lantiq Integrated SLIC */
	MV_BOARD_SLIC_ISI_ID, /* Silicon Labs ISI Bus */
	MV_BOARD_SLIC_ZSI_ID, /* Zarlink ZSI Bus */
	MV_BOARD_SLIC_TDM2C_ID, /* Cross vendor external SLIC for TDM2C */
	MV_BOARD_SLIC_TDMMC_ID, /* Cross vendor external SLIC for TDMMC */
	BOARD_TDM_SLIC_COUNT,
	MV_BOARD_SLIC_MAX_OPTION = BOARD_TDM_SLIC_COUNT,
} MV_BOARD_SLIC_TYPE;

typedef enum _devBoardOtherTypeClass {
	MV_BOARD_NONE    = 0x00000000,
	MV_BOARD_LVDS    = 0x00000001,
	MV_BOARD_PEX     = 0x00000002,
	MV_BOARD_SWITCH  = 0x00000004,
	MV_BOARD_SETM    = 0x00000008,
	MV_BOARD_UNKNOWN = 0x80000000
} MV_BOARD_OTHER_TYPE_CLASS;

/* omriii:  decide between MODULE_TYPE or MPP_TYPE */
typedef struct _boardMppTypeInfo {
	MV_BOARD_SLIC_TYPE boardMppSlic;

	/* Ethernet / Sata complex                      */
	/* A bitmask of MV_ETH_SATA_COMPLEX_OPTIONS     */
	MV_U32 ethSataComplexOpt;
	MV_U32 ethPortsMode;    /* bitmask of MV_ETH_PORT_MODE */
} MV_BOARD_MPP_TYPE_INFO;

typedef enum _devBoardClass {
	BOARD_DEV_NOR_FLASH,
	BOARD_DEV_NAND_FLASH,
	BOARD_DEV_SEVEN_SEG,
	BOARD_DEV_FPGA,
	BOARD_DEV_SRAM,
	BOARD_DEV_SPI_FLASH,
	BOARD_DEV_OTHER
} MV_BOARD_DEV_CLASS;

typedef enum _devTwsiBoardClass {
	BOARD_TWSI_RTC,
	BOARD_DEV_TWSI_EXP,
	BOARD_DEV_TWSI_SATR,
	BOARD_TWSI_MUX,
	BOARD_DEV_TWSI_EEPROM,
	BOARD_DEV_TWSI_IO_EXPANDER,
	BOARD_TWSI_OTHER
} MV_BOARD_TWSI_CLASS;

typedef enum _devGppBoardClass {
	BOARD_GPP_RTC,
	BOARD_GPP_MV_SWITCH,
	BOARD_GPP_USB_VBUS,
	BOARD_GPP_USB_VBUS_EN,
	BOARD_GPP_USB_OC,
	BOARD_GPP_USB_HOST_DEVICE,
	BOARD_GPP_REF_CLCK,
	BOARD_GPP_VOIP_SLIC,
	BOARD_GPP_LIFELINE,
	BOARD_GPP_BUTTON,
	BOARD_GPP_TS_BUTTON_C,
	BOARD_GPP_TS_BUTTON_U,
	BOARD_GPP_TS_BUTTON_D,
	BOARD_GPP_TS_BUTTON_L,
	BOARD_GPP_TS_BUTTON_R,
	BOARD_GPP_POWER_BUTTON,
	BOARD_GPP_RESTOR_BUTTON,
	BOARD_GPP_WPS_BUTTON,
	BOARD_GPP_HDD0_POWER,
	BOARD_GPP_HDD1_POWER,
	BOARD_GPP_FAN_POWER,
	BOARD_GPP_RESET,
	BOARD_GPP_POWER_ON_LED,
	BOARD_GPP_HDD_POWER,
	BOARD_GPP_SDIO_POWER,
	BOARD_GPP_SDIO_DETECT,
	BOARD_GPP_SDIO_WP,
	BOARD_GPP_SWITCH_PHY_INT,
	BOARD_GPP_TSU_DIRCTION,
	BOARD_GPP_PEX_RESET,
	BOARD_GPP_CONF,
	BOARD_GPP_PON_XVR_TX,
	BOARD_GPP_SYS_LED,
	BOARD_GPP_PON_LED,
	BOARD_GPP_OTHER
} MV_BOARD_GPP_CLASS;

typedef struct _devCsInfo {
	MV_U8 deviceCS;
	MV_U32 params;
	MV_U32 devClass;        /* MV_BOARD_DEV_CLASS */
	MV_U8 devWidth;
	MV_U8 busWidth;
} MV_DEV_CS_INFO;

typedef struct _boardLedInfo {
	MV_U8 activeLedsNumber;
	MV_U8 ledsPolarity;     /* '0' or '1' to turn on led */
	MV_U8 *gppPinNum;       /* Pointer to GPP values */
} MV_BOARD_LED_INFO;

typedef struct _boardGppInfo {
	MV_BOARD_GPP_CLASS devClass;
	MV_U8 gppPinNum;
} MV_BOARD_GPP_INFO;

typedef struct _boardTwsiInfo {
	MV_BOARD_TWSI_CLASS devClass;
	MV_U8 devClassId;
	MV_U8 twsiDevAddr;
	MV_U8 twsiDevAddrType;
} MV_BOARD_TWSI_INFO;

typedef struct _boardSatrInfo {
	MV_SATR_TYPE_ID satrId;
	MV_U32 mask;
	MV_U32 offset;
	MV_U32 regNum;
	MV_U32 isWriteable[MV_MARVELL_BOARD_NUM];
} MV_BOARD_SATR_INFO;

typedef struct _boardConfigTypesInfo {
	MV_CONFIG_TYPE_ID configId;
	MV_U32 mask;
	MV_U32 offset;
	MV_U32 expanderNum;
	MV_U32 regNum;
	MV_U32 isActiveForBoard[MV_MARVELL_BOARD_NUM];
} MV_BOARD_CONFIG_TYPE_INFO;

typedef enum _boardMacSpeed {
	BOARD_MAC_SPEED_10M,
	BOARD_MAC_SPEED_100M,
	BOARD_MAC_SPEED_1000M,
	BOARD_MAC_SPEED_2000M,
	BOARD_MAC_SPEED_AUTO
} MV_BOARD_MAC_SPEED;

typedef struct _boardMacInfo {
	MV_BOARD_MAC_SPEED boardMacSpeed;
	MV_32 boardEthSmiAddr;
	MV_32 boardEthSmiAddr0;
} MV_BOARD_MAC_INFO;

typedef struct _boardMppInfo {
	MV_U32 mppGroup[MV_BOARD_MAX_MPP];
} MV_BOARD_MPP_INFO;

typedef enum {
	BOARD_EPON_CONFIG,
	BOARD_GPON_CONFIG,
	BOARD_PON_NONE,
	BOARD_PON_AUTO
} MV_BOARD_PON_CONFIG;

typedef struct {
	MV_U8 spiCs;
} MV_BOARD_TDM_INFO;

typedef struct _boardPexInfo {
	MV_PEX_UNIT_CFG pexUnitCfg[MV_PEX_MAX_UNIT];
	MV_U32 boardPexIfNum;
} MV_BOARD_PEX_INFO;

typedef struct {
	MV_U8 spiId;
} MV_BOARD_TDM_SPI_INFO;

/* Board specific initialization. Performed before initializing the SoC. */
typedef struct {
	MV_U32 reg;
	MV_U32 mask;
	MV_U32 val;
} MV_BOARD_SPEC_INIT;

typedef enum _mvIoExpanderTypeID {
	MV_IO_EXPANDER_SFP0_TX_DIS,
	MV_IO_EXPANDER_SFP0_PRSNT,
	MV_IO_EXPANDER_SFP0_TX_FAULT,
	MV_IO_EXPANDER_SFP0_LOS,
	MV_IO_EXPANDER_SFP1_PRSNT,
	MV_IO_EXPANDER_SFP1_TX_FAULT,
	MV_IO_EXPANDER_SFP1_LOS,
	MV_IO_EXPANDER_SFP1_TX_DIS,
	MV_IO_EXPANDER_USB_VBUS,
	MV_IO_EXPANDER_MAC0_RJ45_PORT_LED,
	MV_IO_EXPANDER_MAC0_SFP_PORT_LED,
	MV_IO_EXPANDER_MAC1_RJ45_PORT_LED,
	MV_IO_EXPANDER_MAC1_SFP_PORT_LED,
	MV_IO_EXPANDER_PON_PORT_LED,
	MV_IO_EXPANDER_SD_STATUS,
	MV_IO_EXPANDER_SD_WRITE_PROTECT,
	MV_IO_EXPANDER_JUMPER1,
	MV_IO_EXPANDER_JUMPER2_EEPROM_ENABLED,
	MV_IO_EXPANDER_JUMPER3,
	MV_IO_EXPANDER_EXT_PHY_SMI_EN,
	MV_IO_EXPANDER_SPI1_CS_MSB0,
	MV_IO_EXPANDER_SPI1_CS_MSB1,
	MV_IO_EXPANDER_INTEG_PHY_PORTS_LED,
	MV_IO_EXPANDER_USB_SUPER_SPEED,
	MV_IO_EXPANDER_PCIE_WAKE_EN0,
	MV_IO_EXPANDER_PCIE_WAKE_DIS0,
	MV_IO_EXPANDER_PCIE_WAKE_EN1,
	MV_IO_EXPANDER_PCIE_WAKE_DIS1,
	MV_IO_EXPANDER_COEX1_1,
	MV_IO_EXPANDER_COEX1_0,
	MV_IO_EXPANDER_COEX2_1,
	MV_IO_EXPANDER_COEX2_0,
	MV_IO_EXPANDER_HDD_SELECT,
	MV_IO_EXPANDER_HDD_PWR_EN,
	MV_IO_EXPANDER_TX_FAULT_RST,
	MV_IO_EXPANDER_MAX_OPTION
} MV_IO_EXPANDER_TYPE_ID;

typedef struct _boardIoExapnderTypesInfo {
	MV_IO_EXPANDER_TYPE_ID ioFieldid;
	MV_U32 offset;
	MV_U32 expanderNum;
	MV_U32 regNum;
} MV_BOARD_IO_EXPANDER_TYPE_INFO;

typedef enum {
	NAND_IF_NFC,
	NAND_IF_SPI,
	NAND_IF_NONE
} MV_NAND_IF_MODE;

typedef struct _boardInfo {
	char boardName[MV_BOARD_NAME_LEN];
	MV_U8 numBoardMppTypeValue;
	MV_BOARD_MPP_TYPE_INFO *pBoardModTypeValue;
	MV_U8 numBoardMppConfigValue;
	MV_BOARD_MPP_INFO *pBoardMppConfigValue;
	MV_U32 intsGppMaskLow;
	MV_U32 intsGppMaskMid;
	MV_U32 intsGppMaskHigh;
	MV_U8 numBoardDeviceIf;
	MV_DEV_CS_INFO *pDevCsInfo;
	MV_U8 numBoardTwsiDev;
	MV_BOARD_TWSI_INFO *pBoardTwsiDev;
	MV_U8 numBoardMacInfo;
	MV_BOARD_MAC_INFO *pBoardMacInfo;
	MV_U8 numBoardGppInfo;
	MV_BOARD_GPP_INFO *pBoardGppInfo;
	MV_U8 activeLedsNumber;
	MV_U8 *pLedGppPin;
	MV_U8 ledsPolarity;     /* '0' or '1' to turn on led */

	MV_U8 pmuPwrUpPolarity;
	MV_U32 pmuPwrUpDelay;
	/* GPP values */
	MV_U32 gppOutEnValLow;
	MV_U32 gppOutEnValMid;
	MV_U32 gppOutEnValHigh;
	MV_U32 gppOutValLow;
	MV_U32 gppOutValMid;
	MV_U32 gppOutValHigh;
	MV_U32 gppPolarityValLow;
	MV_U32 gppPolarityValMid;
	MV_U32 gppPolarityValHigh;

	/* PON configuration. */
	MV_BOARD_PON_CONFIG ponConfigValue;
	/* TDM configuration:
	 * We hold a different configuration array for each possible slic that
	 * can be connected to board.
	 * When modules are scanned, then we select the index of the relevant
	 * slic's information array.
	 * For RD and Customers boards we only need to initialize a single
	 * entry of the arrays below, and set the boardTdmInfoIndex to 0.
	 */
	MV_U8 numBoardTdmInfo[BOARD_TDM_SLIC_COUNT];
	MV_BOARD_TDM_INFO *pBoardTdmInt2CsInfo[BOARD_TDM_SLIC_COUNT];
	MV_16 boardTdmInfoIndex;

	/* Board specific initialization. Performed before initializing the SoC. */
	MV_BOARD_SPEC_INIT      *pBoardSpecInit;
	/* Deep-Idle power up delay */
	MV_U32 deepIdlePwrUpDelay;

	/* NAND init params */
	MV_U32 nandFlashReadParams;
	MV_U32 nandFlashWriteParams;
	MV_U32 nandFlashControl;
	MV_NAND_IF_MODE nandIfMode;

	MV_BOARD_TDM_SPI_INFO *pBoardTdmSpiInfo;
	MV_U32 norFlashReadParams;
	MV_U32 norFlashWriteParams;

	/* Indicates if auto-detection of modules is enabled on this board. */
	/* Set to MV_FALSE for any board that is not a DB. */
	MV_BOOL configAutoDetect;
} MV_BOARD_INFO;

/* {{MV_CONFIG_TYPE_ID ConfigID, MV_U32 Mask,  Offset, expanderNum,  regNum,    isActiveForBoard[]}} */
#define MV_BOARD_CONFIG_INFO { \
{ MV_CONFIG_MAC0,	       0x3,	0,	 0,		0,	{ 0, 1, 1, 1 } }, \
{ MV_CONFIG_MAC1,	       0xC,	2,	 0,		0,	{ 0, 1, 1, 1 } }, \
{ MV_CONFIG_MAC0_SW_SPEED,     0x10,	4,	 0,		0,	{ 0, 1, 0, 1 } }, \
{ MV_CONFIG_PON_BEN_POLARITY,  0x20,	5,	 0,		0,	{ 0, 0, 1, 1 } }, \
{ MV_CONFIG_SGMII0_CAPACITY,   0x40,	6,	 0,		0,	{ 0, 1, 0, 1 } }, \
{ MV_CONFIG_SGMII1_CAPACITY,   0x80,	7,	 0,		0,	{ 0, 1, 1, 1 } }, \
{ MV_CONFIG_SLIC_TDM_DEVICE,   0x7,	0,	 0,		1,	{ 0, 1, 1, 1 } }, \
{ MV_CONFIG_LANE1,	       0x18,	3,	 0,		1,	{ 0, 0, 0, 1 } }, \
{ MV_CONFIG_LANE2,	       0x20,	5,	 0,		1,	{ 0, 0, 0, 1 } }, \
{ MV_CONFIG_LANE3,	       0X40,	6,	 0,		1,	{ 0, 0, 0, 1 } }, \
{ MV_CONFIG_DEVICE_BUS_MODULE, 0x3,	0,	 1,		0,	{ 0, 0, 0, 1 } }, \
{ MV_CONFIG_DDR_BUSWIDTH,      0x4,	2,	 1,		0,	{ 0, 0, 0, 1 } }, \
{ MV_CONFIG_PON_SERDES,        0x18,	3,	 1,		0,	{ 0, 1, 0, 1 } }, \
{ MV_CONFIG_SW_P4,             0x20,	5,	 1,		0,	{ 0, 0, 0, 1 } }, \
};

#define MV_BOARD_CONFIG_CMD_STR "mac0, mac1, mac0_switch_speed, sgmii0capacity, sgmii1capacity, sw_port4\n" \
				"serdes1, serdes2, serdes3, ponserdes, ponpolarity, devicebus, tdm, ddr_buswidth\n\n"
#define MV_BOARD_CONFIG_CMD_MAX_OPTS 6
#define MV_BOARD_CONFIG_MAX_BYTE_COUNT		4
#define MV_BOARD_CONFIG_DEFAULT_VALUE		{0x0 }

/*MV_CMD_TYPE_ID,		command name,		Name,			numOfValues,	Possible Values */
#define MV_BOARD_CONFIG_CMD_INFO {\
{MV_CONFIG_MAC0,		"mac0",			"MAC0",				4, \
	{"Switch port 6", "GBE PHY#0", "RGMII-0", "SGMII(DB-6660)"} }		,\
{MV_CONFIG_MAC1,		"mac1",			"MAC1",				4, \
	{"RGMII-1", "Switch port 4", "GBE PHY#3", "RGMII-0"} }			,\
{MV_CONFIG_PON_SERDES,		"ponserdes",		"PON SERDES",			3, \
	{"PON MAC", "MAC1[SGMII]", "MAC1[SFP]"} }				,\
{MV_CONFIG_PON_BEN_POLARITY,	"ponpolarity",		"PON POLARITY (DB-6660)",	2, \
	{"BEN active low", "BEN active low"} }					,\
{MV_CONFIG_SGMII0_CAPACITY,	"sgmii0capacity",	"SGMII-0 Capacity",		2, \
	{"1G", "2.5G"} }							,\
{MV_CONFIG_SGMII1_CAPACITY,	"sgmii1capacity",	"SGMII-1 Capacity",		2, \
	{"1G", "2.5G"} }							,\
{MV_CONFIG_LANE1,		"serdes1",		"SerDes Lane#1 (DB-6660)",	4, \
	{"PCIe-1", "SGMII-0", "SATA-1", "Unconnected"} }			,\
{MV_CONFIG_LANE2,		"serdes2",		"SerDes Lane#2 (DB-6660)",	2, \
	{"SATA-0", "SGMII-0"} }							,\
{MV_CONFIG_LANE3,		"serdes3",		"SerDes Lane#3 (DB-6660)",	2, \
	{"USB3.0", "SGMII-0"} }							,\
{MV_CONFIG_MAC0_SW_SPEED,	"mac0_switch_speed",	"MAC0 to Switch Speed",		2, \
	{"2G", "1G"} }								,\
{MV_CONFIG_DEVICE_BUS_MODULE,	"devicebus",		"Device Bus Module (DB-6660)",	4, \
	{"None", "RGMII Module", "I2S Audio Module", "SPDIF Audio Module"} }	,\
{MV_CONFIG_SLIC_TDM_DEVICE,	"tdm",			"TDM module",			6, \
	{"None", "SSI", "ISI", "ZSI", "TDM2C", "TDMMC"} }			,\
{MV_CONFIG_DDR_BUSWIDTH,	"ddr_buswidth",		"DDR bus-width (DB-6660)",	2, \
	{"32-bit", "16-bit"} }							,\
{MV_CONFIG_SW_P4,		"switch_p4",		"SW-Port4",			2, \
	{"None", "RGMII0"} }							,\
};

/* MV_CONFIG_TYPE_ID ConfigID,      MV_U32 Offset,	 expanderNum,  regNum,   }} */
#define MV_BOARD_IO_EXP_DB6650_INFO { \
	/* 2nd IO Expander Register*/ \
{ MV_IO_EXPANDER_USB_VBUS,		 0,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_PRSNT,		 2,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_TX_FAULT,		 3,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_LOS,		 4,		 2,	 0}, \
{ MV_IO_EXPANDER_JUMPER2_EEPROM_ENABLED, 6,		 2,	 0}, \
{ MV_IO_EXPANDER_JUMPER3,		 7,		 2,	 0}, \
	/* 3rd IO Expander Register*/ \
{ MV_IO_EXPANDER_EXT_PHY_SMI_EN,	 0,		 2,	 1}, \
{ MV_IO_EXPANDER_SFP1_TX_DIS,		 1,		 2,	 1}, \
{ MV_IO_EXPANDER_MAC0_RJ45_PORT_LED,	 2,		 2,	 1}, \
{ MV_IO_EXPANDER_PON_PORT_LED,		 3,		 2,	 1}, \
{ MV_IO_EXPANDER_MAC1_SFP_PORT_LED,	 4,		 2,	 1}, \
{ MV_IO_EXPANDER_MAC1_RJ45_PORT_LED,	 5,		 2,	 1}, \
{ MV_IO_EXPANDER_INTEG_PHY_PORTS_LED,	 6,		 2,	 1}, \
};

#define MV_BOARD_IO_EXP_DB6660_INFO { \
	/* 1st IO Expander Register*/ \
{ MV_IO_EXPANDER_SFP0_TX_DIS,		 0,		 1,	 1}, \
{ MV_IO_EXPANDER_SFP0_PRSNT,		 1,		 1,	 1}, \
{ MV_IO_EXPANDER_SFP0_TX_FAULT,		 2,		 1,	 1}, \
{ MV_IO_EXPANDER_SFP0_LOS,		 3,		 1,	 1}, \
{ MV_IO_EXPANDER_USB_VBUS,		 4,		 1,	 1}, \
{ MV_IO_EXPANDER_MAC0_RJ45_PORT_LED,	 5,		 1,	 1}, \
{ MV_IO_EXPANDER_MAC0_SFP_PORT_LED,	 6,		 1,	 1}, \
{ MV_IO_EXPANDER_PON_PORT_LED,		 7,		 1,	 1}, \
	/* 2nd IO Expander Register*/ \
{ MV_IO_EXPANDER_SD_STATUS,		 0,		 2,	 0}, \
{ MV_IO_EXPANDER_SD_WRITE_PROTECT,	 1,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_PRSNT,		 2,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_TX_FAULT,		 3,		 2,	 0}, \
{ MV_IO_EXPANDER_SFP1_LOS,		 4,		 2,	 0}, \
{ MV_IO_EXPANDER_JUMPER1,		 6,		 2,	 0}, \
{ MV_IO_EXPANDER_JUMPER2_EEPROM_ENABLED, 7,		 2,	 0}, \
	/* 3rd IO Expander Register*/ \
{ MV_IO_EXPANDER_EXT_PHY_SMI_EN,	 0,		 2,	 1}, \
{ MV_IO_EXPANDER_SFP1_TX_DIS,		 1,		 2,	 1}, \
{ MV_IO_EXPANDER_SPI1_CS_MSB0,		 2,		 2,	 1}, \
{ MV_IO_EXPANDER_SPI1_CS_MSB1,		 3,		 2,	 1}, \
{ MV_IO_EXPANDER_MAC1_SFP_PORT_LED,	 4,		 2,	 1}, \
{ MV_IO_EXPANDER_MAC1_RJ45_PORT_LED,	 5,		 2,	 1}, \
{ MV_IO_EXPANDER_INTEG_PHY_PORTS_LED,	 6,		 2,	 1}, \
{ MV_IO_EXPANDER_USB_SUPER_SPEED,	 7,		 2,	 1}, \
};

#define MV_BOARD_IO_EXP_RD6660_INFO { \
	/* 1st IO Expander Register*/ \
{ MV_IO_EXPANDER_PCIE_WAKE_EN0,		 0,		 0,	 0}, \
{ MV_IO_EXPANDER_PCIE_WAKE_DIS0,	 1,		 0,	 0}, \
{ MV_IO_EXPANDER_PCIE_WAKE_EN1,		 2,		 0,	 0}, \
{ MV_IO_EXPANDER_PCIE_WAKE_DIS1,	 3,		 0,	 0}, \
{ MV_IO_EXPANDER_COEX1_1,		 4,		 0,	 0}, \
{ MV_IO_EXPANDER_COEX1_0,		 5,		 0,	 0}, \
{ MV_IO_EXPANDER_COEX2_0,		 6,		 0,	 0}, \
{ MV_IO_EXPANDER_COEX2_1,		 7,		 0,	 0}, \
	/* 2nd IO Expander Register*/ \
{ MV_IO_EXPANDER_USB_SUPER_SPEED,	 1,		 0,	 1}, \
{ MV_IO_EXPANDER_SD_WRITE_PROTECT,	 2,		 0,	 1}, \
{ MV_IO_EXPANDER_HDD_SELECT,		 3,		 0,	 1}, \
{ MV_IO_EXPANDER_HDD_PWR_EN,		 4,		 0,	 1}, \
{ MV_IO_EXPANDER_TX_FAULT_RST,		 7,		 0,	 1}, \
};

/* Boot device bus width */
#define MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS          3
/* Bus width field meaning for NOR/NAND */
#define MSAR_0_BOOT_DEV_BUS_WIDTH_8BIT          (0x0 << MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS)
#define MSAR_0_BOOT_DEV_BUS_WIDTH_16BIT         (0x1 << MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS)
#define MSAR_0_BOOT_DEV_BUS_WIDTH_32BIT         (0x2 << MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS)
/* Bus width field meaning for SPI */
#define MSAR_0_BOOT_DEV_BUS_WIDTH_SPI_24_16BIT  (0x1 << MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS)
#define MSAR_0_BOOT_DEV_BUS_WIDTH_SPI_32BIT     (0x0 << MSAR_0_BOOT_DEV_BUS_WIDTH_OFFS)

/* NAND page size */
#define MSAR_0_NAND_PAGE_SZ_OFFS                11
#define MSAR_0_NAND_PAGE_SZ_512B                (0x0 << MSAR_0_NAND_PAGE_SZ_OFFS)
#define MSAR_0_NAND_PAGE_SZ_2KB                 (0x1 << MSAR_0_NAND_PAGE_SZ_OFFS)
#define MSAR_0_NAND_PAGE_SZ_4KB                 (0x2 << MSAR_0_NAND_PAGE_SZ_OFFS)
#define MSAR_0_NAND_PAGE_SZ_8KB                 (0x3 << MSAR_0_NAND_PAGE_SZ_OFFS)

/* NAND ECC */
#define MSAR_0_NAND_ECC_OFFS                    14
#define MSAR_0_NAND_ECC_4BIT                    (0x0 << MSAR_0_NAND_ECC_OFFS)
#define MSAR_0_NAND_ECC_8BIT                    (0x1 << MSAR_0_NAND_ECC_OFFS)
#define MSAR_0_NAND_ECC_12BIT                   (0x2 << MSAR_0_NAND_ECC_OFFS)
#define MSAR_0_NAND_ECC_16BIT                   (0x3 << MSAR_0_NAND_ECC_OFFS)

#define MSAR_0_SPI0                             0
#define MSAR_0_SPI1                             1

MV_VOID mvBoardEnvInit(MV_VOID);
MV_U16 mvBoardModelGet(MV_VOID);
MV_U16 mvBoardRevGet(MV_VOID);
MV_STATUS mvBoardNameGet(char *pNameBuff, MV_U32 size);
MV_BOARD_SPEC_INIT *mvBoardSpecInitGet(MV_VOID);
MV_BOOL mvBoardIsPortInSgmii(MV_U32 ethPortNum);
MV_BOOL mvBoardIsPortInGmii(MV_U32 ethPortNum);
MV_BOOL mvBoardIsPortLoopback(MV_U32 ethPortNum);
MV_32 mvBoardPhyAddrGet(MV_U32 ethPortNum);
MV_VOID mvBoardPhyAddrSet(MV_U32 ethPortNum, MV_U32 smiAddr);
MV_32 mvBoardQuadPhyAddr0Get(MV_U32 ethPortNum);
MV_U8 mvBoardIoExpValGet(MV_BOARD_IO_EXPANDER_TYPE_INFO *ioInfo);
MV_STATUS mvBoardIoExpValSet(MV_BOARD_IO_EXPANDER_TYPE_INFO *ioInfo, MV_U8 value);
MV_STATUS mvBoardSatrInfoConfig(MV_SATR_TYPE_ID satrClass, MV_BOARD_SATR_INFO *satrInfo, MV_BOOL read);
MV_STATUS mvBoardConfigTypeGet(MV_CONFIG_TYPE_ID configClass, MV_BOARD_CONFIG_TYPE_INFO *configInfo);
MV_STATUS mvBoardIoExpanderTypeGet(MV_IO_EXPANDER_TYPE_ID ioClass, MV_BOARD_IO_EXPANDER_TYPE_INFO *ioInfo);
MV_STATUS mvBoardExtPhyBufferSelect(MV_BOOL enable);
MV_STATUS mvBoardSgmiiSfp0TxSet(MV_BOOL enable);
MV_STATUS mvBoardSgmiiSfp1TxSet(MV_BOOL enable);
MV_STATUS mvBoardHddExtSet(MV_BOOL enable);
MV_STATUS mvBoardHDDPowerSet(MV_BOOL enable);
MV_STATUS mvBoardUsbSsEnSet(MV_BOOL enable);
MV_U32 mvBoardTclkGet(MV_VOID);
MV_U32 mvBoardL2ClkGet(MV_VOID);
MV_U32 mvBoardSysClkGet(MV_VOID);
MV_U32 mvBoardDebugLedNumGet(MV_U32 boardId);
MV_VOID mvBoardDebugLed(MV_U32 hexNum);
MV_32 mvBoarGpioPinNumGet(MV_BOARD_GPP_CLASS class, MV_U32 index);
MV_VOID mvBoardReset(MV_VOID);
MV_32 mvBoardResetGpioPinGet(MV_VOID);
MV_32 mvBoardSDIOGpioPinGet(MV_BOARD_GPP_CLASS type);
MV_32 mvBoardUSBVbusGpioPinGet(MV_32 devId);
MV_32 mvBoardUSBVbusEnGpioPinGet(MV_32 devId);
MV_BOOL mvBoardIsOurPciSlot(MV_U32 busNum, MV_U32 slotNum);
MV_U32 mvBoardGpioIntMaskGet(MV_U32 gppGrp);
MV_U32 mvBoardSlicUnitTypeGet(MV_VOID);
MV_VOID mvBoardSlicUnitTypeSet(MV_U32 slicType);
MV_32 mvBoardMppGet(MV_U32 mppGroupNum);
MV_VOID mvBoardMppTypeSet(MV_U32 mppGroupNum, MV_U32 groupType);
MV_VOID mvBoardMppSet(MV_U32 mppGroupNum, MV_U32 mppValue);
MV_32 mvBoardTdmSpiModeGet(MV_VOID);
MV_U8 mvBoardTdmDevicesCountGet(void);
MV_U8 mvBoardTdmSpiCsGet(MV_U8 devId);
MV_U8 mvBoardTdmSpiIdGet(MV_VOID);
MV_VOID mvBoardAudioModuleConfigCheck(MV_VOID);
MV_VOID mvBoardDDRBusWidthCheck(MV_VOID);
MV_VOID mvBoardConfigurationPrint(MV_VOID);
MV_BOOL mvBoardIsGbEPortConnected(MV_U32 ethPortNum);
MV_32 mvBoardGetDevicesNumber(MV_BOARD_DEV_CLASS devClass);
MV_32 mvBoardGetDeviceBaseAddr(MV_32 devNum, MV_BOARD_DEV_CLASS devClass);
MV_32 mvBoardGetDeviceBusWidth(MV_32 devNum, MV_BOARD_DEV_CLASS devClass);
MV_32 mvBoardGetDeviceWinSize(MV_32 devNum, MV_BOARD_DEV_CLASS devClass);
MV_U32 boardGetDevCSNum(MV_32 devNum, MV_BOARD_DEV_CLASS devClass);
MV_U8 mvBoardTwsiAddrTypeGet(MV_BOARD_TWSI_CLASS twsiClass, MV_U32 index);
MV_U8 mvBoardTwsiAddrGet(MV_BOARD_TWSI_CLASS twsiClass, MV_U32 index);
MV_U32 mvBoardEthComplexConfigGet(MV_VOID);
MV_VOID mvBoardEthComplexConfigSet(MV_U32 ethConfig);
MV_U32 mvBoardIdIndexGet(MV_U32 boardId);
MV_U32 mvBoardIdGet(MV_VOID);
MV_VOID mvBoardSet(MV_U32 boardId);
MV_U32 mvBoardSledCpuNumGet(MV_VOID);
MV_VOID mvBoardInfoUpdate(MV_VOID);
MV_VOID mvBoardVerifySerdesCofig(MV_VOID);
MV_VOID mvBoardMppIdUpdate(MV_VOID);
MV_STATUS mvBoardEthComplexInfoUpdate(MV_VOID);
MV_ETH_COMPLEX_TOPOLOGY mvBoardEthComplexMacConfigCheck(MV_VOID);
MV_VOID mvBoardConfigWrite(MV_VOID);
MV_ETH_COMPLEX_TOPOLOGY mvBoardMac0ConfigGet(MV_VOID);
MV_ETH_COMPLEX_TOPOLOGY mvBoardMac1ConfigGet(MV_VOID);
MV_BOOL mvBoardLaneSGMIIGet(MV_ETH_COMPLEX_TOPOLOGY *sgmiiConfig);
MV_BOARD_BOOT_SRC mvBoardBootDeviceGroupSet(MV_VOID);
MV_BOARD_BOOT_SRC mvBoardBootDeviceGet(MV_VOID);
MV_U32 mvBoardBootAttrGet(MV_U32 satrBootDeviceValue, MV_U8 attrNum);
MV_STATUS mvBoardTwsiGet(MV_BOARD_TWSI_CLASS twsiClass, MV_U8 devNum, MV_U8 regNum, MV_U8 *pData, MV_U32 byteCnt);
MV_STATUS mvBoardTwsiSet(MV_BOARD_TWSI_CLASS twsiClass, MV_U8 devNum, MV_U8 regNum, MV_U8 *regVal, MV_U32 byteCnt);
MV_STATUS mvBoardEepromInit(void);
MV_BOOL mvBoardIsEepromEnabled(void);
MV_STATUS mvBoardEepromWrite(MV_CONFIG_TYPE_ID configType, MV_U8 value);
MV_U8 mvBoardCpuFreqGet(MV_VOID);
MV_STATUS mvBoardCpuFreqSet(MV_U8 freqVal);
MV_STATUS mvBoardIsInternalSwitchConnected(void);
MV_U32 mvBoardSwitchPortForceLinkGet(MV_U32 switchIdx);
MV_U32 mvBoardFreqModesNumGet(void);
MV_32 mvBoardSmiScanModeGet(MV_U32 switchIdx);
MV_BOARD_MAC_SPEED mvBoardMacSpeedGet(MV_U32 ethPortNum);
MV_VOID mvBoardMacSpeedSet(MV_U32 ethPortNum, MV_BOARD_MAC_SPEED macSpeed);
MV_U32 mvBoardSwitchCpuPortGet(MV_U32 switchIdx);
MV_U32 mvBoardMacCpuPortGet(MV_VOID);
MV_BOOL mvBoardIsEthConnected(MV_U32 ethNum);
MV_BOOL mvBoardIsEthActive(MV_U32 ethNum);
MV_32 mvBoardSwitchIrqGet(MV_VOID);
MV_32 mvBoardSwitchConnectedPortGet(MV_U32 ethPort);
MV_U32 mvBoardSwitchPortsMaskGet(MV_U32 switchIdx);
MV_BOOL mvBoardConfigAutoDetectEnabled(void);
MV_32 mvBoardSmiScanModeGet(MV_U32 switchIdx);
MV_STATUS mvBoardConfIdSet(MV_U16 conf);
MV_U16 mvBoardPexModeGet(MV_VOID);
MV_STATUS mvBoardPexModeSet(MV_U16 conf);
MV_U32 mvBoardLaneSelectorGet(MV_U32 laneNum);
MV_STATUS mvBoardConfigVerify(MV_CONFIG_TYPE_ID field, MV_U8 writeVal);
MV_VOID mvBoardSysConfigInit(void);
MV_STATUS mvBoardSysConfigSet(MV_CONFIG_TYPE_ID configField, MV_U8 value);
MV_U32 mvBoardSysConfigGet(MV_CONFIG_TYPE_ID configField);
MV_NFC_ECC_MODE mvBoardNandECCModeGet(void);
MV_STATUS mvBoardEepromWriteDefaultCfg(void);
MV_NAND_IF_MODE mvBoardNandIfGet(void);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCmvBoardEnvLibh */
