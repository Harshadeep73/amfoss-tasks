#include <poll.h>
#include <stdio.h>
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

        snprintf(stats.uptime, sizeof(stats.uptime), "LIVE");

        draw_ui(processes, process_count, &stats);

        free_processes(processes);

        poll(NULL, 0, 500);
    }

    ui_cleanup();

    return 0;
}
