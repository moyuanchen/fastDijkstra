#ifndef BATCHHEAP_H
#define BATCHHEAP_H
#include <list>
#include <unordered_map>
#include <map>

// a batch of nodes for the custom datastructure
// a simple linked list of key value pairs
struct Block {
    int upper_bound;
    std::list<std::pair<int, double>> block;
};

// a struct to hold the result of a pull call
struct PullResults {
    std::vector<int> vertices; // vertecies in the structure
    double new_bound; //upper bound
};

class BatchHeap {
    private:
    int M; int B;
    std::list<Block> D0;
    std::list<Block> D1;
    // address books for O(1) deletion
    std::unordered_map<int, std::list<Block>::iterator> address_book_l1_D0;
    std::unordered_map<int, std::list<Block>::iterator> address_book_l1_D1;
    std::unordered_map<int, std::list<std::pair<int, double>>::iterator> address_book_l2;

    std::map<double, std::list<Block>::iterator> D1_bound;

    void del(int key);
    void split(std::list<Block>::iterator);

    public:
    BatchHeap(int M, int B);
    void insert(int key, double value);
    void batchPrepend(std::list<std::pair<int, double>> items);
    PullResults pull();


};

#endif