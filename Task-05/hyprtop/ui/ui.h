#ifndef UI_H
#define UI_H

#include "../src/process.h"
#include "../src/system.h"

void ui_init(void);
void ui_cleanup(void);

void draw_ui(
    Process *processes,
    int process_count,
    SystemStats *stats
);

#endif
