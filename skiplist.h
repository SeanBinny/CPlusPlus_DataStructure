#ifndef SKIPLIST_H
#define SKIPLIST_H
#include "dictionary.h"
#include "illegalparametervalue.h"
#include <sstream>
#include <math.h>

/*-------------------------------------------------------------------------*
* Struct  Name  : skipNode
* Descirpyion   : node for skip list
* Data    Field : element
* Pointer Field : next : a array to save n-level pointer of this node
*--------------------------------------------------------------------------*/
template <class K, class E>
struct skipNode
{
    Pair<const K, E> element;
    skipNode<K, E>** next;

    skipNode(const pair<const K, E>& thePair, int size)                    /* size is the level it belongs                      */
        : element(thePair)
    {
        next = new skipNode<K, E>* [size];                                 /* define it has n-level pointers                    */
    }
};

/*************** Class definition *****************************************/
template <class K, class E>
class skipList : public dictionary<K, E>
{
public:
    skipList(K, int maxPairs = 10000, float prob = 0.5);
   ~skipList();

    /*--------- from dictionary ------------------------------------------*/
    bool empty() const;
    int  size() const ;
    Pair<const K, E>* find(const K&) const ;
    void erase(const K&) ;

    void insert(const pair<const K, E>&) ;

protected:
    int   levels;                                                           /* max current nonempty chain                       */
    int   dictionarySize;                                                   /* the number of pairs in the dictionary            */
    int   maxLevel;                                                         /* the max level permitted in this skip list        */
    int   level() const;                                                    /* generate a random level number                   */
    float cutOff;                                                           /* used to decide level number                      */
    K     tailKey;                                                          /* the max keywords                                 */
    skipNode<K, E>*  headerNode;                                            /* header node pointer                              */
    skipNode<K, E>*  tailNode;                                              /* tail node pointer                                */
    skipNode<K, E>** last;                                                  /* last[i] shows the last position on i level before
                                                                               the element you want                             */
    skipNode<K, E>*  search(const K&) const;
};

/***************************************************************************
* Name          : skipList
* Descirpyion   : constructor
* Input         : 1.largerKey : the max keywords
*               : 2.maxPairs  : the max number of pairs
*               : 3.prob      : 0 < prob < 1
* Output        : none
***************************************************************************/
template <class K, class E>
skipList<K, E>::skipList(K largerKey, int maxPairs, float prob)
{
    cutOff         = prob * RAND_MAX;                                      /* cut off the max boundary of rand number           */
    maxLevel       = (int)ceil(logf((float)maxPairs) / logf(1/prob)) - 1;  /* calculate the max level                           */
    levels         = 0;                                                    /* the current biggist level used is 0               */
    dictionarySize = 0;
    tailKey        = largerKey;

    pair<K, E> tailPair;
    tailPair.keywords = tailKey;                                           /* use the max keywords to define a tail pair        */
    headerNode        = new skipNode<K, E>(tailPair, maxLevel+1);          /* create head pointers at all levels                */
    tailNode          = new skipNode<K, E>(tailPair, 0);                   /* created a universal tail node for all level       */
    last              = new skipNode<K, E> *[maxLevel+1];

    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
}

/***************************************************************************
* Name          : ~chain
* Descirpyion   : destructor headerNode, tailNode and array last
* Input         : none
* Output        : none
***************************************************************************/
template <class K, class E>
skipList<K, E>::~skipList()
{
    skipNode<K,E> *nextNode;
    while (headerNode != tailNode)
    {
        nextNode = headerNode->next[0];                                     /* delete all nodes by following level 0 chain     */
        delete headerNode;
        headerNode = nextNode;
    }

    delete tailNode;
    delete [] last;
}

/***************************************************************************
* Name          : find
* Descirpyion   : find element whos keywords is theKey
* Input         : 1.theKey : keywords of element you want
* Output        : 1.element  : element found   2.NULL : element not found
***************************************************************************/
template <class K, class E>
Pair<const K, E>* skipList<K, E>::find(const K& theKey) const
{
    if (theKey >= tailKey)                                                 /* if keywords out of range                          */
        return NULL;

    skipNode<K, E>* beforeNode = headerNode;                               /* use beforeNode to locate a position before theKey */
    for (int i = levels; i >= 0; i--)                                      /* judging from the highest level in turn            */
    {
        while (beforeNode->next[i]->element.keywords < theKey)             /* judging if can use current level                  */
            beforeNode = beforeNode->next[i];                              /* circle to  seek keywords on leve i                */
    }

    if (beforeNode->next[0]->element.keywords == theKey)                   /* if found the element whos keywords is theKey      */
        return &beforeNode->next[0]->element;

    return NULL;                                                           /* if not found the element                          */
}

/***************************************************************************
* Name          : level
* Descirpyion   : get a rand level number
* Input         : none
* Output        : a rand level number
***************************************************************************/
template <class K, class E>
int skipList<K, E>::level() const
{
    int lev = 0;
    while (rand() <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}

/***************************************************************************
* Name          : search
* Descirpyion   : match the positions of theKey at all levels and save it in
*               : array last
* Input         : 1.theKey : keywords of element you want
* Output        :
***************************************************************************/
template <class K, class E>
skipNode<K, E>* skipList<K, E>::search(const K& theKey) const
{
    skipNode<K, E>* beforeNode = headerNode;                               /* use beforeNode to locate a position before theKey */
    for (int i = levels; i >= 0; i--)
    {
        while (beforeNode->next[i]->element.keywords < theKey)
            beforeNode = beforeNode->next[i];

        last[i] = beforeNode;                                              /* save the last node matched on level i             */
    }
    return beforeNode->next[0];                                            /* return the position of element on level 0         */
}

/***************************************************************************
* Name          : insert
* Descirpyion   : insert a data pair in the skip list
* Input         : 1.thePair   : the pair you want to insert
* Output        : none
***************************************************************************/
template <class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair)
{
    if (thePair.keywords >= tailKey)                                       /* if the keywords id out of range                   */
    {
        ostringstream s;
        s << "Key = " << thePair.keywords << " Must be < "
          << tailKey;
        throw illegalParameterValue(s.str());
    }

    skipNode<K, E>* theNode = search(thePair.keywords);                    /* find the position where it belongs on level 0     */
    if (theNode->element.keywords == thePair.keywords)                     /* case for keywords existed, modify it directly     */
    {
        theNode->element.value = thePair.value;
        return;
    }

    /*--- distribution which level it belongs ---------------------------*/
    int theLevel = level();
    if (theLevel > levels)                                                /* if new level is bigger than the current max level  */
    {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel+1);    /* insert it on level i                               */
    for (int i = 0; i <= theLevel; i++)
    {
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    dictionarySize++;
    return;
}

/***************************************************************************
* Name          : erase
* Descirpyion   : delete an pair whos keywords is theKey
* Input         : 1.theKey : keywords of element you want to delete
* Output        : none
***************************************************************************/
template <class K, class E>
void skipList<K, E>::erase(const K& theKey)
{
    if (theKey >= tailKey)
        return;

    skipNode<K, E>* theNode = search(theKey);
    if (theNode->element.keywords != theKey)
        return;
    for (int i = 0; i <= levels && last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];                                /* vacate the node you want to delete and modify the
                                                                               pointer on level i                               */
    while (levels > 0 && headerNode->next[levels] == tailNode)              /* update levels of skiplist, if a level is missing */
        levels--;

    delete theNode;
    dictionarySize--;
}
#endif // SKIPLIST_H
