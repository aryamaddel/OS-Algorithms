def optimal_page_replacement(pages, frames):
    frame_list = []
    page_faults = 0

    for i in range(len(pages)):
        if pages[i] not in frame_list:
            if len(frame_list) < frames:
                frame_list.append(pages[i])
            else:
                future_list = pages[i+1:]
                farthest_page = -1
                farthest_index = -1
                for frame in frame_list:
                    if frame not in future_list:
                        farthest_page = frame
                        break
                    else:
                        frame_index = future_list.index(frame)
                        if frame_index > farthest_index:
                            farthest_index = frame_index
                            farthest_page = frame
                frame_list.remove(farthest_page)
                frame_list.append(pages[i])
            page_faults += 1
    return page_faults


pages = [1, 2, 3, 4, 5, 1, 2, 5, 1, 2, 3, 4, 5, 1,
         6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 4, 5, 4, 2]
frames = 4
page_faults = optimal_page_replacement(pages, frames)
print("The number of page faults is", page_faults)
