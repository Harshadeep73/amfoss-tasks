package main

import "sort"

func FCFS(processes []Process) []Execution {
    sort.Slice(processes, func(i, j int) bool {
        if processes[i].Arrival == processes[j].Arrival {
            return processes[i].PID < processes[j].PID
        }
        return processes[i].Arrival < processes[j].Arrival
    })

    var executions []Execution
    currentTime := 0

    for _, p := range processes {
        if currentTime < p.Arrival {
            executions = append(executions, Execution{
                PID: "IDLE",
                Start: currentTime,
                End: p.Arrival,
            })

            currentTime = p.Arrival
        }

        start := currentTime
        end := start + p.Burst

        turnaround := end - p.Arrival
        waiting := turnaround - p.Burst

        executions = append(executions, Execution{
            PID: p.PID,
            Start: start,
            End: end,
            Waiting: waiting,
            Turnaround: turnaround,
        })

        currentTime = end
    }

    return executions
}
