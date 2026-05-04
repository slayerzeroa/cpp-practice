#include <iostream>


class Calculator{
    public:
    int add(int a, int b) {
        return a + b;
    };

    int multiple(int a, int b) {
        return a * b;
    };

    int subtract(int a, int b) {
        return a - b;
    };

    double divide(int a, int b) {
        return double(a/b);
    };
};

int main(){
    Calculator little_cal;

    std::cout << little_cal.divide(1, 2) << "\n";
}