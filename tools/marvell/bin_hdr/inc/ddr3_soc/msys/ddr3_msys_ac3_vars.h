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

#ifndef _INC_MSYS_AC3_VARS_H
#define _INC_MSYS_AC3_VARS_H

#include "ddr3_msys_ac3_config.h"
#include "ddr3_msys_ac3_mc_static.h"
#include "mvDdr3TopologyDef.h"

#if !defined(CONFIG_CUSTOMER_BOARD_SUPPORT)
#define MV_DDR_TOPOLOGY_UPDATE_FROM_TWSI
#endif

#define ECC_SUPPORT

/*Tune internal training params values*/
#define MV_TUNE_TRAINING_PARAMS_CK_DELAY 		200
#define MV_TUNE_TRAINING_PARAMS_CK_DELAY_16		200
#define MV_TUNE_TRAINING_PARAMS_PFINGER			41
#define MV_TUNE_TRAINING_PARAMS_NFINGER			43
#define MV_TUNE_TRAINING_PARAMS_PHYREG3VAL		0xA

typedef struct __mvDramModes {
    char *mode_name;
    MV_U8 cpuFreq;
    MV_U8 fabFreq;
    MV_U8 chipId;
    MV_U8 chipBoardRev;
    MV_DRAM_MC_INIT *regs;
} MV_DRAM_MODES;

MV_DRAM_MODES ddr_modes[] =
{
/*	Conf name		CPUFreq	     FabFreq   Chip ID		Chip/Board		MC regs*/
#ifdef CONFIG_CUSTOMER_BOARD_SUPPORT
	{"ac3_customer_0_800",	DDR_FREQ_800,	0,	0x0,	AC3_CUSTOMER_BOARD_ID0,	ddr3_customer_800},
	{"ac3_customer_1_800",	DDR_FREQ_800,	0,	0x0,	AC3_CUSTOMER_BOARD_ID1,	ddr3_customer_800},
#else
	{"ac3_533",		DDR_FREQ_533,	0,	0x0,		AC3_MARVELL_BOARD_ID_BASE,		ddr3_msys_ac3_533},
	{"ac3_667",		DDR_FREQ_667,	0,	0x0,		AC3_MARVELL_BOARD_ID_BASE,		ddr3_msys_ac3_667},
	{"ac3_800",		DDR_FREQ_800,	0,	0x0,		AC3_MARVELL_BOARD_ID_BASE,		ddr3_msys_ac3_800},
	{"ac3_933",		DDR_FREQ_933,	0,	0x0,		AC3_MARVELL_BOARD_ID_BASE,		ddr3_msys_ac3_933},
#endif
};

MV_DRAM_DLB_CONFIG ddr3DlbConfigTable[] =
{
	{REG_STATIC_DRAM_DLB_CONTROL, 		0x2000005C},
	{DLB_BUS_OPTIMIZATION_WEIGHTS_REG, 	0x0008C19E},
	{DLB_AGING_REGISTER, 				0x0F7F007F},
	{DLB_EVICTION_CONTROL_REG, 			0x00000209},
	{DLB_EVICTION_TIMERS_REGISTER_REG, 	0x00FF0000},
	{DLB_USER_COMMAND_REG, 				0x00000000},
	{MBUS_UNITS_PRIORITY_CONTROL_REG, 	0x55555555},
	{FABRIC_UNITS_PRIORITY_CONTROL_REG, 0xA},
	{MBUS_UNITS_PREFETCH_CONTROL_REG, 	0xffff},
	{FABRIC_UNITS_PREFETCH_CONTROL_REG, 0xf0f},
	{0x0, 0x0}
};

#endif /* _INC_MSYS_AC3_VARS_H */

