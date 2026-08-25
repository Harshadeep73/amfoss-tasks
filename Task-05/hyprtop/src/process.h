#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    char name[256];
    long memory;
    double cpu;
} Process;

Process *scan_processes(int *count);
long get_process_cpu_time(int pid);

double calculate_cpu_usage(
    long old_process,
    long new_process,
    long old_total,
    long new_total
);

void free_processes(Process *processes);

#endif
