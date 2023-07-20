#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "sd.h"
#include "sketch.h"

static FATFS fs;

uint8_t create_fn(char fn[12], uint8_t sd_mode, char *file_name);

int8_t save_image(char fn[12])
{
	extern uint16_t cur_color, cur_x, cur_y;	extern uint8_t step, style;
	uint16_t x;
	uint16_t y;
	uint16_t pix;
	FIL fh;
	UINT count;
	FRESULT res;
	char file_name[20];
	struct cfg save_cfg;
	
	create_fn(fn, SD_MODE_SKETCH, &file_name[0]);
	save_cfg.cur_x = cur_x;
	save_cfg.cur_y = cur_y;
	save_cfg.cur_color = cur_color;
	save_cfg.step = step;
	save_cfg.style = style;
	
	/* Mount disk*/
	printf("Save Image: %s\n\r",fn);
	gfx_draw_string("Saving ", 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);

	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}

	res = f_open(&fh, (char const *)file_name,FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK) {
		printf("-E- f_open create pb: 0x%X\n\r", res);
		return 0;
	}

	f_write(&fh,&save_cfg,sizeof(save_cfg),&count);
	draw_cursor(cur_x, cur_y);
	
	for(x=MINX; x<=MAXX; x++)
	{
		for(y=MINY; y<=MAXY; y++)
		{
			pix = gfx_get_pixel(x, y);
			f_write(&fh,&pix,sizeof(pix),&count);

		}
	}
	res = f_close(&fh);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}
	draw_cursor(cur_x, cur_y);
	update_status();
	return(0);
}

int8_t load_image(char fn[12])
{
	extern uint16_t cur_color, cur_x, cur_y;	extern uint8_t step, style;
	uint16_t x;
	uint16_t y;
	uint16_t pix;
	FRESULT res;
	FIL fh;
	UINT count;
	char file_name[16];
	struct cfg load_cfg;

	create_fn(fn, SD_MODE_LOAD, &file_name[0]);

	/* Mount disk*/
	printf("Load image: %s\n\r", fn);
	gfx_draw_string("Loading ", 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}

	res = f_open(&fh, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("-E- f_open create pb: 0x%X\n\r", res);
		return 0;
	}

	f_read(&fh,&load_cfg,sizeof(load_cfg),&count);
	cur_x = load_cfg.cur_x;
	cur_y = load_cfg.cur_y;
	cur_color = load_cfg.cur_color;
	step = load_cfg.step;
	style = load_cfg.style;


	for(x=MINX; x<=MAXX; x++)
	{
		for(y=MINY; y<=MAXY; y++)
		{
			res = f_read(&fh,&pix,sizeof(pix),&count);
			if(count > 0)
			{
				gfx_draw_pixel(x, y,pix);
			}
		}
	}
	res = f_close(&fh);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}
	draw_cursor(cur_x, cur_y);
	update_status();
	return(0);
}

int8_t del_image(char fn[12])
{
	FRESULT res;
	char file_name[16];

	create_fn(fn, SD_MODE_LOAD, &file_name[0]);

	/* Mount disk*/
	printf("Delete image: %s\n\r", fn);
	gfx_draw_string("Deleting ", 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}

	res = f_unlink((char const *)file_name);
	if (res != FR_OK) {
		printf("-E- f_unlink: 0x%X\n\r", res);
		return 0;
	}

	update_status();
	return(0);
}

int8_t test_sd(void)
{
	FRESULT res;

	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK)
	{
		return -1;
	}
	return 0;

}

int8_t test_fn(char fn[12], uint8_t sd_mode)
{
	FRESULT res;
	FIL fh;
	char file_name[16];

	create_fn(fn, sd_mode, &file_name[0]);

	/* Mount disk*/
	printf("Test fn\n\r");
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK) {
		return -1;
	}

	res = f_open(&fh, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("File, %s: Not Found\n\r", file_name);
		return -2;
	}
	return 0;
	
}

uint8_t create_fn(char fn[12], uint8_t sd_mode, char *file_name)
{

	switch(sd_mode)
	{
		case SD_MODE_SKETCH:
		sprintf(file_name, "0:/%s.ech", fn);
//		strcat(file_name,".ech");
		break;
		case SD_MODE_LOAD:
		sprintf(file_name, "0:/%s", fn);
		break;
	}
	return(0);
}

int8_t list_files(uint8_t max, struct file_list *fl)
{
	FRESULT res;
	DIR dir;
	static FILINFO fno;
	uint8_t count;
	
	/* Mount disk*/
	printf("list files\n\r");
	count = 0;
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(0, &fs);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}
	res = f_opendir(&dir, "/");                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			if(count >=max) break;							/* Break on count >= max */
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
			if (!(fno.fattrib & AM_DIR)) {                    /* It is a file. */
				if(strstr(fno.fname,".ech")!=0)
				{
					strncpy(&fl[count].filename[0], fno.fname, FN_LEN-1);
					count ++;
					printf("/%s\n", fno.fname);
				}
			}
		}
	}

	return(count);
}