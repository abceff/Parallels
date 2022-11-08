
#include "ConsoleForWinograd.h"

namespace s21 {

ConsoleForWinograd::ConsoleForWinograd() : AbstractConsoleEngine() {
    start_message_ = "Hello. This program allows to test performance in "
                     "Winograd matrix multiplication method and "
                     "compare multithreading and single thread realisation.\n";
}

void ConsoleForWinograd::RequestParamsFromUser() {
    if (M1_) {
        delete M1_;
        M1_ = nullptr;
    }
    if (M2_) {
        delete M2_;
        M2_ = nullptr;
    }
    nmb_of_threads_ = 0;
    nmb_of_repeats_ = 0;
    rows_ = 0;
    cols_ = 0;

    cout << "Enter path to file with matrix, "
            "or you can enter integer N M and matrix will be filled randomly: ";
    while (!GetMatrixInput(&M1_)) {
        rows_ = 0;
        cols_ = 0;
    }
    cout << "First matrix has been load successfully, specify second matrix: ";
    while (!GetMatrixInput(&M2_)) {
        rows_ = 0;
        cols_ = 0;
    }
    cout << "Second matrix has been load successfully." << endl;
    if (M1_->get_cols() != M2_->get_rows()) {
        cout << "Columns of the first matrix should be equal rows of the second matrix" << endl;
        RequestParamsFromUser();
    } else {
        while ((nmb_of_repeats_ = RequestNmbFromUser("Enter number of repeats: ")) <= 0) {
            cout << "Invalid number of repeats, try again pls" << endl;
        }

        nmb_of_threads_ = RequestNmbFromUser("Enter number of threads for classic parallelism: ");
        while (nmb_of_threads_ <= 0 || nmb_of_threads_ % 2 != 0 || nmb_of_threads_ > 24) {
            cout << "Invalid number of thread, try again pls" << endl;
            nmb_of_threads_ = RequestNmbFromUser("Enter number of threads for classic parallelism: ");
        }
    }
}


void ConsoleForWinograd::RunAlgorithm() {
    cout << "Running algorithms..." << std::flush;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nmb_of_repeats_; i++)
        winograd_algorithm_.SolveWithoutParallelism(M1_, M2_);
    duration_without_parallelism_ = std::chrono::high_resolution_clock::now() - start;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nmb_of_repeats_; i++)
        winograd_algorithm_.SolveWithPipelineParallelism(M1_, M2_);
    duration_with_pipeline_parallelism_ = std::chrono::high_resolution_clock::now() - start;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nmb_of_repeats_; i++)
        winograd_algorithm_.SolveWithClassicParallelism(M1_, M2_, nmb_of_threads_);
    duration_with_classic_parallelism_ = std::chrono::high_resolution_clock::now() - start;
    cout << "Done" << endl;
}

void ConsoleForWinograd::PrintResult() {
    cout << "Results:" << endl;
    cout << "Duration without parallelism: " << duration_without_parallelism_.count() << "s" << endl;
    cout << "Duration with pipeline parallelism: " << duration_with_pipeline_parallelism_.count() << "s" << endl;
    cout << "Duration with classic parallelism: " << duration_with_classic_parallelism_.count() << "s" << endl;
    cout << endl;
}

bool ConsoleForWinograd::GetMatrixInput(S21Matrix **mat) {
    string input;
    std::getline(cin, input);
    char ch;
    printf("string = [%s]\n", input.c_str());
    if (sscanf(input.data(), "%d%c%d", &rows_, &ch, &cols_) == 3 && ch == ' ') {
        if (rows_ <= 0 || cols_ <= 0) {
            cout << "Invalid number of rows, cols, try again pls" << endl;
            return false;
        } else {
            *mat = new S21Matrix(rows_, cols_);
            S21Matrix::FillMatrixWithRandValues(*mat);
        }
    } else {
        fstream file(input);
        if (!file) {
            cout << "Invalid input, you need to enter file name or matrix dimensions" << endl;
            return false;
        }
        *mat = S21Matrix::ParseFileWithMatrix(file);
        S21Matrix::FillMatrixWithRandValues(*mat);
        file.close();
    }
    rows_ = 0;
    cols_ = 0;
    return true;
}

int ConsoleForWinograd::RequestNmbFromUser(string message) {
    std::string input;
    cout << message;
    cin >> input;
    int nmb;
    try {
        nmb = std::stoi(input);
    } catch (std::invalid_argument &exp) {
        nmb = 0;
    }
    return nmb;
}

}