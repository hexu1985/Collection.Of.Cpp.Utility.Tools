#pragma once

template <typename T, size_t N>
class RingBuffer {
private:
    T objects[N];
    size_t read;
    size_t write;
    size_t queued;

public:
    RingBuffer(): read(0), write(0), queued(0) {}

    T& push() {
        T& current = objects[write];
        write = (write + 1) % N;
        queued++;
        if (queued > N) {
            queued = N;
            read = write;
        }
        return current;
    }

    const T& pull() {
        if (!queued) {
            throw std::runtime_error("No data in the ring buffer");
        }
        T& current = objects[read];
        read = (read + 1) % N;
        queued--;
        return current;
    }

    bool has_data() {
        return queued != 0;
    }
};
