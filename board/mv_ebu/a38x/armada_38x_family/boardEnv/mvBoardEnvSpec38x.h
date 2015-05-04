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

/* Armada-38x Customer boards */
#define CUTOMER_BOARD_ID_BASE                   A38X_CUTOMER_BOARD_ID_BASE
#define CUSTOMER_BOARD_ID0			A38X_CUSTOMER_BOARD_ID0
#define CUSTOMER_BOARD_ID1			A38X_CUSTOMER_BOARD_ID1
#define MV_MAX_CUSTOMER_BOARD_ID                A38X_MV_MAX_CUSTOMER_BOARD_ID
#define MV_CUSTOMER_BOARD_NUM                   A38X_MV_CUSTOMER_BOARD_NUM

/* Armada-38x Marvell boards */
#define MARVELL_BOARD_ID_BASE                   A38X_MARVELL_BOARD_ID_BASE
#define MV_MAX_MARVELL_BOARD_ID                 A38X_MV_MAX_MARVELL_BOARD_ID
#define MV_MARVELL_BOARD_NUM                    A38X_MV_MARVELL_BOARD_NUM

#define MV_DEFAULT_BOARD_ID			DB_68XX_ID

/****************************** Customer Boards *******************************/
/*******************************************************************************
 * A38x Customer Board 0 - Based on DB-BP
 *******************************************************************************/
#define A38x_CUSTOMER_BOARD_0_MPP0_7		0x11111111
#define A38x_CUSTOMER_BOARD_0_MPP8_15		0x11111111
#define A38x_CUSTOMER_BOARD_0_MPP16_23		0x11266011
#define A38x_CUSTOMER_BOARD_0_MPP24_31		0x22222011
#define A38x_CUSTOMER_BOARD_0_MPP32_39		0x22200002
#define A38x_CUSTOMER_BOARD_0_MPP40_47		0x40042022
#define A38x_CUSTOMER_BOARD_0_MPP48_55		0x55550555
#define A38x_CUSTOMER_BOARD_0_MPP56_63		0x00005550

#define A38x_CUSTOMER_BOARD_0_GPP_OUT_ENA_LOW      0xFFFFFFFF
#define A38x_CUSTOMER_BOARD_0_GPP_OUT_ENA_MID      (~(BIT1 | BIT2 | BIT3))
#define A38x_CUSTOMER_BOARD_0_GPP_OUT_VAL_LOW      0x0
#define A38x_CUSTOMER_BOARD_0_GPP_OUT_VAL_MID      0x0
#define A38x_CUSTOMER_BOARD_0_GPP_POL_LOW          0x0
#define A38x_CUSTOMER_BOARD_0_GPP_POL_MID          0x0

/*******************************************************************************
 * A38x Customer Board 1 - Based on DB-GP
 *******************************************************************************/
#define A38x_CUSTOMER_BOARD_1_MPP0_7		0x11111111
#define A38x_CUSTOMER_BOARD_1_MPP8_15		0x11111111
#define A38x_CUSTOMER_BOARD_1_MPP16_23		0x11244011
#define A38x_CUSTOMER_BOARD_1_MPP24_31		0x22222111
#define A38x_CUSTOMER_BOARD_1_MPP32_39		0x22200002
#define A38x_CUSTOMER_BOARD_1_MPP40_47		0x30042022
#define A38x_CUSTOMER_BOARD_1_MPP48_55		0x55550555
#define A38x_CUSTOMER_BOARD_1_MPP56_63		0x00005550

#define A38x_CUSTOMER_BOARD_1_GPP_OUT_ENA_LOW	(~(BIT1  | BIT4  | BIT6  | BIT7  | BIT8  | BIT9  | BIT10 | \
					   BIT11 | BIT19 | BIT22 | BIT23 | BIT25 | BIT26 | \
					   BIT27 | BIT29 | BIT30 | BIT31))
					/* PIC (STR) GPIO = 33, 34, 35 */
#define A38x_CUSTOMER_BOARD_1_GPP_OUT_ENA_MID	(~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT15 | BIT16 | BIT17 | BIT18))

#define A38x_CUSTOMER_BOARD_1_GPP_OUT_VAL_LOW	0x0
#define A38x_CUSTOMER_BOARD_1_GPP_OUT_VAL_MID	0x0
#define A38x_CUSTOMER_BOARD_1_GPP_POL_LOW		0x0
#define A38x_CUSTOMER_BOARD_1_GPP_POL_MID		0x0

/******************************* Marvell Boards *******************************/

/*******************************************************************************
 * A380 DB-88F6810 board */
/******************************************************************************
   MPP#			DEFAULT UNIT		        MPP Values (respectively)
   -----------------------------------------------------------------------------------
   0-1			UART				1
   2-3			I2C0				1
   4-5			SMI ( MDC/MDIO )		1
   6-17			GE0				1
   18			SGMII , GIGA0			0
   19-20		Uart1				6
   21, 27-32
   37-41	GE1					2
   22-25	SPI					1
   33-35	7 segment				0
   26		USB Device detect			0  (USB3_0)
   36,42	Spare Inputs				0
   43		M_VTT_CTRL (out)			2
   44		SATA3_PRESENT_ACTIVEn			4
45,46,56	Inputs( PIC outputs )			0
	47		SATA1_PRESENT_ACTIVEn		4
   48-50
   52-55
   57-59	SDIO 8 Bit				5
    51		USB Device detect0			0
*/
#define GROUP0_DEFAULT_MPP_TWSI_I2C         0x1111  /* TWSI , I2C */
#define GROUP0_DEFAULT_MPP_TWSI_I2C_MASK    0xFFFF  /* TWSI , I2C */
/*******************************************************************************
* A380 DB-68xx board
*******************************************************************************/
#define DB_88F68XX_MPP0_7               0x11111111
#define DB_88F68XX_MPP8_15              0x11111111
#define DB_88F68XX_MPP16_23             0x11266011
#define DB_88F68XX_MPP24_31             0x22222011
#define DB_88F68XX_MPP32_39             0x22200002
#define DB_88F68XX_MPP40_47             0x40042022
#define DB_88F68XX_MPP48_55             0x55550555
#define DB_88F68XX_MPP56_63             0x00005550

#define DB_88F68XX_GPP_OUT_ENA_LOW      0xFFFFFFFF
#define DB_88F68XX_GPP_OUT_ENA_MID      (~(0x0E))
#define DB_88F68XX_GPP_OUT_ENA_HIGH     (~(BIT1))
#define DB_88F68XX_GPP_OUT_VAL_LOW      0x0
#define DB_88F68XX_GPP_OUT_VAL_MID      0x0
#define DB_88F68XX_GPP_OUT_VAL_HIGH     0x0
#define DB_88F68XX_GPP_POL_LOW          0x0
#define DB_88F68XX_GPP_POL_MID          0x0
#define DB_88F68XX_GPP_POL_HIGH         0x0

/*******************************************************************************
* A380 RD-68XX board
*******************************************************************************/
/******************************************************************************
   MPP#			DEFAULT UNIT		        MPP Values (respectively)
   -----------------------------------------------------------------------------------
   0-1			UART					1
   2-3			I2C0					1
   4-5			SMI ( MDC/MDIO )			1
   6-17			GE0					1
   18			IO_INT					0
   19			SATA1					4
   20			SATA0					4
   21, 27-32
   37-41	GE1						2
   22-26	SPI0						1
   33		CPU Clock Frequency Options[0]			0
   34      CPU Clock Frequency Options[3]			0
   35      CPU Clock Frequency Options[4]			0
   36      CPU1 Enable						0
   42		FAN_CTRL					0
   43		M_VTT_CTRL (out)				2
   44		SATA3_PRESENT_ACTIVEn				4
   45-46	REF_CLK_OUT[0,1] (out)				0
   47		SATA2_PRESENT_ACTIVEn				3
   48-50
   52-55
   57-59	SDIO 8 Bit					5
    56		boot Device mode				0
    51		USB Device detect0				0
 */
#define RD_88F68XX_MPP0_7               0x11111111
#define RD_88F68XX_MPP8_15              0x11111111
#define RD_NAS_88F68XX_MPP16_23         0x11244011
#define RD_WAP_88F68XX_MPP16_23         0x11203011
#define RD_88F68XX_MPP24_31             0x22222111
#define RD_88F68XX_MPP32_39             0x22200002
#define RD_88F68XX_MPP40_47             0x30042022
#define RD_88F68XX_MPP48_55             0x55550555
#define RD_88F68XX_MPP56_63             0x00005550

#define RD_88F68XX_GPP_OUT_ENA_LOW      (~(BIT1  | BIT4  | BIT6  | BIT7  | BIT8  | BIT9  | BIT10 | \
					   BIT11 | BIT19 | BIT20 | BIT22 | BIT23 | BIT25 | BIT26 | \
					   BIT27 | BIT29 | BIT30 | BIT31))
#define RD_88F68XX_GPP_OUT_ENA_MID	(~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT15 | BIT16 | BIT17 | BIT18))
#define RD_WAP_88F68XX_GPP_OUT_ENA_LOW      (~(BIT1  | BIT4  | BIT6  | BIT7  | BIT8  | BIT9  | BIT10 | \
					   BIT11 | BIT19 | BIT22 | BIT23 | BIT25 | BIT26 | \
					   BIT27 | BIT29 | BIT30 | BIT31))
#define RD_88F68XX_GPP_OUT_VAL_LOW	0x0
#define RD_88F68XX_GPP_OUT_VAL_MID	0x0
#define RD_88F68XX_GPP_POL_LOW		0x0
#define RD_88F68XX_GPP_POL_MID		0x0

/*******************************************************************************
* A380 DB-AP-68xx board
*******************************************************************************/
#define DB_AP_88F68XX_MPP0_7            0x11111111
#define DB_AP_88F68XX_MPP8_15           0x11111111
#define DB_AP_88F68XX_MPP16_23          0x55066011
#define DB_AP_88F68XX_MPP24_31          0x05050050
#define DB_AP_88F68XX_MPP32_39          0x05055555
#define DB_AP_88F68XX_MPP40_47          0x01100565
#define DB_AP_88F68XX_MPP48_55          0x00000000
#define DB_AP_88F68XX_MPP56_63          0x00004444

#define DB_AP_88F68XX_GPP_OUT_ENA_LOW	(~(BIT21)) /* PIC (STR) GPIO = 21, 47 */
#define DB_AP_88F68XX_GPP_OUT_ENA_MID	(~(BIT1 | BIT2 | BIT3 | BIT15))
#define DB_AP_88F68XX_GPP_OUT_ENA_HIGH	(~(BIT1))
#define DB_AP_88F68XX_GPP_OUT_VAL_LOW	0x0
#define DB_AP_88F68XX_GPP_OUT_VAL_MID	0x0
#define DB_AP_88F68XX_GPP_OUT_VAL_HIGH	0x0
#define DB_AP_88F68XX_GPP_POL_LOW	0x0
#define DB_AP_88F68XX_GPP_POL_MID	0x0
#define DB_AP_88F68XX_GPP_POL_HIGH	0x0

/*******************************************************************************
* A380 DB-GP-68xx board
*******************************************************************************/
#define DB_GP_88F68XX_MPP0_7		0x11111111
#define DB_GP_88F68XX_MPP8_15		0x11111111
#define DB_GP_88F68XX_MPP16_23		0x11244011
#define DB_GP_88F68XX_MPP24_31		0x22222111
#define DB_GP_88F68XX_MPP32_39		0x22200002
#define DB_GP_88F68XX_MPP40_47		0x30042022
#define DB_GP_88F68XX_MPP48_55		0x55550555
#define DB_GP_88F68XX_MPP56_63		0x00005550

#define DB_GP_88F68XX_GPP_OUT_ENA_LOW	(~(BIT1  | BIT4  | BIT6  | BIT7  | BIT8  | BIT9  | BIT10 | \
					   BIT11 | BIT19 | BIT22 | BIT23 | BIT25 | BIT26 | \
					   BIT27 | BIT29 | BIT30 | BIT31))
					/* PIC (STR) GPIO = 33, 34, 35 */
#define DB_GP_88F68XX_GPP_OUT_ENA_MID	(~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT15 | BIT16 | BIT17 | BIT18))

#define DB_GP_88F68XX_GPP_OUT_VAL_LOW	0x0
#define DB_GP_88F68XX_GPP_OUT_VAL_MID	0x0
#define DB_GP_88F68XX_GPP_POL_LOW		0x0
#define DB_GP_88F68XX_GPP_POL_MID		0x0

/*******************************************************************************
* A380 DB-6821-BP board
*******************************************************************************/
#define DB_BP_88F6821_MPP0_7		0x11111111 /* UA0_[RX,TX], I2C_[TXD,SCK,SDA], SMI0_[MD,MDI], GE0_[TXCL,TXD] */
#define DB_BP_88F6821_MPP8_15		0x11111111 /* GE0_XXX */
#define DB_BP_88F6821_MPP16_23		0x11466011 /* GE0_RXC[T,L],GPIO(interrupt),UA1_[RX,TX],SD_CMD,SPI0_[MOS,SCK] */
#define DB_BP_88F6821_MPP24_31		0x00040011 /* SPI0_[MOS,SCK], GPIO26(USB in), GPIO27, SD_CLK,  GPIO[29,30,31] */
#define DB_BP_88F6821_MPP32_39		0x44400000 /* GPIO31,GPIOx3(7 seg-out),GPIO36(spare-in),SD_D[3,0,1] */
#define DB_BP_88F6821_MPP40_47		0x20012004 /* SD_DD2,GPIO41 ,GPIO(42-new PIC,45,46-pic;out),SATA[0,1]_Active */

#define DB_BP_88F6821_GPP_OUT_ENA_LOW	0xFFFFFFFF
#define DB_BP_88F6821_GPP_OUT_ENA_MID	(~(BIT10 | BIT13 | BIT14)) /* GPIO output for PIC @ MPP 42,45,46 */
#define DB_BP_88F6821_GPP_OUT_VAL_LOW	0x0
#define DB_BP_88F6821_GPP_OUT_VAL_MID	0x0
#define DB_BP_88F6821_GPP_POL_LOW	0x0
#define DB_BP_88F6821_GPP_POL_MID	0x0

/*******************************************************************************
* A38X-AMC (DB-88F6820-AMC) board
*******************************************************************************/
#define DB_AMC_88F68XX_MPP0_7		0x11111111 /* UA0_[RX,TX], I2C_[TXD,SCK,SDA], SMI0_[MD,MDI], GE0_[TXCL,TXD] */
#define DB_AMC_88F68XX_MPP8_15		0x11111111 /* GE0_XXX */
#define DB_AMC_88F68XX_MPP16_23		0x55066011 /* GE0_RXC[T,L],GPIO(USB0 In),UA1[R,T]X,GPIO(USB1 In),ND_IO[0,2]*/
#define DB_AMC_88F68XX_MPP24_31		0x05055550 /* MV_INT#, ND_CE[0,1,2]#, ND_IO[5], QS_SMI_ENA, ND_REn, N/A */
#define DB_AMC_88F68XX_MPP32_39		0x05055555 /* ND_WEn,ND_IO[3,1],ND_[ALE,CLE],PHY0_INT#,ND_IO[4],PHY1_INT#*/
#define DB_AMC_88F68XX_MPP40_47		0x01106565 /* ND[IO6,RBn0,IO7,RBn1],I2C_TO_BOARD_EN,REF_CLK_OUT[0,1],Brd_Led0*/
#define DB_AMC_88F68XX_MPP48_55		0x40000000 /* Brd_Led_[1,2,3,4], Dbg_JP[0,3,2], SPI1_CSn1 */
#define DB_AMC_88F68XX_MPP56_63		0x00004444 /* SPI1_[MOSI,SCK,MISO,CSn0] */

#define DB_AMC_88F68XX_GPP_OUT_ENA_LOW	(~(BIT29))	/* GPIO29: QS_SMI_ENA */
#define DB_AMC_88F68XX_GPP_OUT_ENA_MID	(~(BIT12 | BIT17 | BIT18 | BIT20 | BIT21)) /* 44:I2C_EXT_EN, 49,50,52,53:Leds*/
#define DB_AMC_88F68XX_GPP_OUT_ENA_HIGH	0xFFFFFFFF
#define DB_AMC_88F68XX_GPP_OUT_VAL_LOW	(BIT29) /* GPIO29: QS_SMI_ENA = OUT VAL High */
#define DB_AMC_88F68XX_GPP_OUT_VAL_MID	(BIT12) /* GPIO44: I2C_EXT_EN = OUT VAL High */
#define DB_AMC_88F68XX_GPP_OUT_VAL_HIGH	0x0
#define DB_AMC_88F68XX_GPP_POL_LOW	0x0
#define DB_AMC_88F68XX_GPP_POL_MID	0x0
#define DB_AMC_88F68XX_GPP_POL_HIGH	0x0
