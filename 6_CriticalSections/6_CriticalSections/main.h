#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>

const int kMaxMatrixSize = 1000;
const int kNumIterations = 10000000;

const int kMatrixARows = 3;
const int kMatrixBRows = 4;
const int kMatrixBCols = 3;

std::mutex mutex_;
int matrix_a[kMaxMatrixSize][kMaxMatrixSize];
int matrix_b[kMaxMatrixSize][kMaxMatrixSize];
int matrix_c[kMaxMatrixSize][kMaxMatrixSize];

std::atomic<int> counter = 0;

void MatrixMultiply(int row_index, int col_index, int matrix_size) {
    matrix_c[row_index][col_index] = 0;
    for (int k = 0; k < matrix_size; k++) {
        matrix_c[row_index][col_index] += matrix_a[row_index][k] * matrix_b[k][col_index];
    }
    printf("Matrix C[%d][%d] = %d\n", row_index, col_index, matrix_c[row_index][col_index]);
}

void GenerateMatrix(int rows, int cols, int matrix[][kMaxMatrixSize]) {
    srand(time(NULL));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 200;
            printf("\t%d", matrix[i][j]);
        }
        printf("\n");
    }
}

void WithoutLock(int* counter_ptr) {
    for (int i = 0; i < kNumIterations; i++) {
        (*counter_ptr)++;
    }
}

void WithLock(int* counter_ptr) {
    for (int i = 0; i < kNumIterations; i++) {
        mutex_.lock();
        (*counter_ptr)++;
        mutex_.unlock();
    }
}

void WithLockAtomic() {
    for (int i = 0; i < kNumIterations; i++) {
        mutex_.lock();
        counter++;
        mutex_.unlock();
    }
}

void ParallelThreads() {
    clock_t start_time = clock();

    int* counter1_ptr = new int;
    *counter1_ptr = 0;
    std::thread thread1(WithoutLock, counter1_ptr);
    std::thread thread2(WithoutLock, counter1_ptr);
    thread1.join();
    thread2.join();
    std::cout << "\nWithout lock" << "\n";
    std::cout << *counter1_ptr << std::endl;
    start_time = clock() - start_time;
    printf("%f\n", (double)start_time / CLOCKS_PER_SEC);

    clock_t start_time2 = clock();
    int* counter2_ptr = new int;
    *counter2_ptr = 0;
    std::thread thread3(WithLock, counter2_ptr);
    std::thread thread4(WithLock, counter2_ptr);
    thread3.join();
    thread4.join();
    std::cout << "\nWith lock (mutex)" << "\n";
    std::cout << *counter2_ptr << "\n";
    start_time2 = clock() - start_time2;
    printf("%f\n", (double)start_time2 / CLOCKS_PER_SEC);

    clock_t start_time3 = clock();
    std::thread thread5(WithLockAtomic);
    std::thread thread6(WithLockAtomic);
    thread5.join();
    thread6.join();
    std::cout << "\nWith lock (atomic)" << "\n";
    std::cout << counter << "\n";

    start3 = clock() - start3;
    printf("%f\n", (double)start3 / CLOCKS_PER_SEC);

}

int main()
{
    while (true)
    {
        int choose;
        printf("Choose Lab Work, where %d is Generating and multiplying matrixes\nAnd %d is Threads parallel work", 22, 23);
        printf("\nEnter the task number: ");
        std::cin >> choose;
        switch (choose) {
        case 22:
            function1();
            break;
        case 23:
            function2();
            break;
        default:
            std::cout << "Invalid value. Try again later." << std::endl;
        }
        return 0;
    }
}