#ifndef ILLEGALPARAMETERVALUE_H
#define ILLEGALPARAMETERVALUE_H
#include <iostream>
using namespace std;

class illegalParameterValue
{

public:
    illegalParameterValue():
        message("Illegal parameter value") {}
    illegalParameterValue(string theMessage) {message = theMessage;}           /* avoid warning:  convert string to char*        */
    //illegalParameterValue(char*  theMessage) {message = theMessage;}

    void outputMessage() {std::cout << message << endl;}

private:
    string message;
};

#endif // ILLEGALPARAMETERVALUE_H
