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
    chainNode(const T& element) {this->element = element;}                  /* initialize the data                              */
    chainNode(const T& element, chainNode<T>* next)                         /* initialize the data and pointer                  */
    {
        this->element = element;
        this->next    = next;
    }
};


/*************** Class definition ******************************************/
template <class T>
class chain : public linerList<T>, public  extendedLinearlist<T>             /* inherit extendedLinearlist to get extend method  */
{
public:
    chain(int initialCapacity = 10);                                         /* initialCapacity just for compatible with arraylist*/
    chain(const chain<T>&);
    ~chain();

    /************ ADT method: **********************************************/
    /*----------- from linearlist------------------------------------------*/
    T&   get(int theIndex) const;
    bool empty() const {return listSize == 0;}
    int  size() const {return listSize;}
    int  indexOf(const T& theElement) const;

    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;

    /*----------- from entendedLinearlist----------------------------------*/
    void clear();
    void push_back(const T& theElement);

    /************ Class custom: ********************************************/
    class chainlist_iterator;                                               /* iterator is only a pointer for this structure     */
    chainlist_iterator begin() {return chainlist_iterator(firstNode);}      /* use pointer of this list to construct a iterator  */
    chainlist_iterator end()   {return chainlist_iterator(NULL);}
    
protected:
    void checkIndex(int theIndex) const;                                    /* if theIndex is invalid, throw illegal             */
    chainNode<T>* firstNode;                                                /* point to the first node of linked list
                                                                               fistNode = NULL means linked list is empty        */
    chainNode<T>* lastNode;                                                 /* point to the last node of linked list
                                                                               fistNode = NULL means linked list is empty        */
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
    firstNode = lastNode  = NULL;
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
        firstNode = lastNode  = NULL;
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
    lastNode         = targetNode;
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
    lastNode = NULL;
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
        firstNode  = firstNode->next;                                       /* connect target's next with firstNode              */
    }
    else                                                                    /* delete the inside nodes                           */
    {
        chainNode<T>* p = firstNode;                                        /* use p to pick up the firstNode                    */
        for (int i = 0; i < theIndex-1; i++)
            p = p->next;                                                    /* now p point to the node in front of the target    */
        
        deleteNode = p->next;                                               /* now p->next is the target to delete               */
        p->next    = p->next->next;                                         /* connect target's next with target's last          */
        if (deleteNode == lastNode)
            lastNode = p;                                                   /* update lastNode                                   */
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
    {
        firstNode = new chainNode<T>(theElement, /* next */ firstNode);
        if (listSize == 0)
            lastNode = firstNode;
    }
    else
    {
        chainNode<T>* p = firstNode;
        for (int i = 0; i < theIndex-1; i++)
            p = p->next;                                                    /* now p point to the node in front of the target    */
        p->next = new chainNode<T>(theElement, p->next);                    /* use theElement and next pointer constor a node    */
        if (listSize == theIndex)
            lastNode = p->next;
    }
    listSize++;
}

/***************************************************************************
* Name          : output
* Descirpyion   : put the list into the stream out.
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

/***************************************************************************
* Name          : clear
* Descirpyion   : delete all elements
* Input         : none
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::clear()
{
    while (firstNode != NULL)
    {
        chainNode<T>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
    listSize = 0;
}

/***************************************************************************
* Name          : push_back
* Descirpyion   : insert theElemnt at end of list
* Input         : 1.theElement : element you want to insert at end of list
* Output        : none
***************************************************************************/
template <class T>
void chain<T>::push_back(const T &theElement)
{
    chainNode<T>* newNode = new chainNode<T>(theElement, NULL);            /* use theElement to construct the last node          */
    if (firstNode == NULL)
        firstNode = lastNode = newNode;
    else
    {
        lastNode->next = newNode;
        lastNode       = newNode;                                          /* update the last node                               */
    }
    listSize++;
}

template <class T>
class chainlist_iterator
{
public:
    /************* type define ******************************************/
    typedef bidirectional_iterator_tag  iter_category;
    typedef ptrdiff_t                   difference_type;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;

    /*************** functions  ****************************************/
    chainlist_iterator(chainNode<T>* theNode = NULL) {node = theNode;}  /* construct a iterator point to theNode                 */
    
    T& operator * () const {return  node->element;}                     /* repeated load * and  -> for iterator                  */
    T* operator-> () const {return &node->element;}
    
    chainlist_iterator& operator ++ (){node = node->next; return *this;}/* repeated load forward self increase (++)              */
    chainlist_iterator  operator ++ (int)                               /* repeated load after the self increase (++)            */
    {
        chainlist_iterator old = *this;
        node = node->next;
        return old;
    }
    
    bool operator != (const chainlist_iterator right) const
    {
        return node != right.node;
    }
    bool operator == (const chainlist_iterator right) const
    {
        return node == right.node;
    }
    
protected:
    chainNode<T>* node;
     
};
#endif // CHAIN_H
