/*
 * (C) Copyright 2003
 * Texas Instruments.
 * Kshitij Gupta <kshitij@ti.com>
 * Configuation settings for the TI OMAP Innovator board.
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 * Configuration for Integrator AP board.
 *.
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <config.h>
#include "../../board/mv_ebu/a370/mvSysHwConfig.h"

#define MV_RT_DEBUG

/************/
/* VERSIONS */
/************/
#define CONFIG_IDENT_STRING	" Marvell version: v2011.12 2013_Q1.0"

/* version number passing when loading Kernel */
#define VER_NUM 0x11120000           /* 2011.12 */
#ifndef MV_NAND
	#define CONFIG_SYS_NAND_MAX_CHIPS 1	//pass compilation for non NAND board configuration
#endif // MV_NAND
/**********************************/
/* defines for db88f6710-BP       */
/**********************************/
#ifdef CONFIG_DB_88F6710_BP

	#define DB_88F6710
	#define MV88F6710
	#define MV88F67XX
#endif // CONFIG_DB_88f6710_BP

#ifdef CONFIG_RD_88F6710
	#define RD_88F6710
	#define MV88F6710
	#define MV88F67XX
#endif //CONFIG_RD_88F6710_SERVER

#ifdef MV_NAND
		#define MV_NAND_PIO_MODE
		#define MV_NAND_1CS_MODE
		#define MV_NAND_4BIT_MODE
		#define MTD_NAND_NFC_INIT_RESET
#endif // MV_NAND

/***********/
/* General */
/***********/
#define MV_SEC_64K
#define MV_BOOTSIZE_512K
#define MV_LARGE_PAGE

#define MV_DDR_64BIT
#define MV_MEM_FASTPATH
#define MV_BOOTROM
#define MV_USB
/*********/
/* Debug */
/*********/
/* Faraj: TODO: define the right init address*/
#define CONFIG_SYS_INIT_SP_ADDR		0x00FF0000  /* end of 16M scrubbed by training in bootrom*/
#define	CONFIG_SYS_ATA_BASE_ADDR	0x20000000
#define CONFIG_SYS_ATA_REG_OFFSET	0x0000	    /* Offset for normal register accesses*/
#define CONFIG_SYS_ATA_DATA_OFFSET	0x0000	    /* Offset for data I/O */

/**********************************/
/* Marvell Monitor Extension      */
/**********************************/
#define enaMonExt()( /*(!getenv("enaMonExt")) ||\*/\
		     ( getenv("enaMonExt") && \
                       ((!strcmp(getenv("enaMonExt"),"yes")) ||\
		       (!strcmp(getenv("enaMonExt"),"Yes"))) \
		     )\
		    )
/********/
/* CLKs */
/********/
#ifndef __ASSEMBLY__
extern unsigned int mvSysClkGet(void);
extern unsigned int mvTclkGet(void);
#define UBOOT_CNTR		0		/* counter to use for uboot timer */
#define MV_TCLK_CNTR		1		/* counter to use for uboot timer */
#define MV_REF_CLK_DEV_BIT	1000		/* Number of cycle to eanble timer */
#define MV_REF_CLK_BIT_RATE	100000		/* Ref clock frequency */
#define MV_REF_CLK_INPUT_GPP	6		/* Ref clock frequency input */

#define CONFIG_SYS_HZ					1000 /*800*/
#define CONFIG_SYS_TCLK					mvTclkGet()
#define CONFIG_SYS_BUS_HZ				mvSysClkGet()
#define CONFIG_SYS_BUS_CLK				CONFIG_SYS_BUS_HZ
#endif
#define CONFIG_DISPLAY_CPUINFO

/********************/
/* Page Table settings */
/********************/
#define MV_PT

#ifdef MV_PT
	#define MV_PT_BASE(cpu)  (CONFIG_SYS_MALLOC_BASE - 0x20000 - (cpu*0x20000))
#endif /* #ifdef MV_PT */


/*************************************/
/* High Level Configuration Options  */
/* (easy to change)		     */
/*************************************/
#define CONFIG_MARVELL

#define CONFIG_SYS_64BIT_VSPRINTF
#define CONFIG_SYS_64BIT_STRTOUL
#define CONFIG_API

/* commands */
#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAULT | CONFIG_BOOTP_BOOTFILESIZE)

#define CONFIG_CMDLINE_EDITING		/* Command-line editing */

#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_I2C
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DATE
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_BSP
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_BOOTD
#define CONFIG_CMD_CONSOLE
#define CONFIG_CMD_RUN
#define CONFIG_CMD_MISC
#define CONFIG_CMD_IDE

#define CONFIG_CMD_RCVR
#define CONFIG_CMD_SAR

#if (defined(MV_INCLUDE_PEX) || defined(MV_INCLUDE_PCI))
	#define CONFIG_PCI
	#define CONFIG_CMD_PCI
#endif
/* Pre eXecution Environment (PXE) support*/
#define CONFIG_CMD_PXE
#define CONFIG_BOOTP_PXE
#define CONFIG_MENU
#define CONFIG_BOOTP_PXE_CLIENTARCH	0x100
#define CONFIG_BOOTP_VCI_STRING		"U-boot.armv7.armada_370"

/* Flattened Device Tree (FDT) support */
//#define CONFIG_OF_LIBFDT		1
//#define CONFIG_OF_BOARD_SETUP

/* FS supported */
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_FAT
#define CONFIG_SUPPORT_VFAT

#define CONFIG_SYS_USE_UBI
#ifdef CONFIG_SYS_USE_UBI
	#define CONFIG_CMD_UBI
	#define CONFIG_CMD_UBIFS
	#define CONFIG_MTD_DEVICE
	#define CONFIG_MTD_PARTITIONS
	#define CONFIG_CMD_MTDPARTS
	#define CONFIG_RBTREE
	#define CONFIG_LZO
#endif

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <config_cmd_default.h>

#define	CONFIG_SYS_MAXARGS	32		/* max number of command args	*/

/*-----------------------------------------------------------------------
 * IDE/ATA/SATA stuff (Supports IDE harddisk on PCMCIA Adapter)
 *-----------------------------------------------------------------------
 */

#undef	CONFIG_IDE_8xx_PCCARD		/* Use IDE with PC Card	Adapter	*/

#undef	CONFIG_IDE_8xx_DIRECT		/* Direct IDE    not supported	*/
#undef	CONFIG_IDE_LED			/* LED   for ide not supported	*/
#undef	CONFIG_IDE_RESET		/* reset for ide not supported	*/

#define CONFIG_SYS_IDE_MAXBUS		4	/* max. 1 IDE bus		*/
#define CONFIG_SYS_IDE_MAXDEVICE	CONFIG_SYS_IDE_MAXBUS * 8	/* max. 1 drive per IDE bus	*/

#define CONFIG_SYS_ATA_IDE0_OFFSET	0x0000

#undef CONFIG_MAC_PARTITION
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION

#define CONFIG_LBA48
#define CONFIG_SCSI_AHCI
#ifdef CONFIG_SCSI_AHCI
#define CONFIG_SATA_6121
#define CONFIG_SYS_SCSI_MAX_SCSI_ID	4
#define CONFIG_SYS_SCSI_MAX_LUN	1
#define CONFIG_SYS_SCSI_MAX_DEVICE 	(CONFIG_SYS_SCSI_MAX_SCSI_ID * CONFIG_SYS_SCSI_MAX_LUN)
//#define SCSI_MAXDEVICE	CONFIG_SYS_SCSI_MAX_DEVICE
#endif

/*
 * Staggered Spin-up support for SATA disks
 * ----------------------------------------
 * Set the following U-Boot environment variable:
 * setenv spinup_config <spinup_max>,<spinup_timeout>
 * For example: spinup_config=2,6 will config the module for 2 maximum disks spinning-up with 6 seconds timeout.
 * Parameters explanation:
 * 1. <spinup_max> - The maximum spinning-up disks(can be between 1 and 8) ֲ will be like this:
 *   a. 0 = feature off.
 *   b. 1 ג€“ 8 = number of disks
 *   c. <0,>8 = invalid parameter (will behave like feature off)
 * 2. <spinup_timeout> - The spin-up timeout (can be between 1 and 6) will be like this:
 *   a. 0 = feature off.
 *   b. 1 ג€“ 6 = in seconds
 *   c. <0,>6 = invalid parameter (will behave like feature off)
 * Any parsing error will cause an invalid parameters print and will behave as feature off.
 */
#undef CONFIG_MV_SCATTERED_SPINUP 

/* which initialization functions to call for this board */
#define CONFIG_MISC_INIT_R      /* after relloc initialization*/

#define CONFIG_ENV_OVERWRITE    /* allow to change env parameters */

#undef	CONFIG_WATCHDOG		/* watchdog disabled		*/

/* Cache */
#define CONFIG_SYS_CACHELINE_SIZE	32	

/* global definetions. */
#define CONFIG_SYS_TEXT_BASE		0x00000000
#define	CONFIG_SYS_SDRAM_BASE		0x00000000
#define CONFIG_SYS_RESET_ADDRESS	0xffff0000
#define CONFIG_SYS_MALLOC_BASE		(CONFIG_SYS_TEXT_BASE + (3 << 20)) /* TEXT_BASE + 3M */

/*
 * When locking data in cache you should point the INIT_RAM_ADDRESS
 * To an unused memory region. The stack will remain in cache until RAM
 * is initialized 
*/
/* for running from L2 SRAM
#define CONFIG_SYS_MALLOC_LEN		0xD0000
*/
#define CONFIG_SYS_MALLOC_LEN		(5 << 20)	/* Reserve 5MB for malloc*/
#define CONFIG_SYS_GBL_DATA_SIZE	128  /* size in bytes reserved for init data */
#undef CONFIG_INIT_CRITICAL		/* critical code in start.S */


/********/
/* DRAM */
/********/
#define CONFIG_SYS_DRAM_BANKS		4
#define CONFIG_NR_DRAM_BANKS 		4
/* this defines whether we want to use the lowest CAL or the highest CAL available,*/
/* we also check for the env parameter CASset.	   		*/
#define MV_MIN_CAL

#define CONFIG_SYS_MEMTEST_START     0x00400000
#define CONFIG_SYS_MEMTEST_END       0x007fffff

/********/
/* RTC  */
/********/
#if defined(CONFIG_CMD_DATE)
	#define CONFIG_SYS_NVRAM_SIZE  0x00 /* dummy */
	#define CONFIG_SYS_NVRAM_BASE_ADDR DEVICE_CS1_BASE /* dummy */
#endif /* #if defined(CONFIG_CMD_DATE) */

/********************/
/* Serial + parser  */
/********************/
/*
 * The following defines let you select what serial you want to use
 * for your console driver.
 */
#define CONFIG_BAUDRATE         115200   /* console baudrate = 115200    */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }
 
#if defined(RD_88F6781_AVNG) || defined(DB_88F6781Y0) || defined(RD_88F6781Y0_AVNG) || defined(DB_88F6781X0) || defined(RD_88F6781X0_PLUG) || defined(RD_88F6781X0_AVNG) || defined(DB_88AP510BP_B) || defined(DB_88AP510_PCAC)
#define CONFIG_SYS_DUART_CHAN		0		/* channel to use for console */
#else
#define CONFIG_SYS_DUART_CHAN		1		/* channel to use for console */
#endif

#define CONFIG_SYS_INIT_CHAN1
#if !defined(RD_88F6781_AVNG) && !defined(RD_88F6781Y0_AVNG) && !defined(RD_88F6781X0_AVNG)
#define CONFIG_SYS_INIT_CHAN2
#endif

#define CONFIG_LOADS_ECHO       0       /* echo off for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE           /* allow baudrate changes       */

#define CONFIG_SYS_CONSOLE_INFO_QUIET  /* don't print In/Out/Err console assignment. */
#undef CONFIG_SILENT_CONSOLE  /* define for Pex complince */

/* parser */
#define CONFIG_SYS_HUSH_PARSER

#define CONFIG_AUTO_COMPLETE

#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

#define	CONFIG_SYS_LONGHELP		/* undef to save memory		*/
#define	CONFIG_SYS_PROMPT		"Marvell>> "	/* Monitor Command Prompt	*/
#define	CONFIG_SYS_CBSIZE		1024		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */

/************/
/* ETHERNET */
/************/
/*#define MV_ETH_LEGACY*/
/*#define CONFIG_MV_ETH_LEGACY*/
#define MV_ETH_NETA
#define CONFIG_MV_ETH_NETA
			 
/* to change the default ethernet port, use this define (options: 0, 1, 2) */
#define CONFIG_NET_MULTI
#define CONFIG_IPADDR		10.4.50.136
#define CONFIG_SERVERIP		10.4.50.68
#define CONFIG_NETMASK		255.255.255.0
#define ETHADDR				"00:00:00:00:51:81"
#define ETH1ADDR			"00:00:00:00:51:82"
#define ENV_ETH_PRIME		"egiga0"

/************/
/* PCI	    */
/************/
#if defined(MV_INCLUDE_PCI)

#if defined(DB_PEX_PCI)
	#define ENV_PCI_MODE	"device"	/* PCI */
#else
	#define ENV_PCI_MODE	"host"		/* PCI */
#endif

#endif

/************/
/* USB	    */
/************/
#ifdef MV_USB
	#define MV_INCLUDE_USB
	#define CONFIG_CMD_USB
	#define CONFIG_USB_STORAGE
	#define CONFIG_USB_EHCI
	#define CONFIG_USB_EHCI_MARVELL
	#define CONFIG_EHCI_IS_TDI
	#define CONFIG_DOS_PARTITION
	#define CONFIG_ISO_PARTITION
	#define ENV_USB0_MODE	"host"
	#define ENV_USB1_MODE	"host"
	#define ENV_USB2_MODE		"device"
	#define ENV_USB_ACTIVE		"0"
#else
	#undef MV_INCLUDE_USB
	#undef CONFIG_CMD_USB
	#undef CONFIG_USB_STORAGE
#endif

/************/
/* SDIO/MMC */
/************/
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_MRVL_MMC
#define CONFIG_SYS_MMC_BASE 0xD00d4000
#define CONFIG_SYS_MMC_MAX_DEVICE 1
/***************************************/
/* LINUX BOOT and other ENV PARAMETERS */
/***************************************/
#define MV_BOOTARGS_END			":10.4.50.254:255.255.255.0:KW40:eth0:none"
#define MV_BOOTARGS_END_SWITCH	":::RD88FXX81:eth0:none"
#define RCVR_IP_ADDR			"169.254.100.100"
#define	RCVR_LOAD_ADDR			"0x02000000"

#define CONFIG_ZERO_BOOTDELAY_CHECK

#define	CONFIG_SYS_LOAD_ADDR	0x02000000	/* default load address	*/
#define	CONFIG_SYS_MIN_HDR_DEL_SIZE		0x100

#undef	CONFIG_BOOTARGS

/* auto boot*/
#define CONFIG_BOOTDELAY		3 		/* by default no autoboot */

#define CONFIG_ROOTPATH			"/srv/oneiric"

#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size	*/

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 8 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define	CONFIG_SYS_BOOTMAPSZ		(16<<20)	/* Initial Memory map for Linux */

#define BRIDGE_REG_BASE_BOOTM 0xfbe00000 /* this paramaters are used when booting the linux kernel */ 

#define CONFIG_CMDLINE_TAG				1       /* enable passing of ATAGs  */
#define CONFIG_INITRD_TAG				1	/* enable INITRD tag for ramdisk data */
#define CONFIG_SETUP_MEMORY_TAGS		1
#define CONFIG_MARVELL_TAG
#define ATAG_MARVELL					0x41000403

/********/
/* I2C  */
/********/
#if defined(CONFIG_CMD_I2C)
	#define CONFIG_I2C_MULTI_BUS
	#define CONFIG_SYS_MAX_I2C_BUS			2
	#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
	#define CONFIG_SYS_I2C_MULTI_EEPROMS
	#define CONFIG_SYS_I2C_SPEED			100000		/* I2C speed default */
#endif

/********/
/* PCI  */
/********/
#ifdef CONFIG_PCI
	#define CONFIG_PCI_HOST PCI_HOST_FORCE /* select pci host function     */
	#define CONFIG_PCI_PNP          	/* do pci plug-and-play         */

	/* Pnp PCI Network cards */
	#define CONFIG_EEPRO100		/* Support for Intel 82557/82559/82559ER chips */
	#define CONFIG_E1000
	#define CONFIG_SK98			/* yukon */
	#define YUK_ETHADDR         		"00:00:00:EE:51:81"
	#define CONFIG_DRIVER_RTL8029 

	/* DB_PRPMC support only Yukon */
	#if defined (DB_PRPMC) || defined (DB_MNG)
		#undef CONFIG_EEPRO100	
		#undef CONFIG_DRIVER_RTL8029 
	#endif
#endif /* #ifdef CONFIG_PCI */

#define PCI_HOST_ADAPTER 0              /* configure ar pci adapter     */
#define PCI_HOST_FORCE   1              /* configure as pci host        */
#define PCI_HOST_AUTO    2              /* detected via arbiter enable  */

/* for Yukon */
#define __io_pci(x) x
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))

/***********************/
/* FLASH organization  */
/***********************/

#define CONFIG_SYS_MAX_FLASH_BANKS 1
#define CONFIG_UBOOT_SIZE		0x100000

/* SPI Flash configuration   */
/*****************************/
#if defined(MV_INCLUDE_SPI)
	#define CONFIG_CMD_SPI
	#define CONFIG_CMD_SF
	#define CONFIG_SPI_FLASH
	#define CONFIG_SPI_FLASH_STMICRO
	#define CONFIG_SPI_FLASH_MACRONIX
	#define CONFIG_ENV_SPI_MAX_HZ		50000000
	#define CONFIG_ENV_SPI_CS			0
	#define CONFIG_ENV_SPI_BUS			0

	#ifndef CONFIG_SF_DEFAULT_SPEED
	# define CONFIG_SF_DEFAULT_SPEED	CONFIG_ENV_SPI_MAX_HZ
	#endif
	#ifndef CONFIG_SF_DEFAULT_MODE
	# define CONFIG_SF_DEFAULT_MODE		SPI_MODE_3
	#endif
	#ifndef CONFIG_SF_DEFAULT_CS
	# define CONFIG_SF_DEFAULT_CS		CONFIG_ENV_SPI_CS
	#endif
	#ifndef CONFIG_SF_DEFAULT_BUS
	#define CONFIG_SF_DEFAULT_BUS		CONFIG_ENV_SPI_BUS
	#endif
	//#define MV_SPI_CONN_TO_EXT_FLASH

	/* Boot from SPI settings */
	#if defined(MV_SPI_BOOT)
		#define CONFIG_ENV_IS_IN_SPI_FLASH

		#if defined(MV_SEC_64K)
			#define CONFIG_ENV_SECT_SIZE		0x10000
		#elif defined(MV_SEC_128K)
			#define CONFIG_ENV_SECT_SIZE		0x20000
		#elif defined(MV_SEC_256K)
			#define CONFIG_ENV_SECT_SIZE		0x40000
		#endif

		#define CONFIG_ENV_SIZE				CONFIG_ENV_SECT_SIZE	/* environment takes one sector */
		#define CONFIG_ENV_OFFSET			0x100000		/* (1MB For Image) environment starts here  */
		#define CONFIG_ENV_ADDR				CONFIG_ENV_OFFSET
		#define MONITOR_HEADER_LEN			0x200
		#define CONFIG_SYS_MONITOR_BASE		0
		#define CONFIG_SYS_MONITOR_LEN		0x80000					/*(512 << 10) Reserve 512 kB for Monitor */

		#ifndef MV_INCLUDE_NOR
			#ifdef MV_BOOTROM
				#define CONFIG_SYS_FLASH_BASE		DEVICE_SPI_BASE
				#define CONFIG_SYS_FLASH_SIZE		_16M
			#else
				#define CONFIG_SYS_FLASH_BASE		BOOTDEV_CS_BASE
				#define CONFIG_SYS_FLASH_SIZE		BOOTDEV_CS_SIZE
			#endif /* ifdef MV_BOOTROM */
		#endif
	#endif //#if defined(MV_SPI_BOOT)
#endif //#if defined(MV_SPI)

/* NAND-FLASH stuff     */
/************************/
#if defined(MV_NAND)
	#define CONFIG_SYS_MAX_NAND_DEVICE 1       /* Max number of NAND devices */

	#if defined(MV_NAND_2CS_MODE)
		#define CONFIG_SYS_NAND_MAX_CHIPS 2
	#elif defined(MV_NAND_1CS_MODE)
		#define CONFIG_SYS_NAND_MAX_CHIPS 1
	#endif

	#define CONFIG_CMD_NAND
	//#define CONFIG_NAND_RS_ECC_SUPPORT
	#define CONFIG_MV_MTD_GANG_SUPPORT
	#define CONFIG_MV_MTD_MLC_NAND_SUPPORT
	#define CONFIG_SYS_64BIT_VSPRINTF
	#define CONFIG_SKIP_BAD_BLOCK
	#undef MV_NFC_DBG

	/* Boot from NAND settings */
	#if defined(MV_NAND_BOOT)
		#define CONFIG_ENV_IS_IN_NAND
		#define CONFIG_ENV_SIZE				0x80000		/* environment takes one erase block */
		#define CONFIG_ENV_OFFSET			nand_get_env_offs()
		#define CONFIG_ENV_ADDR				CONFIG_ENV_OFFSET
		#define MONITOR_HEADER_LEN			0x200
		#define CONFIG_SYS_MONITOR_BASE			0
		#define CONFIG_SYS_MONITOR_LEN			0x80000		/* Reserve 512 kB for Monitor */
		#define CONFIG_ENV_RANGE 			CONFIG_ENV_SIZE * 8
		#define MV_NBOOT_BASE				0
		#define MV_NBOOT_LEN				(4 << 10)	/* Reserved 4KB for boot strap */
	#endif /* MV_NAND_BOOT */
#endif //#if defined(MV_NAND)


/* NOR-FLASH stuff	*/
/************************/
#if defined(MV_INCLUDE_NOR)

	#define CONFIG_NOR_FLASH
	#define CONFIG_SYS_FLASH_CFI
	#define CONFIG_SYS_FLASH_PROTECTION
	#define CONFIG_FLASH_CFI_DRIVER
	//#define CONFIG_FLASH_CFI_MTD
	#define CONFIG_SYS_MAX_FLASH_SECT	128
	#define CONFIG_SYS_FLASH_BASE		NOR_CS_BASE
	
	#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
	
	//#define CONFIG_SYS_WRITE_SWAPPED_DATA
	#define CONFIG_FLASH_SHOW_PROGRESS 1
	#define CONFIG_SYS_FLASH_EMPTY_INFO
	#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
	
	//#define CONFIG_SYS_FLASH_CFI_AMD_RESET
	//#define CONFIG_SYS_FLASH_QUIET_TEST
	//#define CONFIG_SYS_FLASH_BANKS_LIST	{BOOTDEV_CS_BASE}
	
	#define CONFIG_CMD_FLASH
	#undef CONFIG_CMD_IMLS
				 
	/* Boot from NOR settings */
	#if defined(MV_NOR_BOOT)
//		#define MV_RUN_FROM_FLASH
		#define CONFIG_ENV_IS_IN_FLASH

		#define CONFIG_ENV_SIZE				0x20000
		#define CONFIG_ENV_SECT_SIZE		0x20000
		#define CONFIG_ENV_OFFSET			0x100000					/* environment starts here  */
		#define CONFIG_ENV_RANGE 		CONFIG_ENV_SIZE * 8
		#define CONFIG_ENV_ADDR				(NOR_CS_BASE + CONFIG_ENV_OFFSET)
		#define MONITOR_HEADER_LEN			0x200
		#define CONFIG_SYS_MONITOR_BASE		0
		#define CONFIG_SYS_MONITOR_LEN		0xC0000					/* Reserve 768 kB for Monitor */
	
	#endif /* MV_NOR_BOOT */
#else
	#define CONFIG_SYS_NO_FLASH
	#undef CONFIG_CMD_FLASH
	#undef CONFIG_CMD_IMLS
#endif /* MV_INCLUDE_NOR */

/*****************/
/* others        */
/*****************/
#define MV_DFL_REGS		0xd0000000 	/* boot time MV_REGS */
#define MV_REGS		INTER_REGS_BASE /* MV Registers will be mapped here */
#define BOARD_LATE_INIT
#undef CONFIG_USE_IRQ
/* for running from L2 SRAM
#define CONFIG_STACKSIZE	(128 << 10)
*/
#define CONFIG_STACKSIZE	(1 << 20)	/* regular stack - up to 4M (in case of exception)*/

#endif	/* __CONFIG_H */
