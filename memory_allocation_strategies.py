# First Fit
block_sizes = [100, 500, 200, 300, 600]
process_sizes = [212, 417, 112, 426]
allocation = [-1] * len(process_sizes)
for i in range(len(process_sizes)):
    for j in range(len(block_sizes)):
        if block_sizes[j] >= process_sizes[i]:
            allocation[i] = j
            block_sizes[j] -= process_sizes[i]
            break
print("First Fit:", allocation)

# Best Fit
block_sizes = [100, 500, 200, 300, 600]
process_sizes = [212, 417, 112, 426]
allocation = [-1] * len(process_sizes)
for i in range(len(process_sizes)):
    best_idx = -1
    for j in range(len(block_sizes)):
        if block_sizes[j] >= process_sizes[i]:
            if best_idx == -1 or block_sizes[best_idx] > block_sizes[j]:
                best_idx = j
    if best_idx != -1:
        allocation[i] = best_idx
        block_sizes[best_idx] -= process_sizes[i]
print("Best Fit:", allocation)

# Worst Fit
block_sizes = [100, 500, 200, 300, 600]
process_sizes = [212, 417, 112, 426]
allocation = [-1] * len(process_sizes)
for i in range(len(process_sizes)):
    worst_idx = -1
    for j in range(len(block_sizes)):
        if block_sizes[j] >= process_sizes[i]:
            if worst_idx == -1 or block_sizes[worst_idx] < block_sizes[j]:
                worst_idx = j
    if worst_idx != -1:
        allocation[i] = worst_idx
        block_sizes[worst_idx] -= process_sizes[i]
print("Worst Fit:", allocation)

# Next Fit
block_sizes = [100, 500, 200, 300, 600]
process_sizes = [212, 417, 112, 426]
allocation = [-1] * len(process_sizes)
start_idx = 0
for i in range(len(process_sizes)):
    for j in range(len(block_sizes)):
        idx = (start_idx + j) % len(block_sizes)
        if block_sizes[idx] >= process_sizes[i]:
            allocation[i] = idx
            block_sizes[idx] -= process_sizes[i]
            start_idx = idx
            break
print("Next Fit:", allocation)