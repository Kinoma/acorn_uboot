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
#include "mv_os.h"
#include "config_marvell.h"     /* Required to identify SOC and Board */
#include "soc_spec.h"
#ifdef MV88F67XX
#include "generalInit.h"

#define REG_XBAR_WIN_19_CTRL_ADDR	0x200e8

MV_VOID sramConfig(void)
{
	MV_U32 *ptr;

	/* disable DRAM Window */
	MV_REG_WRITE(REG_XBAR_WIN_19_CTRL_ADDR, 0x1FFF0E00);	/*Close Mbus Window - 1G */

	/* Fix L2 way 0 size to 256KB */
	MV_REG_WRITE(0x20240,0x40000301);

	/* set L2 way 3 to SRAM at 0 */
	MV_REG_WRITE(0x850C,0x1);
	MV_REG_WRITE(0x20244,0x001);

	ptr = (void*)0x10;
	*ptr = 0xE3A0F02C;		/* mov pc,#0x2C */
	ptr = (void*)0x2C;
	*ptr = 0xE24EE008;		/* sub r14,r14,#0x8 */
	ptr = (void*)0x30;
	*ptr = 0xE1B0F00E;		/* movs pc,r14 */

	changeResetVecBase(0);

	/* remove CPSR bit 8 - don't mask data abort - now we should get data abort */
	setCPSR(0);

	setCPSR(1<<8);

	changeResetVecBase(1<<13);

	/* close L2 way 3 to SRAM at 0 */
	MV_REG_WRITE(0x850C,0x0);
	MV_REG_WRITE(0x20244,0x000);

	/* enable DRAM Window */
	MV_REG_WRITE(REG_XBAR_WIN_19_CTRL_ADDR, 0x1FFF0E01);	/*Close Mbus Window - 1G */
}
#endif /* MV88F67XX */
