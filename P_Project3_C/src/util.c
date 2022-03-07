//
// Created by 叶璨铭 on 2021/10/19.
//
#include "util.h"
//异常处理
void abort_with_message(const char* message) {
    fprintf(stderr, "%s", message);  // code review： 变量字符串必须要%s打印防止问题
    abort();
}
//文件处理
size_t file_len(FILE* file) {  //会不会把file的位置改了，导致到了文件末尾？
    fseek(file, 0L, SEEK_END);
    size_t result = ftell(file);
    fseek(file, 0L, SEEK_SET);  //会的，必须复原
    return result;
}
char* read_all(FILE* file){
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);//不能漏
    char* result = (char*)malloc((file_size+1)*sizeof(char));
    fread(result, file_size, sizeof(char), file);
    fclose(file);
    return result;
}
// float* file_read_floatPtr(const char* filename){
//     FILE* file = fopen(filename, "r");
//
// }
//字符串处理
struct StringBuilder* _StringBuilder(char** stringArray, size_t arrayLength) {
    struct StringBuilder* newStringBuilder = (struct StringBuilder*)malloc(sizeof(struct StringBuilder));
    newStringBuilder->stringArray = stringArray;
    newStringBuilder->arrayLength = arrayLength;
    newStringBuilder->k = 0;
    newStringBuilder->strSize = 0;
    return newStringBuilder;
}
struct StringBuilder* StringBuilder(size_t arrayLength) {
    return _StringBuilder((char**)calloc(arrayLength, sizeof(char*)), arrayLength);
}
struct StringBuilder* append_string(struct StringBuilder* stringBuilder, const char* string) {  //外面的str需要复制一份
    size_t elementLength = strlen(string);
    char* newStringArrayElement = (char*)calloc(elementLength, sizeof(char));
    strcpy(newStringArrayElement, string);
    stringBuilder->stringArray[stringBuilder->k++] = newStringArrayElement;
    stringBuilder->strSize += elementLength;
    return stringBuilder;
}


void fprintf_string_builder(FILE* stream, struct StringBuilder* stringBuilder){
    for(int i = 0; i < stringBuilder->arrayLength; ++i) {
        fprintf(stream, "%s", stringBuilder->stringArray[i]);
    }
}

char* stringBuilderToString(struct StringBuilder* stringBuilder) {
    char* result = (char*)calloc(stringBuilder->strSize, sizeof(char));
    for(size_t i = 0; i < stringBuilder->arrayLength; ++i) {
        strcat(result, stringBuilder->stringArray[i]);  // code review: bad use of strcat
    }
    return result;
}
