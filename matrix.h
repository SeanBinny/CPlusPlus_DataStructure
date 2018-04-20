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
   ~Matrix() {delete [] element;}                                          /* delete array space                                 */

    int rows() const {return theRows;}
    int columns() const {return theColumns;}

    /*********** overloaded function *************************************/
    T&  operator () (int i, int j) const;
    Matrix<T>& operator = (const Matrix<T>&);
    Matrix<T>  operator + (const Matrix<T>&);
    Matrix<T>  operator + () const;
    Matrix<T>  operator - (const Matrix<T>&);
    Matrix<T>  operator - () const;
    Matrix<T>  operator * (const Matrix<T>&);
    Matrix<T>& operator+= (const T&);

private:
    int theRows,                                                            /* number of rows                                    */
        theColumns;                                                         /* number of columns                                 */
    T  *element;                                                            /* point to a array where saves elements             */
};

/***************************************************************************
* Name          : Matrix
* Descirpyion   : construct a matrix
* Input         : 1.theRows : number of rows 2.theColums : number of columns
* Output        : none
***************************************************************************/
template<class T>
Matrix<T>::Matrix(int theRows, int theColums)
{
    if (theRows < 0 || theColums < 0)
        throw illegalParameterValue("Rows and columns must be >= 0");
    if ((theRows == 0 || theColums == 0)
      &&(theRows != 0 || theColums != 0))
    {
        throw illegalParameterValue("Either both or neither rows and"       /* eg: a[0][4] b[4][0] c[0][0]  are all wrong        */
                                    "columns should be zero");
    }

    this->theRows    = theRows;
    this->theColumns = theColums;
    this->element    = new T [theRows * theColums];                         /* apply for array to save elements                  */
}

/***************************************************************************
* Name          : Matrix
* Descirpyion   : using another matrix to construct a matrix
* Input         : 1.m : another matrix you want to copy
* Output        : none
***************************************************************************/
template <class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
    theRows    = m.theRows;
    theColumns = m.theColumns;
    element    = new T [theRows * theColumns];                              /* apply for memory                                  */

    copy(m.element, m.element+theRows*theColumns,                           /* copy elements                                     */
         element);
}

/***************************************************************************
* Name          : operator =
* Descirpyion   : overload = to ensure usage is consistent with math
* Input         : 1.m : another matrix you use to assign
* Output        : 1   : return a same matrix
***************************************************************************/
template <class T>
Matrix<T>& Matrix<T>::operator =(const Matrix<T>& m)
{
    if (this != &m)                                                         /* only this and m are different can achieve         */
    {
        delete [] element;                                                  /* delete original array                             */
        theRows    = m.theRows;
        theColumns = m.theColumns;
        element    = new T [theRows * theColums];

        copy(m.element, m.element+theRows*theColumns,
             element);
    }
    return *this;                                                           /* whatever if assign successfully, return this      */
}

/***************************************************************************
* Name          : operator ()
* Descirpyion   : overload () to ensure usage is consistent with math
* Input         : 1.i : row of index   2.j  :  column of index
* Output        : 1.element  : return an element whos index is (i,j)
***************************************************************************/
template <class T>
T& Matrix<T>::operator ()(int i, int j) const
{
    if (i < 1 || i > theRows ||  j < 1 || j > theColumns)                   /* ensure it is not eg: a[0][4] b[4][0] c[0][0]     */
        throw illegalParameterValue("Matrix index out of bounds");          /* or out of range                                  */

    return element[(i-1)*theColumns + j - 1];                               /* array mapping formula                            */
}

/***************************************************************************
* Name          : operator +
* Descirpyion   : overload + to ensure usage is consistent with math
* Input         : 1.m  : another matrix you want to add
* Output        : 1.w  : return a summation matrix
***************************************************************************/
template <class T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& m) const
{
    if (theRows != m.theRows || theColumns != m.theColumns)                /* size not match                                     */
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, theColumns);
    for (int i = 0; i < theRows*theColumns; i++)
        w.element[i] = element[i] + m.element[i];

    return w;
}

/***************************************************************************
* Name          : operator *
* Descirpyion   : overload * to ensure usage is consistent with math
* Input         : 1.m  : another matrix you want to multiply
* Output        : 1.w  : return a product matrix
***************************************************************************/
template <class T>
Matrix<T> Matrix<T>::operator *(const Matrix<T>& m)
{
    if (theColumns != m.theRows)                                           /* size not match                                     */
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, m.theColumns);                                    /* define a result matrix                             */

    int ct = 0, cm = 0, cw = 0;                                            /* they are cursor of   *this   m    w                */
    for (int i = 1; i <= theRows; i++)
    {
        for (int j = 1; j <= m.theColumns; j++)
        {
            T sum = element[ct]*m.element[cm];                             /* accumulate first item                              */
            for (int k = 2; k <= theColumns; k++)                          /* accumulate all remaining items, so it start from 2 */
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
