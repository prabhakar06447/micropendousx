// TODO - process a .hex file and add the vector checksum to it
// the code snippets come from OpenOCD

/*
 * flash bank lpc2000 <base> <size> 0 0 <target#> <lpc_variant> <cclk> [calc_checksum]
 */
FLASH_BANK_COMMAND_HANDLER(lpc2000_flash_bank_command)
{
	struct lpc2000_flash_bank *lpc2000_info;

	if (CMD_ARGC < 8)
	{
		LOG_WARNING("incomplete flash_bank lpc2000 configuration");
		return ERROR_FLASH_BANK_INVALID;
	}

	lpc2000_info = malloc(sizeof(struct lpc2000_flash_bank));
	bank->driver_priv = lpc2000_info;

	if (strcmp(CMD_ARGV[6], "lpc2000_v1") == 0)
	{
		lpc2000_info->variant = lpc2000_v1;
		lpc2000_info->cmd51_dst_boundary = 512;
		lpc2000_info->cmd51_can_256b = 0;
		lpc2000_info->cmd51_can_8192b = 1;
		lpc2000_info->checksum_vector = 5;
	}
	else if (strcmp(CMD_ARGV[6], "lpc2000_v2") == 0)
	{
		lpc2000_info->variant = lpc2000_v2;
		lpc2000_info->cmd51_dst_boundary = 256;
		lpc2000_info->cmd51_can_256b = 1;
		lpc2000_info->cmd51_can_8192b = 0;
		lpc2000_info->checksum_vector = 5;
	}
	else if (strcmp(CMD_ARGV[6], "lpc1700") == 0)
	{
		lpc2000_info->variant = lpc1700;
		lpc2000_info->cmd51_dst_boundary = 256;
		lpc2000_info->cmd51_can_256b = 1;
		lpc2000_info->cmd51_can_8192b = 0;
		lpc2000_info->checksum_vector = 7;
	}
	else
	{
		LOG_ERROR("unknown LPC2000 variant: %s", CMD_ARGV[6]);
		free(lpc2000_info);
		return ERROR_FLASH_BANK_INVALID;
	}

	lpc2000_info->iap_working_area = NULL;
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[7], lpc2000_info->cclk);
	lpc2000_info->calc_checksum = 0;
	lpc2000_build_sector_list(bank);

	if (CMD_ARGC >= 9)
	{
		if (strcmp(CMD_ARGV[8], "calc_checksum") == 0)
			lpc2000_info->calc_checksum = 1;
	}

	return ERROR_OK;
}








	/* check if exception vectors should be flashed */
	if ((offset == 0) && (count >= 0x20) && lpc2000_info->calc_checksum)
	{
		uint32_t checksum = 0;
		int i;
		for (i = 0; i < 8; i++)
		{
			LOG_DEBUG("Vector 0x%2.2x: 0x%8.8" PRIx32, i * 4, buf_get_u32(buffer + (i * 4), 0, 32));
			if (i != lpc2000_info->checksum_vector)
				checksum += buf_get_u32(buffer + (i * 4), 0, 32);
		}
		checksum = 0 - checksum;
		LOG_DEBUG("checksum: 0x%8.8" PRIx32, checksum);

		uint32_t original_value = buf_get_u32(buffer + (lpc2000_info->checksum_vector * 4), 0, 32);
		if (original_value != checksum)
		{
			LOG_WARNING("Verification will fail since checksum in image (0x%8.8" PRIx32 ") to be written to flash is different from calculated vector checksum (0x%8.8" PRIx32 ").",
					original_value, checksum);
			LOG_WARNING("To remove this warning modify build tools on developer PC to inject correct LPC vector checksum.");
		}

		buf_set_u32(buffer + (lpc2000_info->checksum_vector * 4), 0, 32, checksum);
	}