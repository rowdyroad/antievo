#ifndef __WINDOWED_QUEUE_H__
#define __WINDOWED_QUEUE_H__
#include <stdio.h>
#include <math.h>

template<typename T>
class WindowedQueue
{
    private:
        const size_t capacity_;
        size_t top_, bottom_;
        T* data_;
    public:

        class NoItemsException {};

        WindowedQueue(size_t capacity)
            : capacity_(capacity)
            , top_(0)
            , bottom_(0)
        {
            data_ = new T[capacity];
        }

        ~WindowedQueue()
        {
            clear();
            delete [] data_;
        }

        size_t capacity() const
        {
            return capacity_;
        }

        size_t count() const
        {
            return bottom_ >= top_ ? bottom_ - top_ : capacity_ - top_ + bottom_;
        }

        bool empty() const
        {
            return bottom_ == top_;
        }

        bool full() const
        {
            return count() == capacity_;
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
            data_[bottom_++ % capacity_] = value;
        }

        void push(const WindowedQueue<T>& queue)
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
            T value = data_[top_++ % capacity_];
            return value;
        }


        T get(size_t index) const
        {
            if (empty()) {
                throw new NoItemsException;
            }
            index = (index + top_)  % capacity_;
            return data_[index];
        }

        T sum() const
        {
            size_t c = count();
            T value = 0;
            for (size_t i = 0; i < c; ++i) {
                printf("%lu - %f\n",i ,get(i));
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