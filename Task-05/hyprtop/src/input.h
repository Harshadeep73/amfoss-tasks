#ifndef INPUT_H
#define INPUT_H
#define KEY_NONE -1
#define KEY_F1 1001
#define KEY_F2 1002
#define KEY_ESCAPE 27
#define KEY_F3 1003
#define KEY_F10 1010
#define KEY_F4 1004
#define KEY_UP 2001
#define KEY_DOWN 2002
#define KEY_F9 1009

int input_get_key(void);
void input_init(void);
void input_cleanup(void);
int input_get_key(void);

#endif
