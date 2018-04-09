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
        // ====== Code change 1 ====== v_sse128_reinterpret ======

        template <typename to_raw_type, typename from_raw_type>
        to_raw_type v_sse_reinterpret(const from_raw_type& val);

#define OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(to_raw_type, from_raw_type, raw_cast_intrin) \
        template<> inline  \
        to_raw_type v_sse_reinterpret(const from_raw_type& a) \
        { return raw_cast_intrin(a); }

        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128i, __m128i, OPENCV_HAL_NOP);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128i, __m128, _mm_castps_si128);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128i, __m128d, _mm_castpd_si128);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128, __m128i, _mm_castsi128_ps);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128, __m128, OPENCV_HAL_NOP);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128, __m128d, _mm_castpd_ps);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128d, __m128i, _mm_castsi128_pd);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128d, __m128, _mm_castps_pd);
        OPENCV_HAL_IMPL_SSE_REINTERPRET_RAW(__m128d, __m128d, OPENCV_HAL_NOP);

    }
    // internal

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

} 
// cv
