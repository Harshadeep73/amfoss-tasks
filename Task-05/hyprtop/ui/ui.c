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
    int process_count,
    const char *search_query,
    int selected_process
)
{
    int start_row = 14;
    int visible = 25;

    MOVE_TO(start_row, 1);
    printf("\033[K");

    MOVE_TO(start_row, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ PROCESSES"
        RESET
    );

    if (search_query[0] != '\0') {
        printf(
            "  SEARCH: "
            COLOR_PRIMARY "%s" RESET,
            search_query
        );
    }

    MOVE_TO(start_row + 2, 1);
    printf("\033[K");

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

    int displayed = 0;

    for (int i = 0;
         i < process_count && displayed < visible;
         i++) {

        if (!process_matches_search(
                &processes[i],
                search_query
            ))
            continue;

        int row = start_row + 3 + displayed;

        MOVE_TO(row, 1);
        printf("\033[K");

        MOVE_TO(row, 4);

        if (displayed == selected_process)
            printf(HEADER_BG);

        printf(
            "%-8d %-36s %10.2f%% ",
            processes[i].pid,
            processes[i].name,
            processes[i].cpu
        );

        print_memory(processes[i].memory);

        if (displayed == selected_process)
            printf(RESET);

        displayed++;
    }

    while (displayed < visible) {
        int row = start_row + 3 + displayed;

        MOVE_TO(row, 1);
        printf("\033[K");

        displayed++;
    }
}

void ui_draw_footer(int process_count)
{
    MOVE_TO(get_terminal_rows(), 1);

    printf(
        COLOR_PRIMARY "F1" RESET " Help    "
        COLOR_PRIMARY "F2" RESET " Terminate    "
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

void ui_draw_help(void)
{
    printf(CLEAR_SCREEN);
    MOVE_TO(1, 1);

    printf(
        COLOR_PRIMARY BOLD
        "  HYPRTOP HELP"
        RESET
    );

    MOVE_TO(3, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ KEYBOARD"
        RESET
    );

    MOVE_TO(5, 5);
    printf(
        COLOR_PRIMARY "F1" RESET
        "     Help"
    );

    MOVE_TO(6, 5);
    printf(
        COLOR_PRIMARY "F4" RESET
        "     Toggle sorting: CPU / RAM"
    );

    MOVE_TO(7, 5);
    printf(
        COLOR_PRIMARY "F10" RESET
        "    Quit"
    );

    MOVE_TO(9, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ PROCESS SORTING"
        RESET
    );

    MOVE_TO(11, 5);

    printf(
        "CPU ↓   Highest CPU-consuming processes first"
    );

    MOVE_TO(12, 5);

    printf(
        "RAM ↓   Highest memory-consuming processes first"
    );

    MOVE_TO(14, 3);

    printf(
        COLOR_PRIMARY BOLD
        "┌─ SYSTEM MONITOR"
        RESET
    );

    MOVE_TO(16, 5);
    printf("CPU MATRIX   Per-core CPU utilization");

    MOVE_TO(17, 5);
    printf("MEMORY       RAM and swap utilization");

    MOVE_TO(18, 5);
    printf("PROCESSES    Top processes by selected metric");

    MOVE_TO(get_terminal_rows() - 1, 3);

    printf(
        DIM "Press any key to return to HYPRTOP..." RESET
    );

    fflush(stdout);
}
