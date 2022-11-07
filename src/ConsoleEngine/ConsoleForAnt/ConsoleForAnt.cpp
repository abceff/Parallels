#include "ConsoleForAnt.h"

namespace s21 {
    ConsoleForAnt::ConsoleForAnt() : AbstractConsoleEngine() {}

    void ConsoleForAnt::RequestParamsFromUser() {
        int N;
        cout << "Enter the count of iterations:\n";
        cin >> N;
        int rows;
        cout << "Enter the edge of square matrix:\n";
        cin >> rows;
        S21Matrix matrix(rows, rows);
        cout << "Fill the matrix:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                cin >> matrix(i, j);
            }
        }
        ant_solver_.SetData(matrix, N);
    }

    void ConsoleForAnt::RunAlgorithm() {
        auto start = std::chrono::high_resolution_clock::now();
        ant_solver_.SolveWithoutUsingParallelism();
        auto duration = std::chrono::high_resolution_clock::now() - start;
        cout << "\nDefault algorithm takes " << duration.count() << " microseconds\n\n";

       start = std::chrono::high_resolution_clock::now();
       ant_solver_.SolveUsingParallelism();
       duration = std::chrono::high_resolution_clock::now() - start;
       cout << "Algorithm with parallelism takes " << duration.count() << " microseconds\n";
    }

    void ConsoleForAnt::PrintResult() {
        ;
    }
}
