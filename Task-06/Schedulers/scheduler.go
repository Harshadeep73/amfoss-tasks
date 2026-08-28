package main

import "fmt"

type Process struct {
	PID            string
	Arrival        int
	Burst          int
	Remaining      int
	CompletionTime int
	WaitingTime    int
	TurnaroundTime int
	Completed      bool
}

type Execution struct {
	PID        string
	Start      int
	End        int
	Waiting    int
	Turnaround int
}

type Result struct {
	PID        string
	Waiting    int
	Turnaround int
}

func GetAvgs(executions []Execution) {
	if len(executions) == 0 {
		return
	}
	seen := make(map[string]bool)
	totalWaiting := 0
	totalTurnaround := 0
	count := 0
	for i := len(executions) - 1; i >= 0; i-- {
		e := executions[i]
		if e.PID == "IDLE" || seen[e.PID] {
			continue
		}
		seen[e.PID] = true
		totalWaiting += e.Waiting
		totalTurnaround += e.Turnaround
		count++
	}
	avgWaiting := float32(totalWaiting) / float32(count)
	avgTurnaround := float32(totalTurnaround) / float32(count)
	fmt.Printf("\nAverage Waiting Time: %.2f\n", avgWaiting)
	fmt.Printf("Average Turnaround Time: %.2f\n", avgTurnaround)
}

func DrawGanttChart(executions []Execution) {
	if len(executions) == 0 {
		return
	}

	fmt.Println("\nGantt Chart:")

	fmt.Print("+-------")
	for range executions {
		fmt.Print("+-------")
	}
	fmt.Println("+")

	fmt.Print("|  PID  ")
	for _, e := range executions {
		fmt.Printf("| %-5s ", e.PID)
	}
	fmt.Println("|")

	fmt.Print("+-------")
	for range executions {
		fmt.Print("+-------")
	}
	fmt.Println("+")

	fmt.Print("| Start ")
	for _, e := range executions {
		fmt.Printf("| %-5d ", e.Start)
	}
	fmt.Println("|")

	fmt.Print("| End   ")
	for _, e := range executions {
		fmt.Printf("| %-5d ", e.End)
	}
	fmt.Println("|")

	fmt.Print("+-------")
	for range executions {
		fmt.Print("+-------")
	}
	fmt.Println("+")
}
