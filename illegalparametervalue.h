#ifndef ILLEGALPARAMETERVALUE_H
#define ILLEGALPARAMETERVALUE_H
#include <iostream>
using namespace std;

class illegalParameterValue
{

public:
    illegalParameterValue():
        message("Illegal parameter value") {}
    illegalParameterValue(char*  theMessage) {message = theMessage;}
    illegalParameterValue(string theMessage) {message = theMessage;}                        /* avoid prameter is not char*        */

    void outputMessage() {std::cout << message << endl;}

private:
    string message;
};

#endif // ILLEGALPARAMETERVALUE_H
