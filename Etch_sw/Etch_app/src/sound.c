#include <asf.h>
#include <stdio.h>
#include "sound.h"

extern uint8_t audio_en;

int8_t play_song(void)
{
	uint8_t i;
	uint16_t notes[] = {3000, 2680, 2385, 2250, 2010, 1770, 1595, 1490, 1340, 1190, 1125, 1000, 895, 795, 750};
	uint8_t song[] = {4, 4, 5, 4, 7, 6, 4, 4, 5, 4, 8, 7, 4, 4, 11, 9, 7, 6, 5, 10, 10, 9, 7, 8, 7};

	for(i=0;i<25;i++)
	{
		play_note(notes[song[i]],8);
	}
	
	
	return(0);
}
int8_t play_note(uint16_t note, uint8_t duration)
{
	//Note is a count termination related to a musical note
	//Duration should be set to 1,2,4, or 8 with 8 being treated as a full note
	audio_en =1;
	tc_write_period(&TCC0, note);
	delay_ms(32*duration);
	audio_en =0;
	delay_ms(5);
	
	return(0);
}
