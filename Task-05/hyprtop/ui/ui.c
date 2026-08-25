#include <stdio.h>
#include <string.h>

#include "ui.h"

#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

#define GREEN       "\033[38;2;0;255;65m"
#define BG          "\033[48;2;19;19;19m"
#define HEADER_BG   "\033[48;2;58;57;57m"
#define YELLOW      "\033[33m"
#define RED         "\033[31m"

#define MOVE_TO(r,c) printf("\033[%d;%dH", (r), (c))

void ui_init(void)
{
    printf(HIDE_CURSOR BG "\033[2J");
    fflush(stdout);
}

void ui_cleanup(void)
{
    printf(SHOW_CURSOR RESET "\033[2J\033[H");
    fflush(stdout);
}

static void draw_header(SystemStats *stats)
{
    MOVE_TO(1, 1);

    printf(
        GREEN BOLD
        " HYPRTOP V2.4.0 "
        RESET
        " Nodes  Threads  "
        GREEN BOLD "CPU_Core" RESET
        "  Memory  Disk"
    );

    MOVE_TO(1, 80);

    printf(
        DIM "UP: %s" RESET,
        stats->uptime
    );

    MOVE_TO(2, 1);

    printf(HEADER_BG);

    for (int i = 0; i < 100; i++)
        printf(" ");

    printf(RESET);
}

static void draw_cpu_matrix(SystemStats *stats)
{
    MOVE_TO(4, 2);

    printf(
        GREEN BOLD
        "┌─ CPU_MATRIX [%d CORES] "
        RESET,
        stats->cpu_count
    );

    int rows = (stats->cpu_count + 1) / 2;

    for (int row = 0; row < rows; row++) {

        int left = row;
        int right = row + rows;

        MOVE_TO(6 + row, 4);

        if (left < stats->cpu_count) {
            printf(
                "%d [",
                left
            );

            int bars = (int)(stats->cpu_usage[left] / 5.0f);

            for (int j = 0; j < 20; j++) {
                if (j < bars)
                    printf(GREEN "|");
                else
                    printf(DIM ".");
            }

            printf(
                RESET "] %5.1f%%",
                stats->cpu_usage[left]
            );
        }

        if (right < stats->cpu_count) {
            printf("    %d [", right);

            int bars = (int)(stats->cpu_usage[right] / 5.0f);

            for (int j = 0; j < 20; j++) {
                if (j < bars)
                    printf(GREEN "|");
                else
                    printf(DIM ".");
            }

            printf(
                RESET "] %5.1f%%",
                stats->cpu_usage[right]
            );
        }
    }
}

static void draw_memory(SystemStats *stats)
{
    MOVE_TO(4, 60);

    printf(
        GREEN BOLD
        "┌─ MEM_ALLOC "
        RESET
    );

    MOVE_TO(6, 60);

    printf(
        "RAM [%.1fG/%.1fG]",
        stats->ram_used,
        stats->ram_total
    );

    MOVE_TO(7, 60);

    printf("[");

    int bars =
        (int)((stats->ram_used / stats->ram_total) * 30);

    for (int i = 0; i < 30; i++) {
        if (i < bars)
            printf(GREEN "|");
        else
            printf(DIM ".");
    }

    printf(
        RESET "] %.1f%%",
        (stats->ram_used / stats->ram_total) * 100.0
    );

    MOVE_TO(9, 60);

    printf(
        "SWP [%.1fG/%.1fG]",
        stats->swap_used,
        stats->swap_total
    );

    MOVE_TO(10, 60);

    printf("[");

    bars =
        (int)((stats->swap_used / stats->swap_total) * 30);

    for (int i = 0; i < 30; i++) {
        if (i < bars)
            printf(GREEN "|");
        else
            printf(DIM ".");
    }

    printf(
        RESET "] %.1f%%",
        (stats->swap_used / stats->swap_total) * 100.0
    );
}

static void draw_processes(
    Process *processes,
    int process_count
)
{
    int start_row = 13;

    MOVE_TO(start_row, 1);

    printf(
        GREEN BOLD
        "┌─ PROCESS_TABLE "
        RESET
    );

    MOVE_TO(start_row + 1, 1);

    printf(
        HEADER_BG
        " PID      PROCESS                 CPU        MEMORY"
        RESET
    );

    int visible = 8;

    for (int i = 0; i < visible; i++) {

        MOVE_TO(start_row + 2 + i, 1);

        if (i < process_count) {

            printf(
                "%-8d %-24s %7.2f%% %12ld kB",
                processes[i].pid,
                processes[i].name,
                processes[i].cpu,
                processes[i].memory
            );

        } else {
            printf(
                "%-8s %-24s %7s %12s",
                "",
                "",
                "",
                ""
            );
        }
    }
}

static void draw_footer(int process_count)
{
    MOVE_TO(24, 1);

    printf(
        GREEN " F1" RESET " Help "
        GREEN " F2" RESET " Setup "
        GREEN " F3" RESET " Search "
        GREEN " F4" RESET " Filter "
        GREEN " F9" RESET " Kill "
        GREEN " F10" RESET " Quit "
        DIM "    %d processes" RESET,
        process_count
    );
}

void draw_ui(
    Process *processes,
    int process_count,
    SystemStats *stats
)
{
    printf("\033[H");

    draw_header(stats);
    draw_cpu_matrix(stats);
    draw_memory(stats);
    draw_processes(processes, process_count);
    draw_footer(process_count);

    fflush(stdout);
}
