
## Task-06

These algo's were taught to us in our OS class so they were easy to manage but the real task was doing it in go.
I was kinda excited to learn a new language based on principles rather than syntax cuz in the end other than
functional languages like haskell, there isn't much of a difference..

Note: Usually, I guess we'd use a queue to do this, but I looked it up and found that Slicing is the most effective way
to simulate a queue in golang. I only used it in Round Robin and used sorting in the other 2.

Note: I handled the IDLE parts simply, The logic is the same across the algorithms, take a current_time variable
and check if the next process' arrival time is higher than it, if it is, then there is an IDLE time. The only thing that
changes across the algorithms is the 'next process'

# FCFS:

First Come First Serve.

As the name says, the first to arrive gets the resources faster.
First, sort based on arrival time and of eq, on pid. Then, 
just put them into a seperate struct called executions and send it to the gantt chart maker.

# SJF

Shortest Job First.

The non-preemptive version of Shortest Remaining time First (SRTF), I made some changes to my struct as well, I added
a Completed field which was a bool flag. I would just check whatever processes are available to me at the time and look
for the min of them, then run it if it wasn't already done executing. Then, just send the executions object to the gantt
chart maker.

# RR

Round Robin.

This one was a bit trickier than the other 2. I used a slice to simulate a queue and sorted the processes by arrival
time first.I kept track of the next process to be added to the queue and ran each process for either the given quantum 
or its remaining burst time. If it still had time left, I put it back into the queue. I also had to account for processes
arriving while another process was executing.I added a Remaining field so the original Burst Time stayed intact for 
calculating Waiting and Turnaround Time. Since a process can appear multiple times in the Gantt chart, I used a map while
calculating the averages so each process was only counted once.

Concepts Learned:

* GoLang {

  1.Type inference while being statically typed
  2.Loops (no while, for felt like a combo of loops from python (cuz of the enumerate func like notation) and java/c)
  3.Structs (Very similar to C but kind of a refresher really)
  4.Maps (very similar to dictionaries in python)
}

Resources used:

1.https://go.dev/tour/list (as mentioned)
2.https://www.geeksforgeeks.org/go-language/go-decision-making-if-if-else-nested-if-if-else-if/
3.Chatgpt (learning)
