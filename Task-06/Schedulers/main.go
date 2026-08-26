package main

import "fmt"

func main() {
    var n int

    fmt.Print("Enter number of processes: ")
    fmt.Scan(&n)

    processes := make([]Process, n)

    for i := 0; i < n; i++ {
        fmt.Printf("\nProcess %d\n", i+1)

        fmt.Print("PID: (String) ")
        fmt.Scan(&processes[i].PID)

        fmt.Print("Arrival Time: (int) ")
        fmt.Scan(&processes[i].Arrival)

        fmt.Print("Burst Time: (int) ")
        fmt.Scan(&processes[i].Burst)
    }

    fmt.Println("Choose a scheduling algorithm:")
    fmt.Println("1. FCFS")
    fmt.Println("2. SJF")
    fmt.Println("3. Round Robin\n")

    var choice int
    fmt.Scan(&choice)

    var Executed []Execution
    switch choice {
    case 1:
        Executed = FCFS(processes)
    case 2:
        Executed = SJF(processes)
    case 3:
        //Executed = roundRobin(processes)
    default:
        fmt.Println("Invalid choice")
    }

    DrawGanttChart(Executed)
    GetAvgs(Executed)

}
