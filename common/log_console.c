// SPDX-License-Identifier: GPL-2.0+
/*
 * Logging support
 *
 * Copyright (c) 2017 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 */

#include <common.h>
#include <log.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

static int log_console_emit(struct log_device *ldev, struct log_rec *rec)
{
	int fmt = gd->log_fmt;

	/*
	 * The output format is designed to give someone a fighting chance of
	 * figuring out which field is which:
	 *    - level is in CAPS
	 *    - cat is lower case and ends with comma
	 *    - file normally has a .c extension and ends with a colon
	 *    - line is integer and ends with a -
	 *    - function is an identifier and ends with ()
	 *    - message has a space before it unless it is on its own
	 */
	if (fmt & BIT(LOGF_LEVEL))
		printf("%s.", log_get_level_name(rec->level));
	if (fmt & BIT(LOGF_CAT))
		printf("%s,", log_get_cat_name(rec->cat));
	if (fmt & BIT(LOGF_FILE))
		printf("%s:", rec->file);
	if (fmt & BIT(LOGF_LINE))
		printf("%d-", rec->line);
	if (fmt & BIT(LOGF_FUNC))
		printf("%s()", rec->func);
	if (fmt & BIT(LOGF_MSG))
		printf("%s%s", fmt != BIT(LOGF_MSG) ? " " : "", rec->msg);

	return 0;
}

LOG_DRIVER(console) = {
	.name	= "console",
	.emit	= log_console_emit,
	.flags	= LOGDF_ENABLE,
};
