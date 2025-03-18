//
//  main.cpp
//  CS3360-Code5 - CPU Scheduling
//
//  Created by Darren Bowers on 3/6/25, following Professor Manouchehr Mohandesi's examples.
//

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::queue;
using std::cin;

// Structure of a process to keep track of its own metrics through the scheduling process
struct Process{
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    bool finished = false;
};

// First Come First Serve Scheduling
void FCFS(vector<Process>& processes){
    int time = 0; // Initiate timer 
    for (auto& p: processes){ // Loops through each element in processes vector
        if (time < p.arrival_time) time = p.arrival_time; // If the current time is less than the arrival time of the current process, skip to arrival time
        p.completion_time = time + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.finished = true; 
        time += p.burst_time;
    }
}

// Shortest Job First Scheduling (Non-Preemptive)
void SJF(vector<Process>& processes){
    int time = 0, completed = 0; // Initiate timer and completion variables for while loop
    int n = processes.size(); // Set variable to stop while loop
    while (completed < n){
        // Find all processes that have arrived by the current time and are not finished
        vector<Process*> ready_queue; // Create or recreate ready queue for each loop
        for (auto& p : processes){ // Loops through each element in processes vector
            if (p.arrival_time <= time && !p.finished){ // If the process has arrived and hasn't been previously completed, add it to the queue
                ready_queue.push_back(&p);
            }
        }
        if(!ready_queue.empty()){
            // Sort the ready queue by burst time (shortest burst time first)
            sort(ready_queue.begin(), ready_queue.end(), [](Process* p1, Process* p2) { // LEARNED LAMBDA 
                return p1->burst_time < p2->burst_time;});
            // Execute the process with the shortest burst time
            Process* p = ready_queue[0]; // After sort, choose the process with the shortest burst time and update variables
            time += p->burst_time;
            p->completion_time = time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->finished = true; 
            completed++; 
        } else {
            time++;
        }
    }
}

// Round Robin Scheduling
void RR(vector<Process>& processes, int time_quantum){
    queue<Process*> ready_queue;
    int time = 0, completed = 0; // Initiate timer and completion variables for while loop
    int n = processes.size();
    int current_index = 0; // Track which processes have arrived
    while(completed < n){
        // Add processes that have arrived by the current time to the ready queue
        while (current_index < n && processes[current_index].arrival_time <= time){
            ready_queue.push(&processes[current_index]);
            current_index++;
        }
        if(!ready_queue.empty()){ // If the ready queue isn't empty, grab the first process and remove it from ready queue
            Process* p = ready_queue.front();
            ready_queue.pop();
            if(p->remaining_time > time_quantum){ // If the remaining process time is greater than the quantum time, increase time by quantum time
                time+= time_quantum; 
                p->remaining_time -= time_quantum;
            } else{ // If remaining process time is less than quantum time, increase time by the remaining process time and adjust variables
                time += p->remaining_time;
                p->remaining_time -= time_quantum;
                p->completion_time = time;
                p->turnaround_time = p->completion_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->burst_time;
                p->finished = true;
                completed++;
            }
            // Re-add the process to ready queue if it has any remaining process time
            if (p->remaining_time > 0){
                ready_queue.push(p);
            }
            // Add any new processes that arrived during execution 

            /***********
            This loop does the same thing as the original nested while loop, but it only makes sense to place it here again if the program was expecting more processes
            than it was initially given. However, since the program is designed to only process the exact number of processes it was given, this loop becomes redundant. 
            If this loop runs and theres still incomplete processes then the outer loop restarts and immediately runs this same while loop.
            If it runs and there isn't any incomplete processes then the results of the loop weren't needed to begin with. 
            while (current_index < n && processes[current_index].arrival_time <= time){
                ready_queue.push(&processes[current_index]);
                current_index++;
            }***********/
        } else {
            time++; // If no processess are ready, advance time
        }
    }
}

// Function to display results

void displayResults(const vector<Process>& processes){
    double total_waiting_time = 0, total_turnaround_time = 0;
    cout << "Process\tArrival\tBurst\tComp\tWaiting\tTurnaround\n";
    for (const auto& p: processes){
        cout << "P" << p.id << "\t" << p.arrival_time << "\t" << p.burst_time << "\t" << p.completion_time << "\t" << p.waiting_time << "\t" << p.turnaround_time << "\n";
        total_waiting_time += p.waiting_time;
        total_turnaround_time += p.turnaround_time;
    }
    cout << "Average waiting time: " << total_waiting_time / processes.size() << endl;
    cout << "Average turnaround time: " << total_turnaround_time / processes.size() << endl;
}

int main(){
    int n; // Initialize parameters
    cout << "Enter the number of processes: ";
    cin >> n; 

    vector<Process> processes(n);
    cout << "Enter the arrival times and burst times for each process: " << endl;
    for (int i = 0; i<n; ++i){
        processes[i].id = i;
        cout << "P" << i << ": ";
        cin >> processes[i].arrival_time >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    vector<Process> processes_fcfs = processes; // Seperate out data to be used in each algorithm type
    vector<Process> processes_sjf = processes;
    vector<Process> processes_rr = processes;

    // FCFS
    cout << endl << "--- FCFS Scheduling Results --- " << endl;
    FCFS(processes_fcfs);
    displayResults(processes_fcfs);

    // SJF
    cout << endl << "--- SJF Scheduling Results --- " << endl;
    SJF(processes_sjf);
    displayResults(processes_sjf);

    // RR
    int time_quantum;
    cout << endl << "Enter time quantum for Round Robin: ";
    cin >> time_quantum;
    cout << endl << "--- RR Scheduling Results --- " << endl;
    RR(processes_rr, time_quantum);
    displayResults(processes_rr);

    return 0;
}