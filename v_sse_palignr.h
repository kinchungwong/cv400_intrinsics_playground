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

#if !defined CV_DISABLE_OPTIMIZATION && defined CV_ENABLE_INTRINSICS && defined CV_CPU_COMPILE_SSSE3

        template <int front_bytes_to_skip>
        __m128i v_sse_palignr(const __m128i& front_value, const __m128i& back_value)
        {
            return _mm_alignr_epi8(back_value, front_value, front_bytes_to_skip);
        }

#else

        template <int front_bytes_to_skip>
        __m128i v_sse_palignr(const __m128i& front_value, const __m128i& back_value)
        {
            const int back_bytes_to_skip = (sizeof(__m128i) - front_bytes_to_skip);
            __m128i front_shuffled = _mm_srli_si128(front_value, front_bytes_to_skip);
            __m128i back_shuffled = _mm_slli_si128(back_value, back_bytes_to_skip);
            return _mm_or_si128(front_shuffled, back_shuffled);
        }

#endif

    }
    // internal

    CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
