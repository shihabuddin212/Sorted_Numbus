#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

template<typename T, typename Compare = std::less<T>>
class BinaryInsertionSort {
private:
    std::vector<T> arr;
    Compare comp;
    mutable size_t operations = 0;

    size_t binarySearch(const T& key, size_t low, size_t high) const {
        while (low < high) {
            operations++;
            size_t mid = low + (high - low) / 2;
            if (comp(key, arr[mid])) high = mid;
            else low = mid + 1;
        }
        return low;
    }

public:
    BinaryInsertionSort(std::initializer_list<T> init, Compare c = Compare{}) 
        : arr(init), comp(c) {}
    
    void sort() {
        for (size_t i = 1; i < arr.size(); ++i) {
            T key = std::move(arr[i]);
            size_t pos = binarySearch(key, 0, i);
            
            // Shift elements
            for (size_t j = i; j > pos; --j) {
                arr[j] = std::move(arr[j - 1]);
                operations++;
            }
            arr[pos] = std::move(key);
        }
    }
    
    void shellSort() {
        // Knuth sequence: 1, 4, 13, 40, 121...
        std::vector<size_t> gaps;
        for (size_t gap = 1; gap < arr.size(); gap = gap * 3 + 1) 
            gaps.push_back(gap);
        
        for (auto it = gaps.rbegin(); it != gaps.rend(); ++it) {
            size_t gap = *it;
            for (size_t i = gap; i < arr.size(); ++i) {
                T temp = arr[i];
                size_t j = i;
                while (j >= gap && comp(temp, arr[j - gap])) {
                    arr[j] = arr[j - gap];
                    j -= gap; operations++;
                }
                arr[j] = temp;
            }
        }
    }
    
    void display() const {
        for (const auto& elem : arr) std::cout << elem << " ";
        std::cout << "\nOperations: " << operations << "\n";
    }
};

int main() {
    BinaryInsertionSort<double> sorter{3.14, 2.71, 1.41, 9.81, 6.28, 0.57, 4.67, 8.85};
    
    auto start = std::chrono::steady_clock::now();
    sorter.sort();
    auto mid = std::chrono::steady_clock::now();
    
    std::cout << "Binary Insertion Sort: ";
    sorter.display();
    
    BinaryInsertionSort<double> sorter2{3.14, 2.71, 1.41, 9.81, 6.28, 0.57, 4.67, 8.85};
    sorter2.shellSort();
    auto end = std::chrono::steady_clock::now();
    
    std::cout << "Shell Sort: ";
    sorter2.display();
    
    std::cout << "Times: " << std::chrono::duration<double, std::micro>(mid - start).count() 
              << "μs, " << std::chrono::duration<double, std::micro>(end - mid).count() << "μs\n";
    return 0;
}
