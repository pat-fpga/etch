#ifndef SKETCH_H_INCLUDED
#define SKETCH_H_INCLUDED

void init_sketch(void);
void update_sketch(int8_t delta_color, int8_t delta_step);
void update_status(void);
void draw_cursor(uint16_t x, uint16_t y);
void clear_stat(void);
int8_t sketch_loop(void);

struct cfg{	uint16_t cur_x;	uint16_t cur_y;	uint16_t cur_color;	uint8_t step;	uint8_t style;};
#define MODE_MENU     0
#define MODE_SUB_MENU 1
#define MODE_SKETCH   2
#define MODE_FILE     3

#define MINX 3
#define MINY 3
#define MAXX 316
#define MAXY 220

#define PB_ENC_R pb&0x2
#define PB_ENC_L pb&0x20
#define PB_R pb&0x1
#define PB_L pb&0x10

#endif /* SKETCH_H_INCLUDED */
