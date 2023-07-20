/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

//INPUTS
#define CONF_ENC_L_PB              IOPORT_CREATE_PIN(PORTA, 0)
#define CONF_ENC_L_A               IOPORT_CREATE_PIN(PORTA, 1)
#define CONF_ENC_L_B               IOPORT_CREATE_PIN(PORTA, 2)
#define CONF_TILT_A                IOPORT_CREATE_PIN(PORTA, 3)
#define CONF_TILT_B                IOPORT_CREATE_PIN(PORTA, 4)
#define CONF_PB_L                  IOPORT_CREATE_PIN(PORTA, 5)

#define CONF_ENC_R_PB              IOPORT_CREATE_PIN(PORTB, 0)
#define CONF_ENC_R_A               IOPORT_CREATE_PIN(PORTB, 1)
#define CONF_ENC_R_B               IOPORT_CREATE_PIN(PORTB, 2)
#define CONF_PB_R                  IOPORT_CREATE_PIN(PORTB, 3)


//LEDs
#define LED_R                      IOPORT_CREATE_PIN(PORTF, 0)
#define LED_G                      IOPORT_CREATE_PIN(PORTF, 1)
#define LED_B                      IOPORT_CREATE_PIN(PORTF, 2)

//AUDIO
#define AUDIO_OUT_PIN              IOPORT_CREATE_PIN(PORTC, 7)

//SD
#define SD_MMC_SPI_MEM_CNT              1
#define CONF_BOARD_SD_MMC_SPI

#define SD_MMC_SPI_USES_USART_SPI_SERVICE // To signal that is a USART in SPI mode
#define SD_MMC_SPI                      &USARTC0
#define SD_MMC_SPI_SCK                  IOPORT_CREATE_PIN(PORTC, 1)
#define SD_MMC_SPI_MISO                 IOPORT_CREATE_PIN(PORTC, 2)
#define SD_MMC_SPI_MOSI                 IOPORT_CREATE_PIN(PORTC, 3)
#define SD_MMC_SPI_0_CS                 IOPORT_CREATE_PIN(PORTC, 0)

//#define SD_MMC_SPI_DEBUG


//LCD
/**
 * \brief Select the correct hardware interface
 *
 * Currently supported interfaces are the SPI interface and the USART Master
 * SPI interface.
 */
#define CONF_ILI9341_USART_SPI     &USARTD0

/** \brief Define what MCU pin the ILI9341 chip select pin is connected to */
#define CONF_ILI9341_CS_PIN        IOPORT_CREATE_PIN(PORTD, 0)

/** \brief Define what MCU pin the ILI9341 DC pin is connected to */
#define CONF_ILI9341_DC_PIN        IOPORT_CREATE_PIN(PORTD, 4)

/** \brief Define what MCU pin the ILI9341 back light pin is connected to */
#define CONF_ILI9341_BACKLIGHT_PIN IOPORT_CREATE_PIN(PORTF, 3)

/** \brief Define what MCU pin the ILI9341 reset is connected to */
#define CONF_ILI9341_RESET_PIN     IOPORT_CREATE_PIN(PORTD, 5)

/** \brief Define what MCU pin the ILI9341 DC pin is connected to */
#define CONF_ILI9341_MOSI_PIN        IOPORT_CREATE_PIN(PORTD, 3)

/** \brief Define what MCU pin the ILI9341 DC pin is connected to */
#define CONF_ILI9341_MISO_PIN        IOPORT_CREATE_PIN(PORTD, 2)

/** \brief Define what MCU pin the ILI9341 DC pin is connected to */
#define CONF_ILI9341_SCLK_PIN        IOPORT_CREATE_PIN(PORTD, 1)

//OSC
//#define OSC1_PIN                     IOPORT_CREATE_PIN(PORTE, 6)
//#define OSC2_PIN                     IOPORT_CREATE_PIN(PORTE, 7)

//USB
//#define USB_M_PIN                    IOPORT_CREATE_PIN(PORTD, 6)
//#define USB_P_PIN                    IOPORT_CREATE_PIN(PORTD, 7)




#endif // CONF_BOARD_H
