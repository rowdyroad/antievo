#ifndef __WINDOWED_QUEUE_H__
#define __WINDOWED_QUEUE_H__
#include <stdio.h>
#include <math.h>

template<typename T>
class WindowedQueue
{
    private:
        const size_t capacity_;
        size_t count_;
        typedef struct node {
            T item;
            node* next;
            node(const T& item)
                : item(item)
                , next(NULL)
            {}

        } node;

        node* head_;
        node* tail_;
    public:

        class NoItemsException
        {

        };

        WindowedQueue(size_t capacity)
            : capacity_(capacity)
            , count_(0)
            , head_(NULL)
            , tail_(NULL)
        {}

        ~WindowedQueue()
        {
            clear();
        }

        size_t capacity() const
        {
            return capacity_;
        }

        size_t count() const
        {
            return count_;
        }

        bool empty() const
        {
            return count_ == 0;
        }

        bool full() const
        {
            return count_ == capacity_;
        }

        void clear()
        {
            node* t = head_;
            while (t) {
                node* d = t;
                t  = t->next;
                delete d;
            }
            head_ = NULL;
            count_ = 0;
        }

        size_t push(const T& value)
        {
            node* n = new node(value);
            if (tail_) {
                tail_->next = n;
            }
            tail_ = n;
            if (!head_) {
                head_ = tail_;
            }
            if (++count_ > capacity_) {
                node* old_head = head_;
                head_ = head_->next;
                delete old_head;
                --count_;
            };

            return count_;
        }


        size_t push(const WindowedQueue<T>& queue)
        {
            node* t = queue.head_;
            while (t) {
                push(t->item);
                t = t->next;
            }
            return count_;
        }

        T pop()
        {
            if (!count_) {
                throw new NoItemsException;
            }
            T value = head_->item;
            node* old_head = head_;
            head_ = head_->next;
            delete old_head;
            if (!--count_) {
                tail_ = head_;
            }
            return value;
        }

        T sum() const
        {
            T value = 0;
            node* t = head_;
            while (t) {
                value += t->item;
                t = t->next;
            }
            return value;
        }

        T mean() const
        {
            if (!count_) {
                throw new NoItemsException;
            }
            return sum() / count_;
        }

        T deviation() const
        {
            T m = mean();
            T carry = 0;
            node* t = head_;
            while (t) {
                carry += pow(t->item - m,2);
                t = t->next;
            }

            return sqrt(carry / count_);
        }
};

#endif