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

#include "mvOs.h"
#include "mvEthPhy.h"
#include "mvEth10gPhy.h"
#include "mvEthPhyXsmi.h"

static MV_ETHPHY_HAL_DATA eth10gPhyHalData;

/*******************************************************************************
* mvEth10gPhyHalInit -
*
* DESCRIPTION:
*       Initialize the ethernet 10G phy unit HAL.
*
* INPUT:
*       halData	- Ethernet PHY HAL data.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_OK on success, MV_ERROR otherwise.
*
*******************************************************************************/
MV_STATUS mvEth10gPhyHalInit(MV_ETHPHY_HAL_DATA *halData)
{
	mvOsMemcpy(&eth10gPhyHalData, halData, sizeof(MV_ETHPHY_HAL_DATA));

	return MV_OK;
}

/*******************************************************************************
* mvEthX2242PPhyBasicInit -
*
* DESCRIPTION:
*       Init the 88X2242P phy to 10GBaseR.
*
* INPUT:
*       phyAddr - the XSMI address of the PHY.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_OK on success, MV_ERROR otherwise.
*
******************************************************************************/
MV_STATUS mvEthX2242PPhyBasicInit(MV_U32 ethPortNum)
{
	MV_U32 phyAddr;

	phyAddr = eth10gPhyHalData.phyAddr[ethPortNum];

	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF003, 0xc0c0);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF016, 0x0008);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF013, 0x0ff0);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF012, 0x0dbc);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF014, 0x0000);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF002, 0x7172);
	mvEthPhyXsmiRegWrite(phyAddr, 0x03, 0xF07C, 0x8143);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x9041, 0x0001);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x81FB, 0x0096);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x8060, 0x6D6D);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF003, 0x8080);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x9041, 0x0001);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x8108, 0xF908);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x8102, 0x98a0);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0xb841, 0xe000);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0xb108, 0xF0d0);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0xb121, 0xD938);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0x8000, 0x0018);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1E, 0xb000, 0x00c1);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF403, 0x8000);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF003, 0x0000);
	mvEthPhyXsmiRegWrite(phyAddr, 0x03, 0x0000, 0x2040);
	mvEthPhyXsmiRegWrite(phyAddr, 0x03, 0x1000, 0x0000);
	mvEthPhyXsmiRegWrite(phyAddr, 0x01, 0x0000, 0x2040);
	mvEthPhyXsmiRegWrite(phyAddr, 0x01, 0x1000, 0x2040);
	mvEthPhyXsmiRegWrite(phyAddr, 0x04, 0x0000, 0x2040);
	mvEthPhyXsmiRegWrite(phyAddr, 0x04, 0x1000, 0x2040);
	mvEthPhyXsmiRegWrite(phyAddr, 0x1F, 0xF012, 0x0CBC);

	return MV_OK;
}

/*******************************************************************************
* mvEth10gPhyInit -
*
* DESCRIPTION:
*       Initialize the ethernet 10G phy unit.
*
* INPUT:
*       ethPortNum - The port number on which to initialize the PHY.
*	eeeEnable  - Whether to enable EEE or not.
*
* OUTPUT:
*       None.
*
* RETURN:
*       MV_OK on success, MV_ERROR otherwise.
*
*******************************************************************************/
MV_STATUS mvEth10gPhyInit(MV_U32 ethPortNum, MV_BOOL eeeEnable)
{
	MV_U32     phyAddr = 0;
	MV_U16     deviceId;
	MV_U16     id1, id2;

	if (ethPortNum != ((MV_U32) -1))
		phyAddr = eth10gPhyHalData.phyAddr[ethPortNum];

	/* Reads ID1 */
	if (mvEthPhyXsmiRegRead(phyAddr, 2, 2, &id1) != MV_OK) {
		mvOsPrintf("%s: Port%d: phyAddr=0x%x -  phy read id1 failed\n", __func__, ethPortNum, phyAddr);
		return MV_ERROR;
	}

	/* Reads ID2 */
	if (mvEthPhyXsmiRegRead(phyAddr, 2, 3, &id2) != MV_OK) {
		mvOsPrintf("%s: Port%d: phyAddr=0x%x -  phy read id2 failed\n", __func__, ethPortNum, phyAddr);
		return MV_ERROR;
	}

	if (!MV_IS_MARVELL_OUI(id1, id2)) {
		mvOsPrintf("%s: Port%d: phyAddr=0x%x, Not Marvell PHY id1 %x id2 %x\n",
				__func__, ethPortNum, phyAddr, id1, id2);
		return MV_ERROR;
	}

	deviceId = (id2 & PHY_MODEL_MASK) >> PHY_MODEL_OFFSET;
	switch (deviceId) {
	case MV_PHY_88X2242P:
		mvEthX2242PPhyBasicInit(ethPortNum);
		break;
	default:
		mvOsPrintf("%s: Unknown Device(%#x). Initialization failed\n", __func__ , deviceId);
		return MV_ERROR;
	}
	return MV_OK;
}
