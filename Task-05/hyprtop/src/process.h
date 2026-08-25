#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    char name[256];
    long memory;
    double cpu;
    unsigned long cpu_ticks;
} Process;

Process *scan_processes(int *count, unsigned long total_delta);
long get_process_cpu_time(int pid);
void set_sort_mode(int mode);
int get_sort_mode(void);
int process_matches_search(const Process *process, const char *query);

double calculate_cpu_usage(
    long old_process,
    long new_process,
    long old_total,
    long new_total
);

void free_processes(Process *processes);

#endif
