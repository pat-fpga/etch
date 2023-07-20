#include <asf.h>
#include "asf.h"
#include "sketch.h"
#include "sound.h"
#include "irq.h"

volatile int8_t seqLA = 0;
volatile int8_t seqLB = 0;
volatile int8_t seqRA = 0;
volatile int8_t seqRB = 0;


static void my_callback(void)
{
	extern uint8_t audio_en;
	// User code to execute when the overflow occurs here
	//Important to clear Interrupt Flag
	if(audio_en == 1)
	{
		gpio_toggle_pin(AUDIO_OUT_PIN);
	}
	
	tc_clear_overflow(&TCC0);
}


ISR(PORTA_INT0_vect)
{
	extern int8_t enc_left;
	extern uint8_t pbl, tilt_in;
	// Record the A and B signals in separate sequences
	seqLA <<= 1;
	seqLA |= gpio_pin_is_high(CONF_ENC_L_A);
	
	seqLB <<= 1;
	seqLB |= gpio_pin_is_high(CONF_ENC_L_B);
	
	seqLA &= 0b00000011;
	seqLB &= 0b00000011;
	
	// Compare the recorded sequence with the expected sequence
	if ((seqLA == 0b00000001 && seqLB == 0b00000011) ||  (seqLA == 0b0000011 && seqLB == 0b00000010))
	{
		enc_left ++;
	}
	
	if ((seqLA == 0b00000011 && seqLB == 0b00000001) || (seqLA == 0b00000010 && seqLB == 0b00000011))
	{
		enc_left --;
	}
	pbl |= gpio_pin_is_high(CONF_ENC_L_PB)<<1 | gpio_pin_is_high(CONF_PB_L);
	tilt_in |= gpio_pin_is_high(CONF_TILT_A)<<4 |gpio_pin_is_high(CONF_TILT_B);

}

ISR(PORTB_INT0_vect)
{
	extern int8_t enc_right;
	extern uint8_t pbr;
	// Record the A and B signals in separate sequences
	seqRA <<= 1;
	seqRA |= gpio_pin_is_high(CONF_ENC_R_A);
	
	seqRB <<= 1;
	seqRB |= gpio_pin_is_high(CONF_ENC_R_B);
	
	seqRA &= 0b00000011;
	seqRB &= 0b00000011;
	
	// Compare the recorded sequence with the expected sequence
	if ((seqRA == 0b00000001 && seqRB == 0b00000011) ||  (seqRA == 0b0000011 && seqRB == 0b00000010))
	{
		enc_right ++;
	}
	
	if ((seqRA == 0b00000011 && seqRB == 0b00000001) || (seqRA == 0b00000010 && seqRB == 0b00000011))
	{
		enc_right --;
	}
	pbr |= gpio_pin_is_high(CONF_ENC_R_PB)<<1 | gpio_pin_is_high(CONF_PB_R);

}

int8_t get_inputs(void)
{
	extern int8_t enc_left;
	extern int8_t enc_right;
	extern uint8_t tilt_in, pbl, pbr;
	extern int8_t enc_a, enc_b, pb, tilt;

	
	enc_a = enc_left>>1;
	enc_left = 0;
	enc_b = enc_right>>1;
	enc_right = 0;
	
	pb = pbl<<4 | pbr;
	pbl = 0;
	pbr = 0;
	tilt = tilt_in;
	tilt_in = 0;
	
	return(0);
}

void init_tc(void)
{
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, my_callback);
	tc_set_wgm(&TCC0, TC_WGMODE_NORMAL_gc);
	tc_write_period(&TCC0, 1000);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	cpu_irq_enable();
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);
	tc_set_resolution(&TCC0, 600000);
}