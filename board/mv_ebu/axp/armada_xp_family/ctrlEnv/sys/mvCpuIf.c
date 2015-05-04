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
/*#include "cpu/mvCpu.h" *//* whoAmI() */
#include "ctrlEnv/sys/mvCpuIf.h"
#include "ctrlEnv/sys/mvAhbToMbusRegs.h"
#include "cpu/mvCpu.h"
#include "ctrlEnv/mvCtrlEnvLib.h"
#include "mvSysHwConfig.h"
#include "ddr2_3/mvDramIf.h"
#include "ddr2_3/mvDramIfRegs.h"
#include "pex/mvPexRegs.h"

/*#define MV_DEBUG*/
/* defines  */

#ifdef MV_DEBUG
#define DB(x)	x
#else
#define DB(x)
#endif

/* locals   */
/* static functions */
static MV_BOOL cpuTargetWinOverlap(MV_TARGET target, MV_ADDR_WIN *pAddrWin);

MV_TARGET sampleAtResetTargetArray[] = BOOT_TARGETS_NAME_ARRAY;
/*******************************************************************************
* mvCpuIfInitForCpu - Initialize Controller CPU interface
*
* DESCRIPTION:
*       This function initialize Controller CPU interface:
*       1. Set CPU interface configuration registers.
*       2. Set CPU master Pizza arbiter control according to static
*          configuration described in configuration file.
*       3. Opens CPU address decode windows. DRAM windows are assumed to be
*		   already set (auto detection).
*
* INPUT:
*       cpu      	- CPU id.
*
* OUTPUT:
*       None.
*
* RETURN:
*       None.
*
*******************************************************************************/
MV_STATUS mvCpuIfInitForCpu(MV_U32 cpu, MV_CPU_DEC_WIN *cpuAddrWinMap)
{
	MV_U32 regVal, i;
	MV_TARGET target;
	MV_ADDR_WIN addrWin;

	if (cpuAddrWinMap == NULL) {
		DB(mvOsPrintf("mvCpuIfInit:ERR. cpuAddrWinMap == NULL\n"));
		return MV_ERROR;
	}

	/* Set CPU Configuration register */
	regVal = MV_REG_READ(CPU_CONFIG_REG(cpu));
	regVal &= ~CPU_CONFIG_DEFAULT_MASK;
	regVal |= CPU_CONFIG_DEFAULT;
	MV_REG_WRITE(CPU_CONFIG_REG(cpu), regVal);

	for (i = 0; i < MAX_AHB_TO_MBUS_WINS-2; i++)
		mvAhbToMbusWinEnable(i, MV_FALSE);

	/* Disable all SRAM windows  */
	mvCpuIfSramWinDisable();

	/* First disable all CPU target windows  */
	for (target = 0; cpuAddrWinMap[target].enable != TBL_TERM; target++) {
		if ((MV_TARGET_IS_DRAM(target)) || (target == INTER_REGS))
			continue;

#if defined(MV_MEM_OVER_PEX_WA) || defined(MV_UART_OVER_PEX_WA)
		/* If the target PEX or PCI and memory is over PEX or PCI we don't touch this CPU windows */
		if (MV_TARGET_IS_PEX(target))
			continue;
#endif
#if defined(MV_RUN_FROM_FLASH)
		/* Don't disable the boot device.                               */
		if (target == DEV_BOOCS)
			continue;
#endif /* MV_RUN_FROM_FLASH */
		mvCpuIfTargetWinEnable(MV_CHANGE_BOOT_CS(target), MV_FALSE);
	}

#if defined(MV_RUN_FROM_FLASH)
	/* Resize the bootcs windows before other windows, because this     */
	/* window is enabled and will cause an overlap if not resized.      */
	target = DEV_BOOCS;

	if (MV_OK != mvCpuIfTargetWinSet(target, &cpuAddrWinMap[target])) {
		DB(mvOsPrintf("mvCpuIfInit:ERR. mvCpuIfTargetWinSet fail\n"));
		return MV_ERROR;
	}

	addrWin.baseLow = cpuAddrWinMap[target].addrWin.baseLow;
	addrWin.baseHigh = cpuAddrWinMap[target].addrWin.baseHigh;
	if (0xffffffff == mvAhbToMbusWinRemap(cpuAddrWinMap[target].winNum, &addrWin)) {
		DB(mvOsPrintf("mvCpuIfInit:WARN. mvAhbToMbusWinRemap can't remap winNum=%d\n",
			      cpuAddrWinMap[target].winNum));
	}
#endif /* MV_RUN_FROM_FLASH */

	/* Go through all targets in user table until table terminator                  */
	for (target = 0; cpuAddrWinMap[target].enable != TBL_TERM; target++) {

#if defined(MV_RUN_FROM_FLASH)
		if (target == DEV_BOOCS)
			continue;
#endif /* MV_RUN_FROM_FLASH */

		/* if DRAM auto sizing is used do not initialized DRAM target windows,  */
		/* assuming this already has been done earlier.                         */
#ifdef	MV_DRAM_AUTO_SIZE
		if (MV_TARGET_IS_DRAM(target))
			continue;
#endif

#if defined(MV_MEM_OVER_PEX_WA) || defined(MV_UART_OVER_PEX_WA)
		/* If the target PEX or PCI and memory is over PEX or PCI we don't touch this CPU windows */
		if (MV_TARGET_IS_PEX(target))
			continue;
#endif
		if ((0 == cpuAddrWinMap[target].addrWin.size) || (DIS == cpuAddrWinMap[target].enable)) {
			#if 0 /* TODO: windows are already disabled above, we need to skip only*/
			if (MV_OK != mvCpuIfTargetWinEnable(target, MV_FALSE)) {
				DB(mvOsPrintf("mvCpuIfInit:ERR. mvCpuIfTargetWinEnable fail\n"));
				return MV_ERROR;
			}
			#else
			{continue; }
			#endif
		} else {
			if (MV_OK != mvCpuIfTargetWinSet(target, &cpuAddrWinMap[target])) {
				DB(mvOsPrintf("mvCpuIfInit:ERR. mvCpuIfTargetWinSet fail\n"));
				return MV_ERROR;
			}

			addrWin.baseLow = cpuAddrWinMap[target].addrWin.baseLow;
			addrWin.baseHigh = cpuAddrWinMap[target].addrWin.baseHigh;
			if (0xffffffff == mvAhbToMbusWinRemap(cpuAddrWinMap[target].winNum, &addrWin)) {
				DB(mvOsPrintf("mvCpuIfInit:WARN. mvAhbToMbusWinRemap can't remap winNum=%d\n",
					      cpuAddrWinMap[target].winNum));
			}
		}
	}
#if 0
/* This is not needed anymore cause pex enabled is already at CtrlEnvInit */
#ifdef MV_INCLUDE_PEX
	if (cpu == 0) {		/* Not needed for all CPUs */
		MV_U32 pexUnits = mvCtrlPexMaxUnitGet();
		for (i = 0; i < pexUnits; i++)
			mvCpuIfEnablePex(i);
	}
#endif
#endif
	return MV_OK;
}

/*******************************************************************************/
MV_STATUS mvCpuIfInit(MV_CPU_DEC_WIN *cpuAddrWinMap)
{
	return mvCpuIfInitForCpu(whoAmI(), cpuAddrWinMap);
}


/*******************************************************************************
* mvCpuIfDramInit - Initialize Controller DRAM Fastpath windows
*
* DESCRIPTION:
*       This function initialize Controller DRAM Fastpath windows
*		It takes the CS size information from the 0x1500 scratch registers
*		and sets the correct windows sizes and base addresses accordingly
* INPUT:
*       None.
*
* OUTPUT:
*       None.
*
* RETURN:
*       None.
*
*******************************************************************************/
MV_STATUS mvCpuIfDramInit()
{
	MV_U32 base = 0;
	MV_U32 size, cs, temp;

	for (cs = 0; cs < SDRAM_MAX_CS; cs++) {
		size = MV_REG_READ(SDRAM_SIZE_REG(cs)) & SDRAM_ADDR_MASK;
		if (size > 0 && base < SDRAM_MAX_ADDR) {
			size |= ~(SDRAM_ADDR_MASK);

			/* Set Base Address */
			MV_REG_WRITE(SDRAM_WIN_BASE_REG(cs), (base & SDRAM_ADDR_MASK));

			/* Check if out of max window size and resize the window */
			if (base+size > SDRAM_MAX_ADDR) {
				size = SDRAM_MAX_ADDR - base - 1;
				MV_REG_WRITE(SDRAM_SIZE_REG(cs), 0);
			}

			temp = (MV_REG_READ(SDRAM_WIN_CTRL_REG(cs)) & ~(SDRAM_ADDR_MASK)) | (1<<SDRAM_WIN_CTRL_WIN_ENA_OFFS);
			temp |= (size & SDRAM_ADDR_MASK);

			MV_REG_WRITE(SDRAM_WIN_CTRL_REG(cs), temp);
			base += (size + 1);
		}
	}

	return MV_OK;
}

/*******************************************************************************
* mvCpuIfTargetWinSet - Set CPU-to-peripheral target address window
*
* DESCRIPTION:
*       This function sets a peripheral target (e.g. SDRAM bank0, PCI0_MEM0)
*       address window, also known as address decode window.
*       A new address decode window is set for specified target address window.
*       If address decode window parameter structure enables the window,
*       the routine will also enable the target window, allowing CPU to access
*       the target window.
*
* INPUT:
*       target      - Peripheral target enumerator.
*       pAddrDecWin - CPU target window data structure.
*
* OUTPUT:
*       N/A
*
* RETURN:
*       MV_OK if CPU target window was set correctly, MV_ERROR in case of
*       address window overlapps with other active CPU target window or
*		trying to assign 36bit base address while CPU does not support that.
*       The function returns MV_NOT_SUPPORTED, if the target is unsupported.
*
*******************************************************************************/
MV_STATUS mvCpuIfTargetWinSet(MV_TARGET target, MV_CPU_DEC_WIN *pAddrDecWin)
{
	MV_AHB_TO_MBUS_DEC_WIN decWin;
	MV_U32 existingWinNum;
	MV_DRAM_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinSet: target %d is illegal\n", target);
		return MV_ERROR;
	}

	/* 2) Check if the requested window overlaps with current windows */
	if (MV_TRUE == cpuTargetWinOverlap(target, &pAddrDecWin->addrWin)) {
		mvOsPrintf("mvCpuIfTargetWinSet: ERR. Target %d overlap\n", target);
		return MV_BAD_PARAM;
	}

	if (MV_TARGET_IS_DRAM(target)) {
		/* copy relevant data to MV_DRAM_DEC_WIN structure */
		addrDecWin.addrWin.baseHigh = pAddrDecWin->addrWin.baseHigh;
		addrDecWin.addrWin.baseLow = pAddrDecWin->addrWin.baseLow;
		addrDecWin.addrWin.size = pAddrDecWin->addrWin.size;
		addrDecWin.enable = pAddrDecWin->enable;

		if (mvDramIfWinSet(target, &addrDecWin) != MV_OK) {
			mvOsPrintf("mvCpuIfTargetWinSet: mvDramIfWinSet Failed\n");
			return MV_ERROR;
		}
	} else {
		/* copy relevant data to MV_AHB_TO_MBUS_DEC_WIN structure */
		decWin.addrWin.baseLow = pAddrDecWin->addrWin.baseLow;
		decWin.addrWin.baseHigh = pAddrDecWin->addrWin.baseHigh;
		decWin.addrWin.size = pAddrDecWin->addrWin.size;
		decWin.enable = pAddrDecWin->enable;
		decWin.target = target;

		existingWinNum = mvAhbToMbusWinTargetGet(target);

		/* check if there is already another Window configured
		   for this target */
		if ((existingWinNum < MAX_AHB_TO_MBUS_WINS) && (existingWinNum != pAddrDecWin->winNum)) {
			/* if we want to enable the new window number
			   passed by the user , then the old one should
			   be disabled */
			if (MV_TRUE == pAddrDecWin->enable) {
				/* be sure it is disabled */
				mvAhbToMbusWinEnable(existingWinNum, MV_FALSE);
			}
		}

		if (mvAhbToMbusWinSet(pAddrDecWin->winNum, &decWin) != MV_OK) {
			mvOsPrintf("mvCpuIfTargetWinSet: mvAhbToMbusWinSet Failed\n");
			return MV_ERROR;
		}
	}

	return MV_OK;
}

/*******************************************************************************
* mvCpuIfTargetWinGet - Get CPU-to-peripheral target address window
*
* DESCRIPTION:
*		Get the CPU peripheral target address window.
*
* INPUT:
*       target - Peripheral target enumerator
*
* OUTPUT:
*       pAddrDecWin - CPU target window information data structure.
*
* RETURN:
*       MV_OK if target exist, MV_ERROR otherwise.
*
*******************************************************************************/
MV_STATUS mvCpuIfTargetWinGet(MV_TARGET target, MV_CPU_DEC_WIN *pAddrDecWin)
{
	MV_U32 winNum = 0xffffffff;
	MV_AHB_TO_MBUS_DEC_WIN decWin;
	MV_DRAM_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinGet: target %d is illegal\n", target);
		return MV_ERROR;
	}

	if (MV_TARGET_IS_DRAM(target)) {
		if (mvDramIfWinGet(target, &addrDecWin) != MV_OK) {
			mvOsPrintf("mvCpuIfTargetWinGet: Failed to get window target %d\n", target);
			return MV_ERROR;
		}

		/* copy relevant data to MV_CPU_DEC_WIN structure */
		pAddrDecWin->addrWin.baseLow = addrDecWin.addrWin.baseLow;
		pAddrDecWin->addrWin.baseHigh = addrDecWin.addrWin.baseHigh;
		pAddrDecWin->addrWin.size = addrDecWin.addrWin.size;
		pAddrDecWin->enable = addrDecWin.enable;
		pAddrDecWin->winNum = target;
	} else {
		/* get the Window number associated with this target */
		winNum = mvAhbToMbusWinTargetGet(target);
		if (winNum >= MAX_AHB_TO_MBUS_WINS)
			return MV_NO_SUCH;

		if (mvAhbToMbusWinGet(winNum, &decWin) != MV_OK) {
			mvOsPrintf("%s: mvAhbToMbusWinGet Failed at winNum = %d\n", __func__, winNum);
			return MV_ERROR;
		}

		/* copy relevant data to MV_CPU_DEC_WIN structure */
		pAddrDecWin->addrWin.baseLow = decWin.addrWin.baseLow;
		pAddrDecWin->addrWin.baseHigh = decWin.addrWin.baseHigh;
		pAddrDecWin->addrWin.size = decWin.addrWin.size;
		pAddrDecWin->enable = decWin.enable;
		pAddrDecWin->winNum = winNum;
	}

	return MV_OK;
}

/*******************************************************************************
* mvCpuIfTargetWinEnable - Enable/disable a CPU address decode window
*
* DESCRIPTION:
*       This function enable/disable a CPU address decode window.
*       if parameter 'enable' == MV_TRUE the routine will enable the
*       window, thus enabling CPU accesses (before enabling the window it is
*       tested for overlapping). Otherwise, the window will be disabled.
*
* INPUT:
*       target - Peripheral target enumerator.
*       enable - Enable/disable parameter.
*
* OUTPUT:
*       N/A
*
* RETURN:
*       MV_ERROR if protection window number was wrong, or the window
*       overlapps other target window.
*
*******************************************************************************/
MV_STATUS mvCpuIfTargetWinEnable(MV_TARGET target, MV_BOOL enable)
{
	MV_U32 winNum, temp;
	MV_CPU_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinEnable: target %d is illegal\n", target);
		return MV_ERROR;
	}

	/* get the window and check if it exist */
	temp = mvCpuIfTargetWinGet(target, &addrDecWin);
	if (MV_NO_SUCH == temp) {
		return (enable ? MV_ERROR : MV_OK);
	} else if (MV_OK != temp) {
		mvOsPrintf("%s: ERR. Getting target %d failed.\n", __func__, target);
		return MV_ERROR;
	}

	/* check overlap */
	if (MV_TRUE == enable) {
		if (MV_TRUE == cpuTargetWinOverlap(target, &addrDecWin.addrWin)) {
			DB(mvOsPrintf("%s: ERR. Target %d overlap\n", __func__, target));
			return MV_ERROR;
		}
	}

	if (MV_TARGET_IS_DRAM(target)) {
		if (mvDramIfWinEnable(target, enable) != MV_OK) {
			mvOsPrintf("mvCpuIfTargetWinGet: mvDramIfWinEnable Failed at \n");
			return MV_ERROR;
		}
	} else {
		/* get the Window number associated with this target */
		winNum = mvAhbToMbusWinTargetGet(target);

		if (winNum >= MAX_AHB_TO_MBUS_WINS)
			return (enable ? MV_ERROR : MV_OK);

		if (mvAhbToMbusWinEnable(winNum, enable) != MV_OK) {
			mvOsPrintf("mvCpuIfTargetWinGet: Failed to enable window = %d\n", winNum);
			return MV_ERROR;
		}
	}

	return MV_OK;
}

/*******************************************************************************
* mvCpuIfTargetWinSizeGet - Get CPU target address window size
*
* DESCRIPTION:
*		Get the size of CPU-to-peripheral target window.
*
* INPUT:
*       target - Peripheral target enumerator
*
* OUTPUT:
*       None.
*
* RETURN:
*       32bit size. Function also returns '0' if window is closed.
*		Function returns 0xFFFFFFFF in case of an error.
*
*******************************************************************************/
MV_U32 mvCpuIfTargetWinSizeGet(MV_TARGET target)
{
	MV_CPU_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinSizeGet: target %d is illegal\n", target);
		return 0;
	}

	/* Get the winNum window */
	if (MV_OK != mvCpuIfTargetWinGet(target, &addrDecWin)) {
		mvOsPrintf("mvCpuIfTargetWinSizeGet:ERR. Getting target %d failed.\n", target);
		return 0;
	}

	/* Check if window is enabled   */
	if (addrDecWin.enable == MV_TRUE)
		return (addrDecWin.addrWin.size);
	else
		return 0;	/* Window disabled. return 0 */
}

/*******************************************************************************
* mvCpuIfTargetWinBaseLowGet - Get CPU target address window base low
*
* DESCRIPTION:
*       CPU-to-peripheral target address window base is constructed of
*       two parts: Low and high.
*		This function gets the CPU peripheral target low base address.
*
* INPUT:
*       target - Peripheral target enumerator
*
* OUTPUT:
*       None.
*
* RETURN:
*       32bit low base address.
*
*******************************************************************************/
MV_U32 mvCpuIfTargetWinBaseLowGet(MV_TARGET target)
{
	MV_CPU_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinBaseLowGet: target %d is illegal\n", target);
		return 0xffffffff;
	}

	/* Get the target window */
	if (MV_OK != mvCpuIfTargetWinGet(target, &addrDecWin)) {
		mvOsPrintf("mvCpuIfTargetWinBaseLowGet:ERR. Getting target %d failed.\n", target);
		return 0xffffffff;
	}

	if (MV_FALSE == addrDecWin.enable)
		return 0xffffffff;

	return (addrDecWin.addrWin.baseLow);
}

/*******************************************************************************
* mvCpuIfTargetWinBaseHighGet - Get CPU target address window base high
*
* DESCRIPTION:
*       CPU-to-peripheral target address window base is constructed of
*       two parts: Low and high.
*		This function gets the CPU peripheral target high base address.
*
* INPUT:
*       target - Peripheral target enumerator
*
* OUTPUT:
*       None.
*
* RETURN:
*       32bit high base address.
*
*******************************************************************************/
MV_U32 mvCpuIfTargetWinBaseHighGet(MV_TARGET target)
{
	MV_CPU_DEC_WIN addrDecWin;

	target = MV_CHANGE_BOOT_CS(target);

	/* Check parameters */
	if (target >= MAX_TARGETS) {
		mvOsPrintf("mvCpuIfTargetWinBaseLowGet: target %d is illegal\n", target);
		return 0xffffffff;
	}

	/* Get the target window */
	if (MV_OK != mvCpuIfTargetWinGet(target, &addrDecWin)) {
		mvOsPrintf("mvCpuIfTargetWinBaseHighGet:ERR. Getting target %d failed.\n", target);
		return 0xffffffff;
	}

	if (MV_FALSE == addrDecWin.enable)
		return 0;

	return (addrDecWin.addrWin.baseHigh);
}


/*******************************************************************************
* mvCpuIfSramWinDisable
*
* DESCRIPTION:
*	Disable the SRAM windows.
*
* INPUT:
*		None.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_OK on success.
*
*******************************************************************************/
MV_STATUS mvCpuIfSramWinDisable(MV_VOID)
{
	MV_U32 i;

	for (i = 0; i < 4; i++)
		MV_REG_WRITE(SRAM_WIN_CTRL_REG(i), SRAM_WIN_CTRL_DEFAULT_VAL);

	return MV_OK;
}


#if defined(MV_INCLUDE_PEX)
/*******************************************************************************
* mvCpuIfPexRemap - Set CPU remap register for address windows.
*
* DESCRIPTION:
*
* INPUT:
*       pexTarget   - Peripheral target enumerator. Must be a PEX target.
*       pAddrDecWin - CPU target window information data structure.
*                     Note that caller has to fill in the base field only. The
*                     size field is ignored.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_ERROR if target is not a PEX one, MV_OK otherwise.
*
*******************************************************************************/
MV_U32 mvCpuIfPexRemap(MV_TARGET pexTarget, MV_ADDR_WIN *pAddrDecWin)
{
	MV_U32 winNum;

	/* Check parameters */
	if (mvCtrlPexMaxIfGet() > 1) {
		if ((!MV_TARGET_IS_PEX0(pexTarget)) &&
			(!MV_TARGET_IS_PEX1(pexTarget)) &&
			(!MV_TARGET_IS_PEX2(pexTarget)) &&
			(!MV_TARGET_IS_PEX3(pexTarget)) &&
			(!MV_TARGET_IS_PEX4(pexTarget)) &&
			(!MV_TARGET_IS_PEX5(pexTarget)) &&
			(!MV_TARGET_IS_PEX6(pexTarget)) &&
			(!MV_TARGET_IS_PEX7(pexTarget)) &&
			(!MV_TARGET_IS_PEX8(pexTarget)) &&
			(!MV_TARGET_IS_PEX9(pexTarget))) {
			mvOsPrintf("mvCpuIfPexRemap: target %d is illegal\n", pexTarget);
			return 0xffffffff;
		}
	} else {
		if (!MV_TARGET_IS_PEX0(pexTarget)) {
			mvOsPrintf("mvCpuIfPexRemap: target %d is illegal\n", pexTarget);
			return 0xffffffff;
		}
	}

	/* get the Window number associated with this target */
	winNum = mvAhbToMbusWinTargetGet(pexTarget);

	if (winNum >= MAX_AHB_TO_MBUS_WINS) {
		mvOsPrintf("mvCpuIfPexRemap: mvAhbToMbusWinTargetGet Failed\n");
		return 0xffffffff;
	}

	return mvAhbToMbusWinRemap(winNum, pAddrDecWin);
}
#endif

#if defined(MV_INCLUDE_PCI)
/*******************************************************************************
* mvCpuIfPciRemap - Set CPU remap register for address windows.
*
* DESCRIPTION:
*
* INPUT:
*       pciTarget   - Peripheral target enumerator. Must be a PCI target.
*       pAddrDecWin - CPU target window information data structure.
*                     Note that caller has to fill in the base field only. The
*                     size field is ignored.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_ERROR if target is not a PCI one, MV_OK otherwise.
*
*******************************************************************************/
MV_U32 mvCpuIfPciRemap(MV_TARGET pciIfTarget, MV_ADDR_WIN *pAddrDecWin)
{
	MV_U32 winNum;

	/* get the Window number associated with this target */
	winNum = mvAhbToMbusWinTargetGet(pciIfTarget);

	if (winNum >= MAX_AHB_TO_MBUS_WINS) {
		mvOsPrintf("mvCpuIfPexRemap: mvAhbToMbusWinTargetGet Failed\n");
		return 0xffffffff;
	}

	return mvAhbToMbusWinRemap(winNum, pAddrDecWin);
}
#endif

/*******************************************************************************
* mvCpuIfTargetOfBaseAddressGet - Get the target according to base address
*
* DESCRIPTION:
*
* INPUT:
*       baseAddress -  base address to be checked
*
* OUTPUT:
*       None.
*
* RETURN:
*       the target number that baseAddress belongs to or MAX_TARGETS is not
*       found
*
*******************************************************************************/
MV_TARGET mvCpuIfTargetOfBaseAddressGet(MV_U32 baseAddress)
{
	MV_CPU_DEC_WIN win;
	MV_U32 target;

	for (target = 0; target < MAX_TARGETS; target++) {
		if (mvCpuIfTargetWinGet(target, &win) == MV_OK) {
			if (win.enable) {
				if ((baseAddress >= win.addrWin.baseLow) &&
				    (baseAddress < win.addrWin.baseLow + win.addrWin.size))
					break;
			}
		} else
			return MAX_TARGETS;
	}

	return target;
}

/*******************************************************************************
* cpuTargetWinOverlap - Detect CPU address decode windows overlapping
*
* DESCRIPTION:
*       An unpredicted behaviur is expected in case CPU address decode
*       windows overlapps.
*       This function detects CPU address decode windows overlapping of a
*       specified target. The function does not check the target itself for
*       overlapping. The function also skipps disabled address decode windows.
*
* INPUT:
*       target      - Peripheral target enumerator.
*       pAddrDecWin - An address decode window struct.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_TRUE if the given address window overlaps current address
*       decode map, MV_FALSE otherwise.
*
*******************************************************************************/
static MV_BOOL cpuTargetWinOverlap(MV_TARGET target, MV_ADDR_WIN *pAddrWin)
{
	MV_U32 targetNum;
	MV_CPU_DEC_WIN addrDecWin;
	MV_STATUS status;

	for (targetNum = 0; targetNum < MAX_TARGETS; targetNum++) {
		/* don't check our target or illegal targets */
		if (targetNum == target)
			continue;

		/* Get window parameters        */
		status = mvCpuIfTargetWinGet(targetNum, &addrDecWin);
		if (MV_NO_SUCH == status)
			continue;

		if (MV_OK != status) {
			DB(mvOsPrintf("cpuTargetWinOverlap: ERR. TargetWinGet failed\n"));
			return MV_TRUE;
		}

		/* Do not check disabled windows        */
		if (MV_FALSE == addrDecWin.enable)
			continue;

		if (MV_TRUE == mvWinOverlapTest(pAddrWin, &addrDecWin.addrWin)) {
			DB(mvOsPrintf("cpuTargetWinOverlap: Required target %d overlap current %d\n",
				      target, targetNum));
			return MV_TRUE;
		}
	}

	return MV_FALSE;
}

/*******************************************************************************
* mvCpuIfAddDecShow - Print the CPU address decode map.
*
* DESCRIPTION:
*		This function print the CPU address decode map.
*
* INPUT:
*       None.
*
* OUTPUT:
*       None.
*
* RETURN:
*       None.
*
*******************************************************************************/
MV_VOID mvCpuIfAddDecShow(MV_VOID)
{
	MV_CPU_DEC_WIN win;
	MV_U32 target;

	mvOsOutput("\n");
	mvOsOutput("CPU Interface\n");
	mvOsOutput("-------------\n");

	for (target = 0; target < MAX_TARGETS; target++) {
		memset(&win, 0, sizeof(MV_CPU_DEC_WIN));

		mvOsOutput("%s ", mvCtrlTargetNameGet(target));
		mvOsOutput("....");

		if (mvCpuIfTargetWinGet(target, &win) == MV_OK) {
			if (win.enable) {
				mvOsOutput("base %08x, ", win.addrWin.baseLow);
				mvSizePrint(win.addrWin.size);
				mvOsOutput("\n");
			} else
				mvOsOutput("disable\n");
		} else if (mvCpuIfTargetWinGet(target, &win) == MV_NO_SUCH) {
			mvOsOutput("no such\n");
		}
	}
}

#if defined(MV_INCLUDE_PEX)
/*******************************************************************************
* mvCpuIfEnablePex - Enable PCI Express unit.
*
* DESCRIPTION:
*	This function enables PCI Express access to the device address
*	space.
*
* INPUT:
*	pexUnit	- PEX unit (0 - 3).
*
* OUTPUT:
*       None.
*
* RETURN:
*       None.
*
*******************************************************************************/
MV_VOID mvCpuIfEnablePex(MV_U32 pexUnit)
{
	MV_U32 socMaxPexUnit = mvCtrlPexMaxUnitGet();

	if (pexUnit > socMaxPexUnit) {
		DB(mvOsPrintf("mvCpuIfEnablePex: Bad PEX unit ID (%x)\n", pexUnit));
		return;
	}

	/* SOC config register Pex enable */
	MV_REG_BIT_SET(SOC_CTRL_REG, SCR_PEX_ENA_MASK(pexUnit));
}

/*******************************************************************************
* mvCpuIfPex4x1Enable - Enable/Disable the 4x1 mode on PCI Express unit.
*
* DESCRIPTION:
*	This function enables/disables 4x1 mode on PCI Express unit 0 or 1
*
* INPUT:
*	pexUnit	- PEX unit (0 or 1).
*	enable	- enable (MV_TRUE) or disable (MV_FALSE) 4x1 mode
*
* OUTPUT:
*       None.
*
* RETURN:
*       None.
*
*******************************************************************************/
MV_VOID mvCpuIfPex4x1Enable(MV_U32 pexUnit, MV_BOOL enable)
{
	if (pexUnit == 0)
		MV_REG_BIT_SET(SOC_CTRL_REG, SCR_PEX0_4BY1_MASK);
	else if (pexUnit == 1)
		MV_REG_BIT_SET(SOC_CTRL_REG, SCR_PEX1_4BY1_MASK);
	else
		DB(mvOsPrintf("mvCpuIfPex4x1Enable: Bad PEX unit ID (%x)\n", pexUnit));
}

#endif

/*******************************************************************************
* mvCpuIfLvdsPadsEnable
*
* DESCRIPTION:
*	Enable / Disable the LVDS pads.
*
* INPUT:
*	enable - MV_TRUE to enable the pads, MV_FALSE to disable.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_OK on success.
*
*******************************************************************************/
MV_STATUS mvCpuIfLvdsPadsEnable(MV_BOOL enable)
{
	MV_U32 reg;
	MV_U32 i;

	reg = MV_REG_READ(LVDS_PADS_CTRL_REG);

	for (i = 0; i < 5; i++) {
		reg &= ~LVDS_PADS_CONF_PD_MASK(i);
		reg |= LVDS_PADS_CONF_PD_EN(i, enable);
	}

	MV_REG_WRITE(LVDS_PADS_CTRL_REG, reg);

	return MV_OK;
}
