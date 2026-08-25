#include <poll.h>
#include <time.h>

#include "process.h"
#include "system.h"
#include "../ui/ui.h"
#include "input.h"

int main(void)
{
    ui_init();
    input_init();

    while (1) {
        int process_count = 0;

        unsigned long total_delta = get_total_cpu_delta();

        Process *processes = scan_processes(
            &process_count,
            total_delta
        );

        if (processes == NULL)
            continue;

        SystemStats stats = {0};

        get_system_stats(&stats);

        ui_draw_header(&stats);
        ui_draw_cpu_matrix(&stats);
        ui_draw_memory(&stats);
        ui_draw_process_list(processes, process_count);
        ui_draw_footer(process_count);

        int key = input_get_key();

        if (key == KEY_F10)
            break;

        if (key == KEY_F4)
            set_sort_mode(get_sort_mode() == 0 ? 1 : 0);

        free_processes(processes);

        poll(NULL, 0, 500);
    }

    input_cleanup();
    ui_cleanup();

    return 0;
}
