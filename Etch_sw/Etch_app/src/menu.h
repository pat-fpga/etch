#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define ENTRY_TYPE_NONE			0
#define ENTRY_TYPE_LIST			1
#define ENTRY_TYPE_FILENAME		2
#define ENTRY_TYPE_FILELIST		3
#define ENTRY_TYPE_X			4
#define ENTRY_TYPE_XY			5

typedef struct menu_entry_s 
{
	uint8_t entry_type;
	char name[12];
	
//	void (*select)(void *arg, char *name);
//	void *value;
} menu_entry_t;

typedef struct menu_s 
{
	uint8_t top_entry;
	uint8_t current_entry;
	uint8_t num_entries;
//	struct menu_s *previous;
//	menu_entry_t entry[];
} menu_t;

uint8_t mm_start(void);
uint8_t menu_loop(void);
uint8_t mm_loop(void);
uint8_t sm_loop(void);

#endif /* MENU_H_INCLUDED */
