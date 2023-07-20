#ifndef SD_H_INCLUDED
#define SD_H_INCLUDED

#define SD_MODE_SKETCH 1
#define SD_MODE_LOAD 2
#define FN_LEN 20
struct file_list
{
	char filename[FN_LEN];
};

int8_t test_sd(void);
int8_t test_fn(char fn[12], uint8_t sd_mode);
int8_t save_image(char fn[12]);
int8_t load_image(char fn[12]);
int8_t list_files(uint8_t max, struct file_list *fl);
int8_t del_image(char fn[12]);


#endif /* SD_H_INCLUDED */
