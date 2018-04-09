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

    // 
    // the following code replaces existing declarations in cv::hal_baseline, 
    // (namely: "v_extract")
    // therefore they are temporarily put in a "cv400" namespace until
    // a pull-request can be created.
    // 
    namespace cv400
    {
        template <int lanes_to_skip, typename vec_type>
        inline vec_type v_extract(const vec_type& a, const vec_type& b)
        {
            using internal::v_sse_reinterpret;
            using internal::v_sse_palignr;
            using internal::v_sse_traits;
            using sse_type = typename v_sse_traits<vec_type>::sse_type;
            static_assert((lanes_to_skip >= 0), "expects lanes_to_skip to be non-negative");
            static_assert((lanes_to_skip <= vec_type::nlanes), "expects lanes_to_skip to not exceed lanes_per_vec");
            constexpr int bytes_per_lane = sizeof(typename vec_type::lane_type);
            constexpr int bytes_to_skip = (lanes_to_skip * bytes_per_lane);
            return vec_type(v_sse_reinterpret<sse_type>(
                v_sse_palignr<bytes_to_skip>(
                    v_sse_reinterpret<__m128i>(a.val),
                    v_sse_reinterpret<__m128i>(b.val))));
        }
    }
    // cv400

    CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
