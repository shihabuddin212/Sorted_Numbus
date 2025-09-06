#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <algorithm>
#include <chrono>

template<typename T>
class AdvancedQuickSort {
private:
    static constexpr size_t INSERTION_THRESHOLD = 10;
    std::mt19937 rng{std::random_device{}()};

    void insertionSort(std::vector<T>& arr, size_t low, size_t high) {
        for (size_t i = low + 1; i <= high; ++i) {
            T key = arr[i];
            size_t j = i;
            while (j > low && arr[j - 1] > key) {
                arr[j] = arr[j - 1];
                --j;
            }
            arr[j] = key;
        }
    }

    size_t medianOfThree(std::vector<T>& arr, size_t low, size_t high) {
        size_t mid = low + (high - low) / 2;
        if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
        if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);
        if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
        return mid;
    }

    std::pair<size_t, size_t> threeWayPartition(std::vector<T>& arr, size_t low, size_t high) {
        size_t pivotIdx = medianOfThree(arr, low, high);
        std::swap(arr[pivotIdx], arr[high]);
        T pivot = arr[high];
        
        size_t lt = low, gt = high, i = low;
        
        while (i < gt) {
            if (arr[i] < pivot) std::swap(arr[lt++], arr[i++]);
            else if (arr[i] > pivot) std::swap(arr[i], arr[--gt]);
            else ++i;
        }
        std::swap(arr[gt], arr[high]);
        return {lt, gt};
    }

public:
    void dualPivotQuickSort(std::vector<T>& arr, size_t low, size_t high) {
        if (high - low < INSERTION_THRESHOLD) {
            insertionSort(arr, low, high);
            return;
        }
        
        if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
        T pivot1 = arr[low], pivot2 = arr[high];
        
        size_t lt = low + 1, gt = high - 1, k = low + 1;
        
        while (k <= gt) {
            if (arr[k] < pivot1) std::swap(arr[k++], arr[lt++]);
            else if (arr[k] > pivot2) std::swap(arr[k], arr[gt--]);
            else ++k;
        }
        
        std::swap(arr[--lt], arr[low]);
        std::swap(arr[++gt], arr[high]);
        
        if (lt > low + 1) dualPivotQuickSort(arr, low, lt - 1);
        if (gt < high - 1) dualPivotQuickSort(arr, gt + 1, high);
        if (lt + 1 < gt) dualPivotQuickSort(arr, lt + 1, gt - 1);
    }

    void iterativeQuickSort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        
        std::stack<std::pair<size_t, size_t>> stack;
        stack.push({0, arr.size() - 1});
        
        while (!stack.empty()) {
            auto [low, high] = stack.top();
            stack.pop();
            
            if (high - low < INSERTION_THRESHOLD) {
                insertionSort(arr, low, high);
                continue;
            }
            
            auto [lt, gt] = threeWayPartition(arr, low, high);
            
            if (lt > low) stack.push({low, lt - 1});
            if (gt < high) stack.push({gt + 1, high});
        }
    }
    
    void hybridSort(std::vector<T>& arr) {
        std::shuffle(arr.begin(), arr.end(), rng);
        if (!arr.empty()) dualPivotQuickSort(arr, 0, arr.size() - 1);
    }
};

int main() {
    std::vector<int> data1(5000), data2;
    std::iota(data1.begin(), data1.end(), 1);
    std::reverse(data1.begin(), data1.end());
    data2 = data1;
    
    AdvancedQuickSort<int> sorter;
    
    auto start = std::chrono::high_resolution_clock::now();
    sorter.hybridSort(data1);
    auto mid = std::chrono::high_resolution_clock::now();
    
    sorter.iterativeQuickSort(data2);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::cout << "Hybrid: " << std::chrono::duration<double, std::milli>(mid - start).count() << "ms\n";
    std::cout << "Iterative: " << std::chrono::duration<double, std::milli>(end - mid).count() << "ms\n";
    std::cout << "Sorted: " << std::is_sorted(data1.begin(), data1.end()) << "\n";
    
    return 0;
}
