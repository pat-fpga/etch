#include <asf.h>
#include <stdio.h>
#include "sketch.h"
#include "sd.h"
#include "menu.h"
#include "irq.h"

void clear_scr(void);
//void draw_cursor(uint16_t x, uint16_t y);
void invert_pixel(uint16_t x, uint16_t y);
uint16_t  invert_color(uint16_t color);
void change_color(uint16_t delta_color);
void invert_line(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void move_cursor(int8_t delta_x, int8_t delta_y);
//void update_status(void);

//todo: get rid of globals
extern uint8_t step, style;
extern uint16_t cur_x, cur_y, 	cur_color;
extern int16_t new_x, new_y;
void init_sketch(void)
{
	clear_scr();

	cur_x = 160;
	cur_y = 120;
	step  = 1;
	style = 0;
	cur_color = GFX_COLOR_WHITE;

	draw_cursor(cur_x, cur_y);
	update_status();

}

void update_sketch(int8_t delta_color, int8_t delta_step)
{
	extern int8_t enc_a, enc_b;
	
	if(enc_a != 0 || enc_b != 0)
	{
		move_cursor(enc_a*step, 0);
		move_cursor(0, enc_b*step);
	}

	if(delta_step != 0)
	{
		step += delta_step;
		if(step <1)
			step = 1;
		if(step >10)
			step = 1;
		update_status();
	}

	if(delta_color != 0)

	{
		change_color(delta_color);
		update_status();
	}
}

void clear_scr(void)
{

	gfx_draw_filled_rect(0,0,ILI9341_DEFAULT_WIDTH,ILI9341_DEFAULT_HEIGHT, GFX_COLOR_BLACK);
	gfx_set_orientation(GFX_FLIP_X | GFX_FLIP_Y);
	gfx_draw_rect(MINX-1,MINY-1,MAXX+1,MAXY+1,GFX_COLOR_WHITE);
}

void clear_stat(void)
{

	gfx_draw_filled_rect(0,230,ILI9341_DEFAULT_WIDTH,ILI9341_DEFAULT_HEIGHT, GFX_COLOR_BLACK);
}

void draw_cursor(uint16_t x, uint16_t y)
{
	#define CURSOR_SIZE 5
	uint16_t min_x, max_x, min_y, max_y;

	if(x - CURSOR_SIZE <= MINX)
	{
		min_x = MINX;
	}
	else
	{
		min_x = x - CURSOR_SIZE;
	}

	if(y - CURSOR_SIZE <= MINY)
	{
		min_y = MINY;
	}
	else
	{
		min_y = y - CURSOR_SIZE;
	}

	if(x + CURSOR_SIZE >= MAXX)
	{
		max_x = MAXX;
	}
	else
	{
		max_x = x + CURSOR_SIZE;
	}

	if(y + CURSOR_SIZE >= MAXY)
	{
		max_y = MAXY;
	}
	else
	{
		max_y = y + CURSOR_SIZE;
	}

	invert_line(min_x,y,x - 3,y);
	invert_line(x+ 3,y,max_x,y);
	invert_line(x,min_y,x,y - 3);
	invert_line(x,y+ 3,x,max_y);
	invert_pixel(x,y);

	cur_x = x;
	cur_y = y;
}

void invert_pixel(uint16_t x, uint16_t y)
{
	uint16_t pix;

	//Use color pairs to "invert" pixel and have the ability to return it to the original color

	pix = gfx_get_pixel(x, y);

	pix = invert_color(pix);

	gfx_draw_pixel(x, y,pix);
}

uint16_t  invert_color(uint16_t color)
{
	//Use color pairs to "invert" pixel and have the ability to return it to the original color
	uint16_t new_color;

	switch (color)
	{
		case GFX_COLOR_RED:
		new_color = GFX_COLOR_DK_RED;
		break;
		case GFX_COLOR_DK_RED:
		new_color = GFX_COLOR_RED;
		break;
		case GFX_COLOR_DK_BLUE:
		new_color = GFX_COLOR_BLUE;
		break;
		case GFX_COLOR_BLUE:
		new_color = GFX_COLOR_DK_BLUE;
		break;
		case GFX_COLOR_GREEN:
		new_color = GFX_COLOR_DK_GREEN;
		break;
		case GFX_COLOR_DK_GREEN:
		new_color = GFX_COLOR_GREEN;
		break;
		case GFX_COLOR_YELLOW:
		new_color = GFX_COLOR_DK_YELLOW;
		break;
		case GFX_COLOR_DK_YELLOW:
		new_color = GFX_COLOR_YELLOW;
		break;
		case GFX_COLOR_MAGENTA:
		new_color = GFX_COLOR_DK_MAGENTA;
		break;
		case GFX_COLOR_DK_MAGENTA:
		new_color = GFX_COLOR_MAGENTA;
		break;
		case GFX_COLOR_CYAN:
		new_color = GFX_COLOR_DK_CYAN;
		break;
		case GFX_COLOR_DK_CYAN:
		new_color = GFX_COLOR_CYAN;
		break;
		case GFX_COLOR_BLACK:
		new_color = GFX_COLOR_WHITE;
		break;
		case GFX_COLOR_WHITE:
		new_color = GFX_COLOR_BLACK;
		break;
		default:
		new_color = GFX_COLOR_BLACK;
		break;
	}
	return (new_color);
}

void change_color(uint16_t delta_color)
{
	//Use color pairs to "invert" pixel and have the ability to return it to the original color

	if(delta_color != 0)
	{
		switch (cur_color)
		{
			case GFX_COLOR_RED:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_WHITE;
			}
			else
			{
				cur_color = GFX_COLOR_DK_RED;
			}
			break;
			case GFX_COLOR_DK_RED:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_RED;
			}
			else
			{
				cur_color = GFX_COLOR_DK_BLUE;
			}
			break;
			case GFX_COLOR_DK_BLUE:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_RED;
			}
			else
			{
				cur_color = GFX_COLOR_BLUE;
			}
			break;
			case GFX_COLOR_BLUE:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_BLUE;
			}
			else
			{
				cur_color = GFX_COLOR_GREEN;
			}
			break;
			case GFX_COLOR_GREEN:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_BLUE;
			}
			else
			{
				cur_color = GFX_COLOR_DK_GREEN;
			}
			break;
			case GFX_COLOR_DK_GREEN:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_GREEN;
			}
			else
			{
				cur_color = GFX_COLOR_YELLOW;
			}
			break;
			case GFX_COLOR_YELLOW:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_GREEN;
			}
			else
			{
				cur_color = GFX_COLOR_DK_YELLOW;
			}
			break;
			case GFX_COLOR_DK_YELLOW:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_YELLOW;
			}
			else
			{
				cur_color = GFX_COLOR_MAGENTA;
			}
			break;
			case GFX_COLOR_MAGENTA:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_YELLOW;
			}
			else
			{
				cur_color = GFX_COLOR_DK_MAGENTA;
			}
			break;
			case GFX_COLOR_DK_MAGENTA:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_MAGENTA;
			}
			else
			{
				cur_color = GFX_COLOR_CYAN;
			}
			break;
			case GFX_COLOR_CYAN:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_MAGENTA;
			}
			else
			{
				cur_color = GFX_COLOR_DK_CYAN;
			}
			break;
			case GFX_COLOR_DK_CYAN:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_CYAN;
			}
			else
			{
				cur_color = GFX_COLOR_BLACK;
			}
			break;
			case GFX_COLOR_BLACK:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_DK_CYAN;
			}
			else
			{
				cur_color = GFX_COLOR_WHITE;
			}
			break;
			case GFX_COLOR_WHITE:
			if(delta_color < 0)
			{
				cur_color = GFX_COLOR_BLACK;
			}
			else
			{
				cur_color = GFX_COLOR_RED;
			}
			break;
			default:
				cur_color = GFX_COLOR_BLACK;
			break;
		}
	}
}


void invert_line(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
	//Only support orthogonal lines
	uint16_t inc;
	if(xStart == xEnd)
	{
		if(yStart <= yEnd)
		{
			for(inc = yStart; inc <= yEnd; inc++)
			{
				invert_pixel(xStart, inc);
			}
		}
		else
		{
			for(inc = yEnd; inc <= yStart; inc++)
			{
				invert_pixel(xStart, inc);
			}
		}
	}
	if(yStart == yEnd)
	{
		if(xStart <= xEnd)
		{
			for(inc = xStart; inc <= xEnd; inc++)
			{
				invert_pixel(inc, yStart);
			}
		}
		else
		{
			for(inc = xEnd; inc <= xStart; inc++)
			{
				invert_pixel(inc, yStart);
			}
		}
	}
}

void move_cursor(int8_t delta_x, int8_t delta_y)
{

	draw_cursor(cur_x, cur_y);

	new_x = cur_x + delta_x;
	new_y = cur_y + delta_y;

	if(new_x < MINX+3)
	{
		new_x = MINX+3;
	}
	if(new_x > MAXX-3)
	{
		new_x = MAXX-3;
	}
	if(new_y < MINY+3)
	{
		new_y = MINY+3;
	}
	if(new_y > MAXY-3)
	{
		new_y = MAXY-3;
	}

	gfx_draw_line(cur_x, cur_y, new_x, new_y,cur_color);

	draw_cursor(new_x, new_y);
	cur_x = new_x;
	cur_y = new_y;

	update_status();
}

void update_status(void)
{
	char val[4];

	gfx_draw_filled_rect(0, 230, 319, 19, GFX_COLOR_BLACK);

	gfx_draw_string("X: ", 5, 230,  &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	itoa(cur_x,val, 10);
	gfx_draw_string("     ", 29, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string(val, 29, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string("Y: ", 61, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	itoa(cur_y,val, 10);
	gfx_draw_string("     ", 85, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string(val, 85, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	itoa(step,val, 10);
	gfx_draw_string("COLOR:      STEP: ", 117, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string("     ", 237, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);
	gfx_draw_string(val, 237, 230, &sysfont, GFX_COLOR_BLACK, GFX_COLOR_WHITE);

	gfx_draw_filled_rect(173, 230, 8, 8, cur_color);

}

int8_t sketch_loop(void)
{
//todo: get rid of globals
	extern int8_t pb, enc_a, enc_b;
	int8_t delta_color;
	int8_t delta_step;

	delta_color = 0;
	delta_step = 0;
	if((PB_ENC_R)&&(PB_ENC_L))
	{
		init_sketch();
//		play_song();
	}
	else
	{
		if(PB_ENC_R)
		{
			delta_color = 1;
		}
		if(PB_ENC_L)
		{
			delta_step = 1;
		}
	}
	if(PB_L)
	{
		printf("Wait for PB_L to clear");
		get_inputs();
		while(PB_L)
		{
			delay_ms(10);
			get_inputs();
		}
		mm_start();
	}
	update_sketch(delta_color,delta_step);
	enc_a = 0;
	enc_b = 0;
	delta_step = 0;
	delta_color = 0;
	
	return(0);
}