#ifndef __WINDOWED_QUEUE_H__
#define __WINDOWED_QUEUE_H__
#include <stdio.h>
#include <math.h>


class NoItemsException {};

template<typename T, size_t CAPACITY>
class WindowedQueue
{
    private:
        size_t top_, bottom_;
        T data_[CAPACITY];
    public:

        WindowedQueue()
            : top_(0)
            , bottom_(0)
        {}

        size_t capacity() const
        {
            return CAPACITY;
        }

        size_t count() const
        {
            return bottom_ >= top_ ? bottom_ - top_ : capacity() - top_ + bottom_;
        }

        bool empty() const
        {
            return bottom_ == top_;
        }

        bool full() const
        {
            return count() == capacity();
        }

        void clear()
        {
            top_ = bottom_ = 0;
        }

        void push(const T& value)
        {
            if (full()) {
                pop();
            }
            data_[bottom_++ % capacity()] = value;
        }

        template<typename Q>
        void pushQueue(const Q& queue) //todo: optimaze by memmove
        {
            for (size_t i = 0; i < queue.count(); ++i) {
                push(queue.get(i));
            }
        }

        T pop()
        {
            if (empty()) {
                throw new NoItemsException;
            }
            T value = data_[top_++ % capacity()];
            return value;
        }

        T get(size_t index) const
        {
            if (empty()) {
                throw new NoItemsException;
            }
            index = (index + top_)  % capacity();
            return data_[index];
        }

        T sum() const
        {
            size_t c = count();
            T value = 0;
            for (size_t i = 0; i < c; ++i) {
                value += get(i);
            }
            return value;
        }

        T mean() const
        {
            size_t c = count();
            if (!c) {
                throw new NoItemsException;
            }
            return sum() / c;
        }

        T deviation() const
        {
            T m = mean();
            size_t c = count();
            T carry = 0;
            for (size_t i = 0; i < c; ++i) {
                carry += pow(get(i) - m, 2);
            }
            return sqrt(carry / c);
        }
};

#endif