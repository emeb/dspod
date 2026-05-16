/*
 * hsp.c - routines to interface w/ U3C5 HSP
 * 04-05-26 E. Brombaugh
 */

#include "hsp.h"
#include "hsp_fw_def.h"
#include "hsp_fw_ram.h"

HSP_HandleTypeDef hhsp1;

HAL_StatusTypeDef hsp_init(void)
{
	/* turn on the clock */
	__HAL_RCC_HSP1_CLK_ENABLE();

	/* init the HSP */
	hhsp1.Instance = HSP1;
	if(HAL_HSP_Init(&hhsp1) != HAL_OK)
		return HAL_ERROR;
	
	/* load the firmware */
	const hsp_fw_descriptor_t *cram = hsp_fw_getcram();
	const hsp_fw_descriptor_t *dram = hsp_fw_getdram();
	if(HAL_HSP_LoadFirmware(&hhsp1, cram->p_binary, cram->size_byte,
									dram->p_binary, dram->size_byte) != HAL_OK)
		return HAL_ERROR;
	
	/* boot the HSP */
	HAL_HSP_Boot_ConfigTypeDef p_config;
	p_config.boot_cmd_id = HSP_CMD_FW_START;
	p_config.boot_success_code = HSP_BSTAT_BOOTOK;
	p_config.perf_mon = HAL_HSP_PERF_MONITOR_DISABLE;
	p_config.perf_counter_offset = HSP_DRAM_CC_ADDR;
	return HAL_HSP_Boot(&hhsp1, &p_config, 100);
}

