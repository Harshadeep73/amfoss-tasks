#Task 5

A clone of htop, building it was fun as it was exhausting tbh. I learnt a lot about the linux file system though.

I did mine in Systems C.

#Resources:

#Youtube:

1) https://www.youtube.com/watch?v=0XdjODvsRN8

For the core /proc file system and the actual stuff I'd be working off of.

2) https://www.youtube.com/watch?v=I6T7nSO-WNg

For the stuff I'd be cloning and the under-the-hood know of the actual thing.

3) https://www.youtube.com/watch?v=yQ9Ns6Z4Q-s

For the UI of mine, cuz I did mine in C.

#GeeksforGeeks:

https://www.geeksforgeeks.org/linux-unix/proc-file-system-linux/

For better understanding of the proc directory

Now, The actual Process, Imma divide it into Levels:


#LVL 1: Reading off of /proc:

I went to proc and simply format-printed the memory part and process name. Then, I started to do the actual thing.

I saw that I needed specific fields of data within the /proc/pid. So I used the different files inside each PID directory.

For the name I used /proc/pid/comm and for memory I used /proc/pid/status and searched for VmRSS.

For CPU, I went into /proc/pid/stat.

#LVL 2: CPU:

This part had a bit of computation to it.

I needed fields 14 and 15 from /proc/pid/stat:

14 -> utime
15 -> stime

Added them together and got the process CPU ticks.

But those were cumulative, so they weren't actually the CPU percentage.

So I stored the previous value and calculated the difference every 0.25 seconds.

I did the same thing with the total CPU ticks from /proc/stat and used the two deltas to calculate the CPU usage.

This was probably the part where I understood the most about how CPU usage actually works.

#LVL 3: Dynamic storage:

I didn't want to just make some fixed array like Process processes[100], so I used realloc().

So, I just started with 64 and kept on doubling it to make a dynamic allocation with some headroom tho. 

#LVL 4: Sorting:

Once I had all the processes, I wanted the biggest CPU/RAM consumers at the top.

So I used qsort() with two comparison functions.

F4 switches between:

CPU ↓

and:

RAM ↓

This was also my first proper use of qsort with comparator functions.

#LVL 5: UI:

I just used ANSI escape sequences and used manual positioning for now.

I made the CPU matrix, RAM/Swap section, process table and footer.

I also used the alternate terminal screen so that exiting HYPRTOP doesn't mess up the normal terminal.

#LVL 6: Input:

I needed function keys and arrow keys to work without pressing Enter, so I used termios and non-blocking input.

Then I manually parsed the escape sequences and got to know which is what through cat (Spent a lot of time debugging that :( )

#LVL 7: Search + Selection:

F3 became search.

I just compare the search string against the process names and only display the matching ones.

Escape clears the search.

Then I added UP/DOWN to select a process and highlighted the selected row.

#LVL 8: Process control:

Once selection worked, F2 and F9 were easy.

F2 -> SIGTERM
F9 -> SIGKILL

Both are sent to the PID of the selected process.

F10 just quits and restores the terminal.

#Controls:

F1  Help
F2  Terminate
F3  Search
F4  CPU/RAM
↑↓  Select
F9  Kill
F10 Quit

#Result:

A working htop-like process monitor made entirely in C.

It reads directly from /proc, dynamically stores the processes, calculates CPU usage from tick deltas, sorts them, displays everything in a custom terminal 
UI and lets me search, select and terminate processes.

Had a lot of fun btw. It was genuinely exhausting to build, especially the manual positioning of the terminal input/UI stuff, but I learnt a lot 
about Linux and C through this.
