#include "stm32f10x.h"                  // Device header
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"

/*-------------------自己导入的模块--------------------------------*/





//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		500  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define LED2_TASK_PRIO		3
//任务堆栈大小	
#define LED2_STK_SIZE 		500  
//任务句柄
TaskHandle_t LED2Task_Handler;
//任务函数
void led2_task(void *pvParameters);


uint8_t Buf[32];
uint8_t cnm,cnd,reciex;
int main(void)
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	/*模块初始化*/

	
		//创建开始任务
    if(xTaskCreate((TaskFunction_t )led1_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )LED1_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )LED1_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&LED1Task_Handler)!= pdPASS)  //任务句柄   
	{
		//任务1初始化失败
	}
	else
	{
		//任务1初始化成功
	}

    if(xTaskCreate((TaskFunction_t )led2_task,            //任务函数
                (const char*    )"start2_task",          //任务名称
                (uint16_t       )LED2_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )LED2_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&LED2Task_Handler)!= pdPASS)   //任务句柄      				
	{
		//任务2初始化失败
	}
	else
	{
		//任务2初始化成功
	}
				
    vTaskStartScheduler(); 
}



//LED1任务函数
void led1_task(void *pvParameters)
{


}

//LED2任务函数
void led2_task(void *pvParameters)
{
	
  
}

























///*-------------------RTOS堆栈管理及任务声明--------------------------------*/
////任务优先级
//#define TASK1_4G_PRIO		1
////任务堆栈大小	
//#define TASK1_4G_SIZE 		1024
////任务句柄
//TaskHandle_t task1_4g_Handler;
////任务函数
//void task1_4g(void *pvParameters);

////任务优先级
//#define TASK2_24G_PRIO		1
////任务堆栈大小	
//#define TASK2_24G_SIZE 		500
////任务句柄
//TaskHandle_t task2_24g_Handler;
////任务函数
//void task2_24G(void *pvParameters);

////任务优先级
//#define START_TASK_PRIO		1
////任务堆栈大小	
//#define START_STK_SIZE 		500 
////任务句柄
//TaskHandle_t StartTask_Handler;
////任务函数
//void start_task(void *pvParameters);


//TaskStatus_t taskStatus;
//char my_str[100];

///*-------------------自己的声明部分--------------------------------*/
//extern uint8_t TIM1_IRQ_FLAG;

//uint8_t Buf[32];	//接收数据的变量

//int main(void)
//{
//	SysTick_Init(72);
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
//	
//	
//	/*模块初始化*/
//	LED_Init();
//	LED1_OFF();
//	NRF24L01_Init();
//	OLED_Init();
//	RE_CONFIG('F');
//	
//	ESP8266_connect_wifi("MEIZU20","88888888");
//	ESP8266_connect_mqtt();
//	
//	OLED_ShowString(1,1,"init_ok");
//	
//	LED1_ON();
//	send_data(1,1,1,1,1,1,1,1,1);
//	
//    //创建LED2任务
//    if (xTaskCreate((TaskFunction_t )task2_24G,     
//                (const char*    )"nrf24l01",   
//                (uint16_t       )TASK2_24G_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_24G_PRIO,
//                (TaskHandle_t*  )&task2_24g_Handler)!= pdPASS)
//	{
//		// 处理任务创建失败
//		OLED_ShowString(3,1,"24create_laji");
//	}
//	else
//	{
//		OLED_ShowString(3,1,"24create_ok");
//	}
//	

//				
//	//创建开始任务
//	if (xTaskCreate((TaskFunction_t )task1_4g, (const char* )"4G_Serial", 
//					(uint16_t )TASK1_4G_SIZE, (void* )NULL, 
//					(UBaseType_t )TASK1_4G_PRIO, 
//					(TaskHandle_t* )&task1_4g_Handler) != pdPASS)
//	{
//		// 处理任务创建失败
//		OLED_ShowString(2,1,"4create_laji");
//	}
//	else
//	{
//		OLED_ShowString(2,1,"4create_ok");
//	}
//		
//	vTaskGetInfo(task1_4g_Handler, &taskStatus, pdTRUE, eInvalid);
//	sprintf(my_str,"State: %d", taskStatus.eCurrentState);
//	OLED_ShowString(4,1,my_str);

//    vTaskStartScheduler(); 
//				
//}


////开始任务任务函数
//void start_task(void *pvParameters)
//{
//    taskENTER_CRITICAL();           //进入临界区
//      
//    //创建LED1任务
//    xTaskCreate((TaskFunction_t )task2_24G,     
//                (const char*    )"nrf24l01",   
//                (uint16_t       )TASK2_24G_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_24G_PRIO,
//                (TaskHandle_t*  )&task2_24g_Handler); 
//				
//				
////	//创建开始任务
////    xTaskCreate((TaskFunction_t )task1_4g,            //任务函数
////                (const char*    )"4G_Serial",          //任务名称
////                (uint16_t       )TASK1_4G_SIZE,        //任务堆栈大小
////                (void*          )NULL,                  //传递给任务函数的参数
////                (UBaseType_t    )TASK1_4G_PRIO,       //任务优先级
////                (TaskHandle_t*  )&task1_4g_Handler);   //任务句柄    
//				
//				
//	if (xTaskCreate((TaskFunction_t )task1_4g, (const char* )"4G_Serial", 
//					(uint16_t )TASK1_4G_SIZE, (void* )NULL, 
//					(UBaseType_t )TASK1_4G_PRIO, 
//					(TaskHandle_t* )&task1_4g_Handler) != pdPASS)
//	{
//		// 处理任务创建失败
//		OLED_ShowString(2,1,"create_laji");
//	}
//	else
//	{
//		OLED_ShowString(2,1,"create_ok");
//	}
//				
//				
//    vTaskDelete(StartTask_Handler); //删除开始任务
//    taskEXIT_CRITICAL();            //退出临界区
//} 

////LED1任务函数
//void task1_4g(void *pvParameters)
//{
//	while(1)
//	{
//		//LED1_OFF();
//		send_data(1,1,1,1,1,1,1,1,1);
//		vTaskDelay(200);
//		//LED1_ON();
//	}

//}

////LED2任务函数
//void task2_24G(void *pvParameters)
//{
//    while(1)
//    {
////		if (!NRF24L01_RxPacket(Buf))
////		{
////			if(Buf[0] == 10 && Buf[10] == 0x55)
////			{
////				LED1_OFF();
////				//{10,temp_Z,temp_X,humi,bee_state,Rader,smoke,voice,fire,mqtt,0x55};
////				//temp = combine_to_float(Buf[1],Buf[2]);
////				//{\\22temp\\22:%f,\\22humi\\22:%d,\\22voice\22:%d,\\22rader\22:%d,\\22mqtt\22:%d,\\22bee_state\22:%d},
////				//aliyun_send_data(Buf[2],Buf[3],Buf[7],Buf[5],Buf[9],Buf[4],Buf[8],Buf[6],Buf[1]);
////				vTaskDelay(1000);
////				LED1_ON();
////			}
////			
////			if(Buf[1] == 0xFF && Buf[4] == 0x55)
////			{
////				LED1_OFF();
////				//收到签到签退消息，发送至服务器
//////				if(Buf[2] == 0x11)
//////					aliyun_send_check_in(Buf[3]);
//////				else if(Buf[2] == 0x00)
//////					aliyun_send_check_off(Buf[3]);
////				vTaskDelay(1000);
////				LED1_ON();
////			}
////		}
//		LED1_OFF();
//		vTaskDelay(1000);
//		LED1_ON();
//		vTaskDelay(1000);
//    }
//}


