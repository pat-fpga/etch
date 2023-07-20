#include <asf.h>
#include "asf.h"
#include "sketch.h"
#include "sound.h"
#include "irq.h"
#include "init.h"

void init_board(void)
{
	ioport_configure_pin(CONF_ILI9341_SCLK_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(CONF_ILI9341_CS_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(CONF_ILI9341_DC_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_LOW);
	ioport_configure_pin(CONF_ILI9341_BACKLIGHT_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(CONF_ILI9341_RESET_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(CONF_ILI9341_MOSI_PIN, IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(CONF_ILI9341_MISO_PIN, IOPORT_DIR_INPUT);

	ioport_configure_pin(SD_MMC_SPI_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SD_MMC_SPI_MISO, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(SD_MMC_SPI_MOSI, IOPORT_DIR_OUTPUT);
	ioport_configure_pin(SD_MMC_SPI_0_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	//	ioport_configure_pin(SD_MMC_0_CD_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	ioport_configure_pin(CONF_ENC_L_A, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ENC_L_B, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ENC_L_PB, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ENC_R_A, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ENC_R_B, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ENC_R_PB, IOPORT_DIR_INPUT);

	ioport_configure_pin(CONF_PB_L, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_PB_R, IOPORT_DIR_INPUT);

	ioport_configure_pin(CONF_TILT_A, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_TILT_B, IOPORT_DIR_INPUT);

	ioport_configure_pin(AUDIO_OUT_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);


	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	stdio_usb_init();
	stdio_usb_enable();
	delay_ms(250);
//	sleepmgr_init(); // Optional
	sd_mmc_init();
	rtc_init();
	delay_ms(250);
	

	PORTA.INTCTRL = ( PORTA.INTCTRL & ~PORT_INT0LVL_gm ) | PORT_INT0LVL_MED_gc;
	PORTA.INT0MASK = 0x3F;
	PORTB.INTCTRL = ( PORTB.INTCTRL & ~PORT_INT0LVL_gm ) | PORT_INT0LVL_MED_gc;
	PORTB.INT0MASK = 0x0F;
	
	init_tc();

	gfx_init();
	gfx_draw_filled_rect(0,0,ILI9341_DEFAULT_WIDTH,ILI9341_DEFAULT_HEIGHT, GFX_COLOR_BLACK);
	gfx_set_orientation(GFX_FLIP_X | GFX_FLIP_Y);
	gfx_draw_rect(0,0,ILI9341_DEFAULT_WIDTH,ILI9341_DEFAULT_HEIGHT,GFX_COLOR_WHITE);
	
}