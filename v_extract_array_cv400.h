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
        template <int lanes_to_skip, typename vec_type, int vec_array_count, 
            bool is_divisible = ((lanes_to_skip % vec_type::nlanes) == 0)>
        class v_extract_array_helper;

        template <int lanes_to_skip, typename vec_type, int vec_array_count>
        class v_extract_array_helper<lanes_to_skip, vec_type, vec_array_count, false>
        {
            enum
            {
                lanes_per_vec = vec_type::nlanes,
                total_lanes_in_array = (vec_array_count * lanes_per_vec)
            };
            static_assert((vec_array_count > 0), 
                "expects vec_array_count positive");
            static_assert((lanes_to_skip >= 0), 
                "expects lanes_to_skip to be non-negative");
            static_assert((lanes_to_skip + lanes_per_vec <= total_lanes_in_array), 
                "expects lanes_to_skip not reading past end of array");
            static_assert((lanes_to_skip % lanes_per_vec) != 0,
                "expects lanes_to_skip to be non-divisible by lanes_per_vec");
            enum
            {
                lower_index = (lanes_to_skip / lanes_per_vec),
                remain_lanes_to_skip = (lanes_to_skip % lanes_per_vec)
            };
        public:
            vec_type from_raw_ptr(const vec_type* input_vec_array_value) const
            {
                return cv400::v_extract<remain_lanes_to_skip>(
                    input_vec_array_value[lower_index],
                    input_vec_array_value[lower_index + 1]);
            }
            vec_type from_array(const vec_type(&input_vec_array_value)[vec_array_count]) const
            {
                return cv400::v_extract<remain_lanes_to_skip>(
                    input_vec_array_value[lower_index],
                    input_vec_array_value[lower_index + 1]);
            }
            vec_type from_array(const std::array<vec_type, vec_array_count>& input_vec_array_value) const
            {
                return cv400::v_extract<remain_lanes_to_skip>(
                    input_vec_array_value[lower_index],
                    input_vec_array_value[lower_index + 1]);
            }
        };

        template <int lanes_to_skip, typename vec_type, int vec_array_count>
        class v_extract_array_helper<lanes_to_skip, vec_type, vec_array_count, true>
        {
            enum
            {
                lanes_per_vec = vec_type::nlanes,
                total_lanes_in_array = (vec_array_count * lanes_per_vec)
            };
            static_assert((vec_array_count > 0),
                "expects vec_array_count positive");
            static_assert((lanes_to_skip >= 0),
                "expects lanes_to_skip to be non-negative");
            static_assert((lanes_to_skip + lanes_per_vec <= total_lanes_in_array),
                "expects lanes_to_skip not reading past end of array");
            static_assert((lanes_to_skip % lanes_per_vec) == 0,
                "expects lanes_to_skip to be divisible by lanes_per_vec");
            enum
            {
                lower_index = (lanes_to_skip / lanes_per_vec)
            };
        public:
            vec_type from_raw_ptr(const vec_type* input_vec_array_value) const
            {
                return input_vec_array_value[lower_index];
            }
            vec_type from_array(const vec_type(&input_vec_array_value)[vec_array_count]) const
            {
                return input_vec_array_value[lower_index];
            }
            vec_type from_array(const std::array<vec_type, vec_array_count>& input_vec_array_value) const
            {
                return input_vec_array_value[lower_index];
            }
        };
    }
    // internal

    // 
    // the following code replaces existing declarations in cv::hal_baseline, 
    // (namely: "v_extract"), extending functionality to arrays.
    // therefore they are temporarily put in a "cv400" namespace until
    // a pull-request can be created.
    // 
    namespace cv400
    {
        template <int lanes_to_skip, typename vec_type, int vec_array_count>
        inline vec_type v_extract(const vec_type(&input_vec_array_value)[vec_array_count])
        {
            using internal::v_extract_array_helper;
            const v_extract_array_helper<lanes_to_skip, vec_type, vec_array_count> helper;
            return helper.from_array(input_vec_array_value);
        }

        template <int lanes_to_skip, typename vec_type, size_t vec_array_count>
        inline vec_type v_extract(const std::array<vec_type, vec_array_count>& input_vec_array_value)
        {
            using internal::v_extract_array_helper;
            const v_extract_array_helper<lanes_to_skip, vec_type, vec_array_count> helper;
            return helper.from_array(input_vec_array_value);
        }
    }
    // cv400

    CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
