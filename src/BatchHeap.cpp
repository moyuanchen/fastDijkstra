#include <BatchHeap.h>
#include <vector>
#include <iterator>
#include <algorithm>


BatchHeap::BatchHeap(int batch_size, int upper_bound) {
    this->M = batch_size;
    this->B = upper_bound;
    Block block_init; block_init.upper_bound = upper_bound;
    this->D1.push_back(block_init); 
    auto block_iterator =std::prev(this->D1.end());
    this->D1_bound[upper_bound] = block_iterator;
};

// O(1) deletion of a key/value pair by looking up memory address
void BatchHeap::del(int key) {
    if (this->address_book_l1_D0.count(key)) {
        auto address_l1 = this->address_book_l1_D0[key];
        auto address_l2 = this->address_book_l2[key];
        Block& block = *(address_l1);
        block.block.erase(address_l2);
        this->address_book_l2.erase(key);
        this->address_book_l1_D0.erase(key);
    }
    else if (address_book_l1_D1.count(key)) {
        auto address_l1 = this->address_book_l1_D1[key];
        auto address_l2 = this->address_book_l2[key];
        Block& block = *(address_l1);
        block.block.erase(address_l2);
        this->address_book_l2.erase(key);
        this->address_book_l1_D1.erase(key);
        if (block.block.empty()) {
            // remove block if empty
            this->D1.erase(address_l1);
        }
    }
}

void BatchHeap::insert(int key, double value) {
    // check existance
    if (this->address_book_l2.count(key)) {
        // compare values
        auto address = this->address_book_l2[key];
        std::pair<int, double> old = *(address);
        if (old.second > value) {
            this->del(key);
        }
        else {
            return;
        }
    }
    //rest
    auto map_iterator = this->D1_bound.lower_bound(value);
    // check if the value is the largest
    if (map_iterator == this->D1_bound.end()) {
        return;
    }
    auto block_iterator = map_iterator->second;
    Block& block = *(block_iterator);
    block.block.push_back({key, value});
    // add to address book
    this->address_book_l2[key] = std::prev(block.block.end());
    this->address_book_l1_D1[key] = block_iterator;
    // check size and if need to split
    if (block.block.size() > this->M) {
        this->split(block_iterator);
    }
}

void BatchHeap::split(std::list<Block>::iterator block_it) {
    Block& block = *(block_it);
    // copy the list to a vector for splitting
    std::vector<std::pair<int, double>> tmp(block.block.begin(), block.block.end());
    auto middle_it = tmp.begin() + tmp.size() / 2;
    // using FIND quickselect to split
    std::nth_element(
        tmp.begin(), 
        middle_it, 
        tmp.end(),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second;
        });
    // initialize two new blocks
    Block smaller; Block larger;
    std::pair<int, double> middle = *(middle_it);
    smaller.upper_bound = middle.second; larger.upper_bound = block.upper_bound;
    smaller.block.assign(tmp.begin(), middle_it);
    larger.block.assign(middle_it, tmp.end());

    // link the two new blocks into D1
    // Remove the old block that was split
    this->D1_bound.erase(block.upper_bound); // Remove from the map using its key
    auto insert_pos_it = D1.erase(block_it); // Erase from the list, get next position

    // Insert the new blocks into the D1 list and get iterators to them
    auto larger_it = D1.insert(insert_pos_it, larger);
    auto smaller_it = D1.insert(larger_it, smaller);

    // Add the new blocks to the D1_bound map so they can be found
    this->D1_bound[smaller.upper_bound] = smaller_it;
    this->D1_bound[larger.upper_bound] = larger_it;

}

void BatchHeap::batchPrepend(std::list<std::pair<int, double>> items) {
    int L = items.size();
    
    if (L <= this->M) {
        // Simple case: create a new block and add to beginning of D0
        Block newBlock;
        newBlock.block = items;
        newBlock.upper_bound = this->B; // Set to maximum bound for D0
        D0.push_front(newBlock);
        
        // Update address books for all items in the new block
        auto block_it = D0.begin();
        for (auto item_it = block_it->block.begin(); item_it != block_it->block.end(); ++item_it) {
            this->address_book_l1_D0[item_it->first] = block_it;
            this->address_book_l2[item_it->first] = item_it;
        }
        return;
    }
    else {
        // Complex case: create O(L/M) blocks, each with at most ⌈M/2⌉ elements
        // Convert list to vector for efficient median finding
        std::vector<std::pair<int, double>> tmp(items.begin(), items.end());
        
        int max_block_size = (this->M + 1) / 2; // ⌈M/2⌉
        
        // Recursively split using medians until all chunks are small enough
        std::vector<std::vector<std::pair<int, double>>> blocks_to_create;
        std::vector<std::vector<std::pair<int, double>>> current_level;
        current_level.push_back(tmp);
        
        while (!current_level.empty()) {
            std::vector<std::vector<std::pair<int, double>>> next_level;
            
            for (auto& chunk : current_level) {
                if (chunk.size() <= max_block_size) {
                    // This chunk is small enough, add it to final blocks
                    blocks_to_create.push_back(chunk);
                } else {
                    // Split this chunk using median
                    auto middle_it = chunk.begin() + chunk.size() / 2;
                    std::nth_element(
                        chunk.begin(), 
                        middle_it, 
                        chunk.end(),
                        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                            return a.second < b.second;
                        });
                    
                    // Create two sub-chunks
                    std::vector<std::pair<int, double>> left_chunk(chunk.begin(), middle_it);
                    std::vector<std::pair<int, double>> right_chunk(middle_it, chunk.end());
                    
                    if (!left_chunk.empty()) next_level.push_back(left_chunk);
                    if (!right_chunk.empty()) next_level.push_back(right_chunk);
                }
            }
            
            current_level = next_level;
        }
        
        // Create blocks and add them to the beginning of D0
        for (auto& block_data : blocks_to_create) {
            Block newBlock;
            newBlock.block.assign(block_data.begin(), block_data.end());
            newBlock.upper_bound = this->B; // Set to maximum bound for D0
            D0.push_front(newBlock);
            
            // Update address books for all items in the new block
            auto block_it = D0.begin();
            for (auto item_it = block_it->block.begin(); item_it != block_it->block.end(); ++item_it) {
                this->address_book_l1_D0[item_it->first] = block_it;
                this->address_book_l2[item_it->first] = item_it;
            }
        }
    }
}

PullResults BatchHeap::pull() {
    PullResults result;
    
    // Step 1: Collect sufficient prefix of blocks from D0 and D1
    std::vector<std::pair<int, double>> S0_prime, S1_prime;
    
    // Collect from D0 until we have M elements or exhaust D0
    int collected_D0 = 0;
    for (auto& block : this->D0) {
        if (collected_D0 >= this->M) break;
        
        for (auto& item : block.block) {
            if (collected_D0 >= this->M) break;
            S0_prime.push_back(item);
            collected_D0++;
        }
    }
    
    // Collect from D1 until we have M elements or exhaust D1
    int collected_D1 = 0;
    for (auto& block : this->D1) {
        if (collected_D1 >= this->M) break;
        
        for (auto& item : block.block) {
            if (collected_D1 >= this->M) break;
            S1_prime.push_back(item);
            collected_D1++;
        }
    }
    
    // Step 2: Check if we have collected all elements or need to find smallest M
    int total_collected = S0_prime.size() + S1_prime.size();
    
    if (total_collected <= this->M) {
        // Case 1: S'0 ∪ S'1 contains no more than M elements
        // Return all elements and set x to upper bound B
        result.vertices.reserve(total_collected);
        
        for (auto& item : S0_prime) {
            result.vertices.push_back(item.first);
        }
        for (auto& item : S1_prime) {
            result.vertices.push_back(item.first);
        }
        
        result.new_bound = this->B;
        
        // Delete all collected elements from D0 and D1
        for (auto& item : S0_prime) {
            this->del(item.first);
        }
        for (auto& item : S1_prime) {
            this->del(item.first);
        }
    }
    else {
        // Case 2: We need to find the smallest M elements from S'0 ∪ S'1
        // Combine S'0 and S'1
        std::vector<std::pair<int, double>> combined;
        combined.reserve(total_collected);
        combined.insert(combined.end(), S0_prime.begin(), S0_prime.end());
        combined.insert(combined.end(), S1_prime.begin(), S1_prime.end());
        
        // Find the M smallest elements using nth_element
        auto kth_it = combined.begin() + this->M;
        std::nth_element(
            combined.begin(),
            kth_it,
            combined.end(),
            [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                return a.second < b.second;
            }
        );
        
        // Extract the M smallest elements
        result.vertices.reserve(this->M);
        for (auto it = combined.begin(); it != kth_it; ++it) {
            result.vertices.push_back(it->first);
        }
        
        // Set x to the smallest remaining value in D0 ∪ D1 after deletion
        // We need to find this among elements not selected for deletion
        double min_remaining = this->B; // Start with upper bound
        bool found_remaining = false;
        
        // Check remaining elements in combined that weren't selected
        for (auto it = kth_it; it != combined.end(); ++it) {
            min_remaining = std::min(min_remaining, it->second);
            found_remaining = true;
        }
        
        // Check remaining elements in D0 and D1 not in S'0 ∪ S'1
        int checked_D0 = 0;
        for (auto& block : this->D0) {
            for (auto& item : block.block) {
                if (checked_D0 >= this->M) {
                    min_remaining = std::min(min_remaining, item.second);
                    found_remaining = true;
                }
                checked_D0++;
            }
        }
        
        int checked_D1 = 0;
        for (auto& block : this->D1) {
            for (auto& item : block.block) {
                if (checked_D1 >= this->M) {
                    min_remaining = std::min(min_remaining, item.second);
                    found_remaining = true;
                }
                checked_D1++;
            }
        }
        
        result.new_bound = found_remaining ? min_remaining : this->B;
        
        // Delete the M smallest elements from D0 and D1
        for (auto it = combined.begin(); it != kth_it; ++it) {
            this->del(it->first);
        }
    }
    
    return result;
}