package main

import "sort"

func RoundRobin(processes []Process, quant int) []Execution {
    var executions []Execution
    sort.Slice(processes, func(i, j int) bool {
        if processes[i].Arrival == processes[j].Arrival {
            return processes[i].PID < processes[j].PID
        }
        return processes[i].Arrival < processes[j].Arrival
    })
    totalTime:=0
    for i := range processes {
        processes[i].Remaining = processes[i].Burst
        totalTime += processes[i].Burst
    }

    current := 0
    currentTime := 0
    for totalTime > 0 {
        if processes[current].Remaining == 0 {

        	current = (current + 1) % len(processes)
            continue
        }

        if currentTime < processes[current].Arrival {
            executions = append(executions, Execution{
                PID:   "IDLE",
                Start: currentTime,
                End:   processes[current].Arrival,
                s    })
            currentTime = processes[current].Arrival
            continue
        }
        executionTime := min(processes[current].Remaining, quant)
        start := currentTime
        currentTime += executionTime
        processes[current].Remaining -= executionTime
        totalTime -= executionTime
        executions = append(executions, Execution{
            PID:   processes[current].PID,
            Start: start,
            End:   currentTime,
        })
        current = (current + 1) % len(processes)
    }
    return executions
}
