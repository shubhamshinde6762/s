#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Sort
{
    static void merge(vector<int> &arr, int left, int mid, int right)
    {
        vector<int> temp(right - left + 1);

        int ptr = 0, i = left, j = mid + 1;

        while(i <= mid && j <= right)
        {
            if (arr[i] < arr[j])
                temp[ptr++] = arr[i++];
            else    
                temp[ptr++] = arr[j++];
        }

        while(i <= mid)
            temp[ptr++] = arr[i++];

        while(j <= right)
            temp[ptr++] = arr[j++];

        for (int i = left; i <= right; i++)
            arr[i] = temp[i - left];
    }

    static void mergeSortSequentialHelper(vector<int> &arr, int left, int right)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;

        mergeSortSequentialHelper(arr, left, mid);
        mergeSortSequentialHelper(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }

    static void mergeSortParallelHelper(vector<int> &arr, int left, int right)
    {
        if (left >= right)
            return;

        if (right - left <= 1000)
            return void(), mergeSortSequentialHelper(arr, left, right);
            
        int mid = left + (right - left) / 2;
        
        #pragma omp task shared(arr)
        mergeSortParallelHelper(arr, left, mid);

        #pragma omp task shared(arr)
        mergeSortParallelHelper(arr, mid + 1, right);

        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
public:
    static void bubbleSortSequential(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        }
    }

    static void bubbleSortParallel(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n; i++)
        {
            #pragma omp parallel for
            for (int j = i % 2; j < n - 1; j += 2)
            {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        }
    }

    static void mergeSortSequential(vector<int> &arr)
    {
        int n = arr.size();
        mergeSortSequentialHelper(arr, 0, n - 1);
    }

    static void mergeSortParallel(vector<int> &arr)
    {
        int n = arr.size();
        #pragma omp parallel
        {
            #pragma omp single
            {
                mergeSortParallelHelper(arr, 0, n - 1);
            }
        }  
    }
};

int main() 
{
    int n = 1e5;

    vector<int> arr(n);

    for (auto &it : arr)
        it = rand() % 1000000;

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double t1, t2;

    t1 = omp_get_wtime();
    Sort::bubbleSortSequential(arr1);
    t2 = omp_get_wtime();
    cout << "bubble sort seq " << t2 - t1 << "\n";

    t1 = omp_get_wtime();
    Sort::bubbleSortParallel(arr2);
    t2 = omp_get_wtime();
    cout << "bubble sort parallel " << t2 - t1 << "\n";

    t1 = omp_get_wtime();
    Sort::mergeSortSequential(arr3);
    t2 = omp_get_wtime();
    cout << "merge sort seq " << t2 - t1 << "\n";

    t1 = omp_get_wtime();
    Sort::mergeSortParallel(arr4);
    t2 = omp_get_wtime();
    cout << "merge sort parallel " << t2 - t1 << "\n";
}