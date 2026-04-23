#include <iostream>
#include <omp.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ----------------------------
// Sequential Functions
// ----------------------------
int s_min(int arr[], int n) {
    int minv = arr[0];
    for (int i = 0; i < n; i++)
        if (arr[i] < minv) minv = arr[i];
    return minv;
}

int s_max(int arr[], int n) {
    int maxv = arr[0];
    for (int i = 0; i < n; i++)
        if (arr[i] > maxv) maxv = arr[i];
    return maxv;
}

long long s_sum(int arr[], int n) {
    long long s = 0;
    for (int i = 0; i < n; i++)
        s += arr[i];
    return s;
}

// ----------------------------
// Parallel Functions
// ----------------------------
int p_min(int arr[], int n) {
    int minv = arr[0];
    #pragma omp parallel for reduction(min:minv)
    for (int i = 0; i < n; i++)
        if (arr[i] < minv) minv = arr[i];
    return minv;
}

int p_max(int arr[], int n) {
    int maxv = arr[0];
    #pragma omp parallel for reduction(max:maxv)
    for (int i = 0; i < n; i++)
        if (arr[i] > maxv) maxv = arr[i];
    return maxv;
}

long long p_sum(int arr[], int n) {
    long long s = 0;
    #pragma omp parallel for reduction(+:s)
    for (int i = 0; i < n; i++)
        s += arr[i];
    return s;
}

// ----------------------------
// MAIN
// ----------------------------
int main() {
    int n = 1000000;
    int* arr = new int[n];

    for (int i = 0; i < n; i++) arr[i] = i + 1;

    cout << fixed << setprecision(9);

    // 🔹 Sequential Timing (nanoseconds)
    auto start_seq = high_resolution_clock::now();

    int smin = s_min(arr, n);
    int smax = s_max(arr, n);
    long long ssum = s_sum(arr, n);
    double savg = (double)ssum / n;

    auto end_seq = high_resolution_clock::now();
    auto seq_time = duration_cast<nanoseconds>(end_seq - start_seq);

    // 🔹 Parallel Timing (nanoseconds)
    auto start_par = high_resolution_clock::now();

    int pmin = p_min(arr, n);
    int pmax = p_max(arr, n);
    long long psum = p_sum(arr, n);
    double pavg = (double)psum / n;

    auto end_par = high_resolution_clock::now();
    auto par_time = duration_cast<nanoseconds>(end_par - start_par);

    // 🔹 Output
    cout << "Sequential Time: " << seq_time.count() << " ns\n";
    cout << "Parallel Time:   " << par_time.count() << " ns\n\n";

    cout << "Min: " << pmin << endl;
    cout << "Max: " << pmax << endl;
    cout << "Sum: " << psum << endl;
    cout << "Avg: " << pavg << endl;

    // 🔹 Speedup
    double speedup = (double)seq_time.count() / par_time.count();
    cout << "\nSpeedup: " << speedup << endl;

    delete[] arr;
    return 0;
}