#include <iostream>
#include <vector>

using namespace std;

// Generate Hamming code matrix
vector<vector<int>> generateHammingMatrix(int m, int r) {
    int n = (1 << r) - 1;
    vector<vector<int>> matrix(m, vector<int>(n));
    for (int i = 0; i < m; i++) {
        int row = i + 1;
        int col = 0;
        for (int j = 0; j < r; j++) {
            int bit = (row >> j) & 1;
            for (int k = 0; k < n; k++) {
                if ((k + 1) & (1 << j)) {
                    matrix[i][k] = bit;
                    col++;
                }
            }
        }
        for (int j = r; j < col; j++) {
            int x = 0;
            for (int k = 0; k < r; k++) {
                if ((j + 1) & (1 << k)) {
                    x ^= matrix[i][n - 1 - k];
                }
            }
            matrix[i][j] = x;
        }
    }
    return matrix;
}

// Print matrix
void printMatrix(vector<vector<int>> matrix) {
    for (auto row : matrix) {
        for (auto col : row) {
            cout << col << " ";
        }
        cout << endl;
    }
}

int main() {
    int m = 4; // number of parity bits
    int r = 3; // number of data bits
    vector<vector<int>> matrix = generateHammingMatrix(m, r);
    printMatrix(matrix);
    return 0;
}
