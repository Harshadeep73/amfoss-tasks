#include <poll.h>
#include <time.h>

#include "process.h"
#include "system.h"
#include "../ui/ui.h"

int main(void)
{
    ui_init();

    while (1) {
        int process_count = 0;

        Process *processes = scan_processes(&process_count);

        if (processes == NULL)
            continue;

        SystemStats stats = {0};

        get_system_stats(&stats);

        ui_draw_header(&stats);
        ui_draw_cpu_matrix(&stats);
        ui_draw_memory(&stats);
        ui_draw_process_list(processes, process_count);
        ui_draw_footer(process_count);

        free_processes(processes);

        poll(NULL, 0, 500);
    }

    ui_cleanup();

    return 0;
}
