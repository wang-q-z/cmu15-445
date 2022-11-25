//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

/*

    Victim(T*) : Remove the object that was accessed the least recently compared to all the elements being tracked by the Replacer, 
                store its contents in the output parameter and return True. If the Replacer is empty return False.
    Pin(T) : This method should be called after a page is pinned to a frame in the BufferPoolManager. 
                It should remove the frame containing the pinned page from the LRUReplacer.
    Unpin(T) : This method should be called when the pin_count of a page becomes 0.
                 This method should add the frame containing the unpinned page to the LRUReplacer.
    Size() : This method returns the number of frames that are currently in the LRUReplacer.



*/
#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages):capacity(num_pages) {}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) { 
    
    //first get the last node and identify whether it is empty
    std::lock_guard<std::mutex> lock(latch);
    if(lru_list.empty())
        return false;
    frame_id_t last = lru_list.back();
    *frame_id = last;
    lru_cache.erase(last);
    lru_list.pop_back();
    return true; 
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch);
    if(lru_cache.count(frame_id) != 0){
    lru_list.remove(frame_id);
    lru_cache.erase(frame_id);
    }

}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch);
    if(lru_cache.find(frame_id) == lru_cache.end()){
        if(lru_list.size() < capacity){
            lru_list.push_front(frame_id);
            lru_cache[frame_id] = 1;
        }else{
            frame_id_t last = lru_list.back();
            lru_list.pop_back();
            lru_cache.erase(last);
            lru_list.push_front(frame_id);
            lru_cache[frame_id] = 1;
        }
    }
}

size_t LRUReplacer::Size() { return lru_list.size(); }

}  // namespace bustub
