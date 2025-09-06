#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <memory>

template<typename T>
class ParallelMergeSort {
private:
    static constexpr size_t THRESHOLD = 1000;
    std::vector<T> buffer;

    void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right) {
        size_t i = left, j = mid + 1, k = left;
        
        while (i <= mid && j <= right) {
            buffer[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
        }
        while (i <= mid) buffer[k++] = arr[i++];
        while (j <= right) buffer[k++] = arr[j++];
        
        std::move(buffer.begin() + left, buffer.begin() + right + 1, arr.begin() + left);
    }

    void mergeSortSerial(std::vector<T>& arr, size_t left, size_t right) {
        if (left >= right) return;
        size_t mid = left + (right - left) / 2;
        mergeSortSerial(arr, left, mid);
        mergeSortSerial(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    void mergeSortParallel(std::vector<T>& arr, size_t left, size_t right, int depth = 0) {
        if (left >= right) return;
        
        size_t mid = left + (right - left) / 2;
        
        if (depth < 3 && (right - left) > THRESHOLD) {
            auto future = std::async(std::launch::async, 
                [this, &arr, left, mid, depth]() {
                    mergeSortParallel(arr, left, mid, depth + 1);
                });
            mergeSortParallel(arr, mid + 1, right, depth + 1);
            future.wait();
        } else {
            mergeSortSerial(arr, left, mid);
            mergeSortSerial(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }

public:
    void sort(std::vector<T>& arr, bool parallel = true) {
        if (arr.empty()) return;
        buffer.resize(arr.size());
        
        if (parallel) {
            mergeSortParallel(arr, 0, arr.size() - 1);
        } else {
            mergeSortSerial(arr, 0, arr.size() - 1);
        }
    }

    static void naturalMergeSort(std::vector<T>& arr) {
        std::vector<T> aux(arr.size());
        size_t n = arr.size();
        
        for (size_t width = 1; width < n; width *= 2) {
            for (size_t i = 0; i < n; i += 2 * width) {
                size_t mid = std::min(i + width, n);
                size_t right = std::min(i + 2 * width, n);
                
                if (mid < right) {
                    std::merge(arr.begin() + i, arr.begin() + mid,
                              arr.begin() + mid, arr.begin() + right,
                              aux.begin() + i);
                }
            }
            std::swap(arr, aux);
        }
    }
};

int main() {
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 0);
    std::random_shuffle(data.begin(), data.end());
    
    auto data_copy = data;
    ParallelMergeSort<int> sorter;
    
    auto start = std::chrono::high_resolution_clock::now();
    sorter.sort(data, true);
    auto mid = std::chrono::high_resolution_clock::now();
    
    ParallelMergeSort<int>::naturalMergeSort(data_copy);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::cout << "Parallel: " << std::chrono::duration<double, std::milli>(mid - start).count() << "ms\n";
    std::cout << "Natural: " << std::chrono::duration<double, std::milli>(end - mid).count() << "ms\n";
    std::cout << "Sorted: " << std::is_sorted(data.begin(), data.end()) << "\n";
    
    return 0;
}
