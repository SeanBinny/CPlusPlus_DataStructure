#ifndef MATRIX_H
#define MATRIX_H
#include <ostream>
#include "illegalparametervalue.h"

using namespace std;

template <class T>
class Matrix
{
    friend ostream& operator << (ostream&, const Matrix<T>&);
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
        element    = new T [theRows * theColumns];

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
Matrix<T> Matrix<T>::operator +(const Matrix<T>& m)
{
    if (theRows != m.theRows || theColumns != m.theColumns)                /* size not match                                     */
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, theColumns);
    for (int i = 0; i < theRows*theColumns; i++)
        w.element[i] = element[i] + m.element[i];

    return w;
}

/***************************************************************************
* Name          : operator +
* Descirpyion   : overload + to ensure usage is consistent with math
* Input         : none
* Output        : 1.    : return this matrix
***************************************************************************/
template <class T>
Matrix<T> Matrix<T>::operator +() const
{
    return *this;
}

/***************************************************************************
* Name          : operator -
* Descirpyion   : overload - to ensure usage is consistent with math
* Input         : 1.m  : another matrix you want to minus
* Output        : 1.w  : return a minus matrix
***************************************************************************/
template <class T>
Matrix<T> Matrix<T>::operator -(const Matrix<T>& m)
{
    if (theRows != m.theRows || theColumns != m.theColumns)                /* size not match                                     */
        throw illegalParameterValue("Size doesn't match");

    Matrix<T> w(theRows, theColumns);
    for (int i = 0; i < theRows*theColumns; i++)
        w.element[i] = element[i] - m.element[i];

    return w;
}

/***************************************************************************
* Name          : operator -
* Descirpyion   : overload - to ensure usage is consistent with math
* Input         : none
* Output        : 1.    : return this matrix
***************************************************************************/
template <class T>
Matrix<T> Matrix<T>::operator -() const
{
    for (int i; i < theRows*theColumns; i++)
        this->element[i] = -this->element[i];
    return *this;
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
    int ct = 0, cm = 0, cw = 0;                                            /* they are cursor of   *this   m  w;  w = this * m and
                                                                              they are 1D cursor of element array meanwhile 1D
                                                                              cursor means it can index by row (like i.k.j order)*/
    for (int i = 1; i <= theRows; i++)
    {
        for (int j = 1; j <= m.theColumns; j++)
        {
            T sum = element[ct]*m.element[cm];                             /* accumulate first item                              */
            for (int k = 2; k <= theColumns; k++)                          /* accumulate all remaining items, so it start from 2 */
            {
                ct++;                                                      /* the next element in the i row                      */
                cm  += m.theColumns;                                       /* the next element in the j column                   */
                sum += element[ct]*m.element[cm];
            }
            w.element[cw++] = sum;                                         /* save in w(i,j)                                     */
            ct -= theColumns - 1;                                          /* restart from the next row and this column          */
            cm  = j;
        }
        ct += theColumns;                                                  /* restart from the next row and the first column     */
        cm  = 0;
    }
    return w;
}

/***************************************************************************
* Name          : operator +=
* Descirpyion   : overload += to increment all elements of *this by x
* Input         : 1.x   : increment all elements of *this by x
* Output        : 1.    : this matrix
***************************************************************************/
template <class T>
Matrix<T>& Matrix<T>::operator +=(const T& x)
{
    // Increment all elements of *this by x.
   for (int i = 0; i < theRows*theColumns; i++)
       element[i] += x;
   return *this;
}

/***************************************************************************
* Name          : operator <<
* Descirpyion   : overload << to put matrix m into the stream out.
* Input         : 1.out : object of ostream to output
*               : 2.m   : matrix you want to output
* Output        : 1.out : for continuous call
***************************************************************************/
template<class T>
ostream& operator<<(ostream& out, const Matrix<T>& m)
{
   int k = 0;  // index into element array                                 /* element is 1D array so we need a k (1D index)     */
   for (int i = 0; i < m.theRows; i++)
   {
      for (int j = 0; j < m.theColumns; j++)
         out << m.element[k++] << "  ";

      out << endl;
   }

   return out;
}

/****************************************** Special Matrix (square matrix)*******************************************************/
template <class T>
class SpecialMatrix
{
public:
    virtual ~SpecialMatrix() {delete [] element;}

    virtual  T get(int, int) const = 0;
    virtual  void set(int, int, const T&) = 0;

protected:
    int n;
    T *element;
};

/************* Diagonal Matrix *******************************************/
template <class T>
class diagonalMatrix : public SpecialMatrix<T>                            /*      -  X   0   0   0   0  -                        */
{                                                                         /*      |  0   X   0   0   0  |                        */
public:                                                                   /*      |  0   0   X   0   0  |                        */
    diagonalMatrix(int theN = 10);                                        /*      |  0   0   0   X   0  |                        */
   ~diagonalMatrix() {delete [] this->element;}                                 /*      -  0   0   0   0   X  -                        */

    T get(int, int) const;
    void set(int, int, const T &);
};

/***************************************************************************
* Name          : diagonalMatrix
* Descirpyion   : construct a diagonal matrix
* Input         : 1.theN : size of matrix
* Output        : none
***************************************************************************/
template <class T>
diagonalMatrix<T>::diagonalMatrix(int theN)
{
    if (theN < 1)
        throw illegalParameterValue("Matrix size must be > 0");

    this->n       = theN;
    this->element = new T [this->n];
}

/***************************************************************************
* Name          : get
* Descirpyion   : get an element whos index is (i,j)
* Input         : 1. i      : row of index  2. j  : column of index
* Output        : 1.element : element you want to find
*               : 2. 0      : element can't find
***************************************************************************/
template <class T>
T diagonalMatrix<T>::get(int i, int j) const
{
    if (i < 1 || j < 1 || i > this->n || j > this->n)
        throw illegalParameterValue("Index out of range");

    if (i == j)
        return this->element[i-1];
    else
        return 0;
}

/***************************************************************************
* Name          : set
* Descirpyion   : set an element whos index is (i,j)
* Input         : 1. i       : row of index  2. j  : column of index
*               : 3.newValue : new element you want to replace
* Output        : none
***************************************************************************/
template <class T>
void diagonalMatrix<T>::set(int i, int j, const T& newValue)
{
    if (i < 1 || j < 1 || i > this->n || j > this->n)
        throw illegalParameterValue("Index out of range");

    if (i == j)                                                            /* index only in diagonal can replace                */
        this->element[i-1] = newValue;
    else
        throw illegalParameterValue("nodiagonal elements must be zero");
}

/************* tripl diagonal Matrix **************************************/
template <class T>
class tridiagonalMatrix : public SpecialMatrix<T>                          /*      -  X   X   0   0   0  -                      */
{                                                                          /*      |  X   X   X   0   0  |                      */
public:                                                                    /*      |  0   X   X   X   0  |                      */
    tridiagonalMatrix(int theN = 10);                                      /*      |  0   0   X   X   X  |                      */
   ~tridiagonalMatrix(){delete [] this->element;}                          /*      -  0   0   0   X   X  -                      */

    T get(int, int) const;
    void set(int, int, const T &);
};

/***************************************************************************
* Name          : get
* Descirpyion   : get an element whos index is (i,j)
* Input         : 1. i      : row of index  2. j  : column of index
* Output        : 1.element : element you want to find
*               : 2. 0      : element can't find
***************************************************************************/
template <class T>
T tridiagonalMatrix<T>::get(int i, int j) const
{
    if (i < 1 || j < 1 || i > this->n || j > this->n)
        throw illegalParameterValue("Index out of range");

    switch (i-j)
    {
        case 1 : return this->element[i-2];                                /* Lower diagonal                                     */
        case 2 : return this->element[this->n+i-2];                        /* Main  diagonal                                     */
        case 3 : return this->element[2*this->n+i-2];                      /* Upper diagonal                                     */
        default: return 0;
    }
}

/***************************************************************************
* Name          : set
* Descirpyion   : set an element whos index is (i,j)
* Input         : 1. i       : row of index  2. j  : column of index
*               : 3.newValue : new element you want to replace
* Output        : none
***************************************************************************/
template <class T>
void tridiagonalMatrix<T>::set(int i, int j, const T& newValue)
{
    if (i < 1 || j < 1 || i > this->n || j > this->n)
        throw illegalParameterValue("Index out of range");

    switch (i-j)
    {
        case 1 : this->element[i-2]     = newValue;                        /* Lower diagonal                                     */
        case 2 : this->element[this->n+i-2 ]  = newValue;                  /* Main  diagonal                                     */
        case 3 : this->element[2*this->n+i-2] = newValue;                  /* Upper diagonal                                     */
        default:  throw illegalParameterValue("nodiagonal elements "
                                              "must be zero");
    }
}

#endif // MATRIX_H


/* lower Triangular Matrix
 * Upper Triangular Matrix
 * Symmetric matrix           hanen't done      */
