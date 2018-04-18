#ifndef CHAIN_H
#define CHAIN_H
#include "linearlist.h"
#include <sstream>

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


/*************** Class definition ******************************************/
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

    /************ Class custom: ********************************************/
    class iterator;                                                         /* add a iterator for class arraylist                */
    iterator begin() {return iterator(firstNode);}
    iterator end()   {return iterator(NULL);}
    
protected:
    void checkIndex(int theIndex) const;                                    /* if theIndex is invalid, throw illegal             */
    chainNode<T>* firstNode;                                                /* point to the first node of linked list            */
                                                                            /* fistNode = NULL means linked list is empty        */
    int listSize;                                                           /* number of elements in the linked list             */
};

/***************************************************************************
* Name          : chain
* Descirpyion   : construct a empty chain list
* Input         : 1.initialCapacity : symbolic capacity
* Output        : none
***************************************************************************/
template <class T>
chain<T>::chain(int initialCapacity)                                        /* initialCapacity has no real significance         */
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

/***************************************************************************
* Name          : chain
* Descirpyion   : using another list to construct a chain list
* Input         : 1.theList : object of another list
* Output        : none
***************************************************************************/
template <class T>
chain<T>::chain(const chain<T>& theList)
{
    listSize = theList.listSize;

    /*********** situation : empty ****************************************/
    if (listSize == 0)
    {
        firstNode = NULL;
        return;
    }

    /*********** sintuation : not empty ***********************************/
    chainNode<T>* sourceNode = theList.firstNode;                          /* use source to pick up firstNode of theList         */
    firstNode  = new chain<T>*(sourceNode->element);                       /* this List point to first node address              */
    chainNode<T>* targetNode = firstNode;                                  /* use target to pick up firstNode of this list       */

    sourceNode = sourceNode->next;                                         /* from now , source is more than target to the next  */
    while (sourceNode != NULL)
    {
        targetNode->next = new chainNode<T>(sourceNode->element);          /* use theList element to construct a chain node for  *
                                                                              this list and put it as the next node              */
        targetNode       = targetNode->next;
        sourceNode       = sourceNode->next;
    }
    targetNode->next = NULL;
}

/***************************************************************************
* Name          : ~chain
* Descirpyion   : destructor a chain list
* Input         : none
* Output        : none
***************************************************************************/
template <class T>
chain<T>::~chain()
{
    while (firstNode != NULL)
    {
        chainNode<T>* nextNode = firstNode->next;                           /* delete all node                                   */
        delete firstNode;
        firstNode = nextNode;
    }
}

/***************************************************************************
* Name          : checkIndex
* Descirpyion   : check if the index out of range
* Input         : 1.theIndex : index of element
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::checkIndex(int theIndex) const
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
* Descirpyion   : return a element whos index is theIndex
* Input         : 1.theIndex : index you want
* Output        : 1.element  : element whos index is theIndex
***************************************************************************/
template <class T>
T& chain<T>::get(int theIndex) const
{
    checkIndex(theIndex);                                                   /* check if the index is illeagl                     */

    chainNode<T>* currentNode = firstNode;                                  /* use target to pick up firstNode of this list      */
    for (int i = 0; i < theIndex; i++)
        currentNode = currentNode->next;

    return currentNode->element;
}

/***************************************************************************
* Name          : indexOf
* Descirpyion   : return an index of theElement
* Input         : 1.theElement : element you want to get
* Output        : 1.theIndex   : index of element you want
*               : 2.-1         : element not exist
***************************************************************************/
template <class T>
int chain<T>::indexOf(const T &theElement) const
{
    chainNode<T>* currentNode = firstNode;                                 /* using current to pick up the firstNode             */
    int index = 0;                                                         /* for return                                         */
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

/***************************************************************************
* Name          : erase
* Descirpyion   : delete an element whos index is theIndex
* Input         : 1.theIndex : index you want to delete
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::erase(int theIndex)
{
    checkIndex(theIndex);
    
    chainNode<T>* deleteNode;                                               /* use delete point to the node  you want to delete  */

    if (theIndex == 0)                                                      /* delete the first node                             */
    {
        deleteNode = firstNode;
        firstNode  = firstNode->next;
    }
    else                                                                    /* delete the inside nodes                           */
    {
        chainNode<T>* p = firstNode;                                        /* use p to pick up the firstNode                    */
        for (int i = 0; i < theIndex-1; i++)
            p = p->next;                                                    /* now p point to the node in front of the target    */
        
        deleteNode = p->next;                                               /* delete point to the target                        */
    }
    listSize--;
    delete deleteNode;
}

/***************************************************************************
* Name          : insert
* Descirpyion   : insert an element whos index is theIndex
* Input         : 1.theIndex   : index where you want to insert
*               : 2.theElement : element you want to insert
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::insert(int theIndex, const T &theElement)
{
    checkIndex(theIndex);
    
    if (theIndex == 0)
        firstNode = new chainNode<T>(theElement, /* next */ firstNode);
    else
    {
        chainNode<T>* p = firstNode;
        for (int i = 0; i < theIndex-1; i++)
            p = p->next;                                                    /* now p point to the node in front of the target    */
        p->next = new chainNode<T>(theElement, p->next);                    /* use theElement and next pointer constor a node    */
    }
    listSize++;
}

/***************************************************************************
* Name          : output
* Descirpyion   : circle to output the chain list
* Input         : 1.out : object of ostream to output
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::output(ostream &out) const
{
    for (chainNode<T>* currentNode  = firstNode;
                       currentNode != NULL;
                       currentNode  = currentNode->next)
        out << currentNode->element << "  ";
}

/***************************************************************************
* Name          : operator <<
* Descirpyion   : repeated load <<
* Input         : 1.out : object of ostream to output
*               : 2.x   : class which can call '<<'
* Output        : 1.out : for continuous call
***************************************************************************/
template <class T>
ostream& operator << (ostream& out, const chain<T>& x)
{
    x.output(out);
    return out;
}

template <class T>
class iterator
{
public:
    /************* type define ******************************************/
    typedef bidirectional_iterator_tag  iter_category;
    typedef ptrdiff_t                   difference_type;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;

    /*************** functions  ****************************************/
    iterator(chainNode<T>* theNode = NULL) {node = theNode;}
    
    T& operator * () const {return  node->element;}                     /* repeated load * and  -> for iterator                  */
    T* operator-> () const {return &node->element;}
    
    iterator& operator ++ () {node = node->next; return *this;}         /* repeated load forward self increase (++)              */
    iterator  operator ++ (int)                                         /* repeated load after the self increase (++)            */
    {
        iterator old = *this;
        node = node->next;
        return old;
    }
    
    bool operator != (const iterator right) const
    {
        return node != right.node;
    }
    bool operator == (const iterator right) const
    {
        return node == right.node;
    }
    
protected:
    chainNode<T>* node;
     
};
#endif // CHAIN_H
