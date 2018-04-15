#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include "linearlist.h"
#include <sstream>

template <class T>
class arrayList : public linerList<T>
{
public:
    arrayList(int initialCapacity = 10);                                        /* constructor                                  */
    arrayList(const arrayList<T>&);                                             /* copy constructor                             */
    ~arrayList() {delete [] element;}                                           /* destructor                                   */

    /************ ADT method: *************************************************/
    bool empty() const {return listSize == 0;}
    int size() const {return listSize;}
    T& get(int theIndex) const;
    int indexOf(const T& theElement) const;

    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;

    /************ Class custom: ***********************************************/
    int capacity() const {return arrayLength;}
    void reduceCapacity(int capacity = Max(10,arrayLength/2));                  /* reduce number of empty positions to 1/2      */

protected:
    void checkIndex(int theIndex) const;                                        /* if theIndex is invalid, throw illegal        */
    T*  element;                                                                /* a 1D array to save the elements of liner list*/
    int arrayLength;                                                            /* capacity of this 1D array                    */
    int listSize;                                                               /* number of elements in the liner list         */
};



/***************************************************************************
* Name          : arrayList
* Descirpyion   : constructor
* Input         : 1.initialCapacity : initial capacity of this array
* Output        : none
***************************************************************************/
template <class T>
arrayList<T>::arrayList(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0 ";
        throw illegalParameterValue(s.str());
    }
    arrayLength = initialCapacity;
    element     = new T[arrayLength];
    listSize    = 0;
}

/***************************************************************************
* Name          : arrayList
* Descirpyion   : copy constructor
* Input         : 1.theList : object of class arrayList
* Output        : none
***************************************************************************/
template <class T>
arrayList<T>::arrayList(const arrayList<T>& theList)
{
    arrayLength = theList.arrayLength;
    listSize    = theList.listSize;
    element     = new T[arrayLength];
    copy(theList.element, theList.element+listSize, element);
}

/***************************************************************************
* Name          : checkIndex
* Descirpyion   : check if the index out of range
* Input         : 1.theIndex : index of element
* Output        : none
***************************************************************************/
template <class T>
void arrayList<T>::checkIndex(int theIndex) const
{  
    if (theIndex < 0 || theIndex >= listSize)                               /* ensure the index is between 0 and listSize-1     */
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalParameterValue(s.str());
    }
}

/***************************************************************************
* Name          : get
* Descirpyion   : return the element whos index is 'theIndex',if the element
*               : not exist, throw illegal
* Input         : 1.theIndex : index of element
* Output        : element whos index is 'theIndex'
***************************************************************************/
template <class T>
T& arrayList<T>::get(int theIndex) const
{
    checkIndex(theIndex);
    return element[theIndex];
}

/***************************************************************************
* Name          : indexOf
* Descirpyion   : return the index when 'theElement'showed first
* Input         : 1.theElement : element you want to find
* Output        : 1.theIndex   : index of element you want
*               : 2.-1         : element not exist
***************************************************************************/
template <class T>
int arrayList<T>::indexOf(const T &theElement) const
{
    int theIndex = (int) (find(element, element+listSize, theElement) - element);/* find element                                */
    if (theIndex == listSize)
        return -1;
    else
        return theIndex;
}
/***************************************************************************
* Name          : reduceCapacity
* Descirpyion   : reduce number of empty positions to 1/2
* Input         : 1.capacity : capacity you want to change
* Output        : none
***************************************************************************/
template <class T>
void arrayList<T>::reduceCapacity(int capacity)
{
    if(listSize < arrayLength/4)                                           /* only listSize < arrayLength/4 can reduce         */
    {
        T* tempArray = new T [arrayLength/2];
        copy(element, element+listSize, tempArray);
        delete [] element;
        element = tempArray;
        delete [] tempArray;
        arrayLength = arrayLength/2;
    }
}

/***************************************************************************
* Name          : erase
* Descirpyion   : delete the element whos index is theIndex
* Input         : 1.theIndex : index of element
* Output        : none
***************************************************************************/
template <class T>
void arrayList<T>::erase(int theIndex)
{
    checkIndex(theIndex);                                                  /* check if the index is illeagl                     */
    copy(element+theIndex+1, element+listSize, element+theIndex);          /* using copy to take the elements behind a step
                                                                              forward                                           */

    element[--listSize].~T();
}

/***************************************************************************
* Name          : insert
* Descirpyion   : insert a element into the particular index in the array
* Input         : 1.theIndex : index to insert
*               : 2.theElement : element to insert
* Output        : none
***************************************************************************/
template <class T>
void arrayList<T>::insert(int theIndex, const T &theElement)
{
    checkIndex(theIndex);                                                  /* invalid index                                     */

    if (listSize == arrayLength)                                           /* valid index and if the array is full, chang it size*/
    {                                                                      /* to 2 times which convenient for continuous insert  */
        changeLength1D(element, arrayLength, 2*arrayLength);
        arrayLength *= 2;
    }

    copy_backward(element+theIndex, element+listSize,                      /* using copy_backward to move elements to another me-*/
                  element+listSize+1);                                     /* mory (distinguish with copy())                     */
    element[theIndex] = theElement;
    listSize++;                                                            /* increase number of elements                        */
}


#endif // ARRAYLIST_H





//template <class T>
//void arrayList<T>::output(ostream &out) const    cout->out
//{
//    copy(element, element+listSize, ostream_interator<T>(cout, "  "));
//}

//template <class T>
//ostream& operator <<(ostream& out, const arrayList<T>& x)
//{
//    x.output(out);
//    return out;
//}
