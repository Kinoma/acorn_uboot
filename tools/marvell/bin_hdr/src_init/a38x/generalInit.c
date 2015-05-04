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

/* includes */
#include "mv_os.h"
#include "config_marvell.h"  	/* Required to identify SOC and Board */
#include "mvUart.h"
#include "util.h"
#include "generalInit.h"
#include "printf.h"
#include "mvSysEnvLib.h"

/******************************************************************************************
* mvDeviceIdConfig - set SoC Unit configuration and device ID according to detected flavour
*******************************************************************************************/
static MV_VOID mvDeviceIdConfig(void)
{
	MV_DEVICE_ID_VAL dev_id_val[] = MV_DEVICE_ID_VAL_INFO;
	MV_U32 ctrlId, devId = mvSysEnvDeviceIdGet(); /* read Sample at reset for device ID*/

	/* Verify Marvell A38x boards limitation */
#if !defined(CONFIG_CUSTOMER_BOARD_SUPPORT) && defined(MV88F68XX)
	/* only A381/2 board support 6821 device ID */
	if (devId == MV_6811 && mvBoardIdGet() != DB_BP_6821_ID)
		mvPrintf("%s: Error: Device 6811/21 is not supported on current board (only A381/2 DB-BP)\n", __func__);
#endif

	if (devId == MV_NONE) {
		mvPrintf("%s: Error: Read an unknown device ID from 'S@R'\n", __func__);
#if !defined(CONFIG_CUSTOMER_BOARD_SUPPORT)
		mvPrintf("Setting default device ID\n");
		devId = MV_DEFAULT_DEVICE_ID; /* fallback to default only for Marvell boards */
#endif

	}
	/* Configure Units according to detected deviceId (flavor) */
	MV_REG_WRITE(DEVICE_CONFIGURATION_REG0, dev_id_val[devId].wo_reg_val0);
	MV_REG_WRITE(DEVICE_CONFIGURATION_REG1, dev_id_val[devId].wo_reg_val1);

	/* set device ID register (flavor) */
	ctrlId = MV_REG_READ(DEV_ID_REG) & ~DEV_ID_REG_DEVICE_ID_MASK;
	ctrlId |= dev_id_val[devId].ctrlModel << DEV_ID_REG_DEVICE_ID_OFFS;
	MV_REG_WRITE(DEV_ID_REG, ctrlId);
}

/* mvMppConfig() prepares UART and I2C configuration (MPP's and UART interface selection) */
static inline MV_VOID mvMppConfig()
{
        /* UART0 & I2C MPP's (MPP[0:3] = 0x1) */
        MV_U32 regData = (MV_REG_READ(MPP_CONTROL_REG(0))  & MPP_SET_MASK) | MPP_SET_DATA;
        MV_REG_WRITE(MPP_CTRL_REG, regData);

        /* UART1 MPP's (MPP[19:20] = 0x6) - UART1 is the default UART interface on DB-AP-68xx */
        regData = (MV_REG_READ(MPP_CONTROL_REG(2))  & MPP_UART1_SET_MASK) | MPP_UART1_SET_DATA;
        MV_REG_WRITE(MPP_CONTROL_REG(2), regData);
}

MV_STATUS mvGeneralInit(void)
{
	MV_U32 regData;

	/* Update AVS debug control register */
        MV_REG_WRITE(AVS_DEBUG_CNTR_REG, AVS_DEBUG_CNTR_DEFAULT_VALUE);

        regData = MV_REG_READ(AVS_ENABLED_CONTROL);
        regData &= ~(AVS_LOW_VDD_LIMIT_MASK | AVS_HIGH_VDD_LIMIT_MASK);
	regData |= (AVS_LOW_VDD_LIMIT_VAL | AVS_HIGH_VDD_LIMIT_VAL);
        MV_REG_WRITE(AVS_ENABLED_CONTROL, regData);

	mvMppConfig();

	/* - Init the TWSI before all I2C transaction */
	DEBUG_INIT_FULL_S("mvGeneralInit: Init TWSI interface.\n");
	mvHwsTwsiInitWrapper();

#if !defined(MV_NO_PRINT)
	mvUartInit();
	mvPrintf("\n\nGeneral initialization - Version: " GENERAL_VERION "\n");
#endif

	/* Device general configuration was not supported on a38x Z0 revision */
	if (mvSysEnvDeviceRevGet() != MV_88F68XX_Z1_ID)
		mvDeviceIdConfig();

	return MV_OK;
}

