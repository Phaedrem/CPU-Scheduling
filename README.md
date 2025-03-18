# CPU-Scheduling

This program included three CPU scheduling algorithms: First-Come First-Served (FCFS), Shortest Job First (SJF), and Round Robin (RR). Each algorithm processes a list of Process structures, which track key scheduling metrics such as arrival time, burst time, waiting time, turnaround time, and completion time.

First-Come, First-Served (FCFS) Implementation
•	The FCFS function processes jobs in the order they arrive and uses a loop that iterates through each process, checking if the CPU is idle and adjusting the start time if necessary. If the CPU isn’t idle then the process executes for its full burst time, and then its completion, turnaround, and waiting times are recorded.

Shortest Job First (SJF) Implementation
•	The SJF function repeatedly finds the shortest available job and executes it. At each iteration, a ready queue is built from processes that have arrived but are not completed. The queue is sorted by burst time using the sort function and a lambda comparator, which was a new usage to I hadn’t seen before. The shortest job executes fully, updating process metrics.

Round Robin (RR) Implementation
•	The RR function simulates time-sliced execution by using a queue. The processes enter the queue as they arrive. Every loop the first process is dequeued, executes for either the full length of the time quantum or the processes remaining time, then either completes or re-enters the queue. The loop ensures all processes receive fair execution time.

Impact of Time Quantum in Round Robin
•	Smaller time quantum such as 3 increased context switching overhead, reducing efficiency. Larger time quantum such as 20 or higher behaved more like FCFS. The optimal time quantum depended on the average burst time—setting it too low results in overhead, while too high leads to poor response times.
