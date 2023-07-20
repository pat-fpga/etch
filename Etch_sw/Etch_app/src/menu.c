#include <asf.h>

#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "sketch.h"
#include "sd.h"
#include "irq.h"


uint8_t mm_show(void);
uint8_t mm_next(void);
uint8_t mm_prev(void);

uint8_t sm_show(void);

uint8_t fn_loop(void);
uint8_t fn_show(char exist_ch, char cur_letter);

uint8_t fl_loop(void);
uint8_t fl_show(char *cur_fn);

//define Menu Constants 
//todo: Move to EEPROM
menu_entry_t mm_entry[] = {
		{
			.entry_type = ENTRY_TYPE_FILENAME,
			.name = "Etch Save",
		},
		{
			.entry_type = ENTRY_TYPE_FILELIST,
			.name = "Etch Load"
		},
		{
			.entry_type = ENTRY_TYPE_FILELIST,
			.name = "Delete"
		},
		{
			.entry_type = ENTRY_TYPE_NONE,
			.name = "Cursor"
		//},
		//{
			//.entry_type = ENTRY_TYPE_NONE,
			//.name = "RGB LED"
		//},
		//{
			//.entry_type = ENTRY_TYPE_NONE,
			//.name = "Song Load"
		//},
		//{
			//.entry_type = ENTRY_TYPE_NONE,
			//.name = "Song Play"
		}
} ;
#define MM_ETCH_SAVE 0
#define MM_ETCH_LOAD 1
#define MM_ETCH_DEL  2
#define MM_CURCOR    3
#define MM_RGB       4
#define MM_SONG_LOAD 5
#define MM_SONG_PLAY 6

menu_t mm_ctrl = {
	.top_entry = 0,
	.current_entry = 0,
	.num_entries = 3
};

uint8_t sm_ndx;
char buffer[12];
char exist_ch = ' ';
struct file_list fl_buffer[50];
int8_t fl_count;


uint8_t mm_start(void)
{
	extern uint8_t master_mode;
	extern int8_t pb;
	
	printf("mm_start\n");
	master_mode = MODE_MENU;
	mm_ctrl.current_entry = MM_ETCH_SAVE;
	mm_show();

	printf("Wait for PB_L to clear");
	get_inputs();
	while(PB_L)
	{
		delay_ms(10);
		get_inputs();
	}
	
	return(0);
}

uint8_t menu_loop(void)
{
	extern uint8_t master_mode;

	printf("menu loop. Mode: %d\n",master_mode);
	
	switch(master_mode)
	{
		case MODE_MENU:
			mm_loop();
			break;
		case MODE_SUB_MENU:
			printf("start submenu\n");
			sm_loop();
			break;
	}
	return(0);	
}

uint8_t mm_loop(void)
{
	extern uint8_t master_mode;
	extern int8_t enc_a, pb;

	if(enc_a < 0)
	{
		printf("mm_prev\n");
		mm_prev();
	}
	else if(enc_a > 0)
	{
		printf("mm_next\n");
		mm_next();
	}
	if(PB_L)
	{
		update_status();
		master_mode = MODE_SKETCH;
//		printf("Wait for PB_L to clear");
		get_inputs();
		while(PB_L)
		{
			delay_ms(10);
			get_inputs();
		}
	}
	else
	{
		if(PB_ENC_L)
		{
			switch(mm_ctrl.current_entry)
			{
				case(MM_ETCH_SAVE):
				{
					sm_ndx = 0;
					buffer[0] = '\0';
					master_mode = MODE_SUB_MENU;
					break;
				}
				case(MM_ETCH_LOAD):
				{
					fl_count = list_files(50, &fl_buffer[0]);
					sm_ndx = 0;
					buffer[0] = '\0';
					master_mode = MODE_SUB_MENU;
					break;
				}
				case(MM_ETCH_DEL):
				{
					fl_count = list_files(50, &fl_buffer[0]);
					sm_ndx = 0;
					buffer[0] = '\0';
					master_mode = MODE_SUB_MENU;
					break;
				}
			}
			get_inputs();
			printf("Wait for PB_ENC_L to clear");
			while(PB_ENC_L)
			{
				delay_ms(10);
				get_inputs();
			}
		}
		mm_show();
	}
	return(0);
}


uint8_t mm_show(void)
{
	clear_stat();

	gfx_draw_string(mm_entry[mm_ctrl.current_entry].name, 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	return(0);

}

uint8_t mm_next(void)
{
	if(mm_ctrl.current_entry > mm_ctrl.num_entries - 1)
	{
		mm_ctrl.current_entry = 0;
	}
	else
	{
		mm_ctrl.current_entry ++;
	}
	return(0);
}

uint8_t mm_prev(void)
{
	if(mm_ctrl.current_entry <= 0)
	{
		mm_ctrl.current_entry = mm_ctrl.num_entries - 1;
	}
	else
	{
		mm_ctrl.current_entry --;
	}
	return(0);
}

uint8_t sm_loop(void)
{
	extern uint8_t master_mode;
//	extern int8_t enc_a, pb;

	switch(mm_entry[mm_ctrl.current_entry].entry_type)
	{
		case ENTRY_TYPE_FILENAME:
			fn_loop();
			break;
		case ENTRY_TYPE_FILELIST:
			fl_loop();
			break;
	}
	return(0);
	
}

uint8_t sm_show(void)
{
	clear_stat();

	gfx_draw_string(mm_entry[mm_ctrl.current_entry].name, 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_CYAN);
	return(0);

}

uint8_t fn_loop(void)
{
	extern uint8_t master_mode;
	extern int8_t enc_b, pb;
	char alpha[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if(PB_ENC_R)
	{
		if(strlen(buffer)<12)
		{
			strncat(buffer,&alpha[sm_ndx],1);
		}
		if(test_fn(buffer, SD_MODE_SKETCH) == 0)
		{
			exist_ch = '*';
		}
		else
		{
			exist_ch = ' ';
		}
	}
	else if(PB_ENC_L)
	{
		if(mm_ctrl.current_entry == MM_ETCH_SAVE)
		{
			save_image(buffer);
			master_mode = MODE_SKETCH;

		}
		//if(mm_ctrl.current_entry == 1)
		//{
			//load_image(buffer);
			//master_mode = MODE_SKETCH;
		//}
	}
	else if(enc_b < 0)
	{
		if(sm_ndx > 0)
		{
			sm_ndx --;
		}
		else
		{
			sm_ndx = 25;
		}
	}
	else if(enc_b > 0)
	{
		if(sm_ndx < 25)
		{
			sm_ndx ++;
		}
		else
		{
			sm_ndx = 0;
		}
	}
	if(PB_R)
	{
		int len;
		len = strlen(buffer);
		if(len>0)
		{
			buffer[len-1] = '\0';
		}	
	}
	if(PB_L)
	{
		master_mode = MODE_SKETCH;
	}

    fn_show(exist_ch, alpha[sm_ndx]);
	return(0);
}

uint8_t fn_show(char exist_ch, char cur_letter)
{
	sm_show();

	gfx_draw_char(exist_ch, 50, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string(buffer, 55, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_char(cur_letter, 200, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_YELLOW);
	return(0);

}

uint8_t fl_loop(void)
{
	extern uint8_t master_mode;
	extern int8_t enc_b, pb;

	if(PB_ENC_R)
	{
		if(mm_ctrl.current_entry == MM_ETCH_LOAD)
		{
			load_image(&fl_buffer[sm_ndx].filename[0]);
		}
		else if(mm_ctrl.current_entry == MM_ETCH_DEL)
		{
			del_image(&fl_buffer[sm_ndx].filename[0]);
		}
		master_mode = MODE_SKETCH;

	}
	else if(PB_ENC_L)
	{
		NULL;
	}
	else if(enc_b < 0)
	{
		if(sm_ndx > 0)
		{
			sm_ndx --;
		}
		else
		{
			sm_ndx = fl_count-1;
		}
	}
	else if(enc_b > 0)
	{
		if(sm_ndx < fl_count-1)
		{
			sm_ndx ++;
		}
		else
		{
			sm_ndx = 0;
		}
	}
	if(PB_L)
	{
		update_status();
		master_mode = MODE_SKETCH;
	}

	fl_show(fl_buffer[sm_ndx].filename);
	return(0);
}

uint8_t fl_show(char *cur_fn)
{
	sm_show();

	gfx_draw_string(cur_fn, 200, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_YELLOW);
	return(0);

}
