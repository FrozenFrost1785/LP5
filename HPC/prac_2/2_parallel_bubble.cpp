#include <iostream>
#include <omp.h>
#include <iomanip>

using namespace std;

// ----------------------------
// Sequential Bubble Sort
// ----------------------------
void bubble(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ----------------------------
// Improved Parallel Odd-Even Sort
// ----------------------------
void pBubble(int arr[], int n) {

    #pragma omp parallel
    {
        for (int i = 0; i < n; i++) {

            // Even phase
            #pragma omp for schedule(static)
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }

            // Odd phase
            #pragma omp for schedule(static)
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ----------------------------
// Print Array (limited output)
// ----------------------------
void printArray(int arr[], int n) {
    for (int i = 0; i < min(n, 20); i++) // limit output
        cout << arr[i] << " ";
    cout << "... \n";
}

int main() {

    int n = 5000;  // try 5000 / 10000
    int* arr = new int[n];

    double start, end;

    // Enable high precision printing
    cout << fixed << setprecision(9);

    // Initialize array (reverse sorted)
    for (int i = 0; i < n; i++)
        arr[i] = n - i;

    // ----------------------------
    // Sequential Timing
    // ----------------------------
    start = omp_get_wtime();
    bubble(arr, n);
    end = omp_get_wtime();

    cout << "Sequential Time: " << (end - start) << " seconds\n";
    printArray(arr, n);

    // Reset array
    for (int i = 0; i < n; i++)
        arr[i] = n - i;

    // ----------------------------
    // Parallel Timing
    // ----------------------------
    start = omp_get_wtime();
    pBubble(arr, n);
    end = omp_get_wtime();

    cout << "Parallel Time:   " << (end - start) << " seconds\n";
    printArray(arr, n);
    delete[] arr;
    return 0;
}