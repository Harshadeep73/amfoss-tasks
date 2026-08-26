package main
import "fmt"
type Process struct {
    PID string
    Arrival int
    Burst int
    Remaining int
    CompletionTime int
    WaitingTime int
    TurnaroundTime int
}

type Execution struct {
    PID string
    Start int
    End int
    Waiting int
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

    totalWaiting := 0
    totalTurnaround := 0
    for _, e := range executions {
        totalWaiting += e.Waiting
        totalTurnaround += e.Turnaround
    }
    avgWaiting := totalWaiting / len(executions)
    avgTurnaround := totalTurnaround / len(executions)
    fmt.Printf("\n\nAverage Waiting Time: %d\n", avgWaiting)
    fmt.Printf("Average Turnaround Time: %d\n", avgTurnaround)
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
