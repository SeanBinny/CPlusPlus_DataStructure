#ifndef STACK_H
#define STACK_H
#include "arraylist.h"
#include "chain.h"

/*************** ADT class stack definition *********************************/
template <class T>
class stack
{
public:
    virtual ~stack() {}
    virtual  bool empty() const = 0;
    virtual  int  size() const = 0;

    virtual  T&   top() = 0;
    virtual  void pop() = 0;
    virtual  void push(const T& theElement) = 0;
};


/************** inherit class stack******************************************/
template <class T>
class derivedArrayStack : public  stack<T>,
                          private arrayList<T>                               /* inherit by private, we can hide the fuction detail
                                                                                do not need                                      */
{
public:
    derivedArrayStack(int initialCapity = 10)
        : arrayList<T>(initialCapity) {}                                     /* arraylist is not a abstrct class                 */

    /*--------- from stack -------------------------------------------------*/
    bool empty() const {return arrayList<T>::empty();}                       /* convert private functions inherit from the class */
    int  size() const  {return arrayList<T>::size();}                        /* arraylist to public functions                    */

    T& top()                                                                 /* return  an element in top of stack               */
    {
        try                                                                  /* use try-catch to convert illegal type array empty*/
        {                                                                    /* to type stack empty                              */
            return get(arrayList<T>::size()-1);
        }
        catch (illegalParameterValue)
        {
            throw illegalParameterValue("stack is empty!");
        }
    }
    void pop()                                                               /* erase an element in top of stack                 */
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
    void push(const T &theElement)                                          /* add an element to top of stack                    */
    {
        insert(arrayList<T>::size(), theElement);
    }
};


/************** re-create class stack (type array)**************************/
template <class T>
class arrayStack : public stack<T>
{
public:
    arrayStack(int initialCapacity = 10);

    /*--------- from stack -----------------------------------------------*/
   ~arrayStack() {delete [] stack_;}
    bool empty() const {return stackTop == -1;}
    int  size() const  {return stackTop + 1;}                               /* because stack is from stack[0] to stack[stack]    */

    T& top()
    {
       if (stackTop == -1)
           throw illegalParameterValue("stack is empty!");
       return stack_[stackTop];
    }
    void pop()
    {
        if (stackTop == -1)
            throw illegalParameterValue("stack is empty!");

        stack_[stackTop--].~T();                                            /* destructor a T struct space (only for type array) */
    }
    void push(const T& theElement);
private:
    int stackTop;
    int arrayLength;
    T  *stack_;                                                             /* array to save element, add '_ ' to distinguish with
                                                                               STL stack                                         */
};

/***************************************************************************
* Name          : arrayStack
* Descirpyion   : constructor
* Input         : 1.initialCapacity : initial capacity of this stack
* Output        : none
***************************************************************************/
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
    arrayLength = initialCapacity;                                          /* constor a empty stack                             */
    stack_      = new T [arrayLength];
    stackTop    = -1;
}

/***************************************************************************
* Name          : push
* Descirpyion   : add elelment in top of stack
* Input         : 1.theElement : element you want to add
* Output        : none
***************************************************************************/
template <class T>
void arrayStack<T>::push(const T &theElement)
{
    if (stackTop == arrayLength-1)                                         /* if full, doubled it                                */
    {
        changeLength1D(stack_, arrayLength, 2*arrayLength);
        arrayLength *= 2;
    }

    stack_[++stackTop] = theElement;
}


/************** re-create class stack (type chian)************************/
template <class T>
class linkedStack : public stack<T>
{
public:
    linkedStack(int initialCapacity = 10)
    {
        if (initialCapacity < 1)
        {
            ostringstream s;
            s << " Tnitial capacity = " << initialCapacity <<
                 " Must be > 0";
            throw illegalParameterValue(s.str());
        }
        stackTop  = NULL;
        stackSize = 0;
    } 

  ~linkedStack();

    /*--------- from stack -----------------------------------------------*/
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

/***************************************************************************
* Name          : ~linkedStack
* Descirpyion   : destructor a stack chain list
* Input         : none
* Output        : none
***************************************************************************/
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

/***************************************************************************
* Name          : pop
* Descirpyion   : erase elelment in top of stack
* Input         : none
* Output        : none
***************************************************************************/
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
