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

#ifndef _INC_A375_TRAINING_STATIC_H
#define _INC_A375_TRAINING_STATIC_H

/* STATIC_TRAINING - Set only if static parameters for training are set and required */
/*#define STATIC_TRAINING */

typedef struct __mvDramTrainingInit {
	MV_U32 reg_addr;
	MV_U32 reg_value;
} MV_DRAM_TRAINING_INIT;

MV_DRAM_TRAINING_INIT ddr3_db_88F6650[MV_MAX_DDR3_STATIC_SIZE] =
{
#if 0
    /*PUP	RdSampleDly (+CL)	Phase	RL ADLL value */
    /* Read Leveling */
	/* PUP0 */
    {0x000016A0, 0xC0020000},/*CS0*/
    {0x000016A0, 0xC0060000},/*CS1*/
	/* PUP1 */
    {0x000016A0, 0xC0420000},/*CS0*/
    {0x000016A0, 0xC0460000},/*CS1*/

    /* Write Leveling */
    /* PUP0 */
    {0x000016A0, 0xC0008812},/*CS0*/
    {0x000016A0, 0xC0048812},/*CS1*/
    /* PUP1 */
    {0x000016A0, 0xC0408812},/*CS0*/
    {0x000016A0, 0xC0448812},/*CS1*/

    /* DQS ref delay*/
    {0x000016A0, 0xC003000f},/*CS0*/
    {0x000016A0, 0xC007000f},/*CS1*/

    {0x00001538, 0x00000909}, 	/*Read Data Sample Delays Register */
    {0x0000153C, 0x00000d0d}, 	/*Read Data Ready Delay Register */
#endif
    /*init DRAM */
    {0x00001480, 0x00000001},

    /*HW mechanizem*/
    /* WL training*/
    {0x000015B0, 0x80300008},
    /* Preload training pattern to DRAM*/
    {0x000015B0, 0x80300010},
    /* Read Leveling using training pattern at high frequency*/
    {0x000015B0, 0x80300040},

    {0x0, 0x0}
};
MV_DRAM_TRAINING_INIT ddr3_db_88F6660[MV_MAX_DDR3_STATIC_SIZE] =
{
#if 0
    /*PUP	RdSampleDly (+CL)	Phase	RL ADLL value */
    /* Read Leveling */
    /*PUP 0 */
    {0x000016A0, 0xC0020119},
    {0x000016A0, 0xC0060119},
    /*PUP 1 */
    {0x000016A0, 0xC042011b},
    {0x000016A0, 0xC046011b},

    /* Write Leveling */
    /*PUP 0 */
    {0x000016A0, 0xC000700f},
    {0x000016A0, 0xC004700f},
    /*PUP 1 */
    {0x000016A0, 0xC040680d},
    {0x000016A0, 0xC044680d},

    /* DQS ref delay*/
    {0x000016A0, 0xC003000f},/*CS0*/
    {0x000016A0, 0xC007000f},/*CS1*/

    {0x00001538, 0x00000909}, 	/*Read Data Sample Delays Register */
    {0x0000153C, 0x00000d0d}, 	/*Read Data Ready Delay Register */
#endif

    /*init DRAM */
    {0x00001480, 0x00000001},

    /*HW mechanizem*/
    /* WL training*/
    {0x000015B0, 0x80300008},
    /* Preload training pattern to DRAM*/
    {0x000015B0, 0x80300010},
    /* Read Leveling using training pattern at high frequency*/
    {0x000015B0, 0x80300040},

    {0x0, 0x0}
};



#endif /* _INC_A375_TRAINING_STATIC_H */
