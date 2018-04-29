#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
using namespace std;

template <class K, class E>
struct pair
{
    K keywords;
    E value;
};

template <class K, class E>
class dictionary
{
public :
    virtual ~dictionary() {}

    virtual  bool empty() const = 0;
    virtual  int  size() const = 0;
    virtual  pair<const K, E>* find(const K&) const = 0;
    virtual  void erase(const K&) const = 0;

    virtual  void insert(const pair<const K, E>&) = 0;
};

template <class K, class E>
struct pairNode
{
    typedef pair<const K, E> pairType;

    pairType         element;
    pairNode<K, E>*  next;

    pairNode(const pairType& thePair)
        : element(thePair) {}
    pairNode(const pairType& theElement, pairNode<K, E>* theNext)
        : element(theElement), next(theNext) {}
};

template <class K, class E>
class sortedDicChain : public dictionary<K, E>
{
public:
    sortedDicChain() {dictionarySize = 0; firstNode = NULL;}
   ~sortedDicChain();

    bool empty() const {return dictionarySize == 0;}
    int  size() const {return dictionarySize;}
    void erase(const K &) const;
    pair<const K, E>* find(const K &) const;

    void insert(const pair<const K, E> &);

protected:
    pairNode<K, E>* firstNode;
    int             dictionarySize;
};

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

template <class K, class E>
bool sortedDicChain<K, E>::erase(const K& theKey) const
{
    pairNode<K, E>* p  = firstNode,
                  * tp = NULL;

    while (p != NULL && p->element.keywords < theKey)
    {
        tp = p;
        p  = p->next;
    }

    if (p != NULL && p->element.keywords == theKey)
    {
        if (tp == NULL)
            firstNode = p->next;
        else
            tp->next  = p->next;

        delete p;
        dictionarySize--;
    }
}

template <class K, class E>
void sortedDicChain<K, E>::insert(const pair<const K, E>& thePair)
{
    pairNode<K, E>* p  = firstNode,
                  * tp = NULL;

    while (p != NULL && p->element.keywords < thePair.keywords)
    {
        tp = p;
        p  = p->next;
    }

    if (p != NULL && p->element.keywords == thePair.keywords)
    {
        p->element.value = thePair.value;
        return;
    }

    pairNode<K, E> *newNode = new pairNode<K, E>(thePair, p);

    if (tp == NULL)
        firstNode = p->next;
    else
        tp->next  = p->next;

    delete p;
    dictionarySize--;
}

template <class K, class E>
pair<const K, E>* sortedDicChain<K, E>::find(const K& theKey) const
{
    pairNode<K, E>* currentNode = firstNode;

    while (currentNode != NULL &&
           currentNode->element.keywords != theKey)
        currentNode = currentNode->next;

    if (currentNode != NULL && currentNode->element.keywords == theKey)
        return &currentNode->element;

    return NULL;
}
#endif // DICTIONARY_H


