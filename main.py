pages = [1, 2, 3, 4, 5, 1, 2, 5, 1, 2, 3, 4, 5, 1,
         6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 4, 5, 4, 2]

frames = 4

# Optimal
print("\n\nOPTIMAL")
optimal_faults = 0
optimal_hits = 0
optimal_pages = []

for i in range(len(pages)):
    if pages[i] not in optimal_pages:
        if len(optimal_pages) < frames:
            optimal_pages.append(pages[i])
        else:
            future_pages = pages[i+1:]
            replaceable_pages = [
                page for page in optimal_pages if page not in future_pages]
            if replaceable_pages:
                optimal_pages.remove(replaceable_pages[0])
            else:
                for page in optimal_pages:
                    if page not in future_pages[frames:]:
                        optimal_pages.remove(page)
                        break
            optimal_pages.append(pages[i])
        optimal_faults += 1
    else:
        optimal_hits += 1
        print(f'Hit at index {i} for page {pages[i]}')

# FIFO
print("\n\nFIFO")
fifo_faults = 0
fifo_hits = 0
fifo_pages = []

for i, page in enumerate(pages):
    if page not in fifo_pages:
        if len(fifo_pages) < frames:
            fifo_pages.append(page)
        else:
            fifo_pages.pop(0)
            fifo_pages.append(page)
        fifo_faults += 1
    else:
        fifo_hits += 1
        print(f'Hit at index {i} for page {page}')

# LRU
print("\n\nLRU")
lru_faults = 0
lru_hits = 0
lru_pages = []
recently_used = []

for i, page in enumerate(pages):
    if page not in lru_pages:
        if len(lru_pages) < frames:
            lru_pages.append(page)
        else:
            lru_pages.remove(recently_used.pop(0))
            lru_pages.append(page)
        lru_faults += 1
    else:
        recently_used.remove(page)
        lru_hits += 1
        print(f'Hit at index {i} for page {page}')
    recently_used.append(page)

print("\n\nOptimal Page Faults: ", optimal_faults)
print("Optimal Page Hits: ", optimal_hits)
print("FIFO Page Faults: ", fifo_faults)
print("FIFO Page Hits: ", fifo_hits)
print("LRU Page Faults: ", lru_faults)
print("LRU Page Hits: ", lru_hits)
