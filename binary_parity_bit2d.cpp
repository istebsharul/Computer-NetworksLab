#include <bits/stdc++.h>
using namespace std;

const int ROWS = 3;
const int COLS = 7;

int main() {
    int binary_data[ROWS][COLS];
    cout << "Enter binary data (3x7 matrix):" << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cin >> binary_data[i][j];
        }
    }

    int parity_bits[ROWS];
    for (int i = 0; i < ROWS; i++) {
        int num_ones = 0;
        for (int j = 0; j < COLS; j++) {
            if (binary_data[i][j] == 1) {
                num_ones++;
            }
        }
        parity_bits[i] = (num_ones % 2 == 0) ? 0 : 1;
    }

    int column_parity_bits[COLS];
    for (int j = 0; j < COLS; j++) {
        int num_ones = 0;
        for (int i = 0; i < ROWS; i++) {
            if (binary_data[i][j] == 1) {
                num_ones++;
            }
        }
        column_parity_bits[j] = (num_ones % 2 == 0) ? 0 : 1;
    }


    for (int i = 0; i < ROWS; i++) {
        binary_data[i][COLS - 1] = parity_bits[i];
    }

    for (int j = 0; j < COLS; j++) {
        binary_data[ROWS][j] = column_parity_bits[j];
    }

    cout << "Binary data with parity bits:" << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS; j++) {
            cout << binary_data[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
