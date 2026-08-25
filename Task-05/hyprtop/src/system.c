#include <stdio.h>
#include <string.h>

#include "system.h"

static unsigned long previous_total_cpu = 0;
static int previous_total_cpu_valid = 0;

unsigned long get_total_cpu_time(void)
{
    FILE *file = fopen("/proc/stat", "r");

    if (file == NULL)
        return 0;

    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;

    int result = fscanf(
        file,
        "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
        &user,
        &nice,
        &system,
        &idle,
        &iowait,
        &irq,
        &softirq,
        &steal
    );

    fclose(file);

    if (result != 8)
        return 0;

    return user + nice + system + idle +
           iowait + irq + softirq + steal;
}

unsigned long get_total_cpu_delta(void)
{
    unsigned long current = get_total_cpu_time();
    unsigned long delta = 0;

    if (previous_total_cpu_valid && current >= previous_total_cpu)
        delta = current - previous_total_cpu;

    previous_total_cpu = current;
    previous_total_cpu_valid = 1;

    return delta;
}

int get_system_stats(SystemStats *stats)
{
    FILE *file = fopen("/proc/meminfo", "r");

    if (file == NULL)
        return -1;

    char line[256];

    long mem_total = 0;
    long mem_available = 0;
    long swap_total = 0;
    long swap_free = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "MemTotal: %ld kB", &mem_total);
        sscanf(line, "MemAvailable: %ld kB", &mem_available);
        sscanf(line, "SwapTotal: %ld kB", &swap_total);
        sscanf(line, "SwapFree: %ld kB", &swap_free);
    }

    fclose(file);

    stats->ram_total = mem_total / 1024.0f;
    stats->ram_used = (mem_total - mem_available) / 1024.0f;

    stats->swap_total = swap_total / 1024.0f;
    stats->swap_used = (swap_total - swap_free) / 1024.0f;

    file = fopen("/proc/uptime", "r");

    if (file != NULL) {
        double uptime_seconds;

        if (fscanf(file, "%lf", &uptime_seconds) == 1) {
            int days = uptime_seconds / 86400;
            int hours = ((int)uptime_seconds % 86400) / 3600;
            int minutes = ((int)uptime_seconds % 3600) / 60;

            snprintf(
                stats->uptime,
                sizeof(stats->uptime),
                "%dd %dh %dm",
                days,
                hours,
                minutes
            );
        }

        fclose(file);
    }

    file = fopen("/proc/loadavg", "r");

    if (file != NULL) {
        fgets(stats->load_avg, sizeof(stats->load_avg), file);
        stats->load_avg[strcspn(stats->load_avg, "\n")] = '\0';
        fclose(file);
    }

    stats->cpu_count = 0;

    file = fopen("/proc/stat", "r");

    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "cpu", 3) != 0)
                break;

            if (line[3] >= '0' && line[3] <= '9')
                stats->cpu_count++;
        }

        fclose(file);
    }

    return 0;
}
