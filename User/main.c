/**
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_uart4.h"
#include "bsp_uart5.h"
#include "bsp_SysTick.h"
#include "dma.h"
#include "iwdg.h"
#include "SPI.h"
#include "delay.h"
#include "timers.h"
#include "timer4_cap.h"
#include "NVIC_CONFIG.H"
#include "cJSON.h"
#include "stm_flash.h"
#include "PWM.h"
#include "CommunicationConfig.h"
#include "CommunicationProtocol.h"

#include "ESP8266_config.h"
#include "ESP8266_function.h"
#include "oled.h"
#include "dht11.h"
#include "adc.h"
#include "MQ-2.h"
#include "HC-SR501.h"
#include "HC-SR04.h"
#include "BH1750.h"
#include "RC522.h"
#include "E30TTLUART.h"
#include "SDS01.h"
#include "SHT15.h"
#include "T6603.h"
#include "W5500.h"
#include "Beep.h"

#include "contiki-conf.h"
#include <stdint.h>
#include <stdio.h>
#include <debug-uart.h>
#include <process.h>
#include <procinit.h>
#include <etimer.h>
#include <autostart.h>
#include <clock.h>
#include "contiki_delay.h"
#include "ProcessTask.h"


void BSP_Config(void)
{
    /* ��ʼ�� */
    delay_init();
    clock_init();
    srand(STMFLASH_Read_OneWordData(STM32_FLASH_END_PAGE));
    STMFLASH_Write_OneWordData(STM32_FLASH_END_PAGE,rand());
    NVIC_Configuration_Init();
    USART1_Config(921600);
    TIM1_PWM_Init();
//    TIM1_CH1_PWM_Init();
    printf("Start Contiki OS\r\n");

#ifdef __LED_BLINK_ON__
    LED_GPIO_Config(); 
#endif

#ifdef __Beep_MODULE_ON__
    Beep_GPIO_Config(); 
#endif   
    
#ifdef __OLED_MODULE_ON__
    OLED_Init(); //��ʼ��OLEDģ��ʹ�õĽӿں�����
    OLED_ShowString(0 * 8,0," IOT of RFT");
    OLED_ShowString(0,1 * 16,"Tempe:");OLED_ShowString(14 * 8,1 * 16,"`C");
    OLED_ShowString(0,2 * 16,"Humid:");OLED_ShowString(15 * 8,2 * 16,"%");
    OLED_ShowString(0,3 * 16,"Light:");OLED_ShowString(13 * 8,3 * 16,"Lux");
    OLED_Refresh_Gram();//������ʾ
#endif         
    
#ifdef __DHT11_MODULE_ON__
    DHT11_Init(); 
#endif

#ifdef __MQ02_MODULE_ON__
    MQ02_Init();
#endif

#ifdef __MQ135_MODULE_ON__
    MQ135_Init();
#endif

#ifdef __HCSR501_MODULE_ON__
    HCSR501_Init();
#endif

#ifdef __HCSR04_MODULE_ON__
	UltrasonicWave_Configuration();               //�Գ�����ģ���ʼ��
#endif

#ifdef __BH1750_MODULE_ON__
	BH1750_Init();
	BH1750_Start();
#endif

#ifdef __RC522_MODULE_ON__
	RC522_Init();
#endif

#ifdef __SHT15_MODULE_ON__
	SHT15_Init();
#endif

#ifdef __T6603_MODULE_ON__
	T6603_Init();
#endif

#ifdef __SDS01_MODULE_ON__
	SDS01_Init();
#endif

#ifdef __W5500_MODULE_ON__
    W5500_Init();
#endif    

#ifdef __WIFI_MODULE_ON__
    WiFi_Config();
    ESP8266_STA_TCP_Client();
    ChangeUSART2ReceiveMode();// �رմ���2�����ж� ʹ�ܴ���2�����ж� 
#endif  

#ifdef __E30TTLUART_MODULE_ON__
		E30TTLUART_Init();
    E30TTLUART_MultiCountConfig(0x0002,0x50,DISABLE,5);		
    printf("E30-TTL-100 OK.\r\n");
#endif

}


int main(void)
{
    BSP_Config();    
    printf("I LOVE U.\r\n");
    IWDG_Start(2);  // wifiģ��͸��֮�������Ź�
    process_init();
    autostart_start(autostart_processes);

#ifdef __LED_BLINK_ON__
    process_start(&red_blink_process,NULL);
    process_start(&green_blink_process,NULL);
#endif    

#ifdef __Beep_MODULE_ON__
      process_start(&Beep_alert_process,NULL);
#endif    
    
#ifdef __CJSON_LIB_TEST__

    process_start(&cJSON_test_process,NULL);
#endif

#ifdef __COMMUNICAT_PROTOCOL__
    process_start(&Communication_Protocol_Load_process,NULL);
    process_start(&Communication_Protocol_Send_process,NULL);
#endif  
        
#ifdef __CLOCK_TICK_TEST__
    process_start(&clock_test_process,NULL);
#endif    

#ifdef __OLED_MODULE_ON__
    process_start(&OLED_Show_Increment_process,NULL);
#endif
    
#ifdef __DHT11_MODULE_ON__
    process_start(&DHT11_Read_Data_process,NULL);
#endif   

#ifdef __WIFI_MODULE_TEST__     
    process_start(&wifi_send_test_process,NULL);
#endif
    
#ifdef __MQ02_MODULE_ON__     
    process_start(&MQ02_Read_Value_process,NULL);
#endif

#ifdef __MQ135_MODULE_ON__     
    process_start(&MQ135_Read_Value_process,NULL);
#endif

#ifdef __HCSR501_MODULE_ON__     
    process_start(&HCSR501_Read_Status_process,NULL);
#endif

#ifdef __T6603_MODULE_ON__
	process_start(&T6603_Read_CO2_PPM_process,NULL);
#endif

#ifdef __HCSR04_MODULE_ON__     
    process_start(&HCSR04_Measure_Distance_process,NULL);
#endif

#ifdef __BH1750_MODULE_ON__     
    process_start(&BH1750_Measure_Lumen_process,NULL);
#endif

#ifdef __RC522_MODULE_ON__     
    process_start(&RC522_Read_Card_process,NULL);
#endif

#ifdef __SHT15_MODULE_ON__
	process_start(&SHT15_Read_DATA_Value_process,NULL);
#endif

#ifdef __SDS01_MODULE_ON__
	process_start(&SDS01_Read_PM_Value_process,NULL);
#endif

#ifdef __COMMUNICAT_PROTOCOL_SENSOR_DATA__
    process_start(&CommunicatProtocol_Send_Sensor_Data,NULL);
#endif

#ifdef __W5500_SEND_TEST_ON__
    process_start(&W5500_send_test_process,NULL);
#endif

    while (1)
    {
        do
        {
#ifdef __W5500_MODULE_ON__
            W5500_Daemon_Process();
#endif  
        }while (process_run()>0);
    }
}





