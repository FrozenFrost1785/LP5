#include <iostream>
#include <omp.h>
#include <iomanip>

using namespace std;

// Sequential Odd-Even Bubble Sort
void s_bubble(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// Parallel Odd-Even Bubble Sort
void p_bubble(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;

        #pragma omp parallel for schedule(static)
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

int main() {
    int n = 10000;
    int *a = new int[n];
    int *b = new int[n];

    // High precision output
    cout << fixed << setprecision(9);

    // Initialize array
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
        b[i] = n - i;
    }

    double start, end;

    // Sequential
    start = omp_get_wtime();
    s_bubble(a, n);
    end = omp_get_wtime();
    cout << "Sequential Time: " << (end - start) << " sec\n";

    // Parallel
    start = omp_get_wtime();
    p_bubble(b, n);
    end = omp_get_wtime();
    cout << "Parallel Time:   " << (end - start) << " sec\n";

    // Print first few elements
    cout << "Sorted (first 10): ";
    for (int i = 0; i < 10; i++) cout << b[i] << " ";
    cout << endl;

    delete[] a;
    delete[] b;

    return 0;
}