#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

typedef struct {
    int pid;
    char name[256];
    long memory;
    double cpu;
}Process;

double calculate_cpu_usage(long old_process,long new_process,long old_total,long new_total){
    long process_delta = new_process - old_process;
    long total_delta = new_total - old_total;

    if (total_delta <= 0)
        return 0.0;

    return ((double)process_delta / total_delta) * 100.0;
}

long get_total_cpu_time(void) {
    FILE *file = fopen("/proc/stat", "r");

    if (file == NULL) {
        return -1;
    }

    char line[256];
    long user, nice, system, idle, iowait, irq, softirq, steal;

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return -1;
    }

    fclose(file);

    sscanf(line, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",&user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal);

    return user + nice + system + idle + iowait + irq + softirq + steal;
}

long get_process_cpu_time(int pid) {
    char path[256];
    char buffer[4096];

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return -1;
    }

    fclose(file);

    char *close_paren = strrchr(buffer, ')');

    if (close_paren == NULL) {
        return -1;
    }

    char *fields = close_paren + 2;

    char state;
    long value;
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

int main(void) {
    Process *processes = NULL;
    int process_count = 0;
    int capacity = 0;

    DIR *proc = opendir("/proc");

    if (proc == NULL) {
        printf("Could not open /proc\n");
        return 1;
    }

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
                return 1;
            }

            processes = temp;
            capacity = new_capacity;
        }

        Process *p = &processes[process_count];

        p->pid = atoi(entry->d_name);
        p->cpu = 0;
        p->memory = 0;
        p->name[0] = '\0';

        char path[256];
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

    long *old_cpu_times = malloc(process_count * sizeof(long));

    if (old_cpu_times == NULL) {
        free(processes);
        return 1;
    }

    long old_total = get_total_cpu_time();

    for (int i = 0; i < process_count; i++) {
        old_cpu_times[i] = get_process_cpu_time(processes[i].pid);
    }

    usleep(500000);

    long new_total = get_total_cpu_time();

    for (int i = 0; i < process_count; i++) {
        long new_process = get_process_cpu_time(processes[i].pid);

        if (old_cpu_times[i] >= 0 && new_process >= 0) {
            processes[i].cpu = calculate_cpu_usage(
                old_cpu_times[i],
                new_process,
                old_total,
                new_total
            );
        }
    }

    printf("PID\tProcess Name\t\tCPU\tMemory\n");

    long new_total = get_total_cpu_time();

    for (int i = 0; i < process_count; i++) {
        long new_process = get_process_cpu_time(processes[i].pid);

        long process_delta = new_process - old_cpu_times[i];
        long total_delta = new_total - old_total;

        printf(
            "PID %d: old=%ld new=%ld delta=%ld | total delta=%ld\n",
            processes[i].pid,
            old_cpu_times[i],
            new_process,
            process_delta,
            total_delta
        );
    }

    printf("\nTotal Processes: %d\n", process_count);

    free(old_cpu_times);
    free(processes);

    return 0;
}