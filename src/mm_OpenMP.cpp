#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {
    // --- Parse command-line argument N ---
    // Default matrix size = 512, override if user provides argv[1]
    int N = 512;
    if (argc >= 2) {
        try {
            N = stoi(argv[1]);
            if (N <= 0) { cerr << "N must be positive\n"; return 1; }
            if (N > 8192) { cerr << "N too large for this demo\n"; return 1; }
        } catch (...) { cerr << "Invalid N\n"; return 1; }
    }

    // --- Allocate matrices A, B, C ---
    // Using vector-of-vectors for simplicity
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<long long>> C(N, vector<long long>(N, 0));

    // --- Fill A and B with random integers in [0, 99] ---
    // Fixed seed for reproducibility when comparing runs
    srand(42);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }

    // --- Discover number of threads that will be used ---
    int threads_used = 0;
    #pragma omp parallel
    {
        #pragma omp master
        threads_used = omp_get_num_threads();
    }

    // --- Parallel matrix multiplication with OpenMP ---
    // Parallelize the outermost loop (rows of C) since each row is independent.
    // schedule(static): evenly distribute iterations among threads.
    auto start = high_resolution_clock::now();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            long long sum = 0;  // accumulator, private to each (i, j)
            for (int k = 0; k < N; ++k) {
                sum += (long long)A[i][k] * B[k][j];
            }
            C[i][j] = sum;      // safe: each thread writes a unique (i, j)
        }
    }

    auto stop = high_resolution_clock::now();
    long long ms = duration_cast<milliseconds>(stop - start).count();

    // --- Report results ---
    cout << "Matrix size: " << N << "x" << N << "\n";
    cout << "Threads used: " << threads_used << "\n";
    cout << "OpenMP runtime: " << ms << " ms\n";
    cout << "Check: C[0][0]=" << C[0][0]
         << ", C[N-1][N-1]=" << C[N-1][N-1] << "\n";

    return 0;
}