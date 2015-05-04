/*******************************************************************************
*                Copyright 2001, Marvell International Ltd.
* This code contains confidential information of Marvell semiconductor, inc.
* no rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
********************************************************************************
* mvHwsTrainingIpDdr3Training.c
*
* DESCRIPTION: DDR3 training IP configuration
*
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*       $Revision: 204 $
******************************************************************************/

#include "mvDdr3TrainingIp.h"
#include "mvDdr3TrainingIpFlow.h"
#include "mvDdr3TrainingIpEngine.h"
#include "mvDdr3TrainingIpPbs.h"
#include "mvDdr3TrainingIpCentralization.h"
#include "mvDdr3TrainingIpStatic.h"
#include "mvDdrTrainingIpDb.h"
#include "mvDdr3TrainingHwAlgo.h"
#include "mvDdr3TrainingLeveling.h"
#include "mvDdr3TrainingIpPrvIf.h"
#include "mvDdr3LoggingDef.h"

#ifdef CONFIG_DDR4
#include "mvHwsDdr4Training.h"
#include "mvHwsDdr4MprPdaIf.h"
#endif
/************************** definitions ******************************/
#ifdef FreeRTOS
#define DFX_BAR1_BASE        (0x80000000)
#else
#define DFX_BAR1_BASE        (0x82000000)
#endif
#define DFX_SERVER_BASE      (0x100000)


#define GET_MAX_VALUE(x,y)              ((x)>(y)) ? (x):(y);
#define CEIL_DIVIDE(x,y)                ((x - (x/y)*y) == 0)? ((x/y)- 1) : (x/y);

/*#define TIME_2_CLOCK_CYCLES(prm, clk)   ((prm-1)/clk)*/
#define TIME_2_CLOCK_CYCLES             CEIL_DIVIDE


#define GET_CS_FROM_MASK(mask) (csMask2Num[mask])
#define CS_CBE_VALUE(csNum)   (csCbeReg[csNum])

extern GT_U8 debugTraining;
extern GT_U8 isRegDump;

GT_U32  windowMemAddr = 0;

GT_U32  PhyReg0Val = 0;
GT_U32  PhyReg1Val = 8;
GT_U32  PhyReg2Val = 0;
GT_U32  PhyReg3Val = 0xA;
MV_HWS_DDR_FREQ initFreq = DDR_FREQ_667;
MV_HWS_DDR_FREQ lowFreq = DDR_FREQ_LOW_FREQ;
MV_HWS_DDR_FREQ mediumFreq;
GT_U32  debugDunit = 0;
GT_U32 odtAdditional = 1;
GT_U32* dqMapTable = NULL;
GT_U32 odtConfig = 1;

#if defined(CONFIG_ARMADA_38X) || defined(CONFIG_ALLEYCAT3) || defined (CONFIG_ARMADA_39X)
GT_U8  isPllBeforeInit = 0, isAdllCalibBeforeInit = 0, isDfsInInit = 0;
GT_U32 dfsLowFreq = 130;
#else
GT_U8  isPllBeforeInit = 0, isAdllCalibBeforeInit = 1, isDfsInInit = 0;
GT_U32 dfsLowFreq = 100;
#endif
GT_U32 gRttNomCS0, gRttNomCS1;
GT_U8 calibrationUpdateControl; /*2 external only, 1 is internal only*/

#if defined(CHX_FAMILY) || defined(EXMXPM_FAMILY)
GT_U8 genericInitController = 1;
#else
extern GT_U8 genericInitController;
#endif

MV_HWS_RESULT trainingResult[MAX_STAGE_LIMIT][MAX_INTERFACE_NUM];
AUTO_TUNE_STAGE trainingStage = INIT_CONTROLLER;
GT_U32 LoadAtHigh = 1;
GT_U32 freqVal[DDR_FREQ_LIMIT];
GT_U32 fingerTest = 0, pFingerStart = 11,  pFingerEnd = 64,  nFingerStart = 11, nFingerEnd = 64, pFingerStep = 3, nFingerStep = 3;
GT_U32 clampTbl[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
GT_U32 mode2T = 0xFF;  /*Initiate to 0xFF , this variable is define by user in debug mode*/
GT_U32 xsbValidateType = 0;
GT_U32 xsbValidationBaseAddress = 0xf000;
GT_U8 disableRL = 1;
GT_U32 selectMDelay = 100, selectEZDelay = 100;
GT_U32 firstActiveIf = 0;
MV_HWS_TOPOLOGY_MAP *topologyMap;
GT_U32 dfsLowPhy1 = 0x1f;
GT_U32 multicastId = 0;
GT_BOOL useBroadcast = GT_FALSE;
MV_HWS_TIP_FREQ_CONFIG_INFO *freqInfoTable = NULL;
GT_U8 isCbeRequired = GT_FALSE;
GT_U32 debugMode = GT_FALSE;
GT_U32 delayEnable = 0;
static GT_U32 freqMask[HWS_MAX_DEVICE_NUM][DDR_FREQ_LIMIT];
GT_BOOL rlMidFreqWA = GT_FALSE;
extern GT_U32 ddr4TipConfigurePhyVrefTap;

#if defined (CONFIG_ALLEYCAT3)
GT_U32 mvHwsmemSize[] = { ADDR_SIZE_512Mb, ADDR_SIZE_1Gb, ADDR_SIZE_2Gb, ADDR_SIZE_4Gb ,ADDR_SIZE_8Gb };
#define  MV_DEVICE_MAX_DRAM_ADDRESS_SIZE          ADDR_SIZE_2Gb
#endif

/************************** globals ***************************************/

GT_U32 effective_cs = 0;

GT_U32 maskTuneFunc = (SET_MEDIUM_FREQ_MASK_BIT     |
                       WRITE_LEVELING_MASK_BIT      |
                       LOAD_PATTERN_2_MASK_BIT      |
                       READ_LEVELING_MASK_BIT       |
                       SET_TARGET_FREQ_MASK_BIT     |
                       WRITE_LEVELING_TF_MASK_BIT   |
#ifdef CONFIG_DDR3
                       READ_LEVELING_TF_MASK_BIT    |
#else
                       SW_READ_LEVELING_MASK_BIT    |
#endif
                       CENTRALIZATION_RX_MASK_BIT   |
                       CENTRALIZATION_TX_MASK_BIT   );

void ddr3PrintVersion()
{
	mvPrintf(DDR3_TIP_VERSION_STRING);
}



extern GT_U32 isPllOld;
extern ClValuePerFreq casLatencyTable[];
extern PatternInfo patternTable[] ;
extern ClValuePerFreq casWriteLatencyTable[];
extern GT_U8  debugTraining;
extern GT_U8  debugCentralization ,debugTrainingIp, debugTrainingBist, debugPbs, debugTrainingStatic, debugLeveling;
extern GT_U32  pipeMulticastMask;
extern MV_HWS_TIP_CONFIG_FUNC_DB configFuncInfo[HWS_MAX_DEVICE_NUM];
extern GT_U8 csMaskReg[];
extern GT_U8 twrMaskTable[];
extern GT_U8 clMaskTable[];
extern GT_U8 cwlMaskTable[];
extern GT_U16 rfcTable[];
extern GT_U32 speedBinTableTRc[];
extern GT_U32 speedBinTableTRcdTRp[];
extern GT_U32 ckDelay, ckDelay_16;
extern GT_U32 gZpriData;
extern GT_U32 gZnriData;
extern GT_U32 gZpriCtrl;
extern GT_U32 gZnriCtrl;

extern GT_U32 gZpodtData;
extern GT_U32 gZnodtData;
extern GT_U32 gZpodtCtrl;
extern GT_U32 gZnodtCtrl;

extern GT_U32 gDic;
extern GT_U32 uiODTConfig;
extern GT_U32 gRttNom;


/************************** pre-declarations ******************************/
static GT_STATUS    ddr3TipDDR3Ddr3TrainingMainFlow
( 
    GT_U32               devNum
);


static GT_STATUS ddr3TipWriteOdt
(
    GT_U32			        devNum,
    MV_HWS_ACCESS_TYPE	    accessType,
    GT_U32					interfaceId,
    GT_U32                  clValue,
    GT_U32                  cwlValue
);

static GT_STATUS    ddr3TipDDR3AutoTune
( 
    GT_U32  devNum
);

static GT_STATUS isBusAccessDone
(
    GT_U32 devNum,
    GT_U32 interfaceId,
    GT_U32 dunitRegAdrr,
    GT_U32 bit
);

#ifdef ODT_TEST_SUPPORT
static GT_STATUS odtTest
(
    GT_U32              devNum,
    MV_HWS_ALGO_TYPE    algoType
);
#endif

GT_STATUS AdllCalibration
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE		accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency
);

static GT_STATUS ddr3TipSetTiming
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE	    accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency
);

/************************** global data ******************************/

static PageElement pageParam[] =
{
   /*8bits        16 bits
   page-size(K)   page-size(K)      mask */
   {1 ,           2,             2},
      /* 512M */
   {1 ,           2,             3},
      /* 1G */
   {1 ,           2,             0},
      /* 2G */
   {1 ,           2,             4},
      /* 4G */
   {2 ,           2,             5}
      /* 8G */
};

static GT_U8 memSizeConfig[MEM_SIZE_LAST] =
{
	0x2, /* 512Mbit  */
	0x3, /* 1Gbit    */
	0x0, /* 2Gbit    */
	0x4, /* 4Gbit    */
	0x5  /* 8Gbit    */
};

static GT_U8 csMask2Num[]= {0, 0, 1, 1,  2, 2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3};

static RegData odpgDefaultValue[]=
{
    {0x1034, 0x38000,   MASK_ALL_BITS},
    {0x1038, 0x0,       MASK_ALL_BITS},
    {0x10b0, 0x0,       MASK_ALL_BITS},
    {0x10b8, 0x0,       MASK_ALL_BITS},
    {0x10c0, 0x0,       MASK_ALL_BITS},
    {0x10f0, 0x0,       MASK_ALL_BITS},
    {0x10f4, 0x0,       MASK_ALL_BITS},
    {0x10f8, 0xff,      MASK_ALL_BITS},
    {0x10fc, 0xffff,    MASK_ALL_BITS},
    {0x1130, 0x0,       MASK_ALL_BITS},
    {0x1830, 0x2000000, MASK_ALL_BITS},
    {0x14d0, 0x0,       MASK_ALL_BITS},
    {0x14d4, 0x0,       MASK_ALL_BITS},
    {0x14d8, 0x0,       MASK_ALL_BITS},
    {0x14dc, 0x0,       MASK_ALL_BITS},
    {0x1454, 0x0,       MASK_ALL_BITS},
    {0x1594, 0x0,       MASK_ALL_BITS},
    {0x1598, 0x0,       MASK_ALL_BITS},
    {0x159c, 0x0,       MASK_ALL_BITS},
    {0x15a0, 0x0,       MASK_ALL_BITS},
    {0x15a4, 0x0,       MASK_ALL_BITS},
    {0x15a8, 0x0,       MASK_ALL_BITS},
    {0x15ac, 0x0,       MASK_ALL_BITS},
    {0x1604, 0x0,       MASK_ALL_BITS},
    {0x1608, 0x0,       MASK_ALL_BITS},
    {0x160c, 0x0,       MASK_ALL_BITS},
    {0x1610, 0x0,       MASK_ALL_BITS},
    {0x1614, 0x0,       MASK_ALL_BITS},
    {0x1618, 0x0,       MASK_ALL_BITS},
    {0x1624, 0x0,       MASK_ALL_BITS},
    {0x1690, 0x0,       MASK_ALL_BITS},
    {0x1694, 0x0,       MASK_ALL_BITS},
    {0x1698, 0x0,       MASK_ALL_BITS},
    {0x169c, 0x0,       MASK_ALL_BITS},
    {0x14b8, 0x6f67,    MASK_ALL_BITS},
    {0x1630, 0x0,       MASK_ALL_BITS},
    {0x1634, 0x0,       MASK_ALL_BITS},
    {0x1638, 0x0,       MASK_ALL_BITS},
    {0x163c, 0x0,       MASK_ALL_BITS},
    {0x16b0, 0x0,       MASK_ALL_BITS},
    {0x16b4, 0x0,       MASK_ALL_BITS},
    {0x16b8, 0x0,       MASK_ALL_BITS},
    {0x16bc, 0x0,       MASK_ALL_BITS},
    {0x16c0, 0x0,       MASK_ALL_BITS},
    {0x16c4, 0x0,       MASK_ALL_BITS},
    {0x16c8, 0x0,       MASK_ALL_BITS},
    {0x16cc, 0x1,       MASK_ALL_BITS},
    {0x16f0, 0x1,       MASK_ALL_BITS},
    {0x16f4, 0x0,       MASK_ALL_BITS},
    {0x16f8, 0x0,       MASK_ALL_BITS},
    {0x16fc, 0x0,       MASK_ALL_BITS}     
};

/**************************** internal function header ************************/
GT_STATUS ddr3TipEnableInitSequence(GT_U32 devNum);


static GT_STATUS    ddr3TipBusAccess
(
    GT_U32              devNum,
    MV_HWS_ACCESS_TYPE  interfaceAccess,
    GT_U32              interfaceId,
    MV_HWS_ACCESS_TYPE  phyAccess,
    GT_U32              phyId,
    MV_HWS_DDR_PHY      phyType,
    GT_U32              regAddr,
    GT_U32              dataValue,
    MV_HWS_Operation    operType
);


static GT_STATUS ddr3TipPadInv(GT_U32 devNum, GT_U32 interfaceId);
static GT_STATUS ddr3TipRankControl(GT_U32 devNum, GT_U32 interfaceId);

/*****************************************************************************
Register freq mask
******************************************************************************/
GT_STATUS mvHwsDdr3TipRegisterFreqMask(GT_U32 devNum, GT_U32* freqMaskUsr)
{
    GT_U32 cnt;
    for(cnt=0 ; cnt < DDR_FREQ_LIMIT; cnt++)
    {
        freqMask[devNum][cnt] = freqMaskUsr[cnt];
    }
    return GT_OK;
}

/*****************************************************************************
Update global training parameters by data from user
******************************************************************************/
GT_STATUS ddr3TipTuneTrainingParams
(
    GT_U32                  devNum,
    GT_TUNE_TRAINING_PARAMS *params
)
{
	devNum = devNum; /* avoid warnings */

	if(params->ckDelay != -1)
		ckDelay = params->ckDelay;

	if(params->ckDelay_16 != -1)
		ckDelay_16 =  params->ckDelay_16;
	if(params->PhyReg3Val != -1)
		PhyReg3Val = params->PhyReg3Val;

	return GT_OK;
}

/*****************************************************************************
Configure CS
******************************************************************************/
GT_STATUS ddr3TipConfigureCs(GT_U32 devNum, GT_U32 interfaceId, GT_U32 csNum, GT_U32 enable)
{
    GT_U32 data, addrHi, dataHigh;
	GT_U32 memIndex;

    if (enable == 1)
    {
        data = (topologyMap->interfaceParams[interfaceId].busWidth == BUS_WIDTH_8) ? 0:1;
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, SDRAM_ACCESS_CONTROL_REG, (data << (csNum*4)), 0x3 << (csNum*4)));
		memIndex = topologyMap->interfaceParams[interfaceId].memorySize;

        addrHi  = memSizeConfig[memIndex] & 0x3;
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, SDRAM_ACCESS_CONTROL_REG, (addrHi << (2+csNum*4)), 0x3 <<(2+csNum*4)));

		dataHigh = (memSizeConfig[memIndex] & 0x4) >> 2;
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, SDRAM_ACCESS_CONTROL_REG, dataHigh<<(20 +csNum) , 1 << (20 +csNum)));

		/*Enable Address Select Mode */
		CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, SDRAM_ACCESS_CONTROL_REG, 1 <<(16+csNum), 1 <<(16+csNum)));
    }
    switch(csNum)
    {
        case 0:
        case 1:
        case 2:
	        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, DDR_CONTROL_LOW_REG, (enable << (csNum + 11)), 1 << ( csNum + 11) ));
            break;
        case 3:
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, DDR_CONTROL_LOW_REG, (enable << 15), 1 << 15));
            break;


    }
	return GT_OK;
}
/*****************************************************************************
Calculate number of CS
******************************************************************************/
GT_STATUS    mvCalcCsNum
(
    GT_U32          devNum,
	GT_U32          interfaceId,
	GT_U32          *csNum
)
{
	GT_U32 cs;
	GT_U32 busCnt;
	GT_U32 csCount;
	GT_U32 csBitmask;
	GT_U32 currCsNum = 0;
	for(busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
	{
		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
		csCount = 0;
		csBitmask = topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask;
		for(cs = 0; cs < MAX_CS_NUM; cs++) {
			if((csBitmask >> cs) & 1) {
				csCount++;
			}
		}
		if (currCsNum == 0) {
			currCsNum = csCount;
		}
		else if(csCount != currCsNum) {
			DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("CS number is different per bus (IF %d BUS %d csNum %d currCsNum %d)\n",
												  interfaceId, busCnt, csCount, currCsNum));
			return GT_NOT_SUPPORTED;
		}
	}
	*csNum = currCsNum;
	return GT_OK;
}

/*****************************************************************************
Init Controller Flow
******************************************************************************/
GT_STATUS    mvHwsDdr3TipInitController
(
    GT_U32          devNum,
    InitCntrParam   *initCntrPrm
)
{
    GT_U32 interfaceId;
	GT_U32 csNum;
    GT_U32 tREFI = 0, tHCLK = 0, tCKCLK = 0, tFAW = 0,  tPD = 0, tWR = 0, uiT2t = 0, uiTxpdll = 0;
    GT_U32 dataValue = 0, busWidth = 0, pageSize = 0,csCnt = 0, memMask = 0, busIndex = 0;
    MV_HWS_SPEED_BIN  speedBinIndex = SPEED_BIN_DDR_2133N;
    MV_HWS_MEM_SIZE memorySize = MEM_2G;
    MV_HWS_DDR_FREQ freq = initFreq;
    GT_U32 csMask = 0;
    GT_U32 clValue = 0, cwlVal = 0;
    GT_U32 refreshIntervalCnt = 0,  busCnt = 0, adllTap = 0;
    MV_HWS_ACCESS_TYPE	  accessType = ACCESS_TYPE_UNICAST;
     GT_U32 dataRead[MAX_INTERFACE_NUM];

    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("InitController, doMrsPhy=%d, isCtrl64Bit=%d\n", initCntrPrm->doMrsPhy, initCntrPrm->isCtrl64Bit));

    if (initCntrPrm->initPhy == 1)
    {
        CHECK_STATUS(ddr3TipConfigurePhy(devNum));
    }

    if (genericInitController == 1)
    {
        for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
        {
            VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
            DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("active IF %d\n", interfaceId));
            memMask = 0;
            for(busIndex=0; busIndex < GET_TOPOLOGY_NUM_OF_BUSES(devNum) ; busIndex++)
            {
        		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busIndex)
                memMask |= topologyMap->interfaceParams[interfaceId].asBusParams[busIndex].mirrorEnableBitmask;
            }

            if (memMask != 0)
            {
                CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,ACCESS_TYPE_MULTICAST, interfaceId, CS_ENABLE_REG, 0, 0x8));
            }
            memorySize = topologyMap->interfaceParams[interfaceId].memorySize;
            speedBinIndex = topologyMap->interfaceParams[interfaceId].speedBinIndex;
            freq = initFreq;
            tREFI = (topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? TREFI_HIGH:TREFI_LOW;
            tREFI *= 1000; /*psec */
            DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("memySize %d speedBinInd %d freq %d tREFI %d\n", memorySize,speedBinIndex, freq, tREFI));
            /* HCLK & CK CLK in 2:1 [ps]*/
            /* tCKCLK is external clock */
            tCKCLK = (MEGA/freqVal[freq]);
            /* tHCLK is internal clock */
            tHCLK = 2*tCKCLK;
            refreshIntervalCnt = tREFI/tHCLK; /* no units */
 			busWidth = (DDR3_IS_16BIT_DRAM_MODE(topologyMap->activeBusMask) == GT_TRUE)?(16):(32);

            if (initCntrPrm->isCtrl64Bit)
                busWidth = 64;
            dataValue = (refreshIntervalCnt | 0x4000 | ((busWidth == 32) ? 0x8000 : 0) | 0x1000000) & ~(1<<26);
            /* Interface Bus Width */
            /* SRMode */
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_CONFIGURATION_REG, dataValue, 0x100FFFF));

			/* Intrleave first command pre-charge enable (TBD) */
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_OPEN_PAGE_CONTROL_REG, (1 << 10), (1 << 10)));

            /* PHY configuration*/
            /* Postamble Length = 1.5cc, Addresscntl to clk skew \BD, Preamble length normal, parralal ADLL enable*/
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DRAM_PHY_CONFIGURATION, 0x28, 0x3E));
            if (initCntrPrm->isCtrl64Bit)
            {
              /* positive edge */
              CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DRAM_PHY_CONFIGURATION, 0x0, 0xff80));
            }
            
#if !defined(CONFIG_ARMADA_38X) && !defined (CONFIG_ARMADA_39X)
            /*Controller PHY sample stage support*/
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CS_ENABLE_REG, 0x2, 0x3));
#endif
            /*calibration block disable*/
            /* Xbar Read buffer select (for Internal access)*/
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CALIB_MACHINE_CTRL_REG, 0x1200C, 0x7DFFE01C));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CALIB_MACHINE_CTRL_REG, calibrationUpdateControl<<3, 0x3<<3));

            /*Pad calibration control - enable*/
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CALIB_MACHINE_CTRL_REG, 0x1, 0x1));
#if !defined(CONFIG_ARMADA_38X) && !defined(CONFIG_ALLEYCAT3) && !defined (CONFIG_ARMADA_39X)
            /* DDR3_Rank_Control \96 Part of the Generic code */
            /*: CS1 Mirroring enable + w/a for JIRA DUNIT-14581 */ 
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, RANK_CTRL_REG, 0x27, MASK_ALL_BITS));
#endif
            csMask = 0;
            dataValue = 0x7;
            /* Address ctrl \96 Part of the Generic code 
            The next configuration is done: 
            1)  Memory Size
            2) BusWidth
            3) CS#
            4) Page Number
            5) tFAW
            Per Dunit get from the MapTopology the parameters:  BusWidth
            tFAW is per Dunit not per CS*/
            pageSize = (topologyMap->interfaceParams[interfaceId].busWidth == BUS_WIDTH_8) ? pageParam[memorySize].ePageSize_8_BITS : pageParam[memorySize].ePageSize_16_BITS;
            /*uiPageMask = pageParam[memorySize].uiPageMask;*/
            tFAW = (pageSize == 1) ? speedBinTable(speedBinIndex, speedBinTableElements_tFAW1K) : speedBinTable(speedBinIndex, speedBinTableElements_tFAW2K);

#ifdef CONFIG_DDR4
				tFAW = GET_MAX_VALUE(tCKCLK * (pageSize == 1)?(20):(28),tFAW);
#endif

			dataValue = TIME_2_CLOCK_CYCLES(tFAW, tCKCLK);
			dataValue = dataValue << 24;
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_ACCESS_CONTROL_REG, dataValue, 0x7F000000));

            dataValue = (topologyMap->interfaceParams[interfaceId].busWidth == BUS_WIDTH_8) ? 0 : 1;
            /* create merge cs mask for all cs available in dunit */
            for(busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
            {
       			VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
                csMask |= topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask;
            }
            DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("InitController IF %d csMask %d\n",interfaceId, csMask));
            /*Configure the next upon the Map Topology \96 If the Dunit is CS0 Configure CS0 if it is multi CS configure them both:  The BustWidth it\92s the Memory Bus width \96 x8 or x16*/
            for(csCnt = 0; csCnt < NUM_OF_CS; csCnt++)
            {
                ddr3TipConfigureCs(devNum, interfaceId, csCnt,((csMask & (1 << csCnt)) ? 1: 0));
            }

            if (initCntrPrm->doMrsPhy)
            {
	            /* MR0 \96 Part of the Generic code
	            The next configuration is done: 
	            1)  Burst Length
	            2)CAS Latency
	            get for each dunit what is it SpeedBin & Target Frequency. From those both parameters get the appropriate CasL from the CL table*/
	            clValue = topologyMap->interfaceParams[interfaceId].casL;
	            cwlVal = topologyMap->interfaceParams[interfaceId].casWL;
	            DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("clValue 0x%x cwlVal 0x%x \n", clValue, cwlVal));

	            dataValue = ((clMaskTable[clValue] & 0x1) << 2) | ((clMaskTable[clValue] & 0xE)  <<  3);
	            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, MR0_REG, dataValue,(0x7 << 4) | (1 << 2)));
				CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, MR0_REG, twrMaskTable[tWR + 1],0xE00));

				/* MR1: Set RTT and DIC Design GL values configured by user */
				CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, MR1_REG, gDic | gRttNom, 0x266));

	            /* MR2 - Part of the Generic code */
	            /* The next configuration is done: 
	            1)  SRT 
	            2)CAS Write  Latency */
	            dataValue = (cwlMaskTable[cwlVal] << 3);
	            dataValue |= ((topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? (1 << 7) : 0);
	            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR2_REG, dataValue , (0x7<<3) | (0x1<<7) | (0x3<<9)));
            }

			ddr3TipWriteOdt(devNum,  accessType, interfaceId, clValue, cwlVal);
            ddr3TipSetTiming(devNum, accessType, interfaceId, freq);

#if !defined(CONFIG_ARMADA_38X) && !defined(CONFIG_ALLEYCAT3) && !defined (CONFIG_ARMADA_39X)
			/*WrBuff, RdBuff*/
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_CONTROL_HIGH_REG, 0x1000119,0x100017F));
#else
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_CONTROL_HIGH_REG, 0x177,0x1000177));
#endif
		    if (initCntrPrm->isCtrl64Bit)
	      	{
	        /* disable 0.25 cc delay */
    		    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_CONTROL_HIGH_REG, 0x0, 0x800));
		    }
            /* reset bit 7 */
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_CONTROL_HIGH_REG, (initCntrPrm->msysInit << 7) , (1 << 7)));

            if (mode2T != 0xFF)
            {
                uiT2t = mode2T;
            }
            else
            {
				/* calculate number of CS (per interface)*/
				CHECK_STATUS(mvCalcCsNum(devNum, interfaceId, &csNum));
				uiT2t = (csNum == 1) ? 0 : 1;
            }
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DDR_CONTROL_LOW_REG, uiT2t << 3,0x3 << 3));
			/*move the block to ddr3TipSetTiming -start */
			tPD = GET_MAX_VALUE(tCKCLK*3, speedBinTable(speedBinIndex,speedBinTableElements_tPD));
            tPD = TIME_2_CLOCK_CYCLES(tPD, tCKCLK);
            uiTxpdll = GET_MAX_VALUE(tCKCLK*10, 24);
            uiTxpdll = CEIL_DIVIDE((uiTxpdll-1), tCKCLK);
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DDR_TIMING_REG, uiTxpdll<<4,0x1f << 4));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DDR_TIMING_REG, 0x28<<9,0x3f << 9));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DDR_TIMING_REG, 0xA<<21,0xff << 21));
			/*move the block to ddr3TipSetTiming - end*/
			/* AUTO_ZQC_TIMING*/
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, TIMING_REG, (AUTO_ZQC_TIMING | (2<<20)),0x3FFFFF));
			CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, accessType, interfaceId, DRAM_PHY_CONFIGURATION, dataRead, 0x30));
			dataValue = (dataRead[interfaceId] == 0) ? (1 << 11): 0;
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_CONTROL_HIGH_REG, dataValue, (1 << 11)));

			/*Set Active control for ODT write transactions*/
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, 0x1494, uiODTConfig, MASK_ALL_BITS));
#if defined (CONFIG_ALLEYCAT3)
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, 0x14a8, 0x900,0x900));
			/*WA: Controls whether to float The Control pups outputs during Self Refresh*/
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, 0x16d0, 0,0x8000));
#endif
        }
    }
    else
    {
#ifdef STATIC_ALGO_SUPPORT
        CHECK_STATUS(ddr3TipStaticInitController(devNum));
#if defined(CONFIG_ARMADA_38X) || defined (CONFIG_ARMADA_39X)
		CHECK_STATUS(ddr3TipStaticPhyInitController(devNum));
#endif
#endif/*STATIC_ALGO_SUPPORT*/
    }
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        CHECK_STATUS(ddr3TipRankControl(devNum,  interfaceId));

	    if (initCntrPrm->doMrsPhy)
	    {
	      CHECK_STATUS(ddr3TipPadInv(devNum, interfaceId));
	    }
    

    /*Pad calibration control - disable*/
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CALIB_MACHINE_CTRL_REG, 0x0, 0x1));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CALIB_MACHINE_CTRL_REG, calibrationUpdateControl<<3, 0x3<<3));
    }
#ifdef CONFIG_DDR4
	CHECK_STATUS(ddr4TipCalibrationAdjust(devNum, ddr4TipConfigurePhyVrefTap,1,0));/*devNum,VrefTap,Vref_en,POD_Only*/
    ddr4ModeRegsInit(devNum);
    ddr4SdramConfig(devNum);
#endif

    CHECK_STATUS(ddr3TipEnableInitSequence(devNum));

	if (delayEnable != 0) {
		adllTap =  MEGA/(freqVal[freq]*64);
		ddr3TipCmdAddrInitDelay(devNum, adllTap);
	}

    return GT_OK;
}


/*****************************************************************************
Load Topology map 
******************************************************************************/
GT_STATUS    mvHwsDdr3TipLoadTopologyMap
(
    GT_U32                devNum,
    MV_HWS_TOPOLOGY_MAP   *topologyMapPtr 
)
{
    MV_HWS_SPEED_BIN speedBinIndex;
    MV_HWS_DDR_FREQ freq = DDR_FREQ_LIMIT;
    GT_U32 interfaceId = 0;


    freqVal[DDR_FREQ_LOW_FREQ] = dfsLowFreq;
    /*osMemCpy(&topologyMap[devNum], (void*) topology,sizeof(MV_HWS_TOPOLOGY_MAP));*/
    ddr3TipSetTopologyMap(devNum, topologyMapPtr);
    topologyMap = ddr3TipGetTopologyMap(devNum);
    CHECK_STATUS(ddr3TipGetFirstActiveIf((GT_U8)devNum, topologyMap->interfaceActiveMask, &firstActiveIf));
    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("board IF_Mask=0x%x numOfBusPerInterface=0x%x\n",
					   topologyMap->interfaceActiveMask, topologyMap->numOfBusPerInterface));

    /* if CL, CWL values are missing in topology map, then fill them according to speedbin tables */
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        speedBinIndex = topologyMap->interfaceParams[interfaceId].speedBinIndex;
        /* TBD memory frequency of interface 0 only is used ! */
        freq = topologyMap->interfaceParams[firstActiveIf].memoryFreq;

        DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("speedBinIndex =%d freq=%d cl=%d cwl=%d\n",
                           speedBinIndex, freqVal[freq],topologyMap->interfaceParams[interfaceId].casL,
                           topologyMap->interfaceParams[interfaceId].casWL));
        
        if (topologyMap->interfaceParams[interfaceId].casL == 0)
        {
            topologyMap->interfaceParams[interfaceId].casL = casLatencyTable[speedBinIndex].clVal[freq];
        }
        if (topologyMap->interfaceParams[interfaceId].casWL == 0)
        {
            topologyMap->interfaceParams[interfaceId].casWL = casWriteLatencyTable[speedBinIndex].clVal[freq];
        }
    }
    return GT_OK;
}

/*****************************************************************************
RANK Control Flow
******************************************************************************/
#if !defined(CONFIG_ARMADA_38X) &&  !defined (CONFIG_ARMADA_39X) && !defined(CONFIG_ALLEYCAT3)
static GT_STATUS ddr3TipRankControl(GT_U32 devNum, GT_U32 interfaceId)
{
    GT_U32 dataValue = 0,  busCnt= 0;

    for(busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
    {
   		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
        dataValue |= topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask;
      
        if (topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].mirrorEnableBitmask == GT_TRUE)
        { 
        /* checking cs mask is same as cs_bitmask - if CS is enabled than  CS+4 bit in word shall be '1' */
            if ((topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask & 0x1) != 0)
            {
                dataValue |= (1 << 4);
            }
            if ((topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask & 0x2) != 0)
            {
                dataValue |= (1 << 5);
            }
            if ((topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask & 0x4) != 0)
            {
                dataValue |= (1 << 6);
            }
            if ((topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask & 0x8) != 0)
            {
                dataValue |= (1 << 7);
            }
        }
    }
#if !defined(CONFIG_ALLEYCAT3)
    /* jirra CS2 exist */
    dataValue |= (1 << CS2_EXIST_BIT);
#endif
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, RANK_CTRL_REG, dataValue, 0xFF));
     
    return GT_OK;
}
#else
static GT_STATUS ddr3TipRankControl(GT_U32 devNum, GT_U32 interfaceId)
{
    GT_U32 dataValue = 0,  busCnt;
	for (busCnt= 1; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++) {
		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
		if ((topologyMap->interfaceParams[interfaceId].asBusParams[0].csBitmask !=topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].csBitmask) ||
			(topologyMap->interfaceParams[interfaceId].asBusParams[0].mirrorEnableBitmask !=topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].mirrorEnableBitmask))

			DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("WARNING:Wrong configuration for pup #%d CS mask and CS mirroring for all pups should be the same\n",busCnt));
	}
	dataValue |= topologyMap->interfaceParams[interfaceId].asBusParams[0].csBitmask;
	dataValue |= topologyMap->interfaceParams[interfaceId].asBusParams[0].mirrorEnableBitmask << 4;

	CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, RANK_CTRL_REG, dataValue, 0xFF));
return GT_OK;
}
#endif

/*****************************************************************************
PAD Inverse Flow
******************************************************************************/
static GT_STATUS ddr3TipPadInv
(
    GT_U32 devNum, 
    GT_U32 interfaceId
)
{
    GT_U32 busCnt, dataValue, ckSwapPupCtrl;

    for(busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
    {
   		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
        if (topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].isDqsSwap == GT_TRUE)
        {
            /* dqs swap */
            ddr3TipBusReadModifyWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, busCnt, DDR_PHY_DATA, PHY_CONTROL_PHY_REG, 0xC0,0xC0);
        }
        if (topologyMap->interfaceParams[interfaceId].asBusParams[busCnt].isCkSwap == GT_TRUE)
        {
            if(busCnt <= 1)
            {
                dataValue = 0x5 << 2;
            }
            else
            {
                dataValue = 0xA << 2;
            }
            /* mask equals data */
            /* ck swap pup is only control pup #0 ! */
            ckSwapPupCtrl = 0;
            ddr3TipBusReadModifyWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, ckSwapPupCtrl, DDR_PHY_CONTROL, PHY_CONTROL_PHY_REG, dataValue,dataValue);
        }
    }
    return GT_OK;
}

/*****************************************************************************
Run Training Flow
******************************************************************************/
GT_STATUS    mvHwsDdr3TipRunAlg
(
    GT_U32              devNum,
    MV_HWS_ALGO_TYPE    algoType
)
{
    GT_STATUS          retVal = GT_OK , retTune = GT_OK ;

#ifdef ODT_TEST_SUPPORT
   if (fingerTest == 1)
    {
        return odtTest(devNum, algoType);
    }
#endif

    if(algoType == ALGO_TYPE_DYNAMIC)
    {
        retVal = ddr3TipDDR3AutoTune(devNum);
    }
    else
    {
#ifdef STATIC_ALGO_SUPPORT
	{
   		MV_HWS_DDR_FREQ freq;
       	freq = initFreq;

        /* add to mask */
        if (isAdllCalibBeforeInit != 0)
        {
            mvPrintf("with adll calib before init\n");
            AdllCalibration(devNum, ACCESS_TYPE_MULTICAST, 0, freq );
        }
        /* frequency per interface is not relevant, only interface 0 */
        retVal = ddr3TipRunStaticAlg(devNum, freq);
	}
#endif
    }
    if (retVal != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("RunAlg: tuning failed %d\n", retTune));
    }

   return retVal;
}

#ifdef ODT_TEST_SUPPORT
/*****************************************************************************
ODT Test
******************************************************************************/
static GT_STATUS odtTest
(
    GT_U32              devNum,
    MV_HWS_ALGO_TYPE    algoType
)
{
    GT_STATUS          retVal = GT_OK , retTune = GT_OK ;
    GT_STATUS pfingerVal = 0, nfingerVal;

    for (pfingerVal = pFingerStart; pfingerVal <= pFingerEnd; pfingerVal+=pFingerStep)
    {
        for (nfingerVal = nFingerStart; nfingerVal <= nFingerEnd; nfingerVal+=nFingerStep)
        {
            if (fingerTest != 0)
            {
                DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("pfingerVal %d nfingerVal %d\n", pfingerVal, nfingerVal));
                Pfinger = pfingerVal;
                Nfinger = nfingerVal;
            }
            if(algoType == ALGO_TYPE_DYNAMIC)
            {
                retVal = ddr3TipDDR3AutoTune(devNum);
            }
            else
            {
                /* frequency per interface is not relevant, only interface 0 */
                retVal = ddr3TipRunStaticAlg(devNum, initFreq);
            }
        }
    }
    if (retTune != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("RunAlg: tuning failed %d\n", retTune));
        retVal = (retVal == GT_OK) ? retTune : retVal;
    }
   return retVal;
}
#endif

/*****************************************************************************
Select Controller
******************************************************************************/
GT_STATUS    mvHwsDdr3TipSelectDdrController
(
    GT_U32   devNum,
    GT_BOOL  enable
)
{
    if (configFuncInfo[devNum].tipDunitMuxSelectFunc != NULL)
    {
        return configFuncInfo[devNum].tipDunitMuxSelectFunc((GT_U8)devNum, enable);
    }
    return GT_FAIL;
}


/*****************************************************************************
Dunit Register Write
******************************************************************************/
GT_STATUS    mvHwsDdr3TipIFWrite
(
    GT_U32                devNum,
    MV_HWS_ACCESS_TYPE    interfaceAccess,
    GT_U32                interfaceId,
    GT_U32                regAddr,
    GT_U32                dataValue,
    GT_U32                mask
)
{
    if (configFuncInfo[devNum].tipDunitWriteFunc != NULL)
    {
        return configFuncInfo[devNum].tipDunitWriteFunc((GT_U8)devNum, interfaceAccess, interfaceId, regAddr, dataValue, mask);
    }
    return GT_FAIL;
}



/*****************************************************************************
Dunit Register Read
******************************************************************************/
GT_STATUS    mvHwsDdr3TipIFRead
(
    GT_U32                devNum,
    MV_HWS_ACCESS_TYPE    interfaceAccess,
    GT_U32                interfaceId,
    GT_U32                regAddr,
    GT_U32                *data,
    GT_U32                mask
)
{
    if (configFuncInfo[devNum].tipDunitReadFunc != NULL)
    {
        return configFuncInfo[devNum].tipDunitReadFunc((GT_U8)devNum, interfaceAccess, interfaceId, regAddr, data, mask);
    }
    return GT_FAIL;
}

/*****************************************************************************
Dunit Register Polling
******************************************************************************/
GT_STATUS    ddr3TipIfPolling
(
    GT_U32                devNum,
    MV_HWS_ACCESS_TYPE	  accessType,
    GT_U32                interfaceId,
    GT_U32                expValue,
    GT_U32                mask,
    GT_U32                offset,
    GT_U32                pollTries
)
{
    GT_U32 pollCnt = 0, interfaceNum = 0, startIf, endIf;
    GT_U32 readData[MAX_INTERFACE_NUM];
    GT_STATUS retVal;
    GT_BOOL isFail = GT_FALSE, isIfFail;

    if (accessType == ACCESS_TYPE_MULTICAST)
    {
        startIf = 0;
        endIf = MAX_INTERFACE_NUM-1;
    }
    else
    {
        startIf = interfaceId;
        endIf = interfaceId;
    }
    for(interfaceNum = startIf; interfaceNum <= endIf; interfaceNum++)
    {
        /* polling bit 3 for n times */ 
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceNum)

        isIfFail = GT_FALSE;
        /*result[interfaceNum] = TEST_FAILED;*/
        for(pollCnt = 0; pollCnt < pollTries ; pollCnt++)
        {
            retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_UNICAST, interfaceNum, offset, readData, mask);
            if (retVal != GT_OK)
            {
                return retVal;
            }
#ifdef ASIC_SIMULATION
			/* avoid polling failure in simulation */
			readData[interfaceNum] = expValue;
#endif
            if(readData[interfaceNum] == expValue)
            {
                /*result[interfaceNum] = TEST_SUCCESS;*/
                /*isIfFail = GT_FALSE;*/
                break;
            }
        }
        if(pollCnt >= pollTries)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("max poll IF #%d\n", interfaceNum));
            isFail = GT_TRUE;
            isIfFail = GT_TRUE;
        }

        trainingResult[trainingStage][interfaceNum] = (isIfFail == GT_TRUE) ? TEST_FAILED : TEST_SUCCESS;
   }

   return (isFail == GT_FALSE)? GT_OK:GT_FAIL;
}

/*****************************************************************************
Bus read access
******************************************************************************/
GT_STATUS    mvHwsDdr3TipBUSRead
(
    GT_U32              devNum,
    GT_U32              interfaceId, 
    MV_HWS_ACCESS_TYPE  phyAccess,
    GT_U32              phyId, 
    MV_HWS_DDR_PHY      phyType, 
    GT_U32              regAddr, 
    GT_U32              *data 
)
{
    GT_U32 busIndex = 0;
    GT_U32 dataRead[MAX_INTERFACE_NUM];

	if (phyAccess == ACCESS_TYPE_MULTICAST)
	{
		for(busIndex=0; busIndex < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busIndex++)
		{
       		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busIndex)
			CHECK_STATUS(ddr3TipBusAccess(devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, busIndex, phyType , regAddr, 0, Operation_READ));
			CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_UNICAST, interfaceId, PHY_REG_FILE_ACCESS,  dataRead,  MASK_ALL_BITS));
			data[busIndex] = (dataRead[interfaceId] & 0xFFFF);
		}         
    }
    else
    {
		CHECK_STATUS(ddr3TipBusAccess(devNum, ACCESS_TYPE_UNICAST, interfaceId, phyAccess, phyId, phyType , regAddr, 0, Operation_READ));
		CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_UNICAST, interfaceId, PHY_REG_FILE_ACCESS, dataRead, MASK_ALL_BITS));

		/* only 16 lsb bit are valid in Phy (each register is different, some can actually be less than 16 bits)*/
		*data = (dataRead[interfaceId] & 0xFFFF);
    }
    return GT_OK;
}

/*****************************************************************************
Bus write access
******************************************************************************/
GT_STATUS    mvHwsDdr3TipBUSWrite
( 
    GT_U32                devNum,
    MV_HWS_ACCESS_TYPE    interfaceAccess,
    GT_U32                interfaceId, 
    MV_HWS_ACCESS_TYPE    phyAccess,
    GT_U32                phyId, 
    MV_HWS_DDR_PHY        phyType, 
    GT_U32                regAddr, 
    GT_U32                dataValue 
)
{
    CHECK_STATUS(ddr3TipBusAccess(devNum, interfaceAccess, interfaceId, phyAccess, phyId, phyType , regAddr, dataValue, Operation_WRITE));
    return GT_OK;
}

/*****************************************************************************
Bus access routine (relevant for both read & write)
******************************************************************************/
static GT_STATUS    ddr3TipBusAccess
( 
    GT_U32              devNum,
    MV_HWS_ACCESS_TYPE  interfaceAccess,
    GT_U32              interfaceId, 
    MV_HWS_ACCESS_TYPE  phyAccess,
    GT_U32              phyId, 
    MV_HWS_DDR_PHY      phyType, 
    GT_U32              regAddr,
    GT_U32              dataValue,
    MV_HWS_Operation    operType
)
{
    GT_U32 addrLow = 0x3F & regAddr;
    GT_U32 addrHi = ((0xC0 & regAddr) >> 6) ;
    GT_U32 dataP1 = (operType << 30) + (addrHi << 28) + (phyAccess << 27)  + (phyType << 26) + (phyId << 22) + (addrLow << 16) + (dataValue & 0xFFFF);
    GT_U32 dataP2 = dataP1 + (1 << 31);
    GT_U32 startIf, endIf;

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, interfaceAccess, interfaceId, PHY_REG_FILE_ACCESS, dataP1, MASK_ALL_BITS));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, interfaceAccess, interfaceId, PHY_REG_FILE_ACCESS, dataP2, MASK_ALL_BITS));
    if (interfaceAccess == ACCESS_TYPE_UNICAST)
    {
        startIf = interfaceId;
        endIf = interfaceId;
    }
    else
    {
        startIf = 0;
        endIf = MAX_INTERFACE_NUM-1;
    }
    /* polling for read/write execution done */
    for(interfaceId = startIf; interfaceId <= endIf; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        CHECK_STATUS(isBusAccessDone(devNum, interfaceId, PHY_REG_FILE_ACCESS, 31));
    }
    return GT_OK;
}

/*****************************************************************************
Check bus access done
******************************************************************************/
static GT_STATUS isBusAccessDone
(
    GT_U32 devNum,
    GT_U32 interfaceId,
    GT_U32 dunitRegAdrr,
    GT_U32 bit
)
{
    GT_U32 rdData = 1;
    GT_U32 cnt = 0;
    GT_U32 dataRead[MAX_INTERFACE_NUM];

    CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_UNICAST, interfaceId, dunitRegAdrr, dataRead, MASK_ALL_BITS));
    rdData = dataRead[interfaceId];
    rdData &= (1 << bit);

    while(rdData != 0)
    {
        if (cnt++ >= MAX_POLLING_ITERATIONS)
            break;
        CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_UNICAST, interfaceId, dunitRegAdrr, dataRead, MASK_ALL_BITS));
		rdData = dataRead[interfaceId];
        rdData &= (1 << bit);
    }
    if (cnt < MAX_POLLING_ITERATIONS)
    {
        return GT_OK;
    }
    else
    {
        return GT_FAIL;
    }
}

/*****************************************************************************
Phy read-modify-write
******************************************************************************/
GT_STATUS    ddr3TipBusReadModifyWrite
(
    GT_U32          devNum,
    MV_HWS_ACCESS_TYPE		accessType,
    GT_U32          interfaceId, 
    GT_U32          phyId, 
    MV_HWS_DDR_PHY  phyType, 
    GT_U32          regAddr, 
    GT_U32          dataValue,
    GT_U32          regMask 
)
{
    GT_U32 dataVal = 0, ifId, startIf, endIf;
    if (accessType == ACCESS_TYPE_MULTICAST)
    {
        startIf = 0;
        endIf = MAX_INTERFACE_NUM - 1;
    }  
    else
    {
        startIf = interfaceId;
        endIf = interfaceId;
    }

    for(ifId = startIf; ifId <= endIf; ifId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, ifId)
        CHECK_STATUS(mvHwsDdr3TipBUSRead(   devNum, ifId, ACCESS_TYPE_UNICAST, phyId,  phyType,  regAddr, &dataVal));
        dataValue = (dataVal & (~regMask)) | (dataValue & regMask);
        CHECK_STATUS(mvHwsDdr3TipBUSWrite(  devNum, ACCESS_TYPE_UNICAST, ifId, ACCESS_TYPE_UNICAST,phyId,phyType,regAddr, dataValue));
    }
    return GT_OK;
}

/*****************************************************************************
ADLL Calibration
******************************************************************************/
GT_STATUS AdllCalibration
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE		accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency
)
{
    MV_HWS_TIP_FREQ_CONFIG_INFO		freqConfigInfo;
    GT_U32 busCnt = 0;

    /* Reset Diver_b assert -> de-assert*/
    CHECK_STATUS (mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0, 0x10000000));
    CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum, 10));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0x10000000, 0x10000000));

    if (configFuncInfo[devNum].tipGetFreqConfigInfoFunc != NULL)
    {
        CHECK_STATUS(configFuncInfo[devNum].tipGetFreqConfigInfoFunc((GT_U8)devNum, frequency, &freqConfigInfo));
    }
	else
	{
		DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("tipGetFreqConfigInfoFunc is NULL"));
		return GT_NOT_INITIALIZED;
	}
	for (busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
    {
   		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
        CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum, accessType, interfaceId,  busCnt, DDR_PHY_DATA, BW_PHY_REG,   freqConfigInfo.bwPerFreq << 8 /*freqMask[devNum][frequency] << 8*/, 0x700));
        CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum, accessType, interfaceId,  busCnt, DDR_PHY_DATA,  RATE_PHY_REG,  freqConfigInfo.ratePerFreq , 0x7));
    }
#if !defined(CONFIG_ARMADA_38X) && !defined(CONFIG_ARMADA_39X)
    for (busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
    {
        CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId,  busCnt, DDR_PHY_CONTROL, BW_PHY_REG,   freqConfigInfo.bwPerFreq << 8 /*freqMask[devNum][frequency] << 8*/, 0x700));
        CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum, ACCESS_TYPE_UNICAST,  interfaceId,  busCnt, DDR_PHY_CONTROL,  RATE_PHY_REG,  freqConfigInfo.ratePerFreq , 0x7));
    }
#endif

    /* DUnit to Phy drive post edge, ADLL reset  assert  de-assert*/ 
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DRAM_PHY_CONFIGURATION, 0, (0x80000000  | 0x40000000)));
    CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, 0, 100/(freqVal[frequency]/freqVal[DDR_FREQ_LOW_FREQ])));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DRAM_PHY_CONFIGURATION, (0x80000000  | 0x40000000), (0x80000000  | 0x40000000)));

    /*polling for ADLL Done */
    if (ddr3TipIfPolling(devNum,accessType, interfaceId, 0x3FF03FF, 0x3FF03FF,PHY_LOCK_STATUS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(1)"));
    }

    /* pup data_pup reset assert-> deassert */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0, 0x60000000));
    CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum,  10)); 
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0x60000000, 0x60000000));
    return GT_OK;
}

#if !defined(CONFIG_ARMADA_38X) && !defined (CONFIG_ARMADA_39X)
/*****************************************************************************
DFS - set frequency
******************************************************************************/
GT_STATUS    ddr3TipFreqSet
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE		accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency 
)
{
    GT_U32 clValue = 0, cwlValue = 0, memMask = 0, dataValue = 0, tHCLK = 0, tWR = 0, refreshIntervalCnt = 0, cntId, interfaceIdx = 0;
	GT_U32 startIf, endIf;
    GT_U32 tREFI = 0;
    GT_BOOL isDllOff = GT_FALSE;
    GT_U32 busIndex = 0, adllTap = 0;
    MV_HWS_SPEED_BIN      speedBinIndex = 0;
	GT_U32   csMask[MAX_INTERFACE_NUM];

    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("dev %d access %d IF %d freq %d\n",devNum , accessType , interfaceId , frequency));

    if(frequency == DDR_FREQ_LOW_FREQ)
    {
        isDllOff = GT_TRUE;
    }

	if (accessType == ACCESS_TYPE_MULTICAST)
    {
        startIf = 0;
        endIf = MAX_INTERFACE_NUM-1;
    }
    else
    {
        startIf = interfaceId;
        endIf = interfaceId;
    }

	/* calculate interface cs mask - Oferb 4/11*/
    /* speed bin can be different for each interface */
    for(interfaceIdx = 0; interfaceIdx <= MAX_INTERFACE_NUM-1; interfaceIdx++)
    {
		/* cs enable is active low */
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceIdx)
        csMask[interfaceId] = CS_BIT_MASK;
        trainingResult[trainingStage][interfaceIdx] = TEST_SUCCESS;

		ddr3TipCalcCsMask( devNum, interfaceId, effective_cs, &csMask[interfaceId]);
    }
    /* assumption: all frequency & speed bin for all interface is identical ! */
    speedBinIndex = topologyMap->interfaceParams[interfaceId].speedBinIndex;
    /* TBD memory frequency of interface 0 only is used ! */
	if(topologyMap->interfaceParams[firstActiveIf].memoryFreq == frequency)
    {
        clValue = topologyMap->interfaceParams[firstActiveIf].casL;
        cwlValue = topologyMap->interfaceParams[firstActiveIf].casWL;
    }
    else
    { 
        clValue = casLatencyTable[speedBinIndex].clVal[frequency];
        cwlValue = casWriteLatencyTable[speedBinIndex].clVal[frequency];
    }

    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("FreqSet dev 0x%x access 0x%x if 0x%x freq 0x%x speed %d:\n\t", 
					    devNum,accessType ,interfaceId, frequency, speedBinIndex));
    for(cntId=0; cntId<DDR_FREQ_LIMIT; cntId++)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("%d ", casLatencyTable[speedBinIndex].clVal[cntId]));
    }
    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("\n"));
    memMask = 0;
	/*moti TBD - need to insert loop on interface*/
    for(busIndex=0; busIndex < GET_TOPOLOGY_NUM_OF_BUSES(devNum) ; busIndex++)
    {
   		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busIndex)
        memMask |= topologyMap->interfaceParams[interfaceId].asBusParams[busIndex].mirrorEnableBitmask;
    }
    if (memMask != 0)
    {
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, CS_ENABLE_REG, 0, 0x8));
    }

    CHECK_STATUS (mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DUNIT_MMASK_REG, 0, 0x1));
    /*DFS  - CL/CWL parameters after exiting SR*/

    /*DFS  - Enter Self-Refresh*/
    tWR = speedBinTable(speedBinIndex,speedBinTableElements_tWR);
    tWR = (tWR / 1000);
    /* dataValue = (clMaskTable[clValue] << 8) |  (cwlMaskTable[cwlValue] << 12) | (1 << 1) | (1 << 2) | (twrMaskTable[tWR+1] << 16);*/
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DFS_REG, (clMaskTable[clValue] << 8) , 0xF00));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DFS_REG, (cwlMaskTable[cwlValue] << 12) , 0x7000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DFS_REG, (twrMaskTable[tWR+1] << 16) , 0x70000));

    if(isDllOff == GT_TRUE)
    {
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DFS_REG, 0x1 , 0x1)); /* dll off */
    }
    else
    {
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, DFS_REG, 0 , 0x1));
    }
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0x2 , 0x2));
	/*Oferb - 31/10 disable RTT_nom and RTT_WR(only when DLL off mode*/
	if(isDllOff == GT_TRUE)
    {
#ifdef CONFIG_DDR3
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1874, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1884, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1894, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x18a4, 0 , 0x244));
#else /* CONFIG_DDR4 */
            CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, accessType,  PARAM_NOT_CARE,  0x1974,  &gRttNomCS0,  MASK_ALL_BITS));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1974, 0 , (0x7 << 8)));
            CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, accessType,  PARAM_NOT_CARE,  0x1A74,  &gRttNomCS1,  MASK_ALL_BITS));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1A74, 0 , (0x7 << 8)));
#endif
    }
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0x4 , 0x4));
	/*Oferb - 31/10 polling - enter Self refresh*/
	if (ddr3TipIfPolling(devNum,accessType, interfaceId, 0x8, 0x8,DFS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(1)\n"));
    }
	/*moti TBD - need to insert loop on interface*/
    tREFI = (topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? TREFI_HIGH:TREFI_LOW;
    tREFI *= 1000; /*psec */
    /* HCLK in [ps] */
    tHCLK = MEGA/(freqVal[frequency]/2);
    refreshIntervalCnt = tREFI/tHCLK; /* no units */

    dataValue = 0x4000  | refreshIntervalCnt;
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, dataValue, 0x7FFF));

    /* PLL configuration */
    if (configFuncInfo[devNum].tipSetFreqDividerFunc != NULL)
    {
		/* Ofer b 5/11- assert ADLL */
		CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0, 0x60000000));
		/* configure pll devider*/
		for(interfaceIdx = startIf; interfaceIdx <= endIf; interfaceIdx++)
		{
			VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceIdx)
			configFuncInfo[devNum].tipSetFreqDividerFunc((GT_U8)devNum, interfaceIdx, frequency);
		}
    }

    /* PLL configuration End */
    CHECK_STATUS(AdllCalibration(devNum, accessType, interfaceId, frequency ));

	/* Oferb - 31/10, restoring the RTT values if in DLL off mode*/
	if(isDllOff == GT_TRUE)
    {
#ifdef CONFIG_DDR3
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1874, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1884, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1894, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x18a4, gDic | gRttNom, 0x266));
#else /* CONFIG_DDR4 */
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1974, gRttNomCS0 , (0x7 << 8)));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1A74, gRttNomCS1 , (0x7 << 8)));
#endif
    }

    ddr3TipSetTiming(devNum, accessType, interfaceId, frequency);

	if (delayEnable != 0)
	{
		adllTap =  MEGA/(freqVal[frequency]*64);
		ddr3TipCmdAddrInitDelay(devNum, adllTap);
	}

	/*///////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /* Exit SR */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0, 0x4));
    if (ddr3TipIfPolling(devNum, accessType, interfaceId, 0, 0x8, DFS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(2)\n"));
    }


    /* Refresh Command */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_OPERATION_REG, 0x2, 0xF1F));
    if (ddr3TipIfPolling(devNum, accessType, interfaceId,  0, 0x1f, SDRAM_OPERATION_REG, MAX_POLLING_ITERATIONS) != GT_OK)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(3)\n"));
    }


    /* Release DFS Block */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0, 0x2));
    /* Controller to MBUS Retry \96 normal */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DUNIT_MMASK_REG, 0x1, 0x1));
    /* MRO: Burst Length 8, CL , AutoPrecharge 0x16cc */      
    dataValue = ((clMaskTable[clValue] & 0x1) << 2) | ((clMaskTable[clValue] & 0xE)  <<  3);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR0_REG, dataValue, (0x7 << 4) | (1 << 2)));
    /*MR2:  CWL = 10 , Auto Self-Refresh - disable */
    dataValue = (cwlMaskTable[cwlValue] << 3);
    /*dataValue |= (1 << 9); removed by Ofer 31/10
    dataValue |= ((topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? (1 << 7) : 0);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR2_REG, dataValue , (0x7<<3) | (0x1<<7) | (0x3<<9)));*/

	CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR2_REG, dataValue , (0x7<<3)));

    ddr3TipWriteOdt(devNum, accessType, interfaceId, clValue, cwlValue);

	CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum, 5));
    /* re-write CL*/
    dataValue = ((clMaskTable[clValue] & 0x1) << 2) | ((clMaskTable[clValue] & 0xE)  <<  3);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR0_REG, dataValue, (0x7 << 4) | (1 << 2)));

    /* re-write CWL */
    dataValue = (cwlMaskTable[cwlValue] << 3);
    CHECK_STATUS(ddr3TipWriteMRSCmd(devNum, csMask, MRS2_CMD, dataValue, (0x7 << 3) ));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR2_REG, dataValue, (0x7 << 3)));

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0xC00, 0xF00)); /* CS0 & CS1*/
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0x3, 0x1F));     /* MR0 Update Command */
    CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum, 10));

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0x8, 0x1F));     /* MR2 Update Command */
    CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum, 10));

    if (memMask != 0)
    {
        /*Disable MBus Retry */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, CS_ENABLE_REG, 1<<3, 0x8));
    }  
 	
    return GT_OK;
}
#else
GT_STATUS    ddr3TipFreqSet
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE		accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency 
)
{
    GT_U32 clValue = 0, cwlValue = 0, memMask = 0, dataValue = 0, busCnt = 0, tHCLK = 0, tWR = 0, refreshIntervalCnt = 0, cntId;
    GT_U32 tREFI = 0, endIf, startIf;
    GT_U32 busIndex = 0;
	GT_BOOL isDllOff = GT_FALSE;
    MV_HWS_SPEED_BIN      speedBinIndex = 0;
    MV_HWS_TOPOLOGY_MAP *topologyMap = ddr3TipGetTopologyMap(devNum);
    MV_HWS_TIP_FREQ_CONFIG_INFO		freqConfigInfo;
    MV_HWS_RESULT* flowResult = trainingResult[trainingStage];
	GT_U32 adllTap = 0;
	GT_U32   csMask[MAX_INTERFACE_NUM];

    DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("dev %d access %d IF %d freq %d\n",devNum , accessType , interfaceId , frequency));
	if (frequency == DDR_FREQ_LOW_FREQ) {
		isDllOff = GT_TRUE;
	}
    if (accessType == ACCESS_TYPE_MULTICAST)
    {
        startIf = 0;
        endIf = MAX_INTERFACE_NUM - 1;
    }  
    else
    {
        startIf = interfaceId;
        endIf = interfaceId;
    }

	/* calculate interface cs mask - Oferb 4/11*/
    /* speed bin can be different for each interface */
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
		/* cs enable is active low */
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        csMask[interfaceId] = CS_BIT_MASK;
        trainingResult[trainingStage][interfaceId] = TEST_SUCCESS;
		ddr3TipCalcCsMask( devNum, interfaceId, effective_cs, &csMask[interfaceId]);
    }

    /* speed bin can be different for each interface */
	/* moti b - need to remove the loop for multicas access functions and loop the unicast access functions*/
    for(interfaceId =  startIf; interfaceId <= endIf; interfaceId++)
    {
        if (IS_INTERFACE_ACTIVE(topologyMap->interfaceActiveMask, interfaceId) ==  GT_FALSE)
        {
            continue;
        }
        flowResult[interfaceId] = TEST_SUCCESS;
        speedBinIndex = topologyMap->interfaceParams[interfaceId].speedBinIndex;
	    if(topologyMap->interfaceParams[interfaceId].memoryFreq == frequency)
        {
            clValue = topologyMap->interfaceParams[interfaceId].casL;
            cwlValue = topologyMap->interfaceParams[interfaceId].casWL;
        }
        else
        { 
            clValue = casLatencyTable[speedBinIndex].clVal[frequency];
            cwlValue = casWriteLatencyTable[speedBinIndex].clVal[frequency];
        }

        DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("FreqSet dev 0x%x access 0x%x if 0x%x freq 0x%x speed %d:\n\t", 
					      devNum,accessType , interfaceId, frequency, speedBinIndex));
        for(cntId=0; cntId<DDR_FREQ_LIMIT; cntId++)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("%d ",casLatencyTable[speedBinIndex].clVal[cntId]));
        }
        DEBUG_TRAINING_IP(DEBUG_LEVEL_TRACE, ("\n"));
        memMask = 0;
        for(busIndex=0; busIndex < GET_TOPOLOGY_NUM_OF_BUSES(devNum) ; busIndex++)
        {
       		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busIndex)
            memMask |= topologyMap->interfaceParams[interfaceId].asBusParams[busIndex].mirrorEnableBitmask;
        }
        if (memMask != 0)
        {
             CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, CS_ENABLE_REG, 0, 0x8)); /* motib redundent in KW28*/
        }

		/* dll state after exiting SR*/
        if (isDllOff == GT_TRUE) {
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0x1 , 0x1));
		}
		else
		{
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0 , 0x1));
		}

        CHECK_STATUS (mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DUNIT_MMASK_REG, 0, 0x1)); 
       /*DFS  - block  transactions*/
		CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0x2 , 0x2));

		/* disable ODT in case of dll off*/
        if (isDllOff == GT_TRUE) {
#ifdef CONFIG_DDR3
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1874, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1884, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1894, 0 , 0x244));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x18a4, 0 , 0x244));
#else /* CONFIG_DDR4 */
            CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, accessType,  PARAM_NOT_CARE,  0x1974,  &gRttNomCS0,  MASK_ALL_BITS));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1974, 0 , (0x7 << 8)));
            CHECK_STATUS(mvHwsDdr3TipIFRead(devNum, accessType,  PARAM_NOT_CARE,  0x1A74,  &gRttNomCS1,  MASK_ALL_BITS));
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1A74, 0 , (0x7 << 8)));
#endif
		}
           
        /*DFS  - Enter Self-Refresh*/
		CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0x4 , 0x4));
		/* polling on self refresh entry */
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId, 0x8, 0x8, DFS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed on SR entry\n"));
        }

          /* PLL configuration */
        if (configFuncInfo[devNum].tipSetFreqDividerFunc != NULL)
        {
            configFuncInfo[devNum].tipSetFreqDividerFunc(devNum, interfaceId, frequency);
        }
        /* PLL configuration End */

		/* adjust tREFI to new frequency*/
		tREFI = (topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? TREFI_LOW:TREFI_HIGH;
        tREFI *= 1000; /*psec */
        /* HCLK in [ps] */
        tHCLK = MEGA/(freqVal[frequency]/2);
        refreshIntervalCnt = tREFI/tHCLK; /* no units */
        dataValue = 0x4000  | refreshIntervalCnt;
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, dataValue, 0x7FFF));


        /*DFS  - CL/CWL/WR parameters after exiting SR*/
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, (clMaskTable[clValue] << 8) , 0xF00)); 
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, (cwlMaskTable[cwlValue] << 12) , 0x7000));
        tWR = speedBinTable(speedBinIndex,speedBinTableElements_tWR);
        tWR = (tWR / 1000);
       /* dataValue = (clMaskTable[clValue] << 8) |  (cwlMaskTable[cwlValue] << 12) | (1 << 1) | (1 << 2) | (twrMaskTable[tWR+1] << 16);*/
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, (twrMaskTable[tWR+1] << 16) , 0x70000));
	
        /* Restore original RTT values if returning from DLL OFF mode*/
        if(isDllOff == GT_TRUE) {
#ifdef CONFIG_DDR3
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1874, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1884, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1894, gDic | gRttNom, 0x266));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x18a4, gDic | gRttNom, 0x266));
#else /* CONFIG_DDR4 */
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1974, gRttNomCS0 , (0x7 << 8)));
			CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, 0x1A74, gRttNomCS1 , (0x7 << 8)));
#endif
		}
        /* Reset Diver_b assert -> de-assert*/ 
        CHECK_STATUS (mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0, 0x10000000));
        CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum, 10));  
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0x10000000, 0x10000000));

        /* Adll configuration function of process and Frequency*/
        if (configFuncInfo[devNum].tipGetFreqConfigInfoFunc != NULL)
        {
			CHECK_STATUS(configFuncInfo[devNum].tipGetFreqConfigInfoFunc(devNum, frequency, &freqConfigInfo));
        }
        /* TBD check milo5 using device ID ? */
        for (busCnt = 0; busCnt < GET_TOPOLOGY_NUM_OF_BUSES(devNum); busCnt++)
        {
        	VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busCnt)
             CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum,ACCESS_TYPE_UNICAST, interfaceId,  busCnt, DDR_PHY_DATA, 0x92,   freqConfigInfo.bwPerFreq << 8 /*freqMask[devNum][frequency] << 8*/, 0x700));
             CHECK_STATUS(ddr3TipBusReadModifyWrite(devNum,ACCESS_TYPE_UNICAST,  interfaceId,  busCnt, DDR_PHY_DATA,  0x94,  freqConfigInfo.ratePerFreq , 0x7));
        }
        /* DUnit to Phy drive post edge, ADLL reset  assert  de-assert*/ 
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DRAM_PHY_CONFIGURATION, 0, (0x80000000  | 0x40000000)));
        CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, 0, 100/(freqVal[frequency]/freqVal[DDR_FREQ_LOW_FREQ])));
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DRAM_PHY_CONFIGURATION, (0x80000000  | 0x40000000), (0x80000000  | 0x40000000)));
        /*polling for ADLL Done */
        if (ddr3TipIfPolling(devNum,ACCESS_TYPE_UNICAST, interfaceId, 0x3FF03FF,0x3FF03FF,PHY_LOCK_STATUS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(1)\n"));
        }
        /* pup data_pup reset assert-> deassert */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0, 0x60000000));
        CHECK_STATUS(hwsOsExactDelayPtr((GT_U8)devNum, devNum,  10)); 
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_CONFIGURATION_REG, 0x60000000, 0x60000000));

		/*Set proper timing params before existing Self-Refresh*/
		ddr3TipSetTiming(devNum, accessType, interfaceId, frequency);
		if (delayEnable != 0)
		{
			adllTap =  MEGA/(freqVal[frequency]*64);
			ddr3TipCmdAddrInitDelay(devNum, adllTap);
		}

        /* Exit SR */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0, 0x4));
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId, 0, 0x8, DFS_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(2)"));
        }
        /* Refresh Command */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_OPERATION_REG, 0x2, 0xF1F));
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId,  0, 0x1f, SDRAM_OPERATION_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("FreqSet: DDR3 poll failed(3)"));
        }
        /* Release DFS Block */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DFS_REG, 0, 0x2));
        /* Controller to MBUS Retry - normal */
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DUNIT_MMASK_REG, 0x1, 0x1));
        /* MRO: Burst Length 8, CL , AutoPrecharge 0x16cc */      
        dataValue = ((clMaskTable[clValue] & 0x1) << 2) | ((clMaskTable[clValue] & 0xE)  <<  3);
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR0_REG, dataValue, (0x7 << 4) | (1 << 2)));
        /*MR2:  CWL = 10 , Auto Self-Refresh - disable */
        dataValue = (cwlMaskTable[cwlValue] << 3);
		/* nklein 24.10.13 - should not be here - leave value as set in
		the init configuration dataValue |= (1 << 9); 
        dataValue |= ((topologyMap->interfaceParams[interfaceId].interfaceTemp == MV_HWS_TEMP_HIGH) ? (1 << 7) : 0);
        ****/
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, MR2_REG, dataValue , (0x7<<3)));  /* nklein 24.10.13 - see above comment*/
        /*ODT TIMING */
        dataValue = ((clValue-cwlValue+1) << 4) |  ((clValue-cwlValue+6) << 8) |  ((clValue-1) << 12) |  ((clValue+6) << 16);
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, ODT_TIMING_LOW, dataValue, 0xFFFF0));
        dataValue = 0x71 | ((cwlValue - 1) << 8) | ((cwlValue+5) << 12);
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, ODT_TIMING_HI_REG, dataValue, 0xFFFF));
        /* ODT Active*/
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DUNIT_ODT_CONTROL_REG, 0xF, 0xF));

    /* re-write CL*/
    dataValue = ((clMaskTable[clValue] & 0x1) << 2) | ((clMaskTable[clValue] & 0xE)  <<  3);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR0_REG, dataValue, (0x7 << 4) | (1 << 2)));

    /* re-write CWL */
    dataValue = (cwlMaskTable[cwlValue] << 3);
    CHECK_STATUS(ddr3TipWriteMRSCmd(devNum, csMask, MRS2_CMD, dataValue, (0x7 << 3) ));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR2_REG, dataValue, (0x7 << 3)));

    /*    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x3E031F80, 0x3FFFFFFF));*/
        if (memMask != 0)
        {
            CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, CS_ENABLE_REG, 1<<3, 0x8));
        }
    }
    return GT_OK;
}

#endif

/*****************************************************************************
Set ODT values
******************************************************************************/
static GT_STATUS ddr3TipWriteOdt
(
    GT_U32			        devNum,
    MV_HWS_ACCESS_TYPE	    accessType,
    GT_U32					interfaceId,
    GT_U32                  clValue,
    GT_U32                  cwlValue
)
{
    /*ODT TIMING */
    GT_U32 dataValue = (clValue-cwlValue+6) ;
    dataValue = ((clValue-cwlValue+1) << 4) |  ((dataValue & 0xf) << 8) |  (((clValue-1)&0xF) << 12) |  (((clValue+6)&0xF) << 16) | (((dataValue & 0x10) >> 4) << 21);

	dataValue |= (((clValue-1)>>4) << 22) |  (((clValue+6)>>4) << 23);

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, ODT_TIMING_LOW, dataValue, 0xFFFF0));
    dataValue = 0x71 | ((cwlValue - 1) << 8) | ((cwlValue+5) << 12);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, ODT_TIMING_HI_REG, dataValue, 0xFFFF));
	if (odtAdditional == 1) 
	{
	    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, SDRAM_ODT_CONTROL_HIGH_REG, 0xF, 0xF));
	}
	/* ODT Active*/
	CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,accessType, interfaceId, DUNIT_ODT_CONTROL_REG, 0xF, 0xF));
    return GT_OK;
}

/*****************************************************************************
Set Timing values for training
******************************************************************************/
static GT_STATUS ddr3TipSetTiming
(
    GT_U32					devNum,
    MV_HWS_ACCESS_TYPE	    accessType,
    GT_U32					interfaceId,
    MV_HWS_DDR_FREQ			frequency 
)
{
    GT_U32 tCKCLK = 0, tRAS = 0;
    GT_U32 tRCD = 0,tRP = 0 ,tWR = 0, tWTR = 0, tRRD = 0, tRTP = 0, tRFC = 0, tMOD = 0;
    GT_U32 dataValue = 0, pageSize = 0;
    MV_HWS_SPEED_BIN  speedBinIndex;
    MV_HWS_MEM_SIZE memorySize = MEM_2G;

	speedBinIndex = topologyMap->interfaceParams[interfaceId].speedBinIndex;
    memorySize = topologyMap->interfaceParams[interfaceId].memorySize;
    pageSize = (topologyMap->interfaceParams[interfaceId].busWidth == BUS_WIDTH_8) ? pageParam[memorySize].ePageSize_8_BITS : pageParam[memorySize].ePageSize_16_BITS;
    tCKCLK = (MEGA/freqVal[frequency]);
    tRRD = (pageSize == 1) ? speedBinTable(speedBinIndex,speedBinTableElements_tRRD1K) : speedBinTable(speedBinIndex,speedBinTableElements_tRRD2K);
    tRRD = GET_MAX_VALUE(tCKCLK * 4,tRRD);
    tRTP = GET_MAX_VALUE(tCKCLK * 4,speedBinTable(speedBinIndex,speedBinTableElements_tRTP));
    /*tMOD = GET_MAX_VALUE(tCKCLK * 12, 15000);*/
#ifdef CONFIG_DDR4
    tWTR = GET_MAX_VALUE(tCKCLK * 2, speedBinTable(speedBinIndex,speedBinTableElements_tWTR));
#else
    tWTR = GET_MAX_VALUE(tCKCLK * 4, speedBinTable(speedBinIndex,speedBinTableElements_tWTR));
#endif
    tRAS = TIME_2_CLOCK_CYCLES(speedBinTable(speedBinIndex,speedBinTableElements_tRAS), tCKCLK);
    tRCD = TIME_2_CLOCK_CYCLES(speedBinTable(speedBinIndex,speedBinTableElements_tRCD), tCKCLK);
    tRP = TIME_2_CLOCK_CYCLES(speedBinTable(speedBinIndex,speedBinTableElements_tRP), tCKCLK);
    tWR = TIME_2_CLOCK_CYCLES(speedBinTable(speedBinIndex,speedBinTableElements_tWR), tCKCLK);
    tWTR = TIME_2_CLOCK_CYCLES(tWTR, tCKCLK);
    tRRD = TIME_2_CLOCK_CYCLES(tRRD, tCKCLK);
    tRTP = TIME_2_CLOCK_CYCLES(tRTP, tCKCLK); 
    tRFC = TIME_2_CLOCK_CYCLES(rfcTable[memorySize]*1000, tCKCLK);


	tMOD = GET_MAX_VALUE(tCKCLK * 24, 15000);
	tMOD = TIME_2_CLOCK_CYCLES(tMOD, tCKCLK);

	/* SDRAM Timing High*/ 
    dataValue = (tRAS & 0xf) |  (tRCD << 4) | (tRP << 8)  |(tWR << 12) |(tWTR << 16) |(((tRAS & 0x30) >> 4) << 20) | (tRRD << 24) |(tRTP << 28);
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_LOW_REG, dataValue,0xff3fffff));
	/* SDRAM Timing High*/ 
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, tRFC & 0x7f ,0x7f));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x180,0x180));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x600,0x600));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x1800,0xf800));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, ((tRFC & 0x380) >> 7) << 16,0x70000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0,0x380000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, (tMOD & 0xF) << 25 ,0x1E00000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, (tMOD >> 4) << 30 ,0xC0000000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x16000000,0x1E000000));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, accessType, interfaceId, SDRAM_TIMING_HIGH_REG, 0x40000000,0xC0000000));

#ifdef CONFIG_DDR4
    ddr4TipSetTiming(devNum, accessType, interfaceId, frequency);
#endif

   return GT_OK;
}

/*****************************************************************************
Mode Read
******************************************************************************/
GT_STATUS    mvHwsDdr3TipModeRead
(  
    GT_U32    devNum,
    Mode      *modeInfo
)
{
   GT_U32 retVal;

   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST,  PARAM_NOT_CARE,  MR0_REG,  modeInfo->regMR0,  MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }
   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, MR1_REG, modeInfo->regMR1, MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }
   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST,  PARAM_NOT_CARE,  MR2_REG,  modeInfo->regMR2,  MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }
   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST,  PARAM_NOT_CARE,  MR3_REG,  modeInfo->regMR2,  MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }
   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST,  PARAM_NOT_CARE,  READ_DATA_SAMPLE_DELAY,  modeInfo->readDataSample,  MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }

   retVal = mvHwsDdr3TipIFRead(devNum, ACCESS_TYPE_MULTICAST,  PARAM_NOT_CARE, READ_DATA_READY_DELAY, modeInfo->uiReadDataReady, MASK_ALL_BITS);
   if (retVal != GT_OK)
   {
      return retVal;
   }
   return GT_OK;
}


/*****************************************************************************
Get first active IF
******************************************************************************/
GT_STATUS ddr3TipGetFirstActiveIf
(
    GT_U8   devNum,
    GT_U32  interfaceMask,
    GT_U32  *ifId
)
{
   GT_U32 interfaceId;

   devNum = devNum;

   for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
   {
      VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
      if (interfaceMask & (1 << interfaceId))
      {
         *ifId = interfaceId;
          break;
      }
   }
   return GT_OK;
}




/*****************************************************************************
Write CS Result
******************************************************************************/
GT_STATUS    ddr3TipWriteCsResult
(
    GT_U32 devNum,
    GT_U32 offset
)
{
    GT_U32 interfaceId,busNum, csBitmask, dataVal, csNum;

    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
		for (busNum=0; busNum<topologyMap->numOfBusPerInterface; busNum++)
		{
        	VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busNum)
			csBitmask = topologyMap->interfaceParams[interfaceId].asBusParams[busNum].csBitmask;
			if(csBitmask != effective_cs)
			{
                csNum = GET_CS_FROM_MASK(csBitmask);
				mvHwsDdr3TipBUSRead(devNum, interfaceId, ACCESS_TYPE_UNICAST, busNum, DDR_PHY_DATA, offset + CS_REG_VALUE(effective_cs), &dataVal);
    			mvHwsDdr3TipBUSWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, busNum, DDR_PHY_DATA, offset + CS_REG_VALUE(csNum), dataVal);
			}					
		}
	}
    return GT_OK;
}

/*****************************************************************************
Write MRS
******************************************************************************/
GT_STATUS    ddr3TipWriteMRSCmd
(
    GT_U32        devNum,
    GT_U32        *csMaskArr,
    GT_U32        cmd,
    GT_U32        data,
    GT_U32        mask
)
{
    GT_U32    interfaceId, reg;

#ifdef CONFIG_DDR3
    reg = (cmd == MRS1_CMD) ? MR1_REG:MR2_REG;
#else
    reg = (cmd == MRS1_CMD) ? DDR4_MR1_REG:DDR4_MR2_REG;
#endif
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, reg,  data, mask));
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_UNICAST, interfaceId, SDRAM_OPERATION_REG, (csMaskArr[interfaceId] << 8) | cmd, 0xf1f));
    }
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId,  0, 0x1F, SDRAM_OPERATION_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("WriteMRSCmd: Poll cmd fail"));
        }
    }
    return GT_OK;
}


/*****************************************************************************
Dynamic ODT
******************************************************************************/
GT_STATUS    ddr3TipDynamicOdt
(
    GT_U32    devNum,
    GT_BOOL   bIsSet
)                          
{
    GT_U32 mr1Value,mr2Value, interfaceId;

    if (bIsSet == GT_TRUE)
    {
        mr1Value = 0x24;
        mr2Value = 0;
    }
    else
    {
        mr1Value = 0;
        mr2Value = 0x200;
    }
    /* MR1: set RttNom to RZQ/6 */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR1_REG, mr1Value, 0x224));
    /* MR2: disable dynamic ODT*/
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, MR2_REG, mr2Value, 0x600));
    /*Operation command*/
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0xC00, 0xF00));
    /* MR1 Command */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0x4, 0x1F));
    /* check controller back to normal */
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId, 0, 0x1F, SDRAM_OPERATION_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("WL: DDR3 poll failed(1)"));
        }
    }
    /* MR2 Command */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_OPERATION_REG, 0x8, 0x1F));
    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId, 0, 0x1F, SDRAM_OPERATION_REG, MAX_POLLING_ITERATIONS) != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("WL: DDR3 poll failed(2)"));
        }
    }
    return GT_OK;
}



/*****************************************************************************
Reset XSB Read FIFO
******************************************************************************/
GT_STATUS    ddr3TipResetFifoPtr
(
    GT_U32 devNum
)
{
    GT_U32 interfaceId = 0;
    /* Configure PHY reset value to 0 in order to "clean"  the FIFO */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  0x15c8, 0, 0xFF000000));
    /* Move PHY to RL mode (only in RL mode the PHY overrides FIFO values during FIFO reset) */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  TRAINING_SW_2_REG, 0x1, 0x9));
    /* In order that above configuration will influence the PHY */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  0x15B0, 0x80000000, 0x80000000));
    /* Reset read fifo assertion */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  0x1400, 0, 0x40000000));
    /* Reset read fifo deassertion */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  0x1400, 0x40000000, 0x40000000));
    /* Move PHY back to functional mode */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  TRAINING_SW_2_REG, 0x8, 0x9));
    /* stop training machine */
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, interfaceId,  0x15B4, 0x10000, 0x10000));
    return GT_OK;

}

/*****************************************************************************
Reset Phy registers
******************************************************************************/
GT_STATUS    ddr3TipDDR3ResetPhyRegs
( 
    GT_U32               devNum
)
{
    GT_U32 interfaceId, phyId,cs;

    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
        for (phyId=0; phyId<topologyMap->numOfBusPerInterface; phyId++)
        {
        	VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, phyId)
			CHECK_STATUS(mvHwsDdr3TipBUSWrite(  devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, phyId, DDR_PHY_DATA, WL_PHY_REG + CS_REG_VALUE(effective_cs), PhyReg0Val));
            CHECK_STATUS(mvHwsDdr3TipBUSWrite(  devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, phyId, DDR_PHY_DATA, RL_PHY_REG + CS_REG_VALUE(effective_cs), PhyReg2Val));
            CHECK_STATUS(mvHwsDdr3TipBUSWrite(  devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, phyId, DDR_PHY_DATA, READ_CENTRALIZATION_PHY_REG + CS_REG_VALUE(effective_cs), PhyReg3Val));
            CHECK_STATUS(mvHwsDdr3TipBUSWrite(  devNum, ACCESS_TYPE_UNICAST, interfaceId, ACCESS_TYPE_UNICAST, phyId, DDR_PHY_DATA, WRITE_CENTRALIZATION_PHY_REG + CS_REG_VALUE(effective_cs), PhyReg3Val));
		}
    }

	/*Set Receiver Calibration value*/
	for (cs = 0; cs < MAX_CS_NUM; cs++) {
        /* PHY register 0xDB bits [5:0] - configure to 63*/
	    CHECK_STATUS(mvHwsDdr3TipBUSWrite(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE,
									ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, DDR_PHY_DATA, CSn_IOB_VREF_REG(cs), 63));

    }

    return GT_OK;
}

/*****************************************************************************
Restore Dunit registers
******************************************************************************/
GT_STATUS ddr3TipRestoreDunitRegs
( 
    GT_U32               devNum
)
{
    GT_U32 indexCnt;

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, CALIB_MACHINE_CTRL_REG, 0x1, 0x1));
    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, CALIB_MACHINE_CTRL_REG, calibrationUpdateControl<<3, 0x3<<3));

    CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, ODPG_WRITE_READ_MODE_ENABLE_REG, 0xFFFF, MASK_ALL_BITS));

    for(indexCnt = 0; indexCnt < sizeof(odpgDefaultValue)/sizeof(RegData); indexCnt++)
    {
       CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, odpgDefaultValue[indexCnt].regAddr, 
								   odpgDefaultValue[indexCnt].regData, odpgDefaultValue[indexCnt].regMask));
        
    }
    return GT_OK;
}


/*****************************************************************************
Auto tune main flow
******************************************************************************/
static GT_STATUS    ddr3TipDDR3Ddr3TrainingMainFlow
(
   GT_U32               devNum
)
{
    MV_HWS_DDR_FREQ freq = initFreq;
    InitCntrParam   initCntrPrm;
    GT_STATUS retVal = GT_OK;
	GT_U32 interfaceId;
	GT_U32 max_cs = mvHwsDdr3TipMaxCSGet();

#ifndef EXCLUDE_SWITCH_DEBUG
	if(debugTraining == DEBUG_LEVEL_TRACE)
		CHECK_STATUS(printDeviceInfo((GT_U8)devNum));
#endif

	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		CHECK_STATUS(ddr3TipDDR3ResetPhyRegs(devNum));
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/

    freq = initFreq;
    if (isPllBeforeInit != 0 )
    {
		for(interfaceId = 0; interfaceId < MAX_INTERFACE_NUM; interfaceId++) {
			VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)
			configFuncInfo[devNum].tipSetFreqDividerFunc((GT_U8)devNum, interfaceId, freq);
		}
    }

    if (isAdllCalibBeforeInit != 0)
    {
		DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("with adll calib before init\n"));
        AdllCalibration(devNum, ACCESS_TYPE_MULTICAST, 0, freq );
    }

    if (isRegDump != 0)
    {
		DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("Dump before init controller\n"));
        ddr3TipRegDump(devNum);
    }
    
	
    if (maskTuneFunc & INIT_CONTROLLER_MASK_BIT)
    {
        trainingStage = INIT_CONTROLLER;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("INIT_CONTROLLER_MASK_BIT\n"));
		initCntrPrm.doMrsPhy = GT_TRUE;
		initCntrPrm.isCtrl64Bit = GT_FALSE;
		initCntrPrm.initPhy = GT_TRUE;
		initCntrPrm.msysInit = GT_FALSE;
        retVal = mvHwsDdr3TipInitController(devNum, &initCntrPrm);
        if (isRegDump != 0)
        {
            ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("mvHwsDdr3TipInitController failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }

#ifdef STATIC_ALGO_SUPPORT
    if (maskTuneFunc & STATIC_LEVELING_MASK_BIT)
    {
        trainingStage = STATIC_LEVELING;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("STATIC_LEVELING_MASK_BIT\n"));
        retVal = ddr3TipRunStaticAlg(devNum, freq);
        if (isRegDump != 0)
        {
            ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipRunStaticAlg failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }
#endif
    if (maskTuneFunc & SET_LOW_FREQ_MASK_BIT)
    {
       trainingStage = SET_LOW_FREQ;
       DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("SET_LOW_FREQ_MASK_BIT %d\n", freqVal[lowFreq]));
       retVal = ddr3TipFreqSet(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, lowFreq);
       if (isRegDump != 0)
       {
           ddr3TipRegDump(devNum);
       }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipFreqSet failure \n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }

	for(effective_cs = 0; effective_cs < max_cs;effective_cs++){
		if (maskTuneFunc & LOAD_PATTERN_MASK_BIT)
		{
			trainingStage = LOAD_PATTERN;
			DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("LOAD_PATTERN_MASK_BIT #%d\n",effective_cs));
			retVal = ddr3TipLoadAllPatternToMem( devNum);
			if (isRegDump != 0)
			{
				ddr3TipRegDump(devNum);
			}
			if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipLoadAllPatternToMem failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL;
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/


    if (maskTuneFunc & SET_MEDIUM_FREQ_MASK_BIT)
    {
	   trainingStage = SET_MEDIUM_FREQ;
	   DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("SET_MEDIUM_FREQ_MASK_BIT %d\n", freqVal[mediumFreq]));
	   retVal = ddr3TipFreqSet(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, mediumFreq);
	   if (isRegDump != 0)
	   {
		   ddr3TipRegDump(devNum);
	   }
	   if (retVal != GT_OK)
	   {
		   DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipFreqSet failure \n"));
		   if (debugMode == GT_FALSE)
		   {
			   return GT_FAIL;
		   }
	   }
    }
    if (maskTuneFunc & WRITE_LEVELING_MASK_BIT)
    {
        trainingStage = WRITE_LEVELING;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("WRITE_LEVELING_MASK_BIT\n"));
        if( (rlMidFreqWA == GT_FALSE) || (freqVal[mediumFreq] == 533) ){
			retVal = ddr3TipDynamicWriteLeveling(devNum);
		}else{
			/*Use old WL*/
			retVal = ddr3TipLegacyDynamicWriteLeveling(devNum);
		}
 
       if (isRegDump != 0)
       {
           ddr3TipRegDump(devNum);
       }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR,( "ddr3TipDynamicWriteLeveling failure \n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }

        }
    }

	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		if (maskTuneFunc & LOAD_PATTERN_2_MASK_BIT)
		{
			trainingStage = LOAD_PATTERN_2;
			DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("LOAD_PATTERN_2_MASK_BIT CS #%d\n", effective_cs));
			retVal = ddr3TipLoadAllPatternToMem( devNum);
			if (isRegDump != 0)
			{
				ddr3TipRegDump(devNum);
			}
			if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipLoadAllPatternToMem failure CS #%d \n", effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL; 
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/
	
    if (maskTuneFunc & READ_LEVELING_MASK_BIT)
    {
        trainingStage = READ_LEVELING;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("READ_LEVELING_MASK_BIT\n"));
        if( (rlMidFreqWA == GT_FALSE) || (freqVal[mediumFreq] == 533) ){
			retVal = ddr3TipDynamicReadLeveling(devNum, mediumFreq);
		}else{
			/*Use old RL*/
			retVal = ddr3TipLegacyDynamicReadLeveling(devNum);
		}

       if (isRegDump != 0)
       {
           ddr3TipRegDump(devNum);
       }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicReadLeveling failure \n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }
    if (maskTuneFunc & WRITE_LEVELING_SUPP_MASK_BIT)
    {
       trainingStage = WRITE_LEVELING_SUPP;
       DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("WRITE_LEVELING_SUPP_MASK_BIT\n"));
       retVal = ddr3TipDynamicWriteLevelingSupp(devNum);
        if (isRegDump != 0)
        {
           ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicWriteLevelingSupp failure \n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }

#ifdef CONFIG_DDR3
	for(effective_cs = 0; effective_cs < max_cs/*NUM_OF_CS*/; effective_cs++){
		if (maskTuneFunc & PBS_RX_MASK_BIT)
		{
		   trainingStage = PBS_RX;
		   DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("PBS_RX_MASK_BIT CS #%d\n", effective_cs));
		   retVal = ddr3TipPbsRx( devNum);
			if (isRegDump != 0)
			{
			   ddr3TipRegDump(devNum);
			}
		   if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipPbsRx failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL; 
				}
			}
		}
	}

	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		if (maskTuneFunc & PBS_TX_MASK_BIT)
		{
	   		trainingStage = PBS_TX;
	   		DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("PBS_TX_MASK_BIT CS #%d\n",effective_cs));
	   		retVal = ddr3TipPbsTx( devNum);
			if (isRegDump != 0)
			{
		   		ddr3TipRegDump(devNum);
			}
	   		if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR,( "ddr3TipPbsTx failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL; 
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/
#endif

    if (maskTuneFunc & SET_TARGET_FREQ_MASK_BIT)
    {
		trainingStage = SET_TARGET_FREQ;
		DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("SET_TARGET_FREQ_MASK_BIT %d\n", freqVal[topologyMap->interfaceParams[firstActiveIf].memoryFreq]));
		retVal = ddr3TipFreqSet(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, topologyMap->interfaceParams[firstActiveIf].memoryFreq);
		if (isRegDump != 0)
		{
		   ddr3TipRegDump(devNum);
		}
		if (retVal != GT_OK)
		{
			DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR,( "ddr3TipFreqSet failure \n"));
			if (debugMode == GT_FALSE)
			{
				return GT_FAIL;
			}
		}
    }
    /*if (maskTuneFunc & ADJUST_DQS_MASK_BIT)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("ADJUST_DQS_MASK_BIT\n"));
        CHECK_STATUS(ddr3TipAdjustDqs(devNum, ACCESS_TYPE_MULTICAST, PARAM_NOT_CARE, targetFreq));
    }*/

    if (maskTuneFunc & WRITE_LEVELING_TF_MASK_BIT)
    {
       trainingStage = WRITE_LEVELING_TF;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("WRITE_LEVELING_TF_MASK_BIT\n"));
        retVal = ddr3TipDynamicWriteLeveling(devNum);
       if (isRegDump != 0)
       {
          ddr3TipRegDump(devNum);
       }
       if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicWriteLeveling TF failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }
    if (maskTuneFunc & LOAD_PATTERN_HIGH_MASK_BIT)
    {
       trainingStage = LOAD_PATTERN_HIGH;
       DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("LOAD_PATTERN_HIGH\n"));
       retVal = ddr3TipLoadAllPatternToMem( devNum);
        if (isRegDump != 0)
        {
           ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipLoadAllPatternToMem failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }
    if (maskTuneFunc & READ_LEVELING_TF_MASK_BIT)
    {
       trainingStage = READ_LEVELING_TF;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("READ_LEVELING_TF_MASK_BIT \n"));
        retVal = ddr3TipDynamicReadLeveling(devNum, topologyMap->interfaceParams[firstActiveIf].memoryFreq);
        if (isRegDump != 0)
        {
           ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicReadLeveling TF failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
        }
    }

#ifdef CONFIG_DDR4__NOT_USED
	if (maskTuneFunc & PER_BIT_READ_LEVELING_TF_MASK_BIT)
    {
       trainingStage = PER_BIT_READ_LEVELING_TF;
       DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("PER_BIT_READ_LEVELING_TF_MASK_BIT \n"));
       retVal = ddr3TipDynamicPerBitReadLeveling(devNum, topologyMap->interfaceParams[firstActiveIf].memoryFreq);
       if (isRegDump != 0)
       {
           ddr3TipRegDump(devNum);
       }
       if (retVal != GT_OK)
       {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicPerBitReadLeveling TF failure \n")); 
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL; 
            }
       }
    }

    if (maskTuneFunc & SW_READ_LEVELING_MASK_BIT)
    {
        trainingStage = SW_READ_LEVELING;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("SW_READ_LEVELING_MASK_BIT\n"));
		retVal = ddr4TipSoftwareReadLeveling(devNum, mediumFreq);
        if (isRegDump != 0)
        {
           ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr4TipSoftwareReadLeveling failure \n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL;
            }
        }
    }
#endif /* CONFIG_DDR4 */

#ifdef CONFIG_DDR3
    if (maskTuneFunc & DM_PBS_TX_MASK_BIT)
    {
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("DM_PBS_TX_MASK_BIT \n"));
      /* CHECK_STATUS(ddr3TipDmPbsTx(devNum));*/
    }

 	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
	  if (maskTuneFunc & VREF_CALIBRATION_MASK_BIT)
		{
		   	trainingStage = VREF_CALIBRATION;
		    DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("VREF\n"));
		    retVal = ddr3TipVref(devNum);
		    if (isRegDump != 0)
		    {
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("VREF Dump\n"));
				ddr3TipRegDump(devNum);
		    }
		    if (retVal != GT_OK)
		    {
		        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipVref failure \n"));
		        if (debugMode == GT_FALSE)
		        {
		            return GT_FAIL;
		        }
		    }
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/

 	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
	    if (maskTuneFunc & CENTRALIZATION_RX_MASK_BIT)
		 {
			trainingStage = CENTRALIZATION_RX;
			DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("CENTRALIZATION_RX_MASK_BIT CS #%d\n",effective_cs));
			retVal = ddr3TipCentralizationRx(devNum);
			if (isRegDump != 0)
			{
				ddr3TipRegDump(devNum);
			}
			if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipCentralizationRx failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL; 
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/
#endif

	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		if (maskTuneFunc & WRITE_LEVELING_SUPP_TF_MASK_BIT)
		{
			trainingStage = WRITE_LEVELING_SUPP_TF;
			DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("WRITE_LEVELING_SUPP_TF_MASK_BIT CS #%d\n",effective_cs));
			retVal = ddr3TipDynamicWriteLevelingSupp(devNum);
			if (isRegDump != 0)
			{
				ddr3TipRegDump(devNum);
			}
			if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipDynamicWriteLevelingSupp TF failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL;
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/

#ifdef CONFIG_DDR4
	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		if (maskTuneFunc & RECEIVER_CALIBRATION_MASK_BIT)
		{
		   trainingStage = RECEIVER_CALIBRATION;
		    DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("RECEIVER_CALIBRATION_MASK_BIT\n"));
		    retVal = ddr4TipReceiverCalibration(devNum);
		    if (isRegDump != 0)
		    {
		       ddr3TipRegDump(devNum);
		    }
		    if (retVal != GT_OK)
		    {
		        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr4TipReceiverCalibration failure \n"));
		        if (debugMode == GT_FALSE)
		        {
		            return GT_FAIL;
		        }
		    }
		}
		if (maskTuneFunc & WL_PHASE_CORRECTION_MASK_BIT)
		{
		   trainingStage = WL_PHASE_CORRECTION;
		    DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("WL_PHASE_CORRECTION_MASK_BIT\n"));
		    retVal = ddr4TipDynamicWriteLevelingSupp(devNum);
		    if (isRegDump != 0)
		    {
		       ddr3TipRegDump(devNum);
		    }
		    if (retVal != GT_OK)
		    {
		        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr4TipDynamicWriteLevelingSupp failure\n"));
		        if (debugMode == GT_FALSE)
		        {
		            return GT_FAIL;
		        }
		    }
		}
		if (maskTuneFunc & DQ_VREF_CALIBRATION_MASK_BIT)
		{
		   trainingStage = DQ_VREF_CALIBRATION;
		    DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("DQ_VREF_CALIBRATION_MASK_BIT\n"));
		    retVal = ddr4TipDqVrefCalibration(devNum);
		    if (isRegDump != 0)
		    {
		       ddr3TipRegDump(devNum);
		    }
		    if (retVal != GT_OK)
		    {
		        DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr4TipDqVrefCalibration failure\n"));
		        if (debugMode == GT_FALSE)
		        {
		            return GT_FAIL;
		        }
		    }
		}
	}
	effective_cs = 0;
    if (maskTuneFunc & DQ_MAPPING_MASK_BIT)
    {
       trainingStage = DQ_MAPPING;
        DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("DQ_MAPPING_MASK_BIT\n"));
        retVal = ddr4DqPinsMapping(devNum);
        if (isRegDump != 0)
        {
           ddr3TipRegDump(devNum);
        }
        if (retVal != GT_OK)
        {
            DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr4DqPinsMapping failure\n"));
            if (debugMode == GT_FALSE)
            {
                return GT_FAIL;
            }
        }
    }
#endif

#ifdef CONFIG_DDR3
	for(effective_cs = 0; effective_cs < max_cs; effective_cs++){
		if (maskTuneFunc & CENTRALIZATION_TX_MASK_BIT)
		{
			trainingStage = CENTRALIZATION_TX;
			DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("CENTRALIZATION_TX_MASK_BIT CS #%d\n",effective_cs));
			retVal = ddr3TipCentralizationTx(devNum);
			if (isRegDump != 0)
			{
				ddr3TipRegDump(devNum);
			}
			if (retVal != GT_OK)
			{
				DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("ddr3TipCentralizationTx failure CS #%d\n",effective_cs));
				if (debugMode == GT_FALSE)
				{
					return GT_FAIL;
				}
			}
		}
	}
	effective_cs = 0;/*Set to 0 after each loop to avoid illegal value may be used*/
#endif

    DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("restore registers to default\n"));
    /* restore register values */
    CHECK_STATUS(ddr3TipRestoreDunitRegs(devNum));

	if (isRegDump != 0)
	{
		ddr3TipRegDump(devNum);
	}

    return GT_OK;
}

/*****************************************************************************
DDR3 Dynamic training flow
******************************************************************************/
static GT_STATUS    ddr3TipDDR3AutoTune
(
    GT_U32               devNum
)
{
    GT_U32 interfaceId, stage, retVal;
    GT_BOOL isIfFail = GT_FALSE , isAutoTuneFail = GT_FALSE;

    trainingStage = INIT_CONTROLLER;

    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        for(stage = 0; stage < MAX_STAGE_LIMIT; stage++)
        {
            trainingResult[stage][interfaceId] = NO_TEST_DONE;
        }
    }

    retVal = ddr3TipDDR3Ddr3TrainingMainFlow(devNum);

    /* activate XSB test */
    if (xsbValidateType != 0)
    {
        RunXsbTest(devNum, xsbValidationBaseAddress, 1, 1 , 0x1024);
    }

    if (isRegDump != 0)
    {
        ddr3TipRegDump(devNum);
    }
    /* print log */
    CHECK_STATUS(ddr3TipPrintLog(devNum, windowMemAddr));

	if(retVal != GT_OK)
		CHECK_STATUS(ddr3TipPrintStabilityLog(devNum));

    for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
    {
        isIfFail = GT_FALSE;
        for(stage = 0 ; stage < MAX_STAGE_LIMIT; stage++)
        {
            if (trainingResult[stage][interfaceId] == TEST_FAILED)
            {
                isIfFail = GT_TRUE;
            }
        }
        if (isIfFail == GT_TRUE)
        {
            isAutoTuneFail = GT_TRUE;
            DEBUG_TRAINING_IP(DEBUG_LEVEL_INFO, ("Auto Tune failed for IF %d\n", interfaceId));
        }
    }

	if ((retVal== GT_FAIL) || (isAutoTuneFail == GT_TRUE))
		return GT_FAIL;
	else
		return GT_OK;
}

/*****************************************************************************
Enable init sequence
******************************************************************************/
GT_STATUS ddr3TipEnableInitSequence
(
    GT_U32 devNum
)
{
   GT_BOOL isFail = GT_FALSE;
   GT_U32 interfaceId = 0, memMask= 0 , busIndex = 0;

   /*Enable init sequence */
   CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum, ACCESS_TYPE_MULTICAST, 0, SDRAM_INIT_CONTROL_REG, 0x1,0x1));

   for(interfaceId = 0; interfaceId <= MAX_INTERFACE_NUM-1; interfaceId++)
   {
      VALIDATE_IF_ACTIVE(topologyMap->interfaceActiveMask, interfaceId)

      if (ddr3TipIfPolling(devNum, ACCESS_TYPE_UNICAST, interfaceId, 0, 0x1, SDRAM_INIT_CONTROL_REG, MAX_POLLING_ITERATIONS) != GT_OK)
      {
         DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("polling failed IF %d \n",interfaceId)); 
         isFail = GT_TRUE;
         continue;
      }
      memMask = 0;
      for(busIndex=0; busIndex < GET_TOPOLOGY_NUM_OF_BUSES(devNum) ; busIndex++)
      {
       		VALIDATE_BUS_ACTIVE(topologyMap->activeBusMask, busIndex)
         	memMask |= topologyMap->interfaceParams[interfaceId].asBusParams[busIndex].mirrorEnableBitmask;
      }
      if (memMask != 0)
      {
          /*Disable MultiCS */
         CHECK_STATUS(mvHwsDdr3TipIFWrite(devNum,ACCESS_TYPE_MULTICAST, interfaceId, CS_ENABLE_REG, 1<<3, 1<<3));
      }

   }
   return (isFail == GT_FALSE)? GT_OK:GT_FAIL;
}


GT_STATUS ddr3TipRegisterDqTable
(
    GT_U32          devNum,
	GT_U32			*table
)
{
	devNum = devNum; /* avoid warnings*/

	dqMapTable = table;
	return GT_OK;
}

/*****************************************************************************
Check if pup search is locked
******************************************************************************/
GT_BOOL   ddr3TipIsPupLock
(
    GT_U32                   *pupBuf,
    MV_HWS_TRAINING_RESULT   readMode
)
{
    GT_U32 bitStart = 0 , bitEnd = 0, bitId;
    if (readMode == RESULT_PER_BIT)
    {
        bitStart = 0;
        bitEnd = BUS_WIDTH_IN_BITS-1;
    }
    else
    {
        bitStart = 0;
        bitEnd = 0;
    }
    for (bitId = bitStart; bitId <= bitEnd; bitId++)
    {
        if (GET_LOCK_RESULT(pupBuf[bitId]) == GT_FALSE)
        {
            return GT_FALSE;
        }
    }
    return GT_TRUE;
}

/*****************************************************************************
Get minimum buffer value
******************************************************************************/
GT_U8 ddr3TipGetBufMin
(
    GT_U8* bufPtr
)
{
    GT_U8 minVal = 0xff;
    GT_U8 cnt = 0;
    for (cnt=0; cnt < BUS_WIDTH_IN_BITS; cnt++)
    {
        if (bufPtr[cnt] < minVal)
        {
            minVal = bufPtr[cnt];
        }
    }
    return minVal;
}

/*****************************************************************************
Get maximum buffer value
******************************************************************************/
GT_U8 ddr3TipGetBufMax
(
    GT_U8* bufPtr
)
{
    GT_U8 maxVal = 0;
    GT_U8 cnt = 0;
    for (cnt=0; cnt < BUS_WIDTH_IN_BITS; cnt++)
    {
        if (bufPtr[cnt] > maxVal)
        {
            maxVal = bufPtr[cnt];
        }
    }
    return maxVal;
}

/********************************************************************************
The following functions return memory parameters:bus and device width,device size
*********************************************************************************/

GT_U32 mvHwsDdr3GetBusWidth(void) {

  return (DDR3_IS_16BIT_DRAM_MODE(topologyMap->activeBusMask) == GT_TRUE) ? 16 : 32;
}


GT_U32 mvHwsDdr3GetDeviceWidth(GT_U32 interfaceId)
{

    return (topologyMap->interfaceParams[interfaceId].busWidth == BUS_WIDTH_8) ? 8 : 16;
}

GT_U32 mvHwsDdr3GetDeviceSize(GT_U32 interfaceId)
{
	if (topologyMap->interfaceParams[interfaceId].memorySize >= MEM_SIZE_LAST){
		DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("Error: Wrong device size of Cs: ", topologyMap->interfaceParams[interfaceId].memorySize));
		return 0;
	}
	else
		return(1 << topologyMap->interfaceParams[interfaceId].memorySize);
}

GT_STATUS mvHwsDdr3CalcMemCsSize(GT_U32 interfaceId, GT_U32 uiCs, GT_U32* puiCsSize)
{

	GT_U32 uiCsMemSize,devSize;

	if ((devSize = mvHwsDdr3GetDeviceSize(interfaceId))!=0) {
		uiCsMemSize = ((mvHwsDdr3GetBusWidth() / mvHwsDdr3GetDeviceWidth(interfaceId)) * devSize); /* the calculated result in Gbytex16  to avoid float using*/;

		if (uiCsMemSize == 2)
		{
		  *puiCsSize = _128M;
		}
		else if (uiCsMemSize == 4) {
			*puiCsSize = _256M;
		}
		else if (uiCsMemSize == 8) {
			*puiCsSize = _512M;
		}
		else if (uiCsMemSize == 16) {
			*puiCsSize = _1G;
		}
		else if (uiCsMemSize == 32) {
			*puiCsSize = _2G;
		}
		else {
			DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR, ("Error: Wrong Memory size of Cs: ", uiCs));
			return GT_FAIL;
		}
		return GT_OK;
	}
	else
		return GT_FAIL;
}

GT_STATUS mvHwsDdr3CsBaseAdrCalc(GT_U32 interfaceId, GT_U32 uiCs, GT_U32 *csBaseAddr)
{

	GT_U32 uiCsMemSize = 0;

#ifdef MV_DEVICE_MAX_DRAM_ADDRESS_SIZE
	GT_U32 physicalMemSize;
	GT_U32 maxMemSize = MV_DEVICE_MAX_DRAM_ADDRESS_SIZE;
#endif

	if (mvHwsDdr3CalcMemCsSize(interfaceId,uiCs, &uiCsMemSize) != GT_OK)
		return GT_FAIL;

#ifdef MV_DEVICE_MAX_DRAM_ADDRESS_SIZE
	/* if number of address pins doesn't allow to use max mem size that is defined in topology
	 mem size is defined by MV_DEVICE_MAX_DRAM_ADDRESS_SIZE*/
	physicalMemSize = mvHwsmemSize [topologyMap->interfaceParams[0].memorySize];

	if (mvHwsDdr3GetDeviceWidth(uiCs) == 16)
		maxMemSize = MV_DEVICE_MAX_DRAM_ADDRESS_SIZE * 2; /* 16bit mem device can be twice more - no need in less significant pin*/

	if (physicalMemSize > maxMemSize ){
		uiCsMemSize = maxMemSize * (mvHwsDdr3GetBusWidth() / mvHwsDdr3GetDeviceWidth(interfaceId)) ;
		DEBUG_TRAINING_IP(DEBUG_LEVEL_ERROR,  ("Updated Physical Mem size is from 0x%x to %x\n", physicalMemSize, MV_DEVICE_MAX_DRAM_ADDRESS_SIZE));
	}
#endif
	/*calculate CS base addr  */
	*csBaseAddr = ((uiCsMemSize) * uiCs) & 0xFFFF0000;
	return GT_OK;
}

