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
#include "config_marvell.h"  	/* Required to identify SOC and Board */
#include "mv_os.h"
#include "soc_spec.h"
#include "mvUart.h"
#include "mvSysEnvLib.h"

/************************************************************************************
* Name:		suspendWakeup:
* Desc:		- Detects suspend to RAM state
* 		- reads the return PC
* 		- performs registers write according to a list.
*
* 		The data is stored in the following format
* 		boot_info_addr + 0x0 = magic word
* 		boot_info_addr + 0x4 = resume pc
* 		boot_info_addr + 0x8 + 8*n = register addres (0xFFFFFFFF means end list)
*		boot_info_addr + 0xC + 8*n = register value
* Args:	 	None.
* Notes:
* Returns:	None.
*/
#define BOOT_INFO_ADDR				(0x3000)
#define SUSPEND_MAGIC_WORD			(0xDEADB002)
#define REGISTER_LIST_END			(0xFFFFFFFF)
MV_STATUS suspendWakeup(void)
{
	int *boot_info = (int*)(BOOT_INFO_ADDR);
	int  magic_word;
	void (*resumeFunc)(void) = NULL;
	int *reg_addr, reg_value;

	/* Check Suspend Wakeup support and Wakeup indication: (0 = not supported at all) */
	if (mvSysEnvSuspendWakeupCheck() == MV_SUSPEND_WAKEUP_DISABLED)
		return MV_OK;

	/* Read and clear magic word to avoid successive resumes */
	magic_word =  *(boot_info);
	(*boot_info++) =  0x0; /* boot_info -> return pc */

	if(!(magic_word == SUSPEND_MAGIC_WORD)) {
		DEBUG_INIT_S("Not detected suspend to RAM indication\n");
		return MV_OK;
	}
	DEBUG_INIT_S("Detected suspend to RAM indication\n");

	resumeFunc = (void *)(*boot_info++);
	/* boot_info now points to the 1st register address*/
	reg_addr = (int *)(*boot_info++);

	/* Restore registers */
	while (reg_addr != (int *)REGISTER_LIST_END) {
		reg_value = (*boot_info++);
		*reg_addr = reg_value;
		reg_addr = (int *)(*boot_info++);
	}

	/* Jump back to OS */
	DEBUG_INIT_S("Returning to OS\n");
	resumeFunc();

	return MV_OK;
}

