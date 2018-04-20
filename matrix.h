#ifndef MATRIX_H
#define MATRIX_H
#include <ostream>
#include "illegalparametervalue.h"

using namespace std;

template <class T>
class Matrix
{
    friend ostream& operator << (ostream&, const matrix<T>&);
public:
    Matrix(int theRows = 0, int theColums = 0);
    Matrix(const Matrix<T>&);
   ~Matrix() {delete [] element;}
    int rows() const {return theRows;}
    int columns() const {return theColumns;}
    T&  operator () (int i, int j) const;///////////////

    Matrix<T>& operator = (const Matrix<T>&);
    Matrix<T>  operator + (const Matrix<T>&);
    Matrix<T>  operator + () const;
    Matrix<T>  operator - (const Matrix<T>&);
    Matrix<T>  operator - () const;
    Matrix<T>  operator * (const Matrix<T>&);
    Matrix<T>& operator+= (const T&);

private:
    int theRows,
        theColumns;
    T  *element;
};

template<class T>
Matrix<T>::Matrix(int theRows, int theColums)
{
    if (theRows < 0 || theColums < 0)
        throw illegalParameterValue("Rows and columns must be >= 0");
    if ((theRows == 0 || theColums == 0)
      &&(theRows != 0 || theColums != 0))
    {
        throw illegalParameterValue("Either noth or neither rows and"
                                    "columns should be zero");
    }
    this->theRows    = theRows;
    this->theColumns = theColums;
    this->element    = new T [theRows * theColums];
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
    theRows    = m.theRows;
    theColumns = m.theColumns;
    element    = new T [theRows * theColums];

    copy(m.element, m.element+theRows*theColumns,
         element);
}

template <class T>
Matrix<T>& Matrix<T>::operator =(const Matrix<T>& m)
{
    if (this != &m)
    {
        delete [] element;
        theRows    = m.theRows;
        theColumns = m.theColumns;
        element    = new T [theRows * theColums];

        copy(m.element, m.element+theRows*theColumns,
             element);
    }
    return *this;
}

template <class T>
T& Matrix<T>::operator ()(int i, int j) const
{
    if (i < 1 || i > theRows ||  j < 1 || j > theColumns)
        throw illegalParameterValue("Matrix index out of bounds");

    return element[(i-1)*theColumns + j - 1];
}

template <class T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& m) const
{
    if (theRows != m.theRows || theColumns != m.theColumns)
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, theColumns);
    for (int i = 0; i < theRows*theColumns; i++)
        w.element[i] = element[i] + m.element[i];

    return w;
}

template <class T>
Matrix<T> Matrix<T>::operator *(const Matrix<T>& m)
{
    if (theColumns != m.theRows)
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, m.theColumns);

    int ct = 0, cm = 0, cw = 0;
    for (int i = 1; i <= theRows; i++)
    {
        for (int j = 1; j <= m.theColumns; j++)
        {
            T sum = element[ct]*m.element[cm];
            for (int k = 2; k <= theColumns; k++)
            {
                ct++;
                cm  += m.theColumns;
                sum += element[ct]*m.element[cm];
            }
            w.element[cw++] = sum;
            ct  -= theColumns - 1;
            sum  =  0;
        }
        ct += theColumns;
        cm  = 0;
    }
    return w;
}


#endif // MATRIX_H
