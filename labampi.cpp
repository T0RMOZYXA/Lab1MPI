#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>
#include <string>

using namespace std;
using namespace chrono;

void saveMatrix(const vector<double>& mat, int n, const string& fname) {
    ofstream out(fname);
    out << fixed << setprecision(12);
    out << n << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << mat[i * n + j];
            if (j != n - 1) out << ' ';
        }
        out << '\n';
    }
}

int main() {
    vector<int> sizes = { 200, 400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600 };

    ofstream timeLog("timings_seq.txt");
    timeLog << "N\ttime_sec\n";

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 10.0);

    for (int N : sizes) {
        cout << "Processing N = " << N << " ..." << endl;

        vector<double> A(N * N), B(N * N), C(N * N, 0.0);

        for (int i = 0; i < N * N; ++i) {
            A[i] = dist(gen);
            B[i] = dist(gen);
        }

        saveMatrix(A, N, "A_" + to_string(N) + ".txt");
        saveMatrix(B, N, "B_" + to_string(N) + ".txt");

        auto start = high_resolution_clock::now();

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                double sum = 0.0;
                for (int k = 0; k < N; ++k) {
                    sum += A[i * N + k] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }
        }

        auto end = high_resolution_clock::now();
        double elapsed = duration<double>(end - start).count();

        saveMatrix(C, N, "C_" + to_string(N) + ".txt");

        timeLog << N << '\t' << elapsed << '\n';
        cout << "   time = " << elapsed << " s\n";
    }

    timeLog.close();
    cout << "All experiments completed. Results in timings_seq.txt\n";
    return 0;
}