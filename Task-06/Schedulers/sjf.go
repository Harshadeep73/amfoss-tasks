package main

func SJF(processes []Process) []Execution {
	var executions []Execution
	currentTime := 0
	completed := 0
	for completed < len(processes) {
		best := -1
		for i, p := range processes {
			if p.Arrival <= currentTime && !p.Completed {
				if best == -1 || p.Burst < processes[best].Burst {
					best = i
				}
			}
		}
		if best == -1 {
			nextArrival := -1
			for i, p := range processes {
				if !p.Completed {
					if nextArrival == -1 || p.Arrival < processes[nextArrival].Arrival {
						nextArrival = i
					}
				}
			}
			executions = append(executions, Execution{
				PID:   "IDLE",
				Start: currentTime,
				End:   processes[nextArrival].Arrival,
			})

			currentTime = processes[nextArrival].Arrival
			continue
		}
		p := processes[best]
		start := currentTime
		end := start + p.Burst
		turnaround := end - p.Arrival
		waiting := turnaround - p.Burst
		executions = append(executions, Execution{
			PID:        p.PID,
			Start:      start,
			End:        end,
			Waiting:    waiting,
			Turnaround: turnaround,
		})
		currentTime = end
		processes[best].Completed = true
		completed++
	}
	return executions
}
