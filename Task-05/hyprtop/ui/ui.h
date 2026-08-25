#ifndef UI_H
#define UI_H

#include "../src/process.h"
#include "../src/system.h"

#define CLEAR_SCREEN      "\033[2J"
#define RESET             "\033[0m"
#define BOLD              "\033[1m"
#define DIM               "\033[2m"
#define HIDE_CURSOR       "\033[?25l"
#define SHOW_CURSOR       "\033[?25h"
#define MOVE_TO(r, c)     printf("\033[%d;%dH", (r), (c))

#define COLOR_PRIMARY     "\033[38;2;0;255;65m"
#define COLOR_BG          "\033[48;2;19;19;19m"
#define HEADER_BG "\033[48;2;58;57;57m"

void ui_init(void);
void ui_cleanup(void);
void ui_draw_help(void);
void ui_draw_header(SystemStats *stats);
void ui_draw_cpu_matrix(SystemStats *stats);
void ui_draw_memory(SystemStats *stats);

void ui_draw_process_list(
    Process *processes,
    int process_count,
    const char *search_query
);

void ui_draw_footer(int process_count);

#endif
