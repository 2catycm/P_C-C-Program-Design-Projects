#include "matrix_multiplication.h"
#define SIZE_OF_PATH "2048"
#define PROJECT_DIR "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\"
#define pathA PROJECT_DIR"resources\\input_data\\mat-A-" SIZE_OF_PATH ".txt"
#define pathB PROJECT_DIR"resources\\input_data\\mat-B-" SIZE_OF_PATH ".txt"
#define out_path PROJECT_DIR"resources\\output_data\\mat-B-" SIZE_OF_PATH ".txt"
#define pathC PROJECT_DIR"resources\\output_data\\mat-C-" SIZE_OF_PATH ".txt"
//#define Five_Times(statement) statement statement statement statement statement
#define Five_Times(statement) statement
int main(int argc, char const *argv[])
{
    struct TrivialMatrix* a, *b, *c;
    TimerStart;
    Five_Times(a = read_trivial_matrix_from_file(pathA);)
    Five_Times(b = read_trivial_matrix_from_file(pathB);)
    TimerEndPrintlnStart("reading two matrices: ");
    c = matmul_openmp2(a, b);
    c = matmul_openmp2(a, b);
    TimerEndPrintlnStart("warming up: ");
    Five_Times(c = matmul_blas(a, b);)
    TimerEndPrintlnStart("matmul_blas: ");
    Five_Times(c = matmul_openmp2(a, b);)
    TimerEndPrintlnStart("matmul_openmp2: ");
    Five_Times(c = matmul_simd(a, b);)
    TimerEndPrintlnStart("matmul_simd: ");

    Five_Times(c = matmul_simd_openmp(a, b);)
    TimerEndPrintlnStart("matmul_simd_openmp2: ");

    Five_Times(c = matmul_strassen(a, b);)
    TimerEndPrintlnStart("matmul_strassen: ");
    Five_Times(c = matmul_trivial(a, b);)
    TimerEndPrintlnStart("matmul_trivial: ");

    Five_Times(write_trivial_matrix_to_file(c, pathC);)
    TimerEndPrintln("write matrix to file: ");
    return 0;
}
