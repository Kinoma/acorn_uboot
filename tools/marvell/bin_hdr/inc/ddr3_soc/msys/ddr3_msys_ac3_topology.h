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
on the worldwide web_http://www.gnu.org/licenses/gpl.txt.

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
#ifndef _INC_MSYS_AC3_TOPOLOGY_H
#define _INC_MSYS_AC3_TOPOLOGY_H

#include "ddr3_msys_ac3_config.h"
#include "mvDdr3LoggingDef.h"

/*Bus mask variants*/
#define BUS_MASK_32BIT				0xF
#define BUS_MASK_32BIT_ECC			0x1F
#define BUS_MASK_16BIT				0x3
#define BUS_MASK_16BIT_ECC			0x13
#define BUS_MASK_16BIT_ECC_PUP3		0xB

#define  DYNAMIC_CS_SIZE_CONFIG
#define  MV_DEVICE_MAX_DRAM_ADDRESS_SIZE          ADDR_SIZE_2Gb

#ifdef CONFIG_CUSTOMER_BOARD_SUPPORT
/************************************* Customer Boards Topology *************************************/
MV_HWS_TOPOLOGY_MAP TopologyMap[] =
{
    /* 1st Customer board */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs                                                                     speed_bin        memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0}, {0x1,0,0,0}, {0x3,0,0,0}, {0x3,0,0,0}, {0x0,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
    /* 2nd Customer board */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs                                     speed_bin             memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0},{ 0x1,0,0,0},{ 0x2,1,0,0},{ 0x2,1,0,0}, {0,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    }
};

#else /* CONFIG_MARVELL_BOARD_SUPPORT */
/************************************* Marvell Boards Topology *************************************/
MV_HWS_TOPOLOGY_MAP TopologyMap[] =
{
    /* 1st Marvell board - DB_MISL - No ECC (BoardId = 0x0) */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs     speed_bin        memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x3,2,0,0}, {0x3,2,0,0},{0x3,2,0,0}, {0x3,2,0,0}, {0x3,2,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
	/* 2rd Marvell board - RD_MTL_4XG  (BoardId = 0x1) */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs     speed_bin             memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0},{ 0x1,0,0,0},{0x1,0,0,0},{ 0x1,0,0,0},{ 0x1,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
	/* 3rd Marvell board - RD_MTL_2XXG_2XG: No ECC (BoardId = 0x2) */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs     speed_bin             memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
    /* 4th Marvell board - DB_MISL_24G4G - No ECC (BoardId = 0x3) */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs     speed_bin        memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
    /* 5th Marvell board - RD_MTL_24G - No ECC (BoardId = 0x4) */
    {
    0x1, /* active interfaces */
	/* ATTENTION - cs_mask and mirror  HAVE to be the same for all PUPs*/
    /*cs_mask, mirror, dqs_swap, ck_swap X PUPs     speed_bin        memory_width  mem_size     frequency  casL casWL      temperature */
	{{{{0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}, {0x1,0,0,0}}, SPEED_BIN_DDR_1866L, BUS_WIDTH_8 , MEM_4G, DDR_FREQ_400, 0 ,   0 , MV_HWS_TEMP_LOW}},
    5, /* Num Of Bus Per Interface*/
    BUS_MASK_16BIT  /* Buses mask */
    },
};
#endif /* CONFIG_CUSTOMER_BOARD_SUPPORT */

#endif /* _INC_MSYS_AC3_TOPOLOGY_H */
