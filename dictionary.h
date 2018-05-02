#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
using namespace std;

/*-------------------------------------------------------------------------*
* Struct  Name  : pair
* Descirpyion   : data pair for dictionary
* Data    Field : 1.keywords     2.value
*--------------------------------------------------------------------------*/
template <class K, class E>
struct Pair
{
    K keywords;
    E value;
};

/*************** ADT class dictionary definition ***************************/
template <class K, class E>
class dictionary
{
public :
    virtual ~dictionary() {}

    virtual  bool empty() const = 0;
    virtual  int  size() const = 0;
    virtual  Pair<const K, E>* find(const K&) const = 0;
    virtual  void erase(const K&) const = 0;

    virtual  void insert(const Pair<const K, E>&) = 0;
};

/*-------------------------------------------------------------------------*
* Struct  Name  : pairNode
* Descirpyion   : node for pairs of dictionary
* Data    Field : element: belong to struct pair
* Pointer Field : next   : point to the next node
*--------------------------------------------------------------------------*/
template <class K, class E>
struct pairNode
{
    typedef Pair<const K, E> pairType;

    pairType         element;
    pairNode<K, E>*  next;

    pairNode(const pairType& thePair)
        : element(thePair) {}
    pairNode(const pairType& theElement, pairNode<K, E>* theNext)
        : element(theElement), next(theNext) {}
};

/*************** Class definition ******************************************/
template <class K, class E>
class sortedDicChain : public dictionary<K, E>                              /* class to sort and save dictionary data in a chain*/
{
public:
    sortedDicChain() {dictionarySize = 0; firstNode = NULL;}
   ~sortedDicChain();

    /*--------- from dictionary ------------------------------------------*/
    bool empty() const {return dictionarySize == 0;}
    int  size() const {return dictionarySize;}
    Pair<const K, E>* find(const K &) const;

    void erase (const K &);
    void insert(const pair<const K, E> &);

protected:
    pairNode<K, E>* firstNode;
    int             dictionarySize;
};

/***************************************************************************
* Name          : ~sortedDicChain
* Descirpyion   : destructor a sorted dictionary chain list
* Input         : none
* Output        : none
***************************************************************************/
template <class K, class E>
sortedDicChain<K , E>::~sortedDicChain()
{
    while (firstNode != NULL)
    {
        pairNode<K, E>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

/***************************************************************************
* Name          : erase
* Descirpyion   : delete an pair whos keywords is theKey
* Input         : 1.theKey : keywords of element you want to delete
* Output        : none
***************************************************************************/
template <class K, class E>
void sortedDicChain<K, E>::erase(const K& theKey)
{
    pairNode<K, E>* p  = firstNode,
                  * tp = NULL;                                             /* tp trails pointer p                               */

    while (p != NULL && p->element.keywords < theKey)
    {
        tp = p;
        p  = p->next;                                                      /* now ,p point to the target, tp point to last pair */
    }

    if (p != NULL && p->element.keywords == theKey)                        /* confirm p point to the target                     */
    {
        if (tp == NULL)                                                    /* delete the first pair                             */
            firstNode = p->next;
        else
            tp->next  = p->next;                                           /* now pointer tp pick up the next element           */

        delete p;
        dictionarySize--;
    }
}

/***************************************************************************
* Name          : insert
* Descirpyion   : insert a data pair
* Input         : 1.thePair   : the pair you want to insert
* Output        : none
***************************************************************************/
template <class K, class E>
void sortedDicChain<K, E>::insert(const pair<const K, E>& thePair)
{
    pairNode<K, E>* p  = firstNode,
                  * tp = NULL;                                             /* tp trails pointer p                               */

    while (p != NULL && p->element.keywords < thePair.keywords)
    {
        tp = p;
        p  = p->next;                                                      /* now ,p point to the target, tp point to last pair */
    }

    if (p != NULL && p->element.keywords == thePair.keywords)              /* case for keywords existed, modify it directly     */
    {
        p->element.value = thePair.value;
        return;
    }

    pairNode<K, E> *newNode = new pairNode<K, E>(thePair, p);              /* use thePair and p to consructor a node to insert  */
    if (tp == NULL)                                                        /* insert the head node                              */
        firstNode = newNode;
    else                                                                   /* insert the normal nodes                           */
        tp->next  = newNode;

    dictionarySize++;
}

/***************************************************************************
* Name          : find
* Descirpyion   : find the pair whos keywords is theKey
* Input         : 1.theKey   : keywords of the pair
* Output        : none
***************************************************************************/
template <class K, class E>
Pair<const K, E>* sortedDicChain<K, E>::find(const K& theKey) const
{
    pairNode<K, E>* currentNode = firstNode;

    while (currentNode != NULL &&
           currentNode->element.keywords != theKey)
        currentNode = currentNode->next;

    if (currentNode != NULL && currentNode->element.keywords == theKey)
        return &currentNode->element;                                     /* element has been found                             */

    return NULL;                                                          /* element did not find                               */
}
#endif // DICTIONARY_H


