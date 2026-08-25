#define _POSIX_C_SOURCE 200809L
#include <poll.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "process.h"
#include "system.h"
#include "../ui/ui.h"
#include "input.h"

static int search_mode = 0;
static char search_query[64] = "";
static int selected_process = 0;

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
        ui_draw_process_list(processes, process_count, search_query, selected_process);
        ui_draw_footer(process_count);

        int key = input_get_key();

        if (key == KEY_F1) {
            ui_draw_help();

            while (input_get_key() == KEY_NONE)
                poll(NULL, 0, 50);
            printf(CLEAR_SCREEN);
            fflush(stdout);
        }

        if (key == KEY_F3) {
            search_mode = 1;
            search_query[0] = '\0';
        }
        else if (search_mode) {
            if (key == KEY_ESCAPE) {
                search_mode = 0;
                search_query[0] = '\0';
            }
            else if (key == '\n' || key == '\r') {
                search_mode = 0;
            }
            else if (key >= 32 && key <= 126) {
                size_t len = strlen(search_query);

                if (len < sizeof(search_query) - 1) {
                    search_query[len] = key;
                    search_query[len + 1] = '\0';
                }
            }
        }

        if (key == KEY_F2){
            printf("F2 pressed\n");
        }

        if (key == KEY_F4){
            set_sort_mode(get_sort_mode() == 0 ? 1 : 0);
        }

        if (key == KEY_F10){
            break;
        }

        if (key == KEY_F2)
            kill(processes[selected_process].pid, SIGTERM);

        if (key == KEY_F9)
            kill(processes[selected_process].pid, SIGKILL);

        if (key == KEY_UP && selected_process > 0)
            selected_process--;

        if (key == KEY_DOWN && selected_process < process_count - 1)
            selected_process++;

        free_processes(processes);

        poll(NULL, 0, 250);
    }

    input_cleanup();
    ui_cleanup();

    return 0;
}
