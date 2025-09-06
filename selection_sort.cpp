#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <memory>
#include <queue>

template<typename T>
class AdvancedSelectionSort {
private:
    struct HeapNode {
        T value;
        size_t index;
        bool operator>(const HeapNode& other) const {
            return value > other.value;
        }
    };

public:
    static void doubleSelectionSort(std::vector<T>& arr) {
        size_t n = arr.size();
        for (size_t i = 0; i < n / 2; ++i) {
            size_t minIdx = i, maxIdx = i;
            
            for (size_t j = i; j < n - i; ++j) {
                if (arr[j] < arr[minIdx]) minIdx = j;
                if (arr[j] > arr[maxIdx]) maxIdx = j;
            }
            
            if (minIdx == n - i - 1) minIdx = maxIdx;
            
            std::swap(arr[i], arr[minIdx]);
            if (maxIdx == i) maxIdx = minIdx;
            std::swap(arr[n - i - 1], arr[maxIdx]);
        }
    }
    
    static void heapSelectionSort(std::vector<T>& arr) {
        std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> minHeap;
        
        // Build heap
        for (size_t i = 0; i < arr.size(); ++i) {
            minHeap.push({arr[i], i});
        }
        
        // Extract sorted elements
        for (size_t i = 0; i < arr.size(); ++i) {
            auto node = minHeap.top();
            minHeap.pop();
            arr[i] = node.value;
        }
    }
    
    static void cyclicSelectionSort(std::vector<T>& arr) {
        size_t writes = 0;
        
        for (size_t cycleStart = 0; cycleStart < arr.size() - 1; ++cycleStart) {
            T item = arr[cycleStart];
            size_t pos = cycleStart;
            
            // Find position where item should go
            for (size_t i = cycleStart + 1; i < arr.size(); ++i) {
                if (arr[i] < item) ++pos;
            }
            
            if (pos == cycleStart) continue;
            
            // Skip duplicates
            while (item == arr[pos]) ++pos;
            
            if (pos != cycleStart) {
                std::swap(item, arr[pos]);
                ++writes;
            }
            
            // Rotate rest of cycle
            while (pos != cycleStart) {
                pos = cycleStart;
                for (size_t i = cycleStart + 1; i < arr.size(); ++i) {
                    if (arr[i] < item) ++pos;
                }
                
                while (item == arr[pos]) ++pos;
                
                if (item != arr[pos]) {
                    std::swap(item, arr[pos]);
                    ++writes;
                }
            }
        }
        std::cout << "Writes performed: " << writes << "\n";
    }
    
    static void tournamentSort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        size_t n = arr.size();
        std::vector<T> tournament = arr;
        std::vector<T> result;
        
        while (!tournament.empty()) {
            auto minIt = std::min_element(tournament.begin(), tournament.end());
            result.push_back(*minIt);
            tournament.erase(minIt);
        }
        
        arr = std::move(result);
    }
};

template<typename T>
class SelectionSortBenchmark {
public:
    static void runBenchmarks() {
        const std::vector<size_t> sizes = {100, 500, 1000};
        
        for (size_t size : sizes) {
            std::cout << "\n=== Array Size: " << size << " ===\n";
            
            // Generate test data
            std::vector<T> original(size);
            std::iota(original.begin(), original.end(), 1);
            std::random_shuffle(original.begin(), original.end());
            
            auto testData = original;
            auto start = std::chrono::high_resolution_clock::now();
            AdvancedSelectionSort<T>::doubleSelectionSort(testData);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Double Selection: " << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
            
            testData = original;
            start = std::chrono::high_resolution_clock::now();
            AdvancedSelectionSort<T>::heapSelectionSort(testData);
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Heap Selection: " << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
            
            testData = original;
            start = std::chrono::high_resolution_clock::now();
            AdvancedSelectionSort<T>::cyclicSelectionSort(testData);
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Cycle Selection: " << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
        }
    }
};

int main() {
    std::vector<int> data{64, 25, 12, 22, 11, 90, 88, 76, 50, 42};
    
    std::cout << "Original: ";
    for (int x : data) std::cout << x << " ";
    
    auto copy = data;
    AdvancedSelectionSort<int>::tournamentSort(copy);
    std::cout << "\nTournament sorted: ";
    for (int x : copy) std::cout << x << " ";
    std::cout << "\n";
    
    SelectionSortBenchmark<int>::runBenchmarks();
    
    return 0;
}
