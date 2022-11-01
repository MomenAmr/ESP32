#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static char *msg_ptr = NULL;
static volatile uint8_t msg_flag = 0;


// Task: read message from Serial buffer
void readSerial(void *parameters)
{
  char c;
  char buf[255];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, 255);
  
  // Loop forever
  while (1) {

    // Read cahracters from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // Store received character to buffer if not over buffer limit
      if (idx < 254)
      {
        buf[idx] = c;
        idx++;
      }

      // Create a message buffer for print task
      if (c == '\n')
      {
        // The last character in the string '\0' to make it null-terminated
        buf[idx - 1] = '\0';

        if (msg_flag == 0)
        {
          msg_ptr = (char *)pvPortMalloc(idx * sizeof(char));

          // If malloc returns 0 (out of memory), throw an error and reset
          configASSERT(msg_ptr);

          memcpy(msg_ptr, buf, idx);
          msg_flag = 1;
        }

        // Reset receive buffer and index counter
        memset(buf, 0, 255);
        idx = 0;
      }
    }
  }
}

// Task: print message whenever flag is set and free buffer
void printMessage(void *parameters)
{
  while (1)
  {
    // Wait for flag to be set and print message
    if (msg_flag == 1)
    {
      Serial.println(msg_ptr);

      Serial.print("Free heap (bytes): ");
      Serial.println(xPortGetFreeHeapSize());

      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_flag = 0;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  vTaskDelay(1000);
  
  Serial.println("memory managment between two tasks ");
  Serial.println("enter a string");

  // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              readSerial,     //function to call in task created
              "task1",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos


     // the configutaion to create out task
  xTaskCreatePinnedToCore(    //it's modified to use with esp32 like vanilla freertos xTaskCreate()
              printMessage,     //function to call in task created
              "task2",        //name of the task
              1024,           //stack size for that task in bytes for esp32 but in words for freertos
              NULL,           //pointer to pass to function called
              1,              //task priority (from 0 to 24)
              NULL,           //task handle
              app_cpu);       //core to run the task with esp32 not used in vanilla freertos
}

void loop()
{

}