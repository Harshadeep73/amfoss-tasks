#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "process.h"

Process *scan_processes(int *count) {
    Process *processes = NULL;
    int process_count = 0;
    int capacity = 0;

    DIR *proc = opendir("/proc");

    if (proc == NULL)
        return NULL;

    struct dirent *entry;

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit((unsigned char)entry->d_name[0]))
            continue;

        if (process_count >= capacity) {
            int new_capacity = (capacity == 0) ? 64 : capacity * 2;

            Process *temp = realloc(
                processes,
                new_capacity * sizeof(Process)
            );

            if (temp == NULL) {
                free(processes);
                closedir(proc);
                return NULL;
            }

            processes = temp;
            capacity = new_capacity;
        }

        Process *p = &processes[process_count];

        p->pid = atoi(entry->d_name);
        p->cpu = 0.0;
        p->memory = 0;
        p->name[0] = '\0';

        char path[512];
        char line[256];

        snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

        FILE *file = fopen(path, "r");

        if (file == NULL)
            continue;

        if (fgets(p->name, sizeof(p->name), file) == NULL) {
            fclose(file);
            continue;
        }

        p->name[strcspn(p->name, "\n")] = '\0';

        fclose(file);

        snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);

        file = fopen(path, "r");

        if (file == NULL)
            continue;

        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line, "VmRSS: %ld kB", &p->memory);
                break;
            }
        }

        fclose(file);

        process_count++;
    }

    closedir(proc);

    *count = process_count;

    return processes;
}

long get_process_cpu_time(int pid) {
    char path[256];
    char buffer[4096];

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");

    if (file == NULL)
        return -1;

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return -1;
    }

    fclose(file);

    char *close_paren = strrchr(buffer, ')');

    if (close_paren == NULL)
        return -1;

    char *fields = close_paren + 2;

    long utime = 0;
    long stime = 0;

    int field = 3;

    char *token = strtok(fields, " ");

    while (token != NULL) {
        if (field == 14) {
            utime = atol(token);
        } else if (field == 15) {
            stime = atol(token);
            break;
        }

        field++;
        token = strtok(NULL, " ");
    }

    return utime + stime;
}

double calculate_cpu_usage(
    long old_process,
    long new_process,
    long old_total,
    long new_total
) {
    long process_delta = new_process - old_process;
    long total_delta = new_total - old_total;

    if (total_delta <= 0)
        return 0.0;

    return ((double)process_delta / total_delta) * 100.0;
}

void free_processes(Process *processes) {
    free(processes);
}
