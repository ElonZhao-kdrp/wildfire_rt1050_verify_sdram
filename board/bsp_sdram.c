/**
  ******************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SDRAM����
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"  
#include "fsl_iomuxc.h"
#include "fsl_semc.h"  
	
#include "pad_config.h"  
#include "bsp_sdram.h"

/*******************************************************************************
 * ��
 ******************************************************************************/

/* �������ž�ʹ��ͬ����PAD���� */
#define SDRAM_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                            DSE_7_R0_7| \
                                            SPEED_3_MAX_200MHz| \
                                            ODE_0_OPEN_DRAIN_DISABLED| \
                                            PKE_1_PULL_KEEPER_ENABLED| \
                                            PUE_0_KEEPER_SELECTED| \
                                            PUS_0_100K_OHM_PULL_DOWN| \
                                            HYS_1_HYSTERESIS_ENABLED)   
    /* ����˵�� : */
    /* ת������: ת�����ʿ�
        ����ǿ��: R0/7 
        �������� : max(200MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 100Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ʹ�� */



/*******************************************************************************
 * ����
 ******************************************************************************/
static void SDRAM_IOMUXC_MUX_Config(void);
static void SDRAM_IOMUXC_PAD_Config(void);
static status_t SDRAM_SEMC_Config(void);


/**
* @brief  ��ʼ��SDRAM���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void SDRAM_IOMUXC_MUX_Config(void)
{
  /* ��DQS�ź��⣬�������ž��ɲ�����SION���� */
  /* DATA�ź���DATA00~DATA15 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_SEMC_DATA00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_SEMC_DATA01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_SEMC_DATA02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_SEMC_DATA03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_SEMC_DATA04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_SEMC_DATA05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_SEMC_DATA06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_SEMC_DATA07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_30_SEMC_DATA08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_SEMC_DATA09, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_SEMC_DATA10, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_33_SEMC_DATA11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_34_SEMC_DATA12, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_35_SEMC_DATA13, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_36_SEMC_DATA14, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_37_SEMC_DATA15, 0U);  
  
  /* DQS�ź��ߣ�����ʹ��SION���� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_39_SEMC_DQS, 1U);  

  /* ADDR�ź���ADDR00~ADDR12 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_23_SEMC_ADDR10, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, 0U); 

  /* BA0��BA1�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_21_SEMC_BA0, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_22_SEMC_BA1, 0U);  

  /* RAS��CAS�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_25_SEMC_RAS, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_24_SEMC_CAS, 0U);   

  /* WE��CS0�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_28_SEMC_WE, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_29_SEMC_CS0, 0U);    

  /* DM0��DM1�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_08_SEMC_DM00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_38_SEMC_DM01, 0U);    
 
  /* CLK��CKE�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_26_SEMC_CLK, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_27_SEMC_CKE, 0U);  
}

/**
* @brief  ��ʼ��SDRAM���IOMUXC��PAD��������
* @param  ��
* @retval ��
*/
static void SDRAM_IOMUXC_PAD_Config(void)
{
  /* �������ž�ʹ��ͬ����PAD���� */
   /* DATA�ź���DATA00~DATA15 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_SEMC_DATA00, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_SEMC_DATA01, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_SEMC_DATA02, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_SEMC_DATA03, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_SEMC_DATA04, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_SEMC_DATA05, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_SEMC_DATA06, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_SEMC_DATA07, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_30_SEMC_DATA08, SDRAM_PAD_CONFIG_DATA);         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_31_SEMC_DATA09, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_32_SEMC_DATA10, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_33_SEMC_DATA11, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_34_SEMC_DATA12, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_35_SEMC_DATA13, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_36_SEMC_DATA14, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_37_SEMC_DATA15, SDRAM_PAD_CONFIG_DATA);                             
  
  /* DQS�ź��� */
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_39_SEMC_DQS, SDRAM_PAD_CONFIG_DATA);    
  
  /* ADDR�ź���ADDR00~ADDR12 */               
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_23_SEMC_ADDR10, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, SDRAM_PAD_CONFIG_DATA);  
  
  /* BA0��BA1�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_21_SEMC_BA0, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_22_SEMC_BA1, SDRAM_PAD_CONFIG_DATA);                            
  
  /* RAS��CAS�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_25_SEMC_RAS, SDRAM_PAD_CONFIG_DATA);                          
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_24_SEMC_CAS, SDRAM_PAD_CONFIG_DATA);  
  
  /* WE��CS0�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_28_SEMC_WE, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_29_SEMC_CS0, SDRAM_PAD_CONFIG_DATA);  

  /* DM0��DM1�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_08_SEMC_DM00, SDRAM_PAD_CONFIG_DATA);                                     
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_38_SEMC_DM01, SDRAM_PAD_CONFIG_DATA); 

  /* CLK��CKE�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_26_SEMC_CLK, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_27_SEMC_CKE, SDRAM_PAD_CONFIG_DATA);    
}

/**
* @brief  ��ʼ��SDRAM��ص�SEMC����
* @param  ��
* @retval ��
*/
static status_t SDRAM_SEMC_Config(void)
{  
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;

    /* �ṹ�������ȫ������ */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    /* ʹ��Ĭ�����ó�ʼ�� SEMC. */
    SEMC_GetDefaultConfig(&config);
    
    /* ��ע�⣡�˴�����ʹ�����ģʽ��
       ����SDRAM����ʱ�Ӳ����ܵ���Ƶ�� */
    config.dqsMode = kSEMC_Loopbackdqspad;
    SEMC_Init(SEMC, &config);

    /* ����SDRAM */
    /* ѡ��Ƭѡ���ţ���Ӧ��ʹ��CS0����������Ч */
    sdramconfig.csxPinMux = kSEMC_MUXCSX0;    
    /* ��SDRAMӳ��Ļ���ַ */
    sdramconfig.address = 0x80000000;       
    /* SDRAM��������С����λΪKBytes, 32MB = 32*1024*1KBytes*/
    sdramconfig.memsize_kbytes = 32 * 1024; 
    /* SDRAM�������߿�� */
    sdramconfig.portSize = kSEMC_PortSize16Bit;
    /* ͻ����д����󳤶� */
    sdramconfig.burstLen = kSEMC_Sdram_BurstLen8;
    /* �е�ַ��� */
    sdramconfig.columnAddrBitNum = kSEMC_SdramColunm_9bit;
    /* CAS Latency */
    sdramconfig.casLatency = kSEMC_LatencyTwo;
    /* Ԥ���������Ч��ʱ�� Trp */
    sdramconfig.tPrecharge2Act_Ns = 15;   
    /* ����Ч����д��ʱ�� Trcd */
    sdramconfig.tAct2ReadWrite_Ns = 15;   
    /* ˢ�»ָ�ʱ�䣬ʹ��Trfc��Txsr�нϴ��һ��ֵ */
    sdramconfig.tRefreshRecovery_Ns = 67; 
    /* д�ָ�ʱ�䣬2 ��Tclkʱ�� */
    sdramconfig.tWriteRecovery_Ns = 2 * (1000000000 / clockFrq);    
    /* CKE off ����Сʱ�䣬1 ��Tclkʱ�� */
    sdramconfig.tCkeOff_Ns = 1 * (1000000000 / clockFrq);
    /* ����Ч��Ԥ���ʱ�䣬Tras */
    sdramconfig.tAct2Prechage_Ns = 42; 
    /* ��ˢ�µĻָ�ʱ�䣬ȡtRefreshRecovery_Nsͬ����ֵ */
    sdramconfig.tSelfRefRecovery_Ns = 67;
    /* ����ˢ������֮���ʱ�� */
    sdramconfig.tRefresh2Refresh_Ns = 60;
    /* ��������Ч����֮���ʱ�� */
    sdramconfig.tAct2Act_Ns = 60;
    /* ��Ƶ����ʱ���ڣ�160��Tclkʱ�� */
    sdramconfig.tPrescalePeriod_Ns = 160 * (1000000000 / clockFrq); 
    /* ˢ����������ڣ������ˢ��һ�У�64ms/8192  */
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; 
    /* ����ˢ����ֵ */
    sdramconfig.refreshUrgThreshold = sdramconfig.refreshPeriod_nsPerRow;
    /* ˢ�������ͻ������ */
    sdramconfig.refreshBurstLen = 1;
    
    /* ʹ���������ó�ʼ������ʹ��CS0��������Region 0���� */
    return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}

/**
* @brief  ��ʼ��SDRAM
* @param  ��
* @retval ��
*/
status_t SDRAM_Init(void)
{  
  /* ����SDRAMʱ�� */
  /* 0��Periph_clk
     1��SEMC alternative clock
     ʹ�� SEMC alternative clock */
  CLOCK_SetMux(kCLOCK_SemcMux, 1);
  /* 0��PLL2 PFD2
     1��PLL3 PFD1
     alternative clock ʹ��PLL2 PFD2 */
  CLOCK_SetMux(kCLOCK_SemcAltMux, 0);  
  /* ��Ƶ��õ�SEMC_CLK_ROOT��
     SEMC_CLK_ROOT = PLL2 PFD2/(1+1) */
  CLOCK_SetDiv(kCLOCK_SemcDiv, 1);

  SDRAM_IOMUXC_MUX_Config();
  SDRAM_IOMUXC_PAD_Config();
  return SDRAM_SEMC_Config();
}

/*********************************************END OF FILE**********************/
