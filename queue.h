#ifndef QUEUE_H
#define QUEUE_H
#include "illegalparametervalue.h"
#include "stack.h"
#include <algorithm>
#include <sstream>

/*************** ADT class stack definition *******************************/
template <class T>
class queue
{
public:
    virtual ~queue() {}
    virtual  bool empty() const = 0;
    virtual  int  size()  const = 0;

    virtual  T&   front() = 0;                                             /* just return the element in top of queue           */
    virtual  T&   back()  = 0;                                             /* just return the element in back of queue          */
    virtual  void pop()   = 0;
    virtual  void push(const T& theElement) = 0;
};

/************** inherit class stack ***************************************/
template <class T>
class arrayQueue : public queue                                            /* this class queue is definited for circle queue    */
{
public:
    arrayQueue(int initialCapacity = 10) ;
   ~arrayQueue() {delete [] queue_;}

    bool empty() const {return queueFront == queueBack;}
    int  size()  const
    {
        return (queueBack-queueFront+arrayLength) % arrayLength;           /* same as Lengrh - (thefront - theback)             */
    }

    T& front()                                                             /* just return the element in top of queue           */
    {
        if (empty())
            throw illegalParameterValue("queue is empty!");

        return queue_[(queueFront+1) % arrayLength];                       /* % arrayLength can limit the index in a circle     */
    }

    T& back()                                                              /* just return the element in top of queue           */
    {
        if (empty())
            throw illegalParameterValue("queue is empty!");
        return queue_[queueBack];
    }
    void pop()
    {
        if (empty())
            throw illegalParameterValue("queue is empty!");

        queueFront = (queueFront + 1) % arrayLength;                       /* for queurFront's feature, can add Before assign   */
        queue_[queueFront].~T();
        arrayLength--;
    }
private:
    int arrayLength,
        queueFront,                                                        /* queueFront point to the position in front of the
                                                                              first element                                     */
        queueBack;                                                         /* queueBack  point to the position where saves the
                                                                              last  elemrnt                                     */
    T*  queue_;

    void changeQueueLength();                                              /* to extended the queue size                        */

};

/***************************************************************************
* Name          : arrayQueue
* Descirpyion   : constructor
* Input         : 1.initialCapacity : initial capacity of this array
* Output        : none
***************************************************************************/
template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity)
{
   if (initialCapacity < 1)
   {
       ostringstream s;
       s << "Initial capacity = " << initialCapacity << " Must be > 0";
       throw illegalParameterValue(s.str());
   }

   arrayLength = initialCapacity;
   queue_      = new T[arrayLength];
   queueFront  = 0;
   queueBack   = 0;
}

/***************************************************************************
* Name          : changeQueueLength
* Descirpyion   : sblit the circle queue and extend it
* Input         : none
* Output        : none
***************************************************************************/
template <class T>
void arrayQueue<T>::changeQueueLength()
{
    T* newQueue = new T[2*arrayLength];

    int start = (queueFront+1) % arrayLength;                              /* locate to the first element                       */
    if (start < 2)                                                         /* start = 0 ,1 means There is no empty position in  */
        copy(queue_+start, queue_+start+arrayLength-1, newQueue);          /* the middle of the queue, eg: ABCDEFG_             */
    else
    {                                                                      /* eg:  CDEFG_AB -> entend                           */
        copy(queue_+start, queue_+start+arrayLength,   newQueue);          /* CDEFG_AB -> AB_________________                   */
        copy(queue_,       queue_+queueBack+1,         newQueue+arrayLength-start);
    }                                                                      /* CDEFG_AB + AB___________  -> ABCDEFG_____________ */

    queueFront  = 2*arrayLength-1;                                         /* pointer front located in back to point to A[0]    */
    queueBack   =   arrayLength-2;                                         /* because A[0] and [queueFront] ,so need -2         */
    arrayLength *= 2;
    delete [] queue_;
    queue_      = newQueue;
                                                                           /* now newQueue and queue_ point to a same memory ,so
                                                                              we can't use delete newQueue or delete [] newQueue*/
}

/***************************************************************************
* Name          : push
* Descirpyion   : add an element in the tail of queue
* Input         : 1.theElement  : element you want to add
* Output        : none
***************************************************************************/
template <class T>
void arrayQueue<T>::push(const T &theElement)
{
    if ((queueBack+1) % arrayLength == queueFront)
    {
        changeQueueLength();
    }
    queueBack         = (queueBack+1) % arrayLength;
    queue_[queueBack] = theElement;
}

#endif // QUEUE_H
