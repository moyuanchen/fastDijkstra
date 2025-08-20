#include <BatchHeap.h>
#include "Debug.h"
#include <vector>
#include <iterator>
#include <algorithm>


BatchHeap::BatchHeap(int batch_size, int upper_bound) {
    DEBUG_FUNCTION_ENTRY("BatchHeap::BatchHeap", "batch_size=" << batch_size << ", upper_bound=" << upper_bound);
    
    this->M = batch_size;
    this->B = upper_bound;
    
    DEBUG_PRINT("Initializing BatchHeap with M=" << M << ", B=" << B);
    DEBUG_MEMORY("Creating initial block with upper_bound=" << upper_bound);
    
    Block block_init; 
    block_init.upper_bound = upper_bound;
    this->D1.push_back(block_init); 
    auto block_iterator = std::prev(this->D1.end());
    this->D1_bound[upper_bound] = block_iterator;
    
    DEBUG_PRINT("Initial D1 block created, D1.size()=" << D1.size() << ", D1_bound.size()=" << D1_bound.size());
};

// O(1) deletion of a key/value pair by looking up memory address
void BatchHeap::del(int key) {
    DEBUG_FUNCTION_ENTRY("BatchHeap::del", "key=" << key);
    
    if (this->address_book_l1_D0.count(key)) {
        DEBUG_PRINT("Deleting key " << key << " from D0");
        auto address_l1 = this->address_book_l1_D0[key];
        auto address_l2 = this->address_book_l2[key];
        DEBUG_POINTER_CHECK(&(*address_l1), "address_l1 block");
        DEBUG_POINTER_CHECK(&(*address_l2), "address_l2 pair");
        
        Block& block = *(address_l1);
        size_t old_size = block.block.size();
        block.block.erase(address_l2);
        DEBUG_DATASTRUCTURE("ERASE", "D0 block size changed from " << old_size << " to " << block.block.size());
        
        this->address_book_l2.erase(key);
        this->address_book_l1_D0.erase(key);
    }
    else if (address_book_l1_D1.count(key)) {
        DEBUG_PRINT("Deleting key " << key << " from D1");
        auto address_l1 = this->address_book_l1_D1[key];
        auto address_l2 = this->address_book_l2[key];
        DEBUG_POINTER_CHECK(&(*address_l1), "address_l1 block");
        DEBUG_POINTER_CHECK(&(*address_l2), "address_l2 pair");
        
        Block& block = *(address_l1);
        size_t old_size = block.block.size();
        double block_upper_bound = block.upper_bound;  // Store before potential deletion
        block.block.erase(address_l2);
        DEBUG_DATASTRUCTURE("ERASE", "D1 block size changed from " << old_size << " to " << block.block.size());
        
        this->address_book_l2.erase(key);
        this->address_book_l1_D1.erase(key);
        if (block.block.empty()) {
            DEBUG_PRINT("Block is empty after deletion, removing from D1 and D1_bound");
            // CRITICAL FIX: Remove from D1_bound map before erasing from D1
            this->D1_bound.erase(block_upper_bound);
            DEBUG_PRINT("Removed block with upper_bound=" << block_upper_bound << " from D1_bound");
            // remove block if empty
            this->D1.erase(address_l1);
            DEBUG_PRINT("Removed empty block from D1, new D1.size()=" << D1.size());
        }
    } else {
        DEBUG_PRINT("Key " << key << " not found in either D0 or D1");
    }
}

void BatchHeap::insert(int key, double value) {
    DEBUG_FUNCTION_ENTRY("BatchHeap::insert", "key=" << key << ", value=" << value);
    
    // check existence
    if (this->address_book_l2.count(key)) {
        DEBUG_PRINT("Key " << key << " already exists, checking for improvement");
        // compare values
        auto address = this->address_book_l2[key];
        DEBUG_POINTER_CHECK(&(*address), "existing address");
        std::pair<int, double> old = *(address);
        DEBUG_PRINT("Existing value=" << old.second << ", new value=" << value);
        
        if (old.second > value) {
            DEBUG_PRINT("New value is better, deleting old entry");
            this->del(key);
        }
        else {
            DEBUG_PRINT("Existing value is better or equal, skipping insert");
            return;
        }
    }
    
    DEBUG_PRINT("Proceeding with insert for key=" << key << ", value=" << value);
    
    // Debug D1_bound state
    DEBUG_PRINT("D1_bound map has " << this->D1_bound.size() << " entries:");
    for (const auto& pair : this->D1_bound) {
        DEBUG_PRINT("  bound=" << pair.first << " -> block_ptr");
    }
    
    //rest
    auto map_iterator = this->D1_bound.lower_bound(value);
    // check if the value is the largest
    if (map_iterator == this->D1_bound.end()) {
        DEBUG_PRINT("Value " << value << " is larger than any block bound, skipping insert");
        DEBUG_PRINT("Available bounds in D1_bound:");
        for (const auto& pair : this->D1_bound) {
            DEBUG_PRINT("  bound=" << pair.first);
        }
        return;
    }
    
    DEBUG_PRINT("Found suitable block with bound=" << map_iterator->first << " for value=" << value);
    auto block_iterator = map_iterator->second;
    DEBUG_POINTER_CHECK(&(*block_iterator), "block_iterator");
    Block& block = *(block_iterator);
    
    size_t old_size = block.block.size();
    block.block.push_back({key, value});
    DEBUG_DATASTRUCTURE("INSERT", "Added pair (" << key << ", " << value << ") to block, size: " << old_size << " -> " << block.block.size());
    
    // add to address book
    this->address_book_l2[key] = std::prev(block.block.end());
    this->address_book_l1_D1[key] = block_iterator;
    DEBUG_PRINT("Updated address books for key=" << key);
    
    // check size and if need to split
    if (block.block.size() > this->M) {
        DEBUG_PRINT("Block size " << block.block.size() << " > M=" << M << ", triggering split");
        this->split(block_iterator);
    }
}

void BatchHeap::split(std::list<Block>::iterator block_it) {
    DEBUG_FUNCTION_ENTRY("BatchHeap::split", "splitting block");
    
    Block& block = *(block_it);
    size_t original_size = block.block.size();
    DEBUG_PRINT("Splitting block with size=" << original_size << ", upper_bound=" << block.upper_bound);
    DEBUG_PRINT("D1.size() before split=" << D1.size() << ", D1_bound.size()=" << D1_bound.size());
    
    // copy the list to a vector for splitting
    std::vector<std::pair<int, double>> tmp(block.block.begin(), block.block.end());
    auto middle_it = tmp.begin() + tmp.size() / 2;
    
    DEBUG_MEMORY("Created temporary vector for splitting, size=" << tmp.size());
    DEBUG_PRINT("Middle element will be at index=" << (middle_it - tmp.begin()));
    
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
    
    DEBUG_PRINT("Split into smaller block: size=" << smaller.block.size() << ", upper_bound=" << smaller.upper_bound);
    DEBUG_PRINT("Split into larger block: size=" << larger.block.size() << ", upper_bound=" << larger.upper_bound);

    // link the two new blocks into D1
    // Remove the old block that was split
    DEBUG_PRINT("Removing old block with upper_bound=" << block.upper_bound << " from D1_bound");
    this->D1_bound.erase(block.upper_bound); // Remove from the map using its key
    DEBUG_PRINT("Erasing old block from D1 list");
    auto insert_pos_it = D1.erase(block_it); // Erase from the list, get next position

    // Insert the new blocks into the D1 list and get iterators to them
    DEBUG_PRINT("Inserting new blocks into D1 list");
    auto larger_it = D1.insert(insert_pos_it, larger);
    auto smaller_it = D1.insert(larger_it, smaller);

    // Add the new blocks to the D1_bound map so they can be found
    DEBUG_PRINT("Adding new blocks to D1_bound map");
    this->D1_bound[smaller.upper_bound] = smaller_it;
    this->D1_bound[larger.upper_bound] = larger_it;
    
    DEBUG_PRINT("D1.size() after split=" << D1.size() << ", D1_bound.size()=" << D1_bound.size());

    // CRITICAL FIX: Update address books for all items in the new blocks
    // This was the missing piece that caused segmentation faults
    DEBUG_PRINT("Updating address books for smaller block (" << smaller.block.size() << " items)");
    for (auto item_it = smaller_it->block.begin(); item_it != smaller_it->block.end(); ++item_it) {
        this->address_book_l1_D1[item_it->first] = smaller_it;
        this->address_book_l2[item_it->first] = item_it;
        DEBUG_PRINT("  Updated address for key=" << item_it->first << ", value=" << item_it->second);
    }
    
    DEBUG_PRINT("Updating address books for larger block (" << larger.block.size() << " items)");
    for (auto item_it = larger_it->block.begin(); item_it != larger_it->block.end(); ++item_it) {
        this->address_book_l1_D1[item_it->first] = larger_it;
        this->address_book_l2[item_it->first] = item_it;
        DEBUG_PRINT("  Updated address for key=" << item_it->first << ", value=" << item_it->second);
    }

    DEBUG_PRINT("Split operation completed successfully");
}

void BatchHeap::batchPrepend(std::list<std::pair<int, double>> items) {
    int L = items.size();
    DEBUG_FUNCTION_ENTRY("BatchHeap::batchPrepend", "prepending " << L << " items");
    DEBUG_PRINT("Current D0.size()=" << D0.size() << ", M=" << this->M << ", B=" << this->B);
    
    // Log details of first few items being prepended
    int count = 0;
    for (auto& item : items) {
        if (count < 5) {
            DEBUG_PRINT("  Item " << count << ": key=" << item.first << ", value=" << item.second);
        }
        count++;
        if (count >= 5) break;
    }
    if (L > 5) {
        DEBUG_PRINT("  ... and " << (L - 5) << " more items");
    }
    
    if (L <= this->M) {
        DEBUG_PRINT("Simple case: L=" << L << " <= M=" << this->M << ", creating single block in D0");
        // Simple case: create a new block and add to beginning of D0
        Block newBlock;
        newBlock.block = items;
        newBlock.upper_bound = this->B; // Set to maximum bound for D0
        DEBUG_PRINT("Creating new block with size=" << newBlock.block.size() << ", upper_bound=" << newBlock.upper_bound);
        
        D0.push_front(newBlock);
        DEBUG_PRINT("Added block to D0, new D0.size()=" << D0.size());
        
        // Update address books for all items in the new block
        auto block_it = D0.begin();
        DEBUG_PRINT("Updating address books for " << block_it->block.size() << " items");
        for (auto item_it = block_it->block.begin(); item_it != block_it->block.end(); ++item_it) {
            this->address_book_l1_D0[item_it->first] = block_it;
            this->address_book_l2[item_it->first] = item_it;
            DEBUG_PRINT("  Updated address for key=" << item_it->first);
        }
        DEBUG_PRINT("Simple case completed successfully");
        return;
    }
    else {
        DEBUG_PRINT("Complex case: L=" << L << " > M=" << this->M << ", need to split into multiple blocks");
        // Complex case: create O(L/M) blocks, each with at most ⌈M/2⌉ elements
        // Convert list to vector for efficient median finding
        std::vector<std::pair<int, double>> tmp(items.begin(), items.end());
        DEBUG_MEMORY("Converted " << items.size() << " items to vector for splitting");
        
        int max_block_size = (this->M + 1) / 2; // ⌈M/2⌉
        DEBUG_PRINT("Max block size calculated as ⌈M/2⌉ = " << max_block_size);
        
        // Recursively split using medians until all chunks are small enough
        std::vector<std::vector<std::pair<int, double>>> blocks_to_create;
        std::vector<std::vector<std::pair<int, double>>> current_level;
        current_level.push_back(tmp);
        DEBUG_PRINT("Starting recursive splitting with initial chunk of size " << tmp.size());
        
        int level = 0;
        while (!current_level.empty()) {
            DEBUG_PRINT("Processing level " << level << " with " << current_level.size() << " chunks");
            std::vector<std::vector<std::pair<int, double>>> next_level;
            
            for (size_t i = 0; i < current_level.size(); ++i) {
                auto& chunk = current_level[i];
                DEBUG_PRINT("  Chunk " << i << " has size " << chunk.size());
                
                if (chunk.size() <= max_block_size) {
                    // This chunk is small enough, add it to final blocks
                    DEBUG_PRINT("    Chunk is small enough, adding to final blocks");
                    blocks_to_create.push_back(chunk);
                } else {
                    DEBUG_PRINT("    Chunk too large, splitting using median");
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
                    
                    DEBUG_PRINT("    Split into left_chunk size=" << left_chunk.size() << ", right_chunk size=" << right_chunk.size());
                    
                    if (!left_chunk.empty()) next_level.push_back(left_chunk);
                    if (!right_chunk.empty()) next_level.push_back(right_chunk);
                }
            }
            
            current_level = next_level;
            level++;
        }
        
        DEBUG_PRINT("Splitting complete, creating " << blocks_to_create.size() << " final blocks");
        
        // Create blocks and add them to the beginning of D0
        for (size_t i = 0; i < blocks_to_create.size(); ++i) {
            auto& block_data = blocks_to_create[i];
            DEBUG_PRINT("Creating block " << i << " with " << block_data.size() << " items");
            
            Block newBlock;
            newBlock.block.assign(block_data.begin(), block_data.end());
            newBlock.upper_bound = this->B; // Set to maximum bound for D0
            D0.push_front(newBlock);
            
            // Update address books for all items in the new block
            auto block_it = D0.begin();
            DEBUG_PRINT("  Updating address books for block " << i);
            for (auto item_it = block_it->block.begin(); item_it != block_it->block.end(); ++item_it) {
                this->address_book_l1_D0[item_it->first] = block_it;
                this->address_book_l2[item_it->first] = item_it;
            }
        }
        
        DEBUG_PRINT("Complex case completed, D0.size() now=" << D0.size());
    }
    DEBUG_PRINT("batchPrepend completed successfully");
}

PullResults BatchHeap::pull() {
    DEBUG_FUNCTION_ENTRY("BatchHeap::pull", "starting pull operation");
    DEBUG_PRINT("Current state: D0.size()=" << D0.size() << ", D1.size()=" << D1.size() << ", M=" << this->M << ", B=" << this->B);
    
    PullResults result;
    
    // Step 1: Collect sufficient prefix of blocks from D0 and D1
    std::vector<std::pair<int, double>> S0_prime, S1_prime;
    DEBUG_PRINT("Step 1: Collecting prefixes from D0 and D1");
    
    // Collect from D0 until we have M elements or exhaust D0
    int collected_D0 = 0;
    DEBUG_PRINT("Collecting from D0 (target: " << this->M << " elements)");
    for (auto& block : this->D0) {
        if (collected_D0 >= this->M) {
            DEBUG_PRINT("  Reached M=" << this->M << " elements from D0, stopping");
            break;
        }
        
        DEBUG_PRINT("  Processing D0 block with " << block.block.size() << " items, upper_bound=" << block.upper_bound);
        for (auto& item : block.block) {
            if (collected_D0 >= this->M) break;
            S0_prime.push_back(item);
            DEBUG_PRINT("    Collected D0 item: key=" << item.first << ", value=" << item.second << " (total: " << (collected_D0 + 1) << ")");
            collected_D0++;
        }
    }
    DEBUG_PRINT("Collected " << collected_D0 << " items from D0");
    
    // Collect from D1 until we have M elements or exhaust D1
    int collected_D1 = 0;
    DEBUG_PRINT("Collecting from D1 (target: " << this->M << " elements)");
    for (auto& block : this->D1) {
        if (collected_D1 >= this->M) {
            DEBUG_PRINT("  Reached M=" << this->M << " elements from D1, stopping");
            break;
        }
        
        DEBUG_PRINT("  Processing D1 block with " << block.block.size() << " items, upper_bound=" << block.upper_bound);
        for (auto& item : block.block) {
            if (collected_D1 >= this->M) break;
            S1_prime.push_back(item);
            DEBUG_PRINT("    Collected D1 item: key=" << item.first << ", value=" << item.second << " (total: " << (collected_D1 + 1) << ")");
            collected_D1++;
        }
    }
    DEBUG_PRINT("Collected " << collected_D1 << " items from D1");
    
    // Step 2: Check if we have collected all elements or need to find smallest M
    int total_collected = S0_prime.size() + S1_prime.size();
    DEBUG_PRINT("Step 2: Total collected=" << total_collected << ", M=" << this->M);
    
    if (total_collected <= this->M) {
        DEBUG_PRINT("Case 1: Total collected (" << total_collected << ") <= M (" << this->M << "), returning all elements");
        // Case 1: S'0 ∪ S'1 contains no more than M elements
        // Return all elements and set x to upper bound B
        result.vertices.reserve(total_collected);
        
        DEBUG_PRINT("Adding " << S0_prime.size() << " vertices from S0_prime");
        for (auto& item : S0_prime) {
            result.vertices.push_back(item.first);
            DEBUG_PRINT("  Added vertex " << item.first << " (value=" << item.second << ")");
        }
        
        DEBUG_PRINT("Adding " << S1_prime.size() << " vertices from S1_prime");
        for (auto& item : S1_prime) {
            result.vertices.push_back(item.first);
            DEBUG_PRINT("  Added vertex " << item.first << " (value=" << item.second << ")");
        }
        
        result.new_bound = this->B;
        DEBUG_PRINT("Set new_bound to B=" << this->B);
        
        // Delete all collected elements from D0 and D1
        DEBUG_PRINT("Deleting " << S0_prime.size() << " elements from D0");
        for (auto& item : S0_prime) {
            DEBUG_PRINT("  Deleting key=" << item.first);
            this->del(item.first);
        }
        
        DEBUG_PRINT("Deleting " << S1_prime.size() << " elements from D1");
        for (auto& item : S1_prime) {
            DEBUG_PRINT("  Deleting key=" << item.first);
            this->del(item.first);
        }
    }
    else {
        DEBUG_PRINT("Case 2: Total collected (" << total_collected << ") > M (" << this->M << "), finding " << this->M << " smallest elements");
        // Case 2: We need to find the smallest M elements from S'0 ∪ S'1
        // Combine S'0 and S'1
        std::vector<std::pair<int, double>> combined;
        combined.reserve(total_collected);
        combined.insert(combined.end(), S0_prime.begin(), S0_prime.end());
        combined.insert(combined.end(), S1_prime.begin(), S1_prime.end());
        DEBUG_PRINT("Combined S0_prime and S1_prime into vector of size " << combined.size());
        
        // Find the M smallest elements using nth_element
        auto kth_it = combined.begin() + this->M;
        DEBUG_PRINT("Using nth_element to find " << this->M << "th smallest element");
        std::nth_element(
            combined.begin(),
            kth_it,
            combined.end(),
            [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                return a.second < b.second;
            }
        );
        DEBUG_PRINT("nth_element completed, " << this->M << "th element has value=" << kth_it->second);
        
        // Extract the M smallest elements
        result.vertices.reserve(this->M);
        DEBUG_PRINT("Extracting " << this->M << " smallest elements:");
        for (auto it = combined.begin(); it != kth_it; ++it) {
            result.vertices.push_back(it->first);
            DEBUG_PRINT("  Selected vertex " << it->first << " (value=" << it->second << ")");
        }
        
        // Set x to the smallest remaining value in D0 ∪ D1 after deletion
        // We need to find this among elements not selected for deletion
        double min_remaining = this->B; // Start with upper bound
        bool found_remaining = false;
        DEBUG_PRINT("Finding minimum remaining value (starting with B=" << this->B << ")");
        
        // Check remaining elements in combined that weren't selected
        DEBUG_PRINT("Checking remaining elements in combined vector:");
        for (auto it = kth_it; it != combined.end(); ++it) {
            min_remaining = std::min(min_remaining, it->second);
            found_remaining = true;
            DEBUG_PRINT("  Remaining element: key=" << it->first << ", value=" << it->second << ", min_remaining=" << min_remaining);
        }
        
        // Check remaining elements in D0 and D1 not in S'0 ∪ S'1
        int checked_D0 = 0;
        DEBUG_PRINT("Checking remaining elements in D0 beyond first " << this->M << ":");
        for (auto& block : this->D0) {
            for (auto& item : block.block) {
                if (checked_D0 >= this->M) {
                    min_remaining = std::min(min_remaining, item.second);
                    found_remaining = true;
                    DEBUG_PRINT("  D0 remaining: key=" << item.first << ", value=" << item.second << ", min_remaining=" << min_remaining);
                }
                checked_D0++;
            }
        }
        
        int checked_D1 = 0;
        DEBUG_PRINT("Checking remaining elements in D1 beyond first " << this->M << ":");
        for (auto& block : this->D1) {
            for (auto& item : block.block) {
                if (checked_D1 >= this->M) {
                    min_remaining = std::min(min_remaining, item.second);
                    found_remaining = true;
                    DEBUG_PRINT("  D1 remaining: key=" << item.first << ", value=" << item.second << ", min_remaining=" << min_remaining);
                }
                checked_D1++;
            }
        }
        
        result.new_bound = found_remaining ? min_remaining : this->B;
        DEBUG_PRINT("Set new_bound=" << result.new_bound << " (found_remaining=" << found_remaining << ")");
        
        // Delete the M smallest elements from D0 and D1
        DEBUG_PRINT("Deleting " << this->M << " smallest elements:");
        for (auto it = combined.begin(); it != kth_it; ++it) {
            DEBUG_PRINT("  Deleting key=" << it->first << " (value=" << it->second << ")");
            this->del(it->first);
        }
    }
    
    DEBUG_PRINT("Pull completed: returning " << result.vertices.size() << " vertices, new_bound=" << result.new_bound);
    return result;
}