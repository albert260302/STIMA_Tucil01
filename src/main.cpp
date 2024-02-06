#include "main.h"
#include <iostream>
#include <ctime>

int main(){
    const clock_t start = clock();
    int buff, size;
    for (int i=0;i<10000000;i++){
    }
    const clock_t ends = clock();
    std::cout<<"Time: "<<float(ends-start)<< " ms";
    return 0;
}