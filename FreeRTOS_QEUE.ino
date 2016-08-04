#include <Arduino_FreeRTOS.h>
#include <croutine.h>
#include <event_groups.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOSVariant.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <portmacro.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <StackMacros.h>
#include <task.h>
#include <timers.h>

// define two Tasks for DigitalRead & AnalogRead
void TaskSerial_in (void *pvParameters );
void TaskSerial_out( void *pvParameters );

// Global Queue Handle
QueueHandle_t qh = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  qh = xQueueCreate(1, sizeof(int));
  if(qh == NULL )
  {
    Serial.println("Qeue not created");
  }
  // Now set up two Tasks to run independently.
  xTaskCreate(
    TaskSerial_in
    ,  (const portCHAR *)"Serial_in"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );
    
    // Now set up two Tasks to run independently.
  xTaskCreate(
    TaskSerial_out
    ,  (const portCHAR *)"Serial_out"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );
    
    vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void TaskSerial_in (void *pvParameters )
{
  while(1){
      int msg = 1;
      //send to task
      if(Serial.available())
      {
        msg = Serial.read();
        if(!xQueueSend(qh, &msg, 300))
        {
          Serial.println("Qeue not sent");
        }
         vTaskDelay(10);
      }
      else
      {
        vTaskDelay(10);
      }
  }
}
void TaskSerial_out( void *pvParameters )
{
  int msg =0;
  if(!xQueueReceive(qh, &msg, 100))
  {
    Serial.println("Msg not received");
  }
  else
  {
    Serial.println(msg);
    vTaskDelay(10);
  }
}

