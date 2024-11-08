#include <iostream>
using namespace std;

int factorial(){
    int sum = 1;
    for (int j = 2; j <= 10; j++){
        sum *= j;
    }
    return sum;
}


int main(){
    //1
    long s = 1.0;
    for(int i = 1; i <33; i++ )
    {
    s *= i;
    }

    cout<<s;
    
    //2
    for (int j = 0; j < 10000000; j++){
        factorial();
    }


}