#include <iostream>
#include "function1.h"
#include "function2.h"

int main(){
    Function1 func1;
    Function2 func2;

    int num;
    //추후에 번호 말고 명령어 형식으로 바꿔야함.
    //그리고 나중에는 makefile 만들어서 컴파일.
    while(true){

        std::cin >> num;

        if(num==0)break;
        else if(num==1)func1.doSomething();
        else if(num==2)func2.doSomething2();
        else std::cout << "Invalid Input" << std::endl;
    }

    return 0;
}