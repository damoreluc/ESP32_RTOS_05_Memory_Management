/*
 * FreeRTOS Esempio 05: memory management
 *
 * Generazione di un errore di stack overflow in un task
 *
 * Nota: nel file soc.h sono definiti i riferimenti ai due core della ESP32:
 *   #define PRO_CPU_NUM (0)
 *   #define APP_CPU_NUM (1)
 *
 * Qui viene adoperata la APP_CPU
 *
 */

#include <Arduino.h>

// Implementazione dei task
// task che consuma una ampia area di stack
void testTask(void *parameters)
{

  // stampa sul terminale
  while (1)
  {
    // due variabili locali sullo stack del task
    uint32_t a = 1;
    uint32_t b[100];

    // qualche operazione banale sull'array b[]
    for (uint32_t i = 0; i < 100; i++)
    {
      b[i] = a + 1;
    }
    // questa chiamata consuma altra RAM, non direttamente quantificabile
    Serial.println(b[0]);
  }
}

// configurazione del sistema
void setup()
{
  Serial.begin(115200);

  // breve pausa
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS: memory management demo");

  // creazione ed avvio dell'unico task
  // DOMANDA: bastano 1024 byte di stack per questo task?
  xTaskCreatePinnedToCore(
      testTask,    // funzione da richiamare nel task
      "Test Task", // nome del task (etichetta utile per debug)
      1500,        // dimensione in byte dello stack per le variabili locali del task (minimo 768 byte)
      NULL,        // puntatore agli eventuali parametri da passare al task
      1,           // priorità del task
      NULL,        // eventuale task handle per gestire il task da un altro task
      APP_CPU_NUM  // core su cui far girare il task
  );

  // rimuove il task "setup and loop"
  vTaskDelete(NULL);
}

void loop()
{
  // questa funzione non sarà mai eseguita dall'RTOS
}