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

#ifndef __INCmvBoardEnvSpech
#define __INCmvBoardEnvSpech

#include "mvSysHwConfig.h"

/* I2C bus addresses */
#define MV_BOARD_DIMM0_I2C_ADDR		0x56
#define MV_BOARD_DIMM0_I2C_ADDR_TYPE	ADDR7_BIT
#define MV_BOARD_DIMM1_I2C_ADDR		0x57
#define MV_BOARD_DIMM1_I2C_ADDR_TYPE	ADDR7_BIT
#define MV_BOARD_DIMM_I2C_CHANNEL	0x0

/* Board specific configuration */
/* ============================ */
#ifndef MV_ASMLANGUAGE
/* New board ID numbers */

/* Customer boards */
#define CUTOMER_BOARD_ID_BASE		0x0
#define ARMADA_375_CUSTOMER_BOARD_ID0	(CUTOMER_BOARD_ID_BASE + 0)
#define ARMADA_375_CUSTOMER_BOARD_ID1	(CUTOMER_BOARD_ID_BASE + 1)
#define MV_MAX_CUSTOMER_BOARD_ID	(CUTOMER_BOARD_ID_BASE + 2)
#define MV_CUSTOMER_BOARD_NUM		(MV_MAX_CUSTOMER_BOARD_ID - CUTOMER_BOARD_ID_BASE + 1)

/* Marvell boards */
#define MARVELL_BOARD_ID_BASE		0x10
#define DB_6720_ID			(MARVELL_BOARD_ID_BASE + 0)
#define MV_MAX_MARVELL_BOARD_ID		(MARVELL_BOARD_ID_BASE + 1)
#define MV_MARVELL_BOARD_NUM		(MV_MAX_MARVELL_BOARD_ID - MARVELL_BOARD_ID_BASE + 1)

#define MV_INVALID_BOARD_ID		0xFFFFFFFF

#define GROUP1_DEFAULT_MPP8_15_I2C		0x22000022	/* SPI , I2C-0 */
#define GROUP1_DEFAULT_MPP56_63_I2C		0x01144422	/* GE0 , LED_MATRIX, I2C-1 */
/*******************************************************************************
 * ARMADA 375 Customer board - Based on DB_88F6720 */
/******************************************************************************/
/*SPI, NAND cases*/
#define A375_CUSTOMER_BOARD_0_MPP0_7		0x00020020	/* SPI */
#define A375_CUSTOMER_BOARD_0_MPP8_15		0x22000022	/* SPI , I2C */
#define A375_CUSTOMER_BOARD_0_MPP16_23		0x22222222	/* UART, TDM*/
#define A375_CUSTOMER_BOARD_0_MPP24_31		0x33333333      /* SDIO*/
#define A375_CUSTOMER_BOARD_0_MPP32_39		0x04403330	/* SPI, CPU SMI */
#define A375_CUSTOMER_BOARD_0_MPP40_47		0x22002044	/* UART1, GE0  */
#define A375_CUSTOMER_BOARD_0_MPP48_55		0x22222222	/*GE0*/
#define A375_CUSTOMER_BOARD_0_MPP56_63		0x04444422      /* GE0 , LED_MATRIX */
#define A375_CUSTOMER_BOARD_0_MPP64_67		0x014		/* LED_MATRIX */
#define A375_CUSTOMER_BOARD_0_GPP_OUT_ENA_LOW	0xFFFFFFFF
#define A375_CUSTOMER_BOARD_0_GPP_OUT_ENA_MID	0x7FFFFFFF
#define A375_CUSTOMER_BOARD_0_GPP_OUT_ENA_HIGH	0xFFFFFFFB
#define A375_CUSTOMER_BOARD_0_GPP_OUT_VAL_LOW	0x0
#define A375_CUSTOMER_BOARD_0_GPP_OUT_VAL_MID	BIT31		/* SATA-0 Power output enable */
#define A375_CUSTOMER_BOARD_0_GPP_OUT_VAL_HIGH	BIT2		/* SATA-1 Power output enable */
#define A375_CUSTOMER_BOARD_0_GPP_POL_LOW	0x0
#define A375_CUSTOMER_BOARD_0_GPP_POL_MID	0x0
#define A375_CUSTOMER_BOARD_0_GPP_POL_HIGH	0x0

#define A375_MPP32_39_EXT_SMI_MASK		0x0ff00000
#define A375_MPP32_39_SWITCH_SMI_CTRL_VAL	0x05500000
#define A375_MPP32_39_CPU_SMI_CTRL_VAL		0x04400000
#define A375_MPP32_39_NO_SMI_CTRL_VAL		0x0

/*******************************************************************************
 * Armada 375 DB-88F6720 board */
/******************************************************************************/
/*SPI, NAND cases*/
#define DB_88F6720_MPP0_7		0x00020020	/* SPI */
#define DB_88F6720_MPP8_15		0x22000022	/* SPI , I2C */
#define DB_88F6720_MPP16_23		0x22222222	/* UART, TDM*/
#define DB_88F6720_MPP24_31		0x33333333      /* SDIO, SPI1*/
#define DB_88F6720_MPP32_39		0x04403330      /* SPI1, External CPU SMI */
#define DB_88F6720_MPP40_47		0x22002044	/* UART1, GE0, SATA0 LED */
#define DB_88F6720_MPP48_55		0x22222222	/*GE0*/
#define DB_88F6720_MPP56_63		0x04444422      /* GE0 , LED_MATRIX, GPIO */
#define DB_88F6720_MPP64_67		0x014		/* LED_MATRIX, SATA1 LED*/

#define DB_88F6720_GPP_OUT_ENA_LOW	0xFFFFFFFF
#define DB_88F6720_GPP_OUT_ENA_MID	0x7FFFFFFF
#define DB_88F6720_GPP_OUT_ENA_HIGH	0xFFFFFFFB
#define DB_88F6720_GPP_OUT_VAL_LOW	0x0
#define DB_88F6720_GPP_OUT_VAL_MID	BIT31		/* SATA-0 Power output enable */
#define DB_88F6720_GPP_OUT_VAL_HIGH	BIT2		/* SATA-1 Power output enable */
#define DB_88F6720_GPP_POL_LOW		0x0
#define DB_88F6720_GPP_POL_MID		0x0
#define DB_88F6720_GPP_POL_HIGH		0x0


#endif  /* MV_ASMLANGUAGE */

#endif  /* __INCmvBoardEnvSpech */
