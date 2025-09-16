#include <iostream>
#include <vector>
#include <chrono>   // for timing
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {
    // Parse matrix size from CLI: ./mm_serial 512
    int N = 512;
    if (argc >= 2) {
        try {
            N = stoi(argv[1]);
            if (N <= 0) {
                cerr << "N must be positive.\n";
                return 1;
            }
        } catch (...) {
            cerr << "Invalid N.\n";
            return 1;
        }
    }

    // Allocate matrices: A, B as int; C as long long for accumulation
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<long long>> C(N, vector<long long>(N, 0));

    // Fill A and B with random integers in [0, 99]
    srand((unsigned)time(nullptr));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // Start timer
    auto start = high_resolution_clock::now();

    // Triple-loop matrix multiplication: C = A * B
    for (int i = 0; i < N; ++i) {           // row of A/C
        for (int j = 0; j < N; ++j) {       // col of B/C
            long long sum = 0;
            for (int k = 0; k < N; ++k) {   // dot product
                sum += (long long)A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // Stop timer
    auto stop = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(stop - start).count();

    // Minimal summary
    cout << "Matrix size: " << N << "x" << N << "\n";
    cout << "Serial runtime: " << ms << " ms\n";
    cout << "Check: C[0][0]=" << C[0][0]
         << ", C[N-1][N-1]=" << C[N-1][N-1] << "\n";

    return 0;
}