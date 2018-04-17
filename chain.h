#ifndef CHAIN_H
#define CHAIN_H
#include "linearlist.h"

/*-------------------------------------------------------------------------*
* Struct  Name  : chainNode
* Descirpyion   : node for singly linked list
* Data    Field : element
* Pointer Field : next : point to the next node
*--------------------------------------------------------------------------*/
template <class T>
struct chainNode
{
    /***** Data member *****************************************************/
    T element;
    chainNode<T> *next;

    /***** Method **********************************************************/
    chainNode() {}
    chainNode(const T& element) {this->element = element;}                  /* initialize the data                                */
    chainNode(const T& element, chainNode<T>* next)                         /* initialize the data and pointer                    */
    {
        this->element = element;
        this->next    = next;
    }
};


template <class T>
class chain : public linerList<T>
{
public:
    chain(int initialCapacity = 10);                                        /* initialCapacity just for compatible with arraylist */
    chain(const chain<T>&);
    ~chain();

    /************ ADT method: **********************************************/
    T&   get(int theIndex) const;
    bool empty() const {return listSize == 0;}
    int  size() const {return listSize;}
    int  indexOf(const T& theElement) const;

    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;

protected:
    void checkIndex(int theIndex) const;                                    /* if theIndex is invalid, throw illegal             */
    chainNode<T>* firstNode;                                                /* point to the first node of linked list            */
                                                                            /* fistNode = NULL means linked list is empty        */
    int listSize;                                                           /* number of elements in the linked list             */
};

template <class T>
chain<T>::chain(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0 ";
        throw illegalParameterValue(s.str());
    }
    firstNode = NULL;
    listSize  = 0;
}

template <class T>
chain<T>::chain(const chain<T>& theList)
{
    listSize = theList.listSize;

    if (listSize == 0)
    {
        firstNode = NULL;
        return;
    }

    chainNode<T>* sourceNode = theList.firstNode;
    firstNode  = new chain<T>*(sourceNode->element);
    sourceNode = sourceNode->next;
    chainNode<T>* targetNode = firstNode;

    while (sourceNode != NULL)
    {
        targetNode->next = new chainNode<T>(sourceNode->element);
        targetNode       = targetNode->next;
        sourceNode       = sourceNode->next;
    }
    targetNode->next = NULL;
}

template <class T>
chain<T>::~chain()
{
    while (firstNode != NULL)
    {
        chainNode<T>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template <class T>
chain<T>::get(int theIndex) const
{
    checkIndex(theIndex);

    chainNode<T>* currentNode = firstNode;
    for (int i = 0; i < theIndex; i++)
        currentNode = currentNode->next;

    return currentNode->element;
}

template <class T>
int chain<T>::indexOf(const T &theElement) const
{
    chainNode<T>* currentNode = firstNode;
    int index = 0;
    while (currentNode != NULL &&
           currentNode->element != theElement)
    {
        currentNode = currentNode->next;
        index++;
    }

    if (currentNode == NULL)
        return -1;
    else
        return index;
}
#endif // CHAIN_H
