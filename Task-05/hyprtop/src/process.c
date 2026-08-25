#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

#include "process.h"

static int sort_mode = 0;

static unsigned long *previous_cpu = NULL;
static int previous_cpu_size = 0;
static unsigned char *previous_cpu_valid = NULL;

int process_matches_search(const Process *process, const char *query)
{
    if (query[0] == '\0')
        return 1;

    return strstr(process->name, query) != NULL;
}

void set_sort_mode(int mode)
{
    sort_mode = mode;
}

int get_sort_mode(void)
{
    return sort_mode;
}

static int get_pid_max(void)
{
    FILE *file = fopen("/proc/sys/kernel/pid_max", "r");

    if (file == NULL)
        return 4194304;

    int pid_max = 0;

    fscanf(file, "%d", &pid_max);

    fclose(file);

    return pid_max;
}

static int compare_memory(const void *a, const void *b)
{
    const Process *p1 = a;
    const Process *p2 = b;

    if (p1->memory < p2->memory)
        return 1;

    if (p1->memory > p2->memory)
        return -1;

    return 0;
}

static int compare_cpu(const void *a, const void *b)
{
    const Process *p1 = a;
    const Process *p2 = b;

    if (p1->cpu < p2->cpu)
        return 1;

    if (p1->cpu > p2->cpu)
        return -1;

    return 0;
}

static void sort_processes(Process *processes, int count)
{
    if (sort_mode == 0) {
        qsort(
            processes,
            count,
            sizeof(Process),
            compare_cpu
        );
    } else {
        qsort(
            processes,
            count,
            sizeof(Process),
            compare_memory
        );
    }
}

static unsigned long get_process_cpu_ticks(int pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");

    if (file == NULL)
        return 0;

    char buffer[4096];

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return 0;
    }

    fclose(file);

    char *close_paren = strrchr(buffer, ')');

    if (close_paren == NULL)
        return 0;

    char *fields = close_paren + 2;

    char state;
    unsigned long utime;
    unsigned long stime;

    int result = sscanf(
        fields,
        "%c "
        "%*d %*d %*d %*d %*d %*d %*d %*d %*d %*d "
        "%lu %lu",
        &state,
        &utime,
        &stime
    );

    if (result != 3)
        return 0;

    return utime + stime;
}

Process *scan_processes(int *count, unsigned long total_delta)
{
    Process *processes = NULL;
    int process_count = 0;
    int capacity = 0;

    if (previous_cpu == NULL) {
        previous_cpu_size = get_pid_max() + 1;

        previous_cpu = calloc(
            previous_cpu_size,
            sizeof(unsigned long)
        );

        previous_cpu_valid = calloc(
            previous_cpu_size,
            sizeof(unsigned char)
        );

        if (previous_cpu == NULL || previous_cpu_valid == NULL) {
            free(previous_cpu);
            free(previous_cpu_valid);

            previous_cpu = NULL;
            previous_cpu_valid = NULL;

            return NULL;
        }
    }

    long cpu_count = sysconf(_SC_NPROCESSORS_ONLN);

    if (cpu_count < 1)
        cpu_count = 1;

    DIR *proc = opendir("/proc");

    if (proc == NULL)
        return NULL;

    struct dirent *entry;

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit((unsigned char)entry->d_name[0]))
            continue;

        if (process_count >= capacity) {
            int new_capacity = (capacity == 0)
                ? 64
                : capacity * 2;

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
        p->cpu_ticks = get_process_cpu_ticks(p->pid);
        p->name[0] = '\0';

        unsigned long old_ticks = previous_cpu[p->pid];
        unsigned long delta = 0;

        if (previous_cpu_valid[p->pid]) {
            if (p->cpu_ticks >= old_ticks)
                delta = p->cpu_ticks - old_ticks;
        }

        previous_cpu[p->pid] = p->cpu_ticks;
        previous_cpu_valid[p->pid] = 1;

        if (total_delta > 0) {
            p->cpu =
                ((double)delta / total_delta)
                * cpu_count
                * 100.0;
        } else {
            p->cpu = 0.0;
        }

        char path[512];
        char line[256];

        snprintf(
            path,
            sizeof(path),
            "/proc/%s/comm",
            entry->d_name
        );

        FILE *file = fopen(path, "r");

        if (file == NULL)
            continue;

        if (fgets(p->name, sizeof(p->name), file) == NULL) {
            fclose(file);
            continue;
        }

        p->name[strcspn(p->name, "\n")] = '\0';

        fclose(file);

        snprintf(
            path,
            sizeof(path),
            "/proc/%s/status",
            entry->d_name
        );

        file = fopen(path, "r");

        if (file == NULL)
            continue;

        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(
                    line,
                    "VmRSS: %ld kB",
                    &p->memory
                );
                break;
            }
        }

        fclose(file);

        process_count++;
    }

    closedir(proc);

    *count = process_count;

    sort_processes(processes, process_count);

    return processes;
}

void free_processes(Process *processes)
{
    free(processes);
}
