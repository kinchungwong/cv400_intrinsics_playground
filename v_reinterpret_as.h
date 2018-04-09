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

    template <typename dest_vec, typename src_vec,
        typename dest_sse = typename internal::v_sse_traits<dest_vec>::sse_type,
        typename src_sse = typename internal::v_sse_traits<src_vec>::sse_type>
        inline dest_vec v_reinterpret_as(const src_vec& src)
    {
        static_assert(sizeof(src_vec) == sizeof(dest_vec), 
            "v_reinterpret_as<dest_vec, src_vec> expects same byte size of source and dest vector.");
        return dest_vec(internal::v_sse_reinterpret<dest_sse>(src.val));
    }

    // 
    // the following code replaces existing declarations in cv::hal_baseline, 
    // (namely: "v_reinterpret_as_##suffix")
    // therefore they are temporarily put in a "cv400" namespace until
    // a pull-request can be created.
    // 
    namespace cv400 
    {

#define OPENCV_HAL_IMPL_SSE_REINTERPRET(dest_vec, dest_suffix) \
        template <typename src_vec> inline dest_vec v_reinterpret_as_##dest_suffix(const src_vec& src) \
        { return v_reinterpret_as<dest_vec>(src); }

        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_uint8x16, u8);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_int8x16, s8);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_uint16x8, u16);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_int16x8, s16);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_uint32x4, u32);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_int32x4, s32);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_uint64x2, u64);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_int64x2, s64);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_float32x4, f32);
        OPENCV_HAL_IMPL_SSE_REINTERPRET(v_float64x2, f64);

    }
    // cv400

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
