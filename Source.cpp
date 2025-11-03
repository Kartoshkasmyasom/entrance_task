#include <cassert>
#include <filesystem>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T> class Vector {
public:
    Vector() : size_(0), capacity_(1), buf_(new T[1]) {}

    Vector(const std::initializer_list<T>& L) {
        capacity_ = 1;
        while (capacity_ < L.size())
            capacity_ <<= 1;

        buf_ = new T[capacity_];
        for (const T& el : L) {
            PushBack(el);
        }
    }
    Vector(size_t size) : size_(size) {
        capacity_ = 1;
        while (capacity_ < size)
            capacity_ <<= 1;
        buf_ = new T[capacity_];
    }

    Vector(const Vector& other) {
        buf_ = new T[other.capacity_];
        size_ = other.size_;
        capacity_ = other.capacity_;

        for (size_t i = 0; i < other.size_; ++i) {
            buf_[i] = other.buf_[i];
        }
    }

    Vector(Vector&& other) noexcept {
        if (this == &other) {
            return;
        }
        buf_ = other.buf_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.buf_ = nullptr;
        other.Clear();
    }

    Vector operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        if (capacity_ >= other.capacity_) {
            size_ = other.size_;
            for (size_t i = 0; i < other.size_; ++i) {
                buf_[i] = other.buf_[i];
            }
            return *this;
        }
        Clear();
        buf_ = new T[other.capacity_];
        size_ = other.size_;
        capacity_ = other.capacity_;

        for (size_t i = 0; i < other.size_; ++i) {
            buf_[i] = other.buf_[i];
        }
        return *this;
    }

    Vector operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Clear();
        buf_ = other.buf_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.buf_ = nullptr;
        other.Clear();
        return *this;
    }

    size_t Size() const { return size_; }

    bool Empty() const { return size_ == 0; }

    void PopBack() {
        if (Empty())
            throw std::runtime_error("Vector::PopBack: empty vector");
        --size_;
    }

    void PushBack(const T& val) {
        if (capacity_ == 0) {
            capacity_ = 1;
            buf_ = new T[capacity_];
        }
        if (size_ == capacity_) {
            Realloc();
        }
        buf_[size_++] = val;
    }

    const T& operator[](size_t i) const {
        if (i >= size_)
            throw std::runtime_error("Vector::operator[]: index out of range");
        return buf_[i];
    }

    T& operator[](size_t i) {
        if (i >= size_)
            throw std::runtime_error("Vector::operator[]: index out of range");
        return buf_[i];
    }

    const T& Front() const {
        if (Empty())
            throw std::runtime_error("Vector::Front: empty vector");
        return buf_[0];
    }

    const T& Back() const {
        if (Empty())
            throw std::runtime_error("Vector::Back: empty vector");
        return buf_[size_ - 1];
    }

    bool operator==(const Vector& other) {
        if (size_ != other.size_)
            return false;
        for (size_t i = 0; i < size_; ++i) {
            if (buf_[i] != other.buf_[i])
                return false;
        }
        return true;
    }

    ~Vector() { Clear(); }

    void Clear() noexcept {
        capacity_ = 0;
        size_ = 0;
        delete[] buf_;
        buf_ = nullptr;
    }

private:
    void Realloc() {
        capacity_ *= 2;
        T* new_buf_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            new_buf_[i] = buf_[i];
        }
        delete[] buf_;
        buf_ = new_buf_;
    }
    size_t size_ = 0;
    size_t capacity_ = 0;
    T* buf_ = nullptr;
};
