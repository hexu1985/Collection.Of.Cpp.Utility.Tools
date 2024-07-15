#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>

template<class T>
class SharedMemory {
    int fd;
    T* ptr;
    const char* name;

public:
    SharedMemory(const char* name, bool owner=false) {
        fd = shm_open(name, O_RDWR | O_CREAT, 0600);
        if (fd == -1) {
            throw std::runtime_error("Failed to open a shared memory region");
        }

        if (ftruncate(fd, sizeof(T)) < 0) {
            close(fd);
            throw std::runtime_error("Failed to set size of a shared memory region");
        };

        ptr = (T*) mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (!ptr) {
            close(fd);
            throw std::runtime_error("Failed to mmap a shared memory region");
        }

        this->name = owner ? name : nullptr;
    }

    ~SharedMemory() {
        munmap(ptr, sizeof(T));
        close(fd);
        if (name) {
            shm_unlink(name);
        }
    }

    T& get() const {
        return *ptr;
    }
};
