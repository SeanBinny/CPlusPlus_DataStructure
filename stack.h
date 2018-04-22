#ifndef STACK_H
#define STACK_H
#include "arraylist.h"
#include "chain.h"

template <class T>
class stack
{
public:
    virtual ~stack();
    virtual  bool empty() const = 0;
    virtual  int  size() const = 0;

    virtual  T&   top() = 0;
    virtual  void pop() = 0;
    virtual  void push(const T& theElement) = 0;
};


template <class T>
class derivedArrayStack : public  stack<T>,
                          private arrayList<T>
{
public:
    derivedArrayStack(int initialCapity = 10)
        : arrayList(initialCapity) {}
    bool empty() const {return arrayList<T>::empty();}
    int  size() const  {return arrayList<T>::size();}

    T& top()
    {
        try
        {
            return get(arrayList<T>::size()-1);
        }
        catch (illegalParameterValue)
        {
            throw illegalParameterValue("stack is empty!");
        }
    }
    void pop()
    {
        try
        {
            erase(arrayList<T>::size()-1);
        }
        catch (illegalParameterValue)
        {
            throw illegalParameterValue("stack is empty!");
        }
    }
    void push(const T &theElement)
    {
        insert(arrayList<T>::size()), theElement;
    }
};

template <class T>
class arrayStack : public stack<T>
{
public:
    arrayStack(int initialCapacity = 10);
   ~arrayStack() {delete [] stack;}
    bool empty() const {return stackTop == -1;}
    int  size() const  {return stackTop + 1;}

    T& top()
    {
       if (stackTop == -1)
           throw illegalParameterValue("stack is empty!");
       return stack[stackTop];
    }
    void pop()
    {
        if (stackTop == -1)
            throw illegalParameterValue("stack is empty!");

        stack[stackTop--].~T();
    }
    void push(const T& theElement);
private:
    int stackTop;
    int arrayLength;
    T  *stack;
};

template<class T>
arrayStack<T>::arrayStack(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << " Tnitial capacity = " << initialCapacity <<
             " Must be > 0";
        throw illegalParameterValue(s.str());
    }
    arrayLength = initialCapacity;
    stack       = new T [arrayLength];
    stackTop    = -1;
}

template <class T>
void arrayStack<T>::push(const T &theElement)
{
    if (stackTop == arrayLength-1)
    {
        changeLength1D(stack, arrayLength, 2*arrayLength);
        arrayLength *= 2;
    }

    stack[++stackTop] = theElement;
}

template <class T>
class linkedStack : public stack<T>
{
public:
    linkedStack(int initialCapacity = 10)
    {
        stackTop  = NULL;
        stackSize = 0;
    }
   ~linkedStack();
    bool empty() const {return stackSize == 0;}
    int  size()  const {return stackSize;}

    T& top()
    {
        if (stackSize == 0)
            throw illegalParameterValue("stack is empty!");

        return stackTop->element;
    }
    void pop();
    void push(const T &theElement)
    {
        stackTop = new chainNode<T>(theElement, stackTop);
        stackSize++;
    }
private:
    chainNode<T>* stackTop;
    int stackSize;
};

template <class T>
linkedStack<T>::~linkedStack()
{
    while (stackTop != NULL)
    {
        chainNode<T>* nextNode = stackTop->next;
        delete stackTop;
        stackTop = nextNode;
    }
}

template <class T>
void linkedStack<T>::pop()
{
    if (stackSize == 0)
        throw illegalParameterValue("stack is empty!");

    chainNode<T>* nextNode = stackTop->next;
    delete stackTop;
    stackTop = nextNode;
    stackSize--;
}
#endif // STACK_H
