#ifndef INPUT_H
#define INPUT_H
#define KEY_NONE -1
#define KEY_F10 1010
#define KEY_F4 1004

int input_get_key(void);
void input_init(void);
void input_cleanup(void);
int input_get_key(void);

#endif
