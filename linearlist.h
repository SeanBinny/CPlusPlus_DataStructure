#ifndef LINEARLIST_H
#define LINEARLIST_H
#include "illegalparametervalue.h"
#include "mymath.h"
#include <algorithm>

template <class T>
class linerList
{
public:
    virtual ~linerList() {}                                                /* if define the base destructor, it must be implemented
                                                                              you can 1: virture =0 + out of class implementation
                                                                              or      2:  virture   + in class implementation     */
    virtual bool empty() const  = 0;                                       /* return true, when liner list is empty               */
    virtual int  size () const  = 0;                                       /* return the number of elements in the list           */
    virtual T&   get (int theIndex) const = 0;                             /* return the element whos index is 'theindex'         */
    virtual int  indexOf(const T& theElement) const = 0;                   /* return the index when 'theElement'showed first      */

    virtual void erase(int theIndex) = 0;                                  /* delete the element whos index is 'theIndex'         */
    virtual void insert(int theIndex, const T& theElement) = 0;            /* insert 'theElement' into the position is 'theIndex" */
    virtual void output(ostream& out) const = 0;                           /* insert the liner list into ostream 'out'            */

};

/******** extended method for linear list**********************************/
template <class T>
class extendedLinearlist : linerList<T>                                      /* use private inherit to prevent functions repeat     */
{
public :
    virtual ~extendedLinearlist() {}
    virtual void clear() = 0;                                              /* clear all elements of list                          */
    virtual void push_back(const T& theElement) = 0;                       /* insert theElemnt at end of list                     */
};

/***************************************************************************
* Name          : changeLength1D
* Descirpyion   : function to change a 1D array size
* Input         : 1.a : the array you want to change      2.oldLength
*               : 3.newLeng
* Output        : none
***************************************************************************/
template <class T>
void changeLength1D(T*& a, int oldLength, int newLength)
{
    if (newLength < 0)
    {
        throw illegalParameterValue("new length must be >= 0");
    }

    T* temp = new T[newLength];                                            /* new array                                           */
    int number = min(oldLength, newLength);                                /* number of elements need to copy                     */
    copy(a, a+number, temp);
    delete [] a;                                                           /* relase the memory of old array                      */
    a = temp;
}





#endif // LINEARLIST_H
