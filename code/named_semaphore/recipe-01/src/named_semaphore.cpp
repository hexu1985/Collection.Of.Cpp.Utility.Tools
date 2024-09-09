#include "named_semaphore.hpp"

NamedSemaphore::NamedSemaphore() noexcept {
}

NamedSemaphore::~NamedSemaphore() {
}

NamedSemaphore::NamedSemaphore(NamedSemaphore&& other) : impl_(std::move(other.impl_)) {
}

NamedSemaphore& NamedSemaphore::operator= (NamedSemaphore&& other) {
    if (this == &other) {
        return *this;
    }

    impl_ = std::move(other.impl_);
}

NamedSemaphore::NamedSemaphore(SharedMemory<Impl>&& impl): impl_(std::move(impl)) {
}

NamedSemaphore::NamedSemaphore(SharedMemory<Impl>&& impl, unsigned int value): impl_(std::move(impl)) {
    InterprocessOnceFlag& once_flag = impl_.get().once_flag;
    InterprocessSemaphore* semaphore = &impl_.get().semaphore;
    interprocess_call_once(once_flag, [semaphore, value]() { new (semaphore) InterprocessSemaphore(value); });
}

void NamedSemaphore::post() {
    return impl_.get().semaphore.post();
}

void NamedSemaphore::wait() {
    return impl_.get().semaphore.wait();
}

bool NamedSemaphore::try_wait() {
    return impl_.get().semaphore.try_wait();
}

int NamedSemaphore::get_value() {
    return impl_.get().semaphore.get_value();
}

bool NamedSemaphore::exists(const char* name) noexcept {
    return SharedMemory<Impl>::exists(name);
}

bool NamedSemaphore::remove(const char* name) noexcept {
    return SharedMemory<Impl>::remove(name);
}

NamedSemaphore NamedSemaphore::create_only(const char* name, unsigned int value) {
    return NamedSemaphore{SharedMemory<Impl>::create_only(name), value};
}

NamedSemaphore NamedSemaphore::open_or_create(const char* name, unsigned int value) {
    return NamedSemaphore{SharedMemory<Impl>::open_or_create(name), value};
}

NamedSemaphore NamedSemaphore::open_only(const char* name) {
    return NamedSemaphore{SharedMemory<Impl>::open_read_write(name)};
}

