#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_len = 5;
static QueueHandle_t msg_queue;

// Task: read message from Serial buffer
void print_queue(void *parameters)
{
  int item;
  
  // Loop forever
  while (1)
  {
    if(xQueueReceive(msg_queue,&item ,0)== pdTRUE)
    {
      Serial.println(item);
    }
    vTaskDelay(1000);
  }
}

void setup()
{
  Serial.begin(115200);
  vTaskDelay(1000);

  msg_queue = xQueueCreate(msg_len,sizeof(int));
  
  Serial.println("queue with two tasks task1 to fill and task2 to get with half the speed of task1");

  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              print_queue,    //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop()
{
  static int num=0;
  
  if(xQueueSend(msg_queue, &num, 10)!= pdTRUE)
  {
    Serial.println("Queue is full");
  }
  num++;
  vTaskDelay(500);
}