#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>

struct Process {
    int id;
    int burst_time;
    int arrival_time;
    int priority;
    int remaining_time;
    int start_time = 0;
    int completion_time = 0;
    int waiting_time = 0;
    int turnaround_time = 0;
};

void print_results(const std::vector<Process>& processes, const std::string& algorithm_name) {
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    
    std::cout << "\n--- " << algorithm_name << " Scheduling ---" << std::endl;
    std::cout << std::setw(5) << "PID" << std::setw(10) << "Burst" << std::setw(10) << "Arrival"
              << std::setw(10) << "Waiting" << std::setw(15) << "Turnaround" << std::endl;

    for (const auto& p : processes) {
        std::cout << std::setw(5) << p.id << std::setw(10) << p.burst_time << std::setw(10) << p.arrival_time
                  << std::setw(10) << p.waiting_time << std::setw(15) << p.turnaround_time << std::endl;
        total_waiting_time += p.waiting_time;
        total_turnaround_time += p.turnaround_time;
    }
    std::cout << "\nAverage Waiting Time: " << total_waiting_time / processes.size() << std::endl;
    std::cout << "Average Turnaround Time: " << total_turnaround_time / processes.size() << std::endl;
    std::cout << "------------------------------------------\n" << std::endl;
}

// First-Come, First-Served
void fcfs(std::vector<Process> processes) {
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    for (auto& p : processes) {
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        p.waiting_time = current_time - p.arrival_time;
        current_time += p.burst_time;
        p.turnaround_time = current_time - p.arrival_time;
    }
    print_results(processes, "FCFS");
}

// Shortest Job First (Non-Preemptive)
void sjf(std::vector<Process> processes) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::vector<Process> final_processes;
    std::vector<bool> is_completed(n, false);

    while (completed != n) {
        int shortest_job_index = -1;
        int min_burst_time = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst_time) {
                    min_burst_time = processes[i].burst_time;
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index == -1) {
            current_time++;
        } else {
            Process& p = processes[shortest_job_index];
            p.waiting_time = current_time - p.arrival_time;
            current_time += p.burst_time;
            p.turnaround_time = current_time - p.arrival_time;
            is_completed[shortest_job_index] = true;
            final_processes.push_back(p);
            completed++;
        }
    }
    print_results(final_processes, "SJF (Non-Preemptive)");
}

// Priority Scheduling (Non-Preemptive)
void priority(std::vector<Process> processes) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    std::vector<Process> final_processes;
    std::vector<bool> is_completed(n, false);

    while(completed != n) {
        int highest_priority_index = -1;
        int max_priority = INT_MAX; // Lower number means higher priority

        for(int i=0; i<n; i++) {
            if(processes[i].arrival_time <= current_time && !is_completed[i]) {
                if(processes[i].priority < max_priority) {
                    max_priority = processes[i].priority;
                    highest_priority_index = i;
                }
            }
        }
        
        if (highest_priority_index == -1) {
            current_time++;
        } else {
            Process& p = processes[highest_priority_index];
            p.waiting_time = current_time - p.arrival_time;
            current_time += p.burst_time;
            p.turnaround_time = current_time - p.arrival_time;
            is_completed[highest_priority_index] = true;
            final_processes.push_back(p);
            completed++;
        }
    }
    print_results(final_processes, "Priority (Non-Preemptive)");
}


// Round Robin
void round_robin(std::vector<Process> processes, int quantum) {
    int current_time = 0;
    int n = processes.size();
    std::vector<int> ready_queue;
    std::vector<Process> final_processes;

    int remaining_processes = n;
    int arrived_count = 0;
    std::vector<bool> in_queue(n, false);

    while(remaining_processes > 0) {
        // Add processes to ready queue as they arrive
        for(int i=0; i<n; ++i) {
            if(processes[i].arrival_time <= current_time && !in_queue[i] && processes[i].remaining_time > 0) {
                 ready_queue.push_back(i);
                 in_queue[i] = true;
            }
        }
        
        if (ready_queue.empty()) {
            current_time++;
            continue;
        }

        int current_process_index = ready_queue.front();
        ready_queue.erase(ready_queue.begin());
        
        Process& p = processes[current_process_index];

        int time_to_run = std::min(quantum, p.remaining_time);
        current_time += time_to_run;
        p.remaining_time -= time_to_run;

        // Add newly arrived processes before putting the current one back
        for(int i=0; i<n; ++i) {
            if(processes[i].arrival_time <= current_time && !in_queue[i] && processes[i].remaining_time > 0) {
                 ready_queue.push_back(i);
                 in_queue[i] = true;
            }
        }

        if(p.remaining_time > 0) {
            ready_queue.push_back(current_process_index);
        } else {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            final_processes.push_back(p);
            remaining_processes--;
        }
    }
    print_results(final_processes, "Round Robin");
}

int main() {
    std::vector<Process> processes = {
        {1, 10, 0, 3}, {2, 1, 1, 1}, {3, 2, 2, 4}, {4, 1, 3, 5}, {5, 5, 4, 2}
    };
    for(auto& p : processes) {
        p.remaining_time = p.burst_time;
    }

    int time_quantum = 2;

    fcfs(processes);
    sjf(processes);
    priority(processes);
    round_robin(processes, time_quantum);

    return 0;
}
