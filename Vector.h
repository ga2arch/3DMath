//
//  Vector.h
//  Math
//
//  Created by Gabriele on 18/10/14.
//  Copyright (c) 2014 Ga2 & co. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <iostream>
#include <cmath>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <x86intrin.h>

// Curiously Recurring Template Pattern (CRTP)
// http://kaba.hilvi.org/pastel/techniques_specialization.htm

template <std::size_t N, typename Derived>
class BaseVector {
    
public:
    Derived operator+(const Derived& d) const {
        return Derived(_mm_add_ps(data, d()));
    }
    
    Derived& operator+=(const Derived& d) {
        data_ = _mm_add_ps(data, d());
        return static_cast<Derived&>(*this);
    }
    
    Derived operator*(const float s) {
        auto tmp = _mm_set_ps1(s);
        return Derived(_mm_mul_ps(data, tmp));
    }
    
    Derived& operator*=(const float s) {
        auto tmp = _mm_set_ps(s,s,s,s);
        data_ = _mm_mul_ps(tmp, data);
        return static_cast<Derived&>(*this);
    }
    
    __m128 magnitude() const {
        // (1,2,3,4) * (1,2,3,4) = (1,4,9,16)
        auto exp = _mm_mul_ps(data, data);
        
        // (1+4, 9+16, 1+4, 9+16) = (5, 15, 5, 15)
        auto sum = _mm_hadd_ps(exp, exp);
        
        // sqrt(5+15, 5+15, 5+15, 5+15) = sqrt(20, 20, 20, 20)
        return _mm_sqrt_ps(_mm_hadd_ps(sum, sum));
    }
    
    __m128 squared_magnitude() const {
        auto exp = _mm_mul_ps(data, data);
        auto sum = _mm_hadd_ps(exp, exp);
        return _mm_hadd_ps(sum, sum);
    }
    
    const __m128& data;

protected:
    explicit BaseVector(): data(data_) {};
    explicit BaseVector(__m128 v): data_(v), data(data_) {};
    
    __m128 data_;

};

template <std::size_t N>
class Vector : public BaseVector<N, Vector<N>> {
    
private:
    typedef BaseVector<N, Vector<N>> Base;
    
public:
    explicit Vector(const __m128& v): Base(v) {}
    explicit Vector(): Base() {};

};

template <>
class Vector<1> : public BaseVector<1, Vector<1>> {
    
private:
    typedef BaseVector<1, Vector<1>> Base;
    
public:
    explicit Vector<1>(const __m128& v): Base(v) {}
    explicit Vector<1>(float x);
    explicit Vector<1>(): Base() {};
    
    Vector<1>& operator=(Vector<1> other) {
        data_ = other.data;
        return *this;
    }
    
};

template <>
class Vector<2> : public BaseVector<2, Vector<2>> {
    
private:
    typedef BaseVector<2, Vector<2>> Base;
    
public:
    explicit Vector<2>(const __m128& v): Base(v) {}
    explicit Vector<2>(float x, float y);
    explicit Vector<2>(): Base() {};

    Vector<2>& operator=(Vector<2> other) {
        data_ = other.data;
        return *this;
    }
};

template <>
class Vector<3> : public BaseVector<3, Vector<3>> {
    
private:
    typedef BaseVector<3, Vector<3>> Base;
    
public:
    explicit Vector<3>(const __m128& v): Base(v) {}
    explicit Vector<3>(float x, float y, float z);
    explicit Vector<3>(): Base() {};

    Vector<3>& operator=(Vector<3> other) {
        data_ = other.data;
        return *this;
    }
    
};

template <>
class Vector<4> : public BaseVector<4, Vector<4>> {
    
private:
    typedef BaseVector<4, Vector<4>> Base;
    
public:
    explicit Vector<4>(const __m128& v): Base(v) {}
    explicit Vector<4>(float x, float y, float z, float w);
    explicit Vector<4>(): Base() {};

    Vector<4>& operator=(Vector<4> other) {
        data_ = other.data;
        return *this;
    }
    
};

typedef Vector<1> Vec1;
typedef Vector<2> Vec2;
typedef Vector<3> Vec3;
typedef Vector<4> Vec4;

#endif