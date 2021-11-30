/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_semc.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC SEMC
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)

#define SEMC_EXAMPLE_DATALEN 		800*480*2  //(0x1000U)

AT_NONCACHEABLE_SECTION( uint32_t EXAMPLE_SEMC_START_ADDRESS[SEMC_EXAMPLE_DATALEN] );

// LED define
#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN


#if defined SEL_BOARD_K2500UI_HYPERFLASH
#define HELLO_BOARD_STRING "K2500 UI BOARD with Hyper flash starting up"
#elif defined SEL_BOARD_WILDFIRE
#define HELLO_BOARD_STRING "WILD FIRE BOARD with QSPI flash starting up"
#elif  defined SEL_BOARD_NXPEVB_HYPERFLASH
#define HELLO_BOARD_STRING "NXP EVK BOARD with Hyper flash starting up"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);
static bool SEMC_SDRAMReadWrite32Bit(void);
static bool SEMC_SDRAMReadWrite16Bit(void);
static bool SEMC_SDRAMReadWrite8Bit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
status_t BOARD_InitSEMC(void)
{
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    /* Initialize SEMC. */
    SEMC_GetDefaultConfig(&config);
    config.dqsMode = kSEMC_Loopbackdqspad;  /* For more accurate timing. */
    SEMC_Init(SEMC, &config);

    /* Configure SDRAM. */
    sdramconfig.csxPinMux = kSEMC_MUXCSX0;
    sdramconfig.address = 0x80000000;
    sdramconfig.memsize_kbytes = 32 * 1024; /* 32MB = 32*1024*1KBytes*/
    sdramconfig.portSize = kSEMC_PortSize16Bit;
    sdramconfig.burstLen = kSEMC_Sdram_BurstLen8;
    sdramconfig.columnAddrBitNum = kSEMC_SdramColunm_9bit;
    sdramconfig.casLatency = kSEMC_LatencyThree;
    sdramconfig.tPrecharge2Act_Ns = 18;   /* Trp 18ns */
    sdramconfig.tAct2ReadWrite_Ns = 18;   /* Trcd 18ns */
    sdramconfig.tRefreshRecovery_Ns = 67; /* Use the maximum of the (Trfc , Txsr). */
    sdramconfig.tWriteRecovery_Ns = 12;   /* 12ns */
    sdramconfig.tCkeOff_Ns = 42;  /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
    sdramconfig.tAct2Prechage_Ns = 42; /* Tras 42ns */
    sdramconfig.tSelfRefRecovery_Ns = 67;
    sdramconfig.tRefresh2Refresh_Ns = 60;
    sdramconfig.tAct2Act_Ns = 60;
    sdramconfig.tPrescalePeriod_Ns = 160 * (1000000000 / clockFrq);
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
    sdramconfig.refreshUrgThreshold = sdramconfig.refreshPeriod_nsPerRow;
    sdramconfig.refreshBurstLen = 1;
    return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}


/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Init output LED GPIO. */
    GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);

    PRINTF("\r\n\r\n %s, compile date %s %s.\r\n", HELLO_BOARD_STRING, __DATE__, __TIME__);

    PRINTF( "SDRAM Write/Read testing... Data address 0x%X; length: %d (uint32_t)\r\n", EXAMPLE_SEMC_START_ADDRESS, SEMC_EXAMPLE_DATALEN);

#if 0
    if (BOARD_InitSEMC() != kStatus_Success)
    {
    	PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }
#endif



    while (1)
    {
    	static uint32_t count_success = 0;
    	static uint32_t count_failed = 0;
    	bool ret, ret1, ret2;

        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    	GPIO_PortToggle(EXAMPLE_LED_GPIO, 1u << EXAMPLE_LED_GPIO_PIN);

        /* 32Bit data read and write. */
    	ret = SEMC_SDRAMReadWrite32Bit();
        /* 16Bit data read and write. */
    	ret1 = SEMC_SDRAMReadWrite16Bit();
        /* 8Bit data read and write. */
    	ret2 = SEMC_SDRAMReadWrite8Bit();

    	if ((ret == true) && (ret1 == true) && (ret2 == true))
    	{
    		count_success++;
    	}
    	else
    	{
    		count_failed++;
    	}
    	//PRINTF("%d-%d-%d\r\n", ret, ret1, ret2);
    	PRINTF("count: %d - %d\r\n", count_success, count_failed);
    }
}

#define	PRINTF_SDRAM


bool SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint32_t *sdram = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF_SDRAM("\r\n SEMC SDRAM Memory 32 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
    	sdram[index] = index;
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM Read 32 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        if (index != sdram[index])
        {
            result = false;
            break;
        }
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Start!\r\n");

    if (!result)
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Succeed!\r\n");
    }

    return result;
}

static bool SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN * 2;
    uint16_t *sdram = (uint16_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    uint16_t count;
    bool result = true;

    PRINTF_SDRAM("\r\n SEMC SDRAM Memory 16 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    /* Prepare data and write to SDRAM. */
    for (index = 0, count = 0; index < datalen; index++, count++)
    {
    	sdram[index] = count;
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM Read 16 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0, count = 0; index < datalen; index++, count++)
    {
        if (count != sdram[index])
        {
            result = false;
            break;
        }
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Start!\r\n");

    if (!result)
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Succeed!\r\n");
    }

    return result;
}

static bool SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN*4;
    uint8_t *sdram = (uint8_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    uint8_t count;
    bool result = true;

    PRINTF_SDRAM("\r\n SEMC SDRAM Memory 8 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    /* Prepare data and write to SDRAM. */
    for (index = 0, count = 0; index < datalen; index++, count++)
    {
    	sdram[index] = count;
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM Read 8 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0, count = 0; index < datalen; index++, count++)
    {
        if (count != sdram[index])
        {
            result = false;
            break;
        }
    }

    PRINTF_SDRAM("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Start!\r\n");

    if (!result)
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
    	PRINTF_SDRAM("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Succeed!\r\n");
    }

    return result;
}
