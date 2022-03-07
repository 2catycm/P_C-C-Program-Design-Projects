#include "matrix_multiplication.h"
//#define SIZE_OF_PATH "1M1K"
#define SIZE_OF_PATH "2048"
#define PROJECT_DIR "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\"
#define pathA PROJECT_DIR"resources\\input_data\\mat-A-" SIZE_OF_PATH ".txt"
#define pathB PROJECT_DIR"resources\\input_data\\mat-B-" SIZE_OF_PATH ".txt"
#define out_path PROJECT_DIR"resources\\output_data\\mat-B-" SIZE_OF_PATH ".txt"
#define pathC PROJECT_DIR"resources\\output_data\\mat-C-" SIZE_OF_PATH ".txt"
//#define Five_Times(statement) statement statement statement statement statement
#define Five_Times(statement) statement
void generateData(){
    struct TrivialMatrix* a, *b;
    a = create_trivial_matrix(1000000, 1000);
    b = create_trivial_matrix(1000000, 1000);
    NewRandom;
    for(size_t i = 0; i < 1000000; ++i) {
        for(size_t j = 0; j < 1000; ++j) {
            TrivialMatrixGetElement(a, i, j) = RandomNextInt(10);
            TrivialMatrixGetElement(b, i, j) = RandomNextInt(10);
        }
    }
//    abort_with_message("generate finished.");
    write_trivial_matrix_to_file(a, pathA);
    write_trivial_matrix_to_file(a, pathB);
    printf("write finished.");
    delete_trivial_matrix(a);
    delete_trivial_matrix(b);
    printf("delete finished.");
}
struct TrivialMatrix* readMat(char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        abort_with_message("Fatal Error: File not found.\nmatrix multiplication terminated.");
    }

    float* const data = (float*)calloc(1000000000, sizeof(float));
    for(size_t i = 0; i < 1000000000; ++i) {
        fscanf(file, "%f", &data[i]);
    }
    return _trivial_matrix(data, 1000000,1000);
}

int main(int argc, char const *argv[])
{
//    generateData();
    struct TrivialMatrix* a, *b, *c;
    TimerStart;
//    Five_Times(a = readMat(pathA);)
//    Five_Times(b = readMat(pathB);)
    Five_Times(a = read_trivial_matrix_from_file(pathA);)
    Five_Times(b = read_trivial_matrix_from_file(pathB);)
    TimerEndPrintlnStart("reading two matrices: ");
    c = trivial_matrix_addition_simd(a, b);
    c = trivial_matrix_addition_simd(a, b);
    TimerEndPrintlnStart("warming up: ");
    c = trivial_matrix_addition_simd(a,b);
    TimerEndPrintlnStart("simd ");
    c = trivial_matrix_addition(a,b);
    TimerEndPrintlnStart("no simd. ");
    write_trivial_matrix_to_file(c, pathC);
    TimerEndPrintln("write. ");
    return 0;
}
// reading two matrices:  :2036ms
// warming up:  :8ms
// simd  :4ms
// no simd.  :7ms
// write.  :1221ms


//190ms