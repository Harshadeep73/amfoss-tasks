#include "input.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios original_terminal;

void input_init(void)
{
    tcgetattr(STDIN_FILENO, &original_terminal);

    struct termios raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void input_cleanup(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
}

int input_get_key(void)
{
    unsigned char key;

    if (read(STDIN_FILENO, &key, 1) != 1)
        return KEY_NONE;

    if (key != '\033')
        return key;

    unsigned char first;

    if (read(STDIN_FILENO, &first, 1) != 1)
        return KEY_NONE;

    if (first == 'O') {
        unsigned char seq;

        if (read(STDIN_FILENO, &seq, 1) != 1)
            return KEY_NONE;

        if (seq == 'S')
            return KEY_F4;
    }

    if (first == '[') {
        unsigned char seq[3];

        if (read(STDIN_FILENO, seq, 3) != 3)
            return KEY_NONE;

        if (seq[0] == '2' &&
            seq[1] == '1' &&
            seq[2] == '~')
            return KEY_F10;
    }

    return KEY_NONE;
}
