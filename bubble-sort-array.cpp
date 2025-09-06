#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

template<typename T>
class OptimizedBubbleSort {
private:
    std::vector<T> data;
    size_t comparisons = 0, swaps = 0;

public:
    OptimizedBubbleSort(std::initializer_list<T> init) : data(init) {}
    
    void cocktailSort() {
        bool swapped = true;
        size_t start = 0, end = data.size() - 1;
        
        while (swapped) {
            swapped = false;
            for (size_t i = start; i < end; ++i) {
                comparisons++;
                if (data[i] > data[i + 1]) {
                    std::swap(data[i], data[i + 1]);
                    swapped = true; swaps++;
                }
            }
            if (!swapped) break;
            --end;
            
            swapped = false;
            for (size_t i = end; i > start; --i) {
                comparisons++;
                if (data[i] < data[i - 1]) {
                    std::swap(data[i], data[i - 1]);
                    swapped = true; swaps++;
                }
            }
            ++start;
        }
    }
    
    void display() const {
        for (const auto& elem : data) std::cout << elem << " ";
        std::cout << "\nStats: " << comparisons << " comparisons, " << swaps << " swaps\n";
    }
};

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    OptimizedBubbleSort<int> sorter{64, 34, 25, 12, 22, 11, 90, 5, 77, 30};
    sorter.cocktailSort();
    auto end = std::chrono::high_resolution_clock::now();
    
    sorter.display();
    std::cout << "Time: " << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
    return 0;
}
