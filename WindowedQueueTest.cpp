#include <assert.h>
#include <stdio.h>

#include "WindowedQueue.h"

int main()
{
    WindowedQueue<float> q(100);
    assert(q.capacity() == 100);
    assert(q.count() == 0);

    try {
        q.pop();
        assert(false);
    } catch (NoItemsException *e) { }

    try {
        q.mean();
        assert(false);
    } catch (NoItemsException *e) { }

    try {
        q.deviation();
        assert(false);
    } catch (NoItemsException *e) { }

    for (size_t i = 0; i < 10; ++i) {
        q.push(1.0);
    }
    assert(q.count() == 10);
    assert(q.sum() == 10.0);
    assert(q.mean() == 1.0);
    assert(q.deviation() == 0.0);
    for (size_t i = 0; i < 10; ++i) {
        q.push(1.0);
    }
    assert(q.count() == 20);
    assert(q.sum() == 20.0);
    assert(q.mean() == 1.0);
    assert(q.deviation() == 0.0);
    for (size_t i = 0; i < 100; ++i) {
        q.push(1.0);
    }
    assert(q.count() == 100);
    assert(q.sum() == 100.0);
    assert(q.mean() == 1.0);
    for (size_t i = 0; i < 50; ++i) {
        q.push(2.0);
    }
    assert(q.count() == 100);
    assert(q.sum() == 150.0);
    assert(q.mean() == 1.5);

    while (q.count()) {
        q.pop();
    }
    assert(q.count() == 0);
    assert(q.sum() == 0.0);

    q.push(1.0);
    q.push(2.0);
    q.push(3.0);

    assert(q.count() == 3);
    assert(q.mean() == 2.0);
    assert(q.pop() == 1.0);
    assert(q.count() == 2);
    assert(q.pop() == 2.0);
    assert(q.count() == 1);
    assert(q.pop() == 3.0);
    assert(q.count() == 0);

    for (size_t i = 0; i < 100; ++i) {
        q.push((float)i);
    }

    assert(q.mean() == 49.5);

    WindowedQueue<float> a(200);
    a.push(q);
    assert(a.count() == 100);
    assert(a.mean() == 49.5);
    a.push(q);
    assert(a.count() == 200);
    assert(a.mean() == 49.5);
    for (size_t i = 0; i < 200; ++i) {
        a.push(1.0);
    }
    assert(a.count() == 200);
    assert(a.mean() == 1);

    WindowedQueue<long long> c(1000000);
    for (size_t i = 0; i < c.capacity(); ++i) {
        c.push(i);
    }
    c.clear();
    return 0;
}