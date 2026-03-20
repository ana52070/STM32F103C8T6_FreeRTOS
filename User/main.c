#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "FreeRTOS.h"	//导入FreeRTOS内核头文件
#include "Task.h"	//导入Task头文件

TaskHandle_t myTaskHandler;	//创建一个Task任务句柄

//任务函数
void myTask(void *arg)
{
	while(1)
	{
		LED1_Turn();
		vTaskDelay(500);

	}
	
}
int main(void)
{
	LED_Init();
//	LED1_ON();
	xTaskCreate(myTask,"myTask",128,NULL,2,&myTaskHandler);
	vTaskStartScheduler();
	
	while (1)
	{
	}
}

