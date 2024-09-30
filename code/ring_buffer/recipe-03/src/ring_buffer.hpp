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

    void put(const T& value) {
        objects[write] = value;
        write = (write + 1) % N;
        queued++;
        if (queued > N) {
            queued = N;
            read = write;
        }
    }

    void get(T& value) {
        if (!queued) {
            throw std::runtime_error("No data in the ring buffer");
        }
        value = objects[read];
        read = (read + 1) % N;
        queued--;
    }

    bool has_data() {
        return queued != 0;
    }
};
