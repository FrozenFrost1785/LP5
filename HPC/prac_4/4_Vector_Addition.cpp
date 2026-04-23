#include <iostream>
#include <cuda_runtime.h>

using namespace std;

// ----------------------------
// CUDA Kernel
// ----------------------------
__global__
void add(int* A, int* B, int* C, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < size) {
        C[tid] = A[tid] + B[tid];
    }
}

// ----------------------------
// Helper Functions
// ----------------------------
void initialize(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10;
    }
}

void print(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        cout << vector[i] << " ";
    }
    cout << endl;
}

// ----------------------------
// MAIN
// ----------------------------
int main() {
    int N = 8;   // increase size a bit
    size_t bytes = N * sizeof(int);

    int *A, *B, *C;
    A = new int[N];
    B = new int[N];
    C = new int[N];

    initialize(A, N);
    initialize(B, N);

    cout << "Vector A: ";
    print(A, N);

    cout << "Vector B: ";
    print(B, N);

    // Device pointers
    int *d_A, *d_B, *d_C;

    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    cudaMemcpy(d_A, A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, bytes, cudaMemcpyHostToDevice);

    // Kernel launch
    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    add<<<blocks, threads>>>(d_A, d_B, d_C, N);

    // 🔥 Wait for GPU to finish
    cudaDeviceSynchronize();

    // Copy result back
    cudaMemcpy(C, d_C, bytes, cudaMemcpyDeviceToHost);

    cout << "Result (A + B): ";
    print(C, N);

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}