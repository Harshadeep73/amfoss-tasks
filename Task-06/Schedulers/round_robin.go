package main

import "sort"

func RoundRobin(processes []Process, quant int) []Execution {
	var executions []Execution
	var queue []int

	sort.Slice(processes, func(i, j int) bool {
		if processes[i].Arrival == processes[j].Arrival {
			return processes[i].PID < processes[j].PID
		}
		return processes[i].Arrival < processes[j].Arrival
	})

	for i := range processes {
		processes[i].Remaining = processes[i].Burst
	}

	currentTime := 0
	next := 0
	completed := 0

	for completed < len(processes) {
		for next < len(processes) && processes[next].Arrival <= currentTime {
			queue = append(queue, next)
			next++
		}
		if len(queue) == 0 {
			currentTime = processes[next].Arrival
			continue
		}

		current := queue[0]
		queue = queue[1:]
		executionTime := min(processes[current].Remaining, quant)
		start := currentTime
		currentTime += executionTime
		processes[current].Remaining -= executionTime
		waiting := 0
		turnaround := 0

		if processes[current].Remaining == 0 {
			turnaround = currentTime - processes[current].Arrival
			waiting = turnaround - processes[current].Burst
			completed++
		}
		executions = append(executions, Execution{
			PID:        processes[current].PID,
			Start:      start,
			End:        currentTime,
			Waiting:    waiting,
			Turnaround: turnaround,
		})
		for next < len(processes) && processes[next].Arrival <= currentTime {
			queue = append(queue, next)
			next++
		}
		if processes[current].Remaining > 0 {
			queue = append(queue, current)
		}
	}
	return executions
}
