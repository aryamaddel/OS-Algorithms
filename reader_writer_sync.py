import threading
import time
import random

# Shared resource
shared_data = 0

# Synchronization primitives
read_count = 0
read_count_lock = threading.Lock()
resource_lock = threading.Lock()
can_read = threading.Condition(resource_lock)

# -------------------------------
# Reader function (with sync)
# -------------------------------
def reader(reader_id, synchronized=True):
    global read_count, shared_data
    while True:
        time.sleep(random.uniform(0.5, 2))  # simulate delay

        if synchronized:
            # Readers increment count safely
            with read_count_lock:
                read_count += 1
                if read_count == 1:
                    resource_lock.acquire()  # first reader locks writers

            print(f"📖 Reader-{reader_id} reads data: {shared_data}")

            # Readers decrement count safely
            with read_count_lock:
                read_count -= 1
                if read_count == 0:
                    resource_lock.release()  # last reader releases writers
        else:
            # No synchronization (unsafe read)
            print(f"[UNSAFE] 📖 Reader-{reader_id} reads data: {shared_data}")

# -------------------------------
# Writer function (with sync)
# -------------------------------
def writer(writer_id, synchronized=True):
    global shared_data
    while True:
        time.sleep(random.uniform(1, 3))  # simulate delay
        if synchronized:
            with resource_lock:  # exclusive access
                shared_data += 1
                print(f"Writer-{writer_id} writes data: {shared_data}")
        else:
            # No synchronization (unsafe write)
            shared_data += 1
            print(f"[UNSAFE] Writer-{writer_id} writes data: {shared_data}")

# -------------------------------
# Main execution
# -------------------------------
def run_demo(synchronized=True):
    threads = []

    # Create 3 readers and 2 writers
    for i in range(3):
        t = threading.Thread(target=reader, args=(i+1, synchronized))
        threads.append(t)

    for i in range(2):
        t = threading.Thread(target=writer, args=(i+1, synchronized))
        threads.append(t)

    # Start all threads
    for t in threads:
        t.start()

    # Let the program run for some time
    time.sleep(15)
    print("\n--- Demo finished ---\n")

if __name__ == "__main__":
    print("Running WITHOUT synchronization (unsafe):")
    run_demo(synchronized=False)

    print("Running WITH synchronization (safe):")
    run_demo(synchronized=True)
