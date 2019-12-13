/*
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __CONFIG_HI3516EV100_H__
#define __CONFIG_HI3516EV100_H__

#include <asm/arch/platform.h>

#define CONFIG_HI3516CV300

#define CONFIG_SMALL_MEM

/* Arm configuration */
#define CONFIG_L2_OFF

/*-----------------------------------------------------------------------
 * cpu_init configuration
 * if bit[0] = 0b, APB = 24M
 * if bit[0] = 1b, APB = 50M
 *----------------------------------------------------------------------*/
#define HW_REG(a) (*(unsigned long *)(a))
#define SOC_SCALE_REG   (CRG_REG_BASE + REG_CRG12)
#define get_apb_bus_clk() ({\
	unsigned long tmp_reg, busclk = 0;\
	tmp_reg = HW_REG(SOC_SCALE_REG);\
	tmp_reg &= 0x1;\
	if (tmp_reg == 0x0) {\
		busclk = 24000000;\
	} else {\
		busclk = 50000000;\
	} \
	busclk;\
})

/* #define CFG_CLK_BUS		get_bus_clk() */
#define CFG_CLK_APB_BUS		get_apb_bus_clk()
#define	CFG_CLK_FIXED_3M	3000000
#define	CFG_CLK_FIXED_6M	6000000
#define	CFG_CLK_FIXED_24M	24000000

/*-----------------------------------------------------------------------
 * Flash Memory Configuration v100
 *----------------------------------------------------------------------*/
#define CONFIG_HIFMC

#ifdef CONFIG_HIFMC
	#define CONFIG_HIFMC_SPI_NOR
	#define CONFIG_HIFMC_SPI_NAND

	#define CONFIG_HIFMC_REG_BASE		FMC_REG_BASE
	#define CONFIG_HIFMC_BUFFER_BASE	FMC_MEM_BASE
	#define CONFIG_HIFMC_MAX_CS_NUM		1
#endif

#ifdef CONFIG_HIFMC_SPI_NOR
	#define CONFIG_CMD_SF
	#define CONFIG_ENV_IS_IN_SPI_FLASH
	#define CONFIG_SPI_NOR_MAX_CHIP_NUM	1
	#define CONFIG_SPI_NOR_QUIET_TEST
	#define CONFIG_CLOSE_SPI_8PIN_4IO
	#ifndef CONFIG_CLOSE_SPI_8PIN_4IO
		#define CONFIG_DTR_MODE_SUPPORT
	#endif
	#define CONFIG_SPI_BLOCK_PROTECT
#endif

#ifdef CONFIG_HIFMC_SPI_NAND
	#define CONFIG_CMD_NAND
	#define CONFIG_ENV_IS_IN_NAND
	#define CONFIG_SPI_NAND_MAX_CHIP_NUM	1
	#define CONFIG_SYS_MAX_NAND_DEVICE	CONFIG_SPI_NAND_MAX_CHIP_NUM
	#define CONFIG_SYS_NAND_MAX_CHIPS	CONFIG_SPI_NAND_MAX_CHIP_NUM
	#define CONFIG_SYS_NAND_BASE		FMC_MEM_BASE
	#define CONFIG_SYS_NAND_QUIET_TEST
	/* #define CONFIG_FS_MAY_NOT_YAFFS2 */

	#undef CONFIG_HIFMC100_HARDWARE_PAGESIZE_ECC
	#define CONFIG_HIFMC100_AUTO_PAGESIZE_ECC
	#undef CONFIG_HIFMC100_PAGESIZE_AUTO_ECC_NONE
#endif

/*-----------------------------------------------------------------------
 * for cpu/hi3516cv300/start.S
 *----------------------------------------------------------------------*/
#define FMC_TEXT_ADRS			(FMC_MEM_BASE)

#define MEM_BASE_DDR			(DDR_MEM_BASE)
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
/* size in bytes reserved for initial data */
#define CONFIG_SYS_GBL_DATA_SIZE	128

/*-----------------------------------------------------------------------
 * for timer configuration (udelay)
 * arch/arm/cpu/hi3516cv300/hi3516cv300/timer.c
 * enable timer				board/hi3516cv300/board.c
 *----------------------------------------------------------------------*/
#define CFG_TIMER_CLK		CFG_CLK_APB_BUS

#define CFG_TIMERBASE		TIMER0_REG_BASE
/* enable timer.32bit, periodic,mask irq,256 divider. */
#define CFG_TIMER_CTRL		0xCA
#define READ_TIMER \
	(*(volatile unsigned long *)(CFG_TIMERBASE + REG_TIMER_VALUE))
/* how many ticks per second. show the precision of timer. */
#define CONFIG_SYS_HZ		(CFG_TIMER_CLK / 256)
#define CFG_HZ			CONFIG_SYS_HZ

/*allow change env*/
#define  CONFIG_ENV_OVERWRITE
/*-----------------------------------------------------------------------
 * environment && bd_info  && global_data  configure
 * used in file
 *----------------------------------------------------------------------*/
#include "../../product/env_setup.h"
/* env in flash instead of CFG_ENV_IS_NOWHERE */
#define CONFIG_ENV_OFFSET	0x40000      /* environment starts here */
#define CONFIG_ENV_NAND_ADDR	(CONFIG_ENV_OFFSET)
#define CONFIG_ENV_SPI_ADDR	(CONFIG_ENV_OFFSET)
#define CONFIG_CMD_SAVEENV

#define CONFIG_ENV_SIZE		0x10000    /*include ENV_HEADER_SIZE */
#define CONFIG_ENV_SECT_SIZE	CONFIG_ENV_SIZE
#define CONFIG_NR_DRAM_BANKS	1          /* we have 1 bank of DRAM */
/* kernel parameter list phy addr */
#define CFG_BOOT_PARAMS		(MEM_BASE_DDR + 0x0100)

/*-----------------------------------------------------------------------
 *  Environment   Configuration
 *-----------------------------------------------------------------------*/
#define CONFIG_BOOTCOMMAND	"setenv bootargs $(bootargs) ethaddr=$(ethaddr) phyaddru=$(phyaddru) phyaddrd=$(phyaddrd) sensor=$(sensor) linux_cmd=$(linux_cmd); sf probe 0; sf read 0x80000000 0x50000 0x200000; bootm 0x80000000"
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTARGS		"totalmem=64M mem=40M sensor=ar0130 console=ttyAMA0,115200 root=/dev/mtdblock3 rootfstype=squashfs,jffs2 mtdparts=hi_sfc:256k(boot),64k(env),2048k(kernel),5120k(rootfs),-(rootfs_data)"
#define CONFIG_NETMASK  255.255.255.0       /* talk on MY local net */
#define CONFIG_IPADDR   192.168.1.10        /* static IP I currently own */
#define CONFIG_SERVERIP 192.168.1.254     /* current IP of tftp server ip */
#define CONFIG_ETHADDR  00:00:23:34:45:66
#define CONFIG_BOOTFILE		"uImage"        /* file to load */
#define CONFIG_BAUDRATE		115200
/*-----------------------------------------------------------------------
 * for bootm linux
 * used in file  board/hi3516cv300/board.c
 -----------------------------------------------------------------------*/

#define CONFIG_BOOTM_LINUX		1
/* default load address 0x80008000*/
#define CONFIG_SYS_LOAD_ADDR		(CFG_DDR_PHYS_OFFSET + 0x08000)
#define CONFIG_ZERO_BOOTDELAY_CHECK	1 /*use ^c to interrupt*/
/*-----------------------------------------------------------------------
 * for  commond configure
 -----------------------------------------------------------------------*/
/* tftp comm */
#define CONFIG_TFTP_TSIZE

/* do_printenv  do_setenv common/cmd_nvedit.c */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_SYS_MAXARGS		32 /* max number of command args */


#define CONFIG_CMD_LOADB  /* loadb common/cmd_load.c*/

/*-----------------------------------------------------------------------
 * network config
 -----------------------------------------------------------------------*/
#define CONFIG_ARP_TIMEOUT		50000UL
#define CONFIG_NET_RETRY_COUNT		50
#define CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_MII
#define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	1

/*-----------------------------------------------------------------------
 * HIETH driver
 -----------------------------------------------------------------------*/
/* default is hieth-switch-fabric */
#define CONFIG_NET_HISFV300
#ifdef CONFIG_NET_HISFV300
	#define HISFV_RESET_PHY_BY_CRG
	#define HISFV_MII_MODE				0
	#define HISFV_RMII_MODE				1
	#define HIETH_MII_RMII_MODE_U			HISFV_RMII_MODE
	#define HIETH_MII_RMII_MODE_D			HISFV_RMII_MODE
	#define HISFV_PHY_U				1
	#define HISFV_PHY_D				2 /* fix me */
#endif /* CONFIG_NET_HISFV300 */

/* no nor flash */
#define CONFIG_SYS_NO_FLASH
/* cp.b */
#define CONFIG_CMD_MEMORY  /*md mw cp etc.*/

/*-----------------------------------------------------------------------
 * console display  Configuration
 ------------------------------------------------------------------------*/

#define CONFIG_VERSION_VARIABLE	1 /*used in common/main.c*/
#define CONFIG_SYS_PROMPT	"OpenIPC # "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	1024            /* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE \
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CFG_LONGHELP
#define CFG_BARGSIZE		CFG_CBSIZE      /* Boot Argument Buffer Size */
#undef  CFG_CLKS_IN_HZ /* everything, incl board info, in Hz */
/* default load address */
#define CFG_LOAD_ADDR		(CFG_DDR_PHYS_OFFSET + 0x08000)
#define CONFIG_AUTO_COMPLETE	1
#define CFG_CMDLINE_HISTORYS	8
#define CONFIG_CMDLINE_EDITING
#define CFG_DDR_PHYS_OFFSET	MEM_BASE_DDR
#define CFG_DDR_SIZE		(512 * 1024 * 1024UL) /* 512MB */

#define CONFIG_SYS_MEMTEST_START \
	(CFG_DDR_PHYS_OFFSET + sizeof(unsigned long))
#define CONFIG_SYS_MEMTEST_END		(CFG_DDR_PHYS_OFFSET + CFG_DDR_SIZE)
#define CONFIG_SYS_MEMTEST_SCRATCH	CFG_DDR_PHYS_OFFSET

#define CONFIG_CMDLINE_TAG		1  /* enable passing of ATAGs */
#define CONFIG_INITRD_TAG		1  /* support initrd */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_MISC_INIT_R		1  /* call misc_init_r during startup */

#undef CONFIG_SPIID_TAG

/* serial driver */
#define CONFIG_PL011_SERIAL 1
/* Except bootrom, hi3516cv300 UART use fixde clk 24M */
#define CONFIG_PL011_CLOCK	CFG_CLK_FIXED_24M

#define CONFIG_PL01x_PORTS \
	{(void *)UART0_REG_BASE, (void *)UART1_REG_BASE, (void *)UART2_REG_BASE}
#define CONFIG_CONS_INDEX	0

#define CFG_SERIAL0		UART0_REG_BASE

/* according to CONFIG_CONS_INDEX */
#define CONFIG_CUR_UART_BASE	CFG_SERIAL0
#define CONFIG_PRODUCTNAME	"hi3516cv300"

/*-----------------------------------------------------------------------
 * bootrom Configuration
 -----------------------------------------------------------------------*/
#define CONFIG_BOOTROM_SUPPORT
#if defined(CONFIG_BOOTROM_SUPPORT) \
	&& (!defined(REG_START_FLAG) || !defined(CONFIG_START_MAGIC))
#error Please define CONFIG_START_MAGIC and CONFIG_START_MAGIC first
#endif

/*-----------------------------------------------------------------------
 * sdcard or usb storage system update
 * ----------------------------------------------------------------------*/
#define CONFIG_AUTO_UPDATE			1
#ifdef CONFIG_AUTO_UPDATE
	#define CONFIG_AUTO_SD_UPDATE		1
/*	#define CONFIG_AUTO_USB_UPDATE		1 */
#endif

/*-----------------------------------------------------------------------
 * sdcard or  eMMC Flash Configuration
 * ----------------------------------------------------------------------*/
#define CONFIG_EMMC_SUPPORT
#if defined(CONFIG_AUTO_SD_UPDATE) || defined(CONFIG_EMMC_SUPPORT)
	#define CONFIG_HIMCI_V200
#endif

#ifdef CONFIG_HIMCI_V200
	#define CONFIG_MMC_DEVID		0 /* emmc = 1; SD = 0 */
	#define CONFIG_MMC_BOOT_ADDR		0
	#define CONFIG_MMC_POWER_OFF_TIMEOUT	5
	#define CONFIG_MMC_POWER_ON_TIMEROUT	40
	#define CONFIG_MMC_RESET_LOW_TIMEOUT	10
	#define CONFIG_MMC_RESET_HIGH_TIMEROUT	300
	#define CONFIG_GENERIC_MMC
#endif /* CONFIG_HIMCI_V200 */

#ifdef CONFIG_GENERIC_MMC
	/* env in flash instead of CFG_ENV_IS_NOWHERE */
	#define CONFIG_ENV_IS_IN_EMMC		1
	#define CONFIG_CMD_MMC
	#define CONFIG_MMC			1
	#define CONFIG_EXT4
	#define CONFIG_EXT4_SPARSE
#endif /* CONFIG_GENERIC_MMC */

/*-----------------------------------------------------------------------
 * usb
 * ----------------------------------------------------------------------*/
#if defined(CONFIG_AUTO_USB_UPDATE)
#define CONFIG_CMD_USB			1
#define CONFIG_USB_OHCI			1 /* FIXME: CONFIG_USB_OHCI_NEW */
#define CONFIG_USB_STORAGE		1
#define CONFIG_LEGACY_USB_INIT_SEQ
#endif

#define CONFIG_HIUDC
#ifdef CONFIG_HIUDC
#define CONFIG_USBDEV_REV_2_94a               0x4F54294A
#define CONFIG_USBDEV_REV_3_00a               0x4F54300A
#endif

#if (defined(CONFIG_CMD_USB) || defined(CONFIG_MMC))
	#define CONFIG_DOS_PARTITION			1
	#define CONFIG_CMD_FAT				1
	#define CONFIG_CMD_EXT2				1
#endif

/*-----------------------------------------------------------------------
 * Snapshot boot support
 * ----------------------------------------------------------------------*/
#define CONFIG_SNAPSHOT_BOOT		1

#ifdef CONFIG_SNAPSHOT_BOOT
/* #define CONFIG_ARCH_MMU */ /* enable MMU for fast decompress */
#define CONFIG_SHA1
#define CONFIG_HW_DEC
#endif

/*-----------------------------------------------------------------------
 * SVB Configure
 * ----------------------------------------------------------------------*/
#define CONFIG_SVB_ENABLE

/*-----------------------------------------------------------------------
 * DDR Training
 * ----------------------------------------------------------------------*/
#define CONFIG_DDR_TRAINING_V2
/* #define CONFIG_DDR_WRITE_2T_PRE_ENABLE */

/* #define CONFIG_OSD_ENABLE */

/*#define CONFIG_AUDIO_ENABLE*/

/* #define CONFIG_PRODUCT_HDMI_PHY */

#define CONFIG_USE_ARCH_MEMCPY
#define CONFIG_USE_ARCH_MEMSET

/* #define CONFIG_USE_IRQ */

/* #define __LITTLE_ENDIAN	1 */

#define CONFIG_CMD_UGZIP 1

#endif	/* __CONFIG_H */
