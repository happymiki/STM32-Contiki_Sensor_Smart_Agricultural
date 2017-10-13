#include "ProcessTask.h"


//PROCESS(red_blink_process, "Red Blink");
PROCESS(green_blink_process, "Green Blink");
PROCESS(IWDG_Feed_process, "Timing to feed dog");
PROCESS(clock_test_process, "Test system delay");
PROCESS(cJSON_test_process, "Test cJSON Lib");
PROCESS(Communication_Protocol_Send_process, "Communication protocol send packet serviced");
PROCESS(Communication_Protocol_Load_process, "Communication protocol load bytes to packet serviced");
PROCESS(CommunicatProtocol_Send_Sensor_Data, "Communication protocol send sensor data");


PROCESS(wifi_send_test_process, "Wifi module send data test");
PROCESS(OLED_Show_Increment_process, "Show a increment num in OLED");
PROCESS(DHT11_Read_Data_process, "DHT11 read temperature and humidity test");
PROCESS(MQ02_Read_Value_process, "ADC read MQ02 value and print test");
PROCESS(MQ135_Read_Value_process, "ADC read MQ135 value and print test");
PROCESS(HCSR501_Read_Status_process, "Read status of is anyone here");
PROCESS(HCSR04_Measure_Distance_process, "Measure distance with HC-SR04 UltrasonicWave rangefinder");
PROCESS(BH1750_Measure_Lumen_process, "Measure lumen with BH1750 Light Sensor");
PROCESS(RC522_Read_Card_process, "Read card ID and data with RC522 RFID");
PROCESS(SDS01_Read_PM_Value_process, "Get PM2.5 and PM10 data with SDS01");
PROCESS(SHT15_Read_DATA_Value_process, "SHT15 read accurate temperature and humidity");
PROCESS(T6603_Read_CO2_PPM_process, "T6603-5 read CO2 PPM value");
PROCESS(W5500_send_test_process, "Test W5500 module send data");
PROCESS(Beep_alert_process, "Beep alert process");

AUTOSTART_PROCESSES(&etimer_process,&IWDG_Feed_process);

/* �˴�Ӧ����ҳ���û���Ϣ=�����������͵�������ƥ�� */
float temperatureGlobalData;
float humidityGlobalData;
float SHT15_AccurateTemperatureGlobalData;
float SHT15_AccurateHumidityGlobalData;
float smogPercentageGlobalData;
bool someoneStatusGlobalData;
float AirQualityPercentageGlobalData;
uint16_t distanceGlobalData;
float lightIntensityGlobalData;
uint32_t CardID_GlobalData;
float PM2_5_GlobalData;
float PM10_GlobalData;
uint16_t CO2_PPM_Value_GlobalData;

/*******************PROCESS************************/

//PROCESS_THREAD(red_blink_process, ev, data)
//{
//    static struct etimer et;
//    PROCESS_BEGIN();
//    while(1)
//    {
//        Contiki_etimer_DelayMS(500);
//        LED_Red_Off();
//        Contiki_etimer_DelayMS(500);
//        LED_Red_On();
//    }
//    PROCESS_END();
//}

PROCESS_THREAD(green_blink_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {        
        Contiki_etimer_DelayMS(200);
        LED_Green_Off();
        Contiki_etimer_DelayMS(200);
        LED_Green_On();
    }
    PROCESS_END();
}

PROCESS_THREAD(Beep_alert_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {   
        if(beep_flag)
        {
            Beep_On();
        }
        Contiki_etimer_DelayMS(500);
        Beep_Off();
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(wifi_send_test_process, ev, data)
{
    uint8_t *USART2_SendBuff;
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        
        Contiki_etimer_DelayMS(500);
        USART2_SendBuff = " asdfafasdf";
        SendUSART2BytesBuf(USART2_SendBuff, 11);
        
        Contiki_etimer_DelayMS(500);        
        USART2_SendBuff = " 21398416hy";
        SendUSART2BytesBuf(USART2_SendBuff, 11);
    }
    PROCESS_END();
}

PROCESS_THREAD(W5500_send_test_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        W5500_Push_Socket0_SendDataIntoFIFO("Hello W5500 is run!\r\n", 21);
        W5500_Push_Socket0_SendDataIntoFIFO(" Love Live Rewrite Fate/Zreo Angel Beats!\r\n", 43);
        W5500_Push_Socket0_SendDataIntoFIFO("ABBB1234BB345634BBBBCC\r\n", 24);
        W5500_Push_Socket0_SendDataIntoFIFO("Kggggg5678ggggggggggPP\r\n", 24);
        W5500_Push_Socket0_SendDataIntoFIFO("TianTianTianMaoMaoMaoMao\r\n", 26);
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(OLED_Show_Increment_process, ev, data)
{
    static struct etimer et;
    static uint32_t count = 0;
    PROCESS_BEGIN();
    while(1)
    {
#ifdef __DHT11_MODULE_ON__
        OLED_ShowNum(6 * 8,1 * 16, (int)temperatureGlobalData,5,16); 
        OLED_ShowNum(6 * 8,2 * 16, (int)humidityGlobalData,5,16); 
#endif
        
#ifdef __BH1750_MODULE_ON__
        OLED_ShowNum(6 * 8,3 * 16, (int)lightIntensityGlobalData,5,16);
#endif
        OLED_ShowNum(11 * 8,0,count++,5,16);  
        OLED_Refresh_Gram();//������ʾ
        Contiki_etimer_DelayMS(1000);
    }
    PROCESS_END();
}

PROCESS_THREAD(DHT11_Read_Data_process, ev, data)
{
    static struct etimer et;
    uint8_t temperature;
    uint8_t temperature0;	 
		uint8_t humidity;    
    uint8_t humidity0;
    PROCESS_BEGIN();
    while(1)
    {
        DHT11_Read_Data(&temperature,&temperature0,&humidity,&humidity0);
        temperatureGlobalData = (float)temperature+(float)temperature0*0.1;
        humidityGlobalData = (float)humidity+(float)humidity0*0.1;
        
        if(temperatureGlobalData > 40)
        {
            SetBeepAlertSource(TemperatureAlertSource);
        }else
        {
            ResetBeepAlertSource(TemperatureAlertSource);
        }
        
//        printf("temperature: %.2f��C  humidity: %.2f \r\n",(float)temperature+(float)temperature0*0.01,(float)humidity+(float)humidity0*0.01);	
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(SHT15_Read_DATA_Value_process, ev, data)
{
    u16 humi_val, temp_val;
    u8 err = 0, checksum = 0;
    float humi_val_real = 0.0; 
    float temp_val_real = 0.0;
    float dew_point = 0.0;
    static struct etimer et;
    
    PROCESS_BEGIN();
    while(1)
    {
        err += SHT15_Measure(&temp_val, &checksum, TEMP);                  //��ȡ�¶Ȳ���ֵ
        
        if(err != 0)
        {
            SHT15_ConReset();
            printf("SHT15 Err\r\n");
        }
        else
        {
            err += SHT15_Measure(&humi_val, &checksum, HUMI);                  //��ȡʪ�Ȳ���ֵ
            if(err != 0)
            {
                SHT15_ConReset();
                printf("SHT15 Err\r\n");
            }
            else
            {
                SHT15_Calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //����ʵ�ʵ���ʪ��ֵ
                dew_point = SHT15_CalcuDewPoint(temp_val_real, humi_val_real);       //����¶���¶�
                SHT15_AccurateTemperatureGlobalData = temp_val_real;
                SHT15_AccurateHumidityGlobalData = humi_val_real;
            }
        }	
        Contiki_etimer_DelayMS(1000);
    }
    PROCESS_END();
}

PROCESS_THREAD(MQ02_Read_Value_process, ev, data)
{
    static struct etimer et;
    float smogPercentage;
    PROCESS_BEGIN();
    while(1)
    {
        smogPercentage = MQ02_Get_Percentage();
        smogPercentageGlobalData = smogPercentage;
//        printf("MQ02 Sensor Percentage: %.2f \r\n",MQ02_Get_Percentage());	
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(MQ135_Read_Value_process, ev, data)
{
    static struct etimer et;
    float AirQualityPercentage;
    PROCESS_BEGIN();
    while(1)
    {
        AirQualityPercentage = MQ135_Get_Percentage();
        AirQualityPercentageGlobalData = AirQualityPercentage;
//        printf("MQ02 Sensor Percentage: %.2f \r\n",MQ02_Get_Percentage());	
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(HCSR501_Read_Status_process, ev, data)
{
    static struct etimer et;
    bool someoneStatus;
    PROCESS_BEGIN();
    while(1)
    {
        someoneStatus = HCSR501_Read_Data();
        someoneStatusGlobalData = someoneStatus;
//        if(someoneStatus)
//        {
//            printf("HCSR501 monitor people.\r\n");
//        }
//        else
//        {
//            printf("HCSR501 haven't monitor people.\r\n");
//        }
        Contiki_etimer_DelayMS(500);
    }
    PROCESS_END();
}

PROCESS_THREAD(HCSR04_Measure_Distance_process, ev, data)
{
    static struct etimer et;
    uint16_t distance;
    PROCESS_BEGIN();
    while(1)
    {
        UltrasonicWave_StartMeasure();
        Contiki_etimer_DelayMS(200);
        distance = UltrasonicWave_GetDistance();
        distanceGlobalData = distance;
//        printf("Ultrasonic distance : %d cm\r\n",UltrasonicWave_GetDistance());	//
    }
    PROCESS_END();
}

PROCESS_THREAD(BH1750_Measure_Lumen_process, ev, data)
{
    float lightIntensity;
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        Contiki_etimer_DelayMS(500);
        lightIntensity = BH1750_GetLumen();
        lightIntensityGlobalData = lightIntensity;
        if(lightIntensityGlobalData > 3000)
        {
            SetBeepAlertSource(LightAlertSource);
        }else
        {
            ResetBeepAlertSource(LightAlertSource);
        }
    }
    PROCESS_END();
}

PROCESS_THREAD(SDS01_Read_PM_Value_process, ev, data)
{
    float PM2_5_Value = 0;
    float PM10_Value = 0;
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
				// printf("I LOVE.\r\n");
        SDS01_LoadReceiveQueueByteToPacketBlock();
        PM2_5_Value = SDS01_getPM2_5_Value();
        PM10_Value = SDS01_getPM10_Value();
        PM2_5_GlobalData = PM2_5_Value;
        PM10_GlobalData = PM10_Value;
        if(PM2_5_GlobalData > 310)
        {
            SetBeepAlertSource(PM2_5_AlertSource);
        }else
        {
            ResetBeepAlertSource(PM2_5_AlertSource);
        }
        
        Contiki_etimer_DelayMS(2000);
    }
    PROCESS_END();
}

PROCESS_THREAD(RC522_Read_Card_process, ev, data)
{
    int8_t status;
    uint8_t CardClassAndIDBuf[4];
    uint32_t CardID;
    static uint32_t LastCardID;
    const uint32_t PermitCardID[] = {1};
    static uint16_t count = 0;
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        status = PcdRequest(PICC_REQALL,CardClassAndIDBuf);/*ɨ�迨*/
        if(status!=MI_OK)
        {
            status = PcdRequest(PICC_REQALL,CardClassAndIDBuf);/*ɨ�迨*/
        }
        if(status==MI_OK)
        {            
            status = PcdAnticoll(CardClassAndIDBuf);/*����ײ*/ 
            if(status==MI_OK)
            {
                CardID = (uint32_t)CardClassAndIDBuf[0];
                CardID += (uint32_t)CardClassAndIDBuf[1]<<8;
                CardID += (uint32_t)CardClassAndIDBuf[2]<<16;
                CardID += (uint32_t)CardClassAndIDBuf[3]<<24;
                if(LastCardID != CardID)
                {
                    LastCardID = CardID;
                    CardID_GlobalData = CardID;
                    printf("CardID: %d\r\n", CardID);
                    for(count = 0; count < sizeof(PermitCardID) / sizeof(*PermitCardID); count++)
                    {
                        if(CardID == PermitCardID[count])
                        {
                            Gate_On();
                            // LED_Red_On();
                            Contiki_etimer_DelayMS(2000);       
                            Gate_Off();
                            // LED_Red_Off();
                            break;
                        }
                    }
                }
            }
            else
            {
                LastCardID = 0;
            }
        }
        else
        {
            LastCardID = 0;
        }
        Contiki_etimer_DelayMS(200);
    }
    PROCESS_END();
}

PROCESS_THREAD(clock_test_process, ev, data)
{
    static uint16_t i,start_count,end_count,diff;
    PROCESS_BEGIN();

    printf("Clock delay test, (10,000 x i) cycles:\n");
    i = 1;
    while(i<16)
    {
        start_count = clock_time();                   // ��¼��ʼtimer
        Delay_NOP_ms(10 * i);                       // �����ʱ
        end_count = clock_time();                     // ��¼����timer
        diff = end_count - start_count;               // �����ֵ����λΪtick
        printf("Delayed %u \n%u ticks =~ %u ms\n", 10 * i, diff, diff);
        i++;
    }

    printf("\r\nDone!\r\n");

    PROCESS_END();
}

PROCESS_THREAD(cJSON_test_process, ev, data)
{

    cJSON *root;char* cJSONout;
    PROCESS_BEGIN();

	root=cJSON_CreateObject();	
    
	cJSON_AddItemToObject(root, "Device", cJSON_CreateString("ContikiOS on STM32F103"));
    cJSON_AddItemToObject(root, "Address", cJSON_CreateNumber(0xFFFF));
    cJSON_AddItemToObject(root, "InfoType", cJSON_CreateString("Information"));
	cJSON_AddItemToObject(root, "DataName", cJSON_CreateNull());

    cJSONout = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);	
    AssembleProtocolPacketPushSendQueue(0x0000, FunctionWord_StartUP, strlen(cJSONout), (uint8_t*)cJSONout);
    free(cJSONout);
    PROCESS_END();
}

PROCESS_THREAD(T6603_Read_CO2_PPM_process, ev, data)
{
    uint16_t CO2_PPM_Value = 0;
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        T6603_SendReadCMD();
        T6603_LoadReceiveQueueByteToPacketBlock();
        CO2_PPM_Value = T6603_getCO2_PPM();
        CO2_PPM_Value_GlobalData = CO2_PPM_Value;
        if(CO2_PPM_Value_GlobalData > 2200)
        {
            SetBeepAlertSource(CO2_AlertSource);
        }else
        {
            ResetBeepAlertSource(CO2_AlertSource);
        }
        
        Contiki_etimer_DelayMS(5000); 
    }
    PROCESS_END();
}

PROCESS_THREAD(CommunicatProtocol_Send_Sensor_Data, ev, data)
{
    static struct etimer et;
		static uint32_t count = 0;
    cJSON *root;char* cJSONout;
    PROCESS_BEGIN();
    Contiki_etimer_DelayMS(2000);
    while(1)
    {
//				printf("I LOVE U.\r\n");
        root=cJSON_CreateObject();	
        cJSON_AddItemToObject(root, "InfoType", cJSON_CreateString("Data"));
        cJSON_AddItemToObject(root, "Owner", cJSON_CreateString("admin"));

        cJSON_AddItemToObject(root, "Address", cJSON_CreateNumber(Protocol_LocalhostAddress));

#ifdef __DHT11_MODULE_ON__
        cJSON_AddItemToObject(root, "Temperature", cJSON_CreateNumber(temperatureGlobalData));
        cJSON_AddItemToObject(root, "Humidity", cJSON_CreateNumber(humidityGlobalData));
#endif
        
#ifdef __SHT15_MODULE_ON__
        cJSON_AddItemToObject(root, "AccurateTemperature", cJSON_CreateNumber(SHT15_AccurateTemperatureGlobalData));
        cJSON_AddItemToObject(root, "AccurateHumidity", cJSON_CreateNumber(SHT15_AccurateHumidityGlobalData));
#endif

#ifdef __MQ02_MODULE_ON__
        cJSON_AddItemToObject(root, "SmogPercentage", cJSON_CreateNumber(smogPercentageGlobalData));
#endif

#ifdef __MQ135_MODULE_ON__
        cJSON_AddItemToObject(root, "AirQuality", cJSON_CreateNumber(AirQualityPercentageGlobalData));
#endif
			
#ifdef __HCSR501_MODULE_ON__
        cJSON_AddItemToObject(root, "BodyStatus", cJSON_CreateBool(someoneStatusGlobalData));
#endif

#ifdef __HCSR04_MODULE_ON__
        cJSON_AddItemToObject(root, "WaveDistance", cJSON_CreateNumber(distanceGlobalData));
#endif

#ifdef __BH1750_MODULE_ON__
        cJSON_AddItemToObject(root, "LightIntensity", cJSON_CreateNumber(lightIntensityGlobalData));
#endif

#ifdef __RC522_MODULE_ON__
        cJSON_AddItemToObject(root, "CardID", cJSON_CreateNumber(CardID_GlobalData));
#endif

#ifdef __SDS01_MODULE_ON__
        cJSON_AddItemToObject(root, "PM2_5", cJSON_CreateNumber(PM2_5_GlobalData));
        cJSON_AddItemToObject(root, "PM10", cJSON_CreateNumber(PM10_GlobalData));
#endif

#ifdef __T6603_MODULE_ON__
        cJSON_AddItemToObject(root, "CO2_PPM", cJSON_CreateNumber(CO2_PPM_Value_GlobalData));
#endif

        cJSONout = cJSON_PrintUnformatted(root);
        cJSON_Delete(root);
				// ���Ͷ��еĵ�ַ��0x0001(���ܸ�)���͵�����������
        AssembleProtocolPacketPushSendQueue(0x0001, FunctionWord_Data, strlen(cJSONout), (uint8_t*)cJSONout);		
//				printf("Temperature = %f.\n", temperatureGlobalData);
				printf("count = %d\n", count++);
        Contiki_etimer_DelayMS(2000);
    }
    PROCESS_END();
}

PROCESS_THREAD(Communication_Protocol_Load_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        Contiki_etimer_DelayMS(10);
        LoadReceiveQueueByteToPacketBlock();
        DealWithReceivePacketQueue();
    }
    PROCESS_END();
}

PROCESS_THREAD(Communication_Protocol_Send_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        Contiki_etimer_DelayMS(100);
        SendUnsentPacketQueue();
        SendUnackedPacketQueue();
//        IncreaseUnackedPacketQueueResendTime();
    }
    PROCESS_END();
}



PROCESS_THREAD(IWDG_Feed_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {
        IWDG_Feed();
        Contiki_etimer_DelayMS(1000);
    }
    PROCESS_END();
}







