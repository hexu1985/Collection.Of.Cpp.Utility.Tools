#pragma once

#include <stdexcept>

template<class T, size_t N>
class ObjectPool {
private:
    T objects[N];
    size_t available[N];
    size_t top = 0;
public:
    ObjectPool(): top(0) {
        for (size_t i = 0; i < N; i++) {
            available[i] = i;
        }
    }

    T& get() {
        if (top < N) {
            size_t idx = available[top++];
            return objects[idx];
        } else {
            throw std::runtime_error("All objects are in use");
        }
    }

    void free(const T& obj) {
        const T* ptr = &obj;
        size_t idx = (ptr - objects) / sizeof(T);
        if (idx < N) {
            if (top) {
                objects[idx].deinit();
                top--;
                available[top] = idx;
            } else {
                throw std::runtime_error("Some object was freed more than once");
            }
        } else {
            throw std::runtime_error("Freeing object that does not belong to the pool");
        }
    }
    size_t requested() const { return top; }
};
