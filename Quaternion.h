//
//  Quaternion.h
//  Math
//
//  Created by Gabriele on 21/10/14.
//  Copyright (c) 2014 Ga2 & co. All rights reserved.
//

#ifndef Math_Quaternion_h
#define Math_Quaternion_h

#include <mmintrin.h>
#include <xmmintrin.h>
#include <x86intrin.h>

#include "Functions.h"
#include "Vector.h"
#include "SSEMathFun.h"

class Quaternion {
    
public:
    Quaternion(): data(data_) {};
    Quaternion(Vec3 v, float angle): data(data_) {
        __declspec(align(16)) float t[4] = {angle/2.0f};
        auto ___a = _mm_load_ps(t);
        
        auto aaaa = _mm_shuffle_ps(___a, ___a, _MM_SHUFFLE(0,0,0,0));
        aaaa = sin_ps(aaaa);
        
        ___a = _mm_keep_ps(cos_ps(___a), 0);
        
        const auto r = _mm_mul_ps(v.data, aaaa);
        data_ = _mm_add_ps(r, ___a);
    };
    
    Quaternion(Vec3 v, __m128 angle): data(data_) {
        const auto ___a = _mm_keep_ps(v.data, 0);
        data_ = _mm_add_ps(v.data, ___a);
    }

    Quaternion& operator*=(const Quaternion& q) {
        const auto ps = _mm_replicate_w_ps(data);
        const auto qs = _mm_replicate_w_ps(q.data);
        
        const auto s1 = _mm_mul_ps(ps, q.data);
        const auto s2 = _mm_mul_ps(qs, data);
        
        const auto pvXqv = cross(data, q.data);
        
        const auto psqs = _mm_mul_ps(ps, qs);
        const auto pvqv = dot(data, q.data);
        
        auto r = _mm_add_ps(s1, s2);
        r = _mm_reset_ps(_mm_add_ps(r, pvXqv), 0);
        
        auto a = _mm_sub_ps(psqs, pvqv);
        const auto ___a = _mm_keep_ps(a, 0);
        data_ = _mm_add_ps(r, ___a);
        return *this;
    };
    
    const __m128& data;
    
private:
    __m128 data_;
    
};

Quaternion operator*(const Quaternion& p, const Quaternion& q) {
    const auto ps = _mm_replicate_w_ps(p.data);
    const auto qs = _mm_replicate_w_ps(q.data);
    
    const auto s1 = _mm_mul_ps(ps, q.data);
    const auto s2 = _mm_mul_ps(qs, p.data);
    
    const auto pvXqv = cross(p.data, q.data);
    
    const auto psqs = _mm_mul_ps(ps, qs);
    const auto pvqv = dot(p.data, q.data);
    
    auto r = _mm_add_ps(s1, s2);
    r = _mm_add_ps(r, pvXqv);
    
    auto a = _mm_sub_ps(psqs, pvqv);
    return Quaternion(Vec3(r), a);
};


#endif
