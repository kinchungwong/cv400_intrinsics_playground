// This header file contains code that is work-in-progress.
// This file is currently not part of OpenCV project, but its license terms shall be chosen 
// to be forward-compatible with the policy of OpenCV code contribution guideline.

// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#pragma once

namespace cv
{

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_BEGIN

namespace internal
{
    template <typename _Tpvec>
    struct v_sse_traits;

#define OPENCV_HAL_IMPL_SSE_TRAITS_RAW(_Tpvec, _Raw) \
    template<> struct v_sse_traits<_Tpvec> \
    {   using sse_type = _Raw; };

    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_uint8x16, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_int8x16, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_uint16x8, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_int16x8, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_uint32x4, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_int32x4, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_uint64x2, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_int64x2, __m128i);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_float32x4, __m128);
    OPENCV_HAL_IMPL_SSE_TRAITS_RAW(v_float64x2, __m128d);

}
// internal

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
