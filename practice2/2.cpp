#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Sort
{
public:
    static void bubbleSort(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - 1 - i; j++)
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

    static void merge(vector<int> &arr, int left, int mid, int right)
    {
        vector<int> temp(right - left + 1);
        int ptr = 0, i = left, j = mid + 1;

        while (i <= mid && j <= right)
        {
            if (arr[i] < arr[j])
                temp[ptr++] = arr[i++];
            else
                temp[ptr++] = arr[j++];
        }

        while (i <= mid)
            temp[ptr++] = arr[i++];

        while (j <= right)
            temp[ptr++] = arr[j++];

        for (int i = left; i <= right; i++)
        {
            arr[i] = temp[i - left];
        }
    }

    static void mergeSort(vector<int> &arr, int left, int right)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    static void mergeSortParallel(vector<int> &arr, int left, int right)
    {
        if (left >= right)
            return;

        if (right - left < 1000)
            mergeSort(arr, left, right);
        else
        {
            int mid = left + (right - left) / 2;
#pragma omp task shared(arr)
            mergeSortParallel(arr, left, mid);
#pragma omp task shared(arr)
            mergeSortParallel(arr, mid + 1, right);
#pragma omp taskwait
            merge(arr, left, mid, right);
        }
    }

    static void mergeSortHelper(vector<int> &arr, int n)
    {
#pragma omp parallel
        {
#pragma omp single
            {
                mergeSortParallel(arr, 0, n - 1);
            }
        }
    }

    static void bubbleSortHelper(vector<int> &arr, int n)
    {
        bubbleSortParallel(arr);
    }
};

int main()
{
    int n = 1e5;

    vector<int> arr(n);

    for (int i = 0; i < n; i++)
    {
        arr[i] = 100000 - i;
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double t1, t2;
    t1 = omp_get_wtime();
    Sort::bubbleSort(arr1);
    t2 = omp_get_wtime();
    cout << "bubble s " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    Sort::bubbleSortHelper(arr2, n);
    t2 = omp_get_wtime();
    cout << "bubble p " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    Sort::mergeSort(arr3, 0, n - 1);
    t2 = omp_get_wtime();
    cout << "merge s " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    Sort::mergeSortHelper(arr4, n);
    t2 = omp_get_wtime();
    cout << "merge p " << t2 - t1 << endl;
}