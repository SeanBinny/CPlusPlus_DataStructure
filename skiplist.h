#ifndef SKIPLIST_H
#define SKIPLIST_H
#include "dictionary.h"
#include "illegalparametervalue.h"
#include <sstream>

template <class K, class E>
struct skipNode
{
    pair<const K, E> element;
    skipNode<K, E>** next;

    skipNode(const pair<const K, E>& thePair, int size)
        : element(thePair)
    {
        next = new skipNode<K, E>* [size];
    }
};

template <class K, class E>
class skipList : public dictionary<K, E>
{
public:
    skipList();
   ~skipList();

    bool empty() const;
    int  size() const ;
    pair<const K, E>* find(const K&) const ;
    void erase(const K&) const ;

    void insert(const pair<const K, E>&) ;

protected:
    int   levels;
    int   dictionarySize;
    int   maxLevel;
    int   level() const;
    float cutOff;
    K     tailKey;
    skipNode<K, E>*  headerNode;
    skipNode<K, E>*  tailNode;
    skipNode<K, E>** last;

    skipNode<K, E>*  search(const K&) const;
};

template <class K, class E>
skipList<K, E>::skipList(K largerKey, int maxPairs, float prob)
{
    cutOff         = prob * RAND_MAX;
    maxLevel       = (int)ceil(logf((float)maxPairs) / logf(1/prob)) - 1;
    levels         = 0;
    dictionarySize = 0;
    tailKey        = largerKey;

    pair<K, E> tailPair;
    tailPair.keywords = tailKey;
    headerNode        = new skipNode<K, E>(tailPair, maxLevel+1);
    tailNode          = new skipNode<K, E>(tailPair, 0);
    last              = new skipNode<K, E> *[maxLevel+1];

    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
}

template <class K, class E>
skipList<K, E>::~skipList()
{

}

template <class K, class E>
pair<const K, E>* skipList<K, E>::find(const K& theKey) const
{
    if (theKey >= tailKey)
        return NULL;

    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    {
        while (beforeNode->next[i]->element.keywords < theKey)
            beforeNode = beforeNode->next[i];
    }

    if (beforeNode->next[0]->element.keywords == theKey)
        return &beforeNode->next[0]->element;

    return NULL;
}

template <class K, class E>
int skipList<K, E>::level()
{
    int lev = 0;
    while (rand() <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}

template <class K, class E>
skipNode<K, E>* skipList<K, E>::search(const K& theKey) const
{
    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    {
        while (beforeNode->next[i]->element.keywords < theKey)
            beforeNode = beforeNode->next[i];

        last[i] = beforeNode;
    }
    return beforeNode->next[0];
}

template <class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair)
{
    if (thePair.keywords >= tailKey)
    {
        ostringstream s;
        s << "Key = " << thePair.keywords << " Must be < "
          << tailKey;
        throw illegalParameterValue(s.str());
    }

    skipNode<K, E>* theNode = search(thePair.keywords);
    if (theNode->element.keywords == thePair.keywords)
    {
        theNode->element.value = thePair.value;
        return;
    }

    int theLevel = level();
    if (theLevel > levels)
    {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel+1);
    for (int i = 0; i <= theLevel; i++)
    {
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    dictionarySize++;
    return;
}

template <class K, class E>
void skipList<K, E>::erase(const K& theKey) const
{
    if (theKey >= tailKey)
        return;

    skipNode<K, E>* theNode = search(theKey);
    if (theNode->element.keywords != theKey)
        return;
    for (int i = 0; i <= levels && last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dictionarySize--;
}
#endif // SKIPLIST_H
