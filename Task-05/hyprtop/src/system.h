#ifndef SYSTEM_H
#define SYSTEM_H

#define MAX_CPUS 128

typedef struct {
    float cpu_usage[MAX_CPUS];
    int cpu_count;

    float ram_used;
    float ram_total;

    float swap_used;
    float swap_total;

    char uptime[32];
    char load_avg[32];
} SystemStats;

unsigned long get_total_cpu_time(void);
unsigned long get_total_cpu_delta(void);
int get_system_stats(SystemStats *stats);


#endif
