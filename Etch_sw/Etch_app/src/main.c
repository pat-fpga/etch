/**
 * \file
 *
 * \brief Etch a Sketch program designed to foster interest in physical computing
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \ par Sketch Application Top level
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Implements application main loop
 * 
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://asf.microchip.com/docs/latest/">Atmel Support</a>
 */
#include <asf.h>
//#include "asf.h"
#include "sketch.h"
#include "menu.h"
#include "sound.h"
#include "init.h"
#include "irq.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>



volatile int8_t enc_left = 0;
volatile int8_t enc_right = 0;
volatile uint8_t tilt_in, pbl, pbr;
int8_t enc_a, enc_b, pb, tilt;

uint16_t cur_color;uint8_t step;uint8_t style;uint16_t cur_x, cur_y;uint16_t new_x, new_y;uint8_t master_mode;volatile uint8_t audio_en =0;

int main(void)
{
	init_board();

	init_sketch();
	master_mode = MODE_SKETCH;
	
	while (true) 
	{
		get_inputs();
		if(master_mode == MODE_MENU)
		{
			mm_loop();
		}
		if(master_mode == MODE_SUB_MENU)
		{
			sm_loop();
		}
		if(master_mode == MODE_SKETCH)
		{
			sketch_loop();
		}
		if(master_mode == MODE_FILE)
		{

		}
		delay_ms(200);
		
	}
	return(0);

}

