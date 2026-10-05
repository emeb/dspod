/*
 * multicore_audio.c - start audio processing on 2nd core
 * 12-28-24 E. Brombaugh
 */

#include "main.h"
#include "audio.h"

/* tag for logging */
static const char *TAG = "multicore_audio";

uint64_t mc_duration, mc_period;

/*
 * Audio Task - runs on 2nd core
 * WARNING - don't use floating pt here - IRQ uses it and doesn't 
 * save/restore state.
 */
void audio_task( void * pvParameters )
{
	/* start audio on 2nd core */
	audio_init();
	
	/* loop forever watching for request to change algorithms */
	while(1)
	{
		/* update CPU load */
		mc_period = audio_load[0] - audio_load[2];
		mc_duration = audio_load[1] - audio_load[0];
		
		/* pass some time */
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

/*
 * start audio task on 2nd core
 */
void multicore_audio_init(void)
{
	/* start audio task on 2nd core */
	int32_t pvParameters = 0;
	TaskHandle_t pxCreatedTask;
	BaseType_t task_result = xTaskCreatePinnedToCore(
		audio_task,			// task function
		"audio_task",		// name
		8192,				// stack size
		&pvParameters,		// parameters to task
		1,					// priority
		&pxCreatedTask,		// handle to task
		1					// core #
	);
	if(task_result != pdPASS)
		ESP_LOGW(TAG, "Error creating audio task: %d", task_result);
	else
		ESP_LOGI(TAG, "Created audio task");
}

