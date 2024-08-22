#pragma once

template<class T, size_t N>
class RingBuffer {
private:
    T objects[N];
    size_t read;
    size_t write;
    size_t queued;

public:
    RingBuffer(): read(0), write(0), queued(0) {}

    void push(const T& object) {
        objects[write] = object;
        write = (write + 1) % N;
        queued++;
        if (queued > N) {
            queued = N;
            read = write;
        }
    }

    void pop(T& object) {
        if (is_empty()) {
            throw std::runtime_error("No data in the ring buffer");
        }
        object = objects[read];
        read = (read + 1) % N;
        queued--;
    }

    bool is_empty() {
        return queued == 0;
    }

    bool is_full() {
        return queued == N;
    }

    size_t size() {
        return queued;
    }

    bool has_data() {
        return queued != 0;
    }
};
