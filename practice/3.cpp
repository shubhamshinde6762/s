#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

long long sum(vector<int> &arr)
{
    long long sm = 0l;

    for (int i = 0; i < arr.size(); i++)
        sm += arr[i];

    return sm;
}

long long sump(vector<int> &arr)
{
    long long sm = 0l;

    #pragma omp parallel for reduction(+:sm)
    for (int i = 0; i < arr.size(); i++)
        sm += arr[i];

    return sm;
}

int min(vector<int> &arr)
{
    int mini = INT_MAX;

    for (int i = 0; i < arr.size(); i++)
        mini = min(mini, arr[i]);

    return mini;
}

int minp(vector<int> &arr)
{
    int mini = INT_MAX;

    #pragma omp parallel for reduction(min:mini)
    for (int i = 0; i < arr.size(); i++)
        mini = min(mini, arr[i]);

    return mini;
}

int max(vector<int> &arr)
{
    int maxi = INT_MIN;

    for (int i = 0; i < arr.size(); i++)
        maxi = max(maxi, arr[i]);

    return maxi;
}

int maxp(vector<int> &arr)
{
    int maxi = INT_MIN;

    #pragma omp parallel for reduction(max:maxi)
    for (int i = 0; i < arr.size(); i++)
        maxi = max(maxi, arr[i]);

    return maxi;
}

int avg(vector<int> &arr)
{
    long long sm = sum(arr);
    return sm / arr.size();
}

int avgp(vector<int> &arr)
{
    long long sm = sump(arr);
    return sm / arr.size();
}

int main() 
{
    int n = 1 << 24;
    vector<int> arr(n);

    for (auto &it : arr)
        it = rand() % 1000;

    long long sm = 0l;
    double t1, t2;
    t1 = omp_get_wtime();
    sm = sum(arr);
    t2 = omp_get_wtime();
    cout << "sum seq " << t2 - t1 << " " << sm << "\n";

    t1 = omp_get_wtime();
    sm = sump(arr);
    t2 = omp_get_wtime();
    cout << "sump " << t2 - t1 << " " << sm << "\n";

    t1 = omp_get_wtime();
    int mini = min(arr);
    t2 = omp_get_wtime();
    cout << "min seq " << t2 - t1 << " " << mini << "\n";

    t1 = omp_get_wtime();
    mini = minp(arr);
    t2 = omp_get_wtime();
    cout << "min par " << t2 - t1 << " " << mini << "\n";

    t1 = omp_get_wtime();
    int maxi = max(arr);
    t2 = omp_get_wtime();
    cout << "max seq " << t2 - t1 << " " << maxi << "\n";

    t1 = omp_get_wtime();
    maxi = maxp(arr);
    t2 = omp_get_wtime();
    cout << "max par " << t2 - t1 << " " << maxi << "\n";

    t1 = omp_get_wtime();
    int av = avg(arr);
    t2 = omp_get_wtime();
    cout << "avg seq " << t2 - t1 << " " << av << "\n";

    t1 = omp_get_wtime();
    av = avgp(arr);
    t2 = omp_get_wtime();
    cout << "avg par " << t2 - t1 << " " << av << "\n";
}
