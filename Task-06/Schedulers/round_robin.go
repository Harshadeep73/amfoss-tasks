package main
import "sort"

func RoundRobin(processes []Process, quant int) []Execution {
	var executions []Execution
	total_time := 0

	sort.Slice(processes, func(i,j) bool{
		if processes[i].Arrival == processes[j].Arrival{
			return processes[i].PID < processes[j].PID
		}
		return processes[i].Arrival < processes[j].Arrival
	})
	for i := range processes{
		total_time += processes[i].Burst
	}

	//Enqueuing ig
	current := 0
	current_time := 0
	for total_time > 0{
		if current_time < processes[current].Arrival{
			executions = append(executions, Execution{
				PID: "IDLE",
				Start: current_time,
				End: processes[current].Arrival
			})
			current_time = processes[current].Arrival
			continue
		}

		execution = append(executions, Execution{
			PID: processes[current].PID,
			Start: current_time,
			End: current_time+quant,

		})

	}

}
