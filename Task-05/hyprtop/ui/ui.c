#include "ui.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int get_terminal_rows(void)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return 24;

    return ws.ws_row;
}

static int get_terminal_cols(void)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return 120;

    return ws.ws_col;
}

static void print_memory(long kb)
{
    if (kb >= 1024 * 1024) {
        printf("%.1f GB", kb / (1024.0 * 1024.0));
    }
    else if (kb >= 1024) {
        printf("%.1f MB", kb / 1024.0);
    }
    else {
        printf("%ld KB", kb);
    }
}

void ui_init(void)
{
    printf("\033[?1049h");
    printf(HIDE_CURSOR);
    printf(COLOR_BG CLEAR_SCREEN "\033[H");
    fflush(stdout);
}

void ui_cleanup(void)
{
    printf(SHOW_CURSOR RESET "\033[?1049l");
    fflush(stdout);
}

void ui_draw_header(SystemStats *stats)
{
    MOVE_TO(1, 1);

    printf(
        COLOR_PRIMARY BOLD
        "  HYPRTOP V1.0.0"
    );

    MOVE_TO(1, get_terminal_cols()-51);

    printf(
        DIM "UP: %s    LOAD: %s" RESET,
        stats->uptime,
        stats->load_avg
    );

}

void ui_draw_cpu_matrix(SystemStats *stats)
{
    int rows = (stats->cpu_count + 1) / 2;

    MOVE_TO(3, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ CPU MATRIX [%d CORES]"
        RESET,
        stats->cpu_count
    );

    for (int row = 0; row < rows; row++) {

        int left = row;
        int right = row + rows;

        MOVE_TO(5 + row, 5);

        if (left < stats->cpu_count) {

            printf("%-2d [", left);

            int bars = (int)(stats->cpu_usage[left] / 5.0f);

            if (bars > 20)
                bars = 20;

            for (int j = 0; j < 20; j++) {
                if (j < bars)
                    printf(COLOR_PRIMARY "█");
                else
                    printf(DIM "·");
            }

            printf(
                RESET "] %5.1f%%",
                stats->cpu_usage[left]
            );
        }

        if (right < stats->cpu_count) {

            MOVE_TO(5 + row, 42);

            printf("%-2d [", right);

            int bars = (int)(stats->cpu_usage[right] / 5.0f);

            if (bars > 20)
                bars = 20;

            for (int j = 0; j < 20; j++) {
                if (j < bars)
                    printf(COLOR_PRIMARY "█");
                else
                    printf(DIM "·");
            }

            printf(
                RESET "] %5.1f%%",
                stats->cpu_usage[right]
            );
        }
    }
}

void ui_draw_memory(SystemStats *stats)
{
    MOVE_TO(3, 86);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ MEMORY"
        RESET
    );

    MOVE_TO(5, 86);

    printf(
        "RAM   %.1fG / %.1fG",
        stats->ram_used,
        stats->ram_total
    );

    MOVE_TO(6, 86);

    printf("[");

    int bars = 0;

    if (stats->ram_total > 0)
        bars = (int)(
            stats->ram_used / stats->ram_total * 30
        );

    if (bars > 30)
        bars = 30;

    for (int i = 0; i < 30; i++) {
        if (i < bars)
            printf(COLOR_PRIMARY "█");
        else
            printf(DIM "·");
    }

    printf(
        RESET "] %5.1f%%",
        stats->ram_total > 0
            ? stats->ram_used / stats->ram_total * 100.0
            : 0.0
    );

    MOVE_TO(9, 86);

    printf(
        "SWAP  %.1fG / %.1fG",
        stats->swap_used,
        stats->swap_total
    );

    MOVE_TO(10, 86);

    printf("[");

    bars = 0;

    if (stats->swap_total > 0)
        bars = (int)(
            stats->swap_used / stats->swap_total * 30
        );

    if (bars > 30)
        bars = 30;

    for (int i = 0; i < 30; i++) {
        if (i < bars)
            printf(COLOR_PRIMARY "█");
        else
            printf(DIM "·");
    }

    printf(
        RESET "] %5.1f%%",
        stats->swap_total > 0
            ? stats->swap_used / stats->swap_total * 100.0
            : 0.0
    );
}

void ui_draw_process_list(
    Process *processes,
    int process_count
)
{
    int start_row = 14;
    int visible = 25;

    MOVE_TO(start_row, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ PROCESSES"
        RESET
    );

    MOVE_TO(start_row + 2, 4);

    printf(
        HEADER_BG
        "%-8s %-36s %12s %16s"
        RESET,
        "PID",
        "PROCESS",
        "CPU",
        "MEMORY"
    );

    for (int i = 0; i < visible; i++) {

        MOVE_TO(start_row + 3 + i, 1);
        printf("\033[K");

        MOVE_TO(start_row + 3 + i, 4);

        if (i < process_count) {
            printf(
                "%-8d %-36s %10.2f%% ",
                processes[i].pid,
                processes[i].name,
                processes[i].cpu
            );

            print_memory(processes[i].memory);
        } else {
            printf(
                "%-8s %-36s %12s %16s",
                "",
                "",
                "",
                ""
            );
        }
    }
}

void ui_draw_footer(int process_count)
{
    MOVE_TO(get_terminal_rows(), 1);

    printf(
        COLOR_PRIMARY "F1" RESET " Help    "
        COLOR_PRIMARY "F2" RESET " Setup    "
        COLOR_PRIMARY "F3" RESET " Search    "
    );
    if (get_sort_mode() == 0)
        printf(COLOR_PRIMARY "F4" RESET " RAM↓    ");
    else
        printf(COLOR_PRIMARY "F4" RESET " CPU↓    ");
    printf(
        COLOR_PRIMARY "F9" RESET " Kill    "
        COLOR_PRIMARY "F10" RESET " Quit    "
        DIM "%d processes" RESET,
        process_count
    );
}
