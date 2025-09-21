#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

// --- Shared Data ---
int shared_data = 0;
int reader_count = 0;

// --- Synchronization Primitives ---
pthread_mutex_t mutex; // Mutex for reader_count
pthread_mutex_t rw_mutex; // Mutex for shared_data (the "resource")

// --- Function Prototypes ---
void* reader_with_sync(void* arg);
void* writer_with_sync(void* arg);
void* reader_without_sync(void* arg);
void* writer_without_sync(void* arg);

void run_simulation(bool with_synchronization) {
    if (with_synchronization) {
        std::cout << "\n--- Running Simulation WITH Synchronization ---\n" << std::endl;
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&rw_mutex, NULL);
    } else {
        std::cout << "\n--- Running Simulation WITHOUT Synchronization ---\n" << std::endl;
        std::cout << "*** Expect race conditions and inconsistent data ***\n" << std::endl;
    }

    shared_data = 0; // Reset data for each run
    
    // Create multiple reader and writer threads
    pthread_t readers[5], writers[2];
    int reader_ids[5], writer_ids[2];

    for (int i = 0; i < 5; ++i) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, with_synchronization ? reader_with_sync : reader_without_sync, &reader_ids[i]);
    }
    for (int i = 0; i < 2; ++i) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, with_synchronization ? writer_with_sync : writer_without_sync, &writer_ids[i]);
    }

    // Join threads
    for (int i = 0; i < 5; ++i) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(writers[i], NULL);
    }

    if (with_synchronization) {
        pthread_mutex_destroy(&mutex);
        pthread_mutex_destroy(&rw_mutex);
    }
    std::cout << "\nFinal value of shared data: " << shared_data << std::endl;
    std::cout << "----------------------------------------------\n" << std::endl;
}


int main() {
    // Run the simulation without synchronization to see the problem
    run_simulation(false);
    
    sleep(2); // Pause between simulations for clarity

    // Run the simulation with synchronization to see the solution
    run_simulation(true);

    return 0;
}


// --- Implementations WITH Synchronization ---

void* writer_with_sync(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&rw_mutex); // Lock resource for exclusive access
        
        // --- Critical Section for Writing ---
        int old_val = shared_data;
        shared_data++;
        std::cout << "Writer " << id << " changed data from " << old_val << " to " << shared_data << std::endl;
        // --- End of Critical Section ---
        
        pthread_mutex_unlock(&rw_mutex); // Unlock resource
        sleep(1); // Simulate some work
    }
    return NULL;
}

void* reader_with_sync(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; ++i) {
        // Entry section for readers
        pthread_mutex_lock(&mutex);
        reader_count++;
        if (reader_count == 1) {
            pthread_mutex_lock(&rw_mutex); // First reader locks the resource
        }
        pthread_mutex_unlock(&mutex);

        // --- Critical Section for Reading ---
        std::cout << "Reader " << id << " read data as: " << shared_data << std::endl;
        // --- End of Critical Section ---
        
        // Exit section for readers
        pthread_mutex_lock(&mutex);
        reader_count--;
        if (reader_count == 0) {
            pthread_mutex_unlock(&rw_mutex); // Last reader unlocks the resource
        }
        pthread_mutex_unlock(&mutex);
        usleep(500000); // Simulate some work
    }
    return NULL;
}


// --- Implementations WITHOUT Synchronization ---

void* writer_without_sync(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 3; ++i) {
        int old_val = shared_data;
        shared_data++;
        // The read (old_val) and write (shared_data++) are not atomic,
        // leading to a race condition.
        std::cout << "Writer " << id << " changed data from " << old_val << " to " << shared_data << std::endl;
        sleep(1);
    }
    return NULL;
}

void* reader_without_sync(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; ++i) {
        // Reading while a writer might be in the middle of an update
        std::cout << "Reader " << id << " read data as: " << shared_data << std::endl;
        usleep(500000);
    }
    return NULL;
}
