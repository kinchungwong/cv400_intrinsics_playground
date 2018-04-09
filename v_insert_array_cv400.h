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
        template <int lane_start, typename vec_type, int vec_array_count,
            bool is_divisible = ((lane_start % vec_type::nlanes) == 0)>
            class v_insert_array_helper;

        template <int lane_start, typename vec_type, int vec_array_count>
        class v_insert_array_helper<lane_start, vec_type, vec_array_count, false>
        {
            enum
            {
                lanes_per_vec = vec_type::nlanes,
                total_lanes_in_array = (vec_array_count * lanes_per_vec)
            };
            static_assert((vec_array_count > 0),
                "expects vec_array_count positive");
            static_assert((lane_start >= 0),
                "expects lane_start to be non-negative");
            static_assert((lane_start + lanes_per_vec <= total_lanes_in_array),
                "expects lane_start not reading past end of array");
            static_assert((lane_start % lanes_per_vec) != 0,
                "expects lane_start to be non-divisible by lanes_per_vec");
            enum
            {
                lower_index = (lane_start / lanes_per_vec),
                lane_start_modulo_vec = (lane_start % lanes_per_vec)
            };
        public:
            void write_to_raw_ptr(vec_type* dest_vec_array, const vec_type& vec_to_insert) const
            {
                cv400::v_insert<lane_start_modulo_vec>(
                    dest_vec_array[lower_index],
                    dest_vec_array[lower_index + 1],
                    vec_to_insert);
            }
            void write_to_array(vec_type(&dest_vec_array)[vec_array_count], const vec_type& vec_to_insert) const
            {
                cv400::v_insert<lane_start_modulo_vec>(
                    dest_vec_array[lower_index],
                    dest_vec_array[lower_index + 1],
                    vec_to_insert);
            }
            void write_to_array(std::array<vec_type, vec_array_count>& dest_vec_array, const vec_type& vec_to_insert) const
            {
                cv400::v_insert<lane_start_modulo_vec>(
                    dest_vec_array[lower_index],
                    dest_vec_array[lower_index + 1],
                    vec_to_insert);
            }
        };

        template <int lane_start, typename vec_type, int vec_array_count>
        class v_insert_array_helper<lane_start, vec_type, vec_array_count, true>
        {
            enum
            {
                lanes_per_vec = vec_type::nlanes,
                total_lanes_in_array = (vec_array_count * lanes_per_vec)
            };
            static_assert((vec_array_count > 0),
                "expects vec_array_count positive");
            static_assert((lane_start >= 0),
                "expects lane_start to be non-negative");
            static_assert((lane_start + lanes_per_vec <= total_lanes_in_array),
                "expects lane_start not reading past end of array");
            static_assert((lane_start % lanes_per_vec) == 0,
                "expects lane_start to be divisible by lanes_per_vec");
            enum
            {
                lower_index = (lane_start / lanes_per_vec)
            };
        public:
            void write_to_raw_ptr(vec_type* dest_vec_array, const vec_type& vec_to_insert) const
            {
                dest_vec_array[lower_index] = vec_to_insert;
            }
            void write_to_array(vec_type(&dest_vec_array)[vec_array_count], const vec_type& vec_to_insert) const
            {
                dest_vec_array[lower_index] = vec_to_insert;
            }
            void write_to_array(std::array<vec_type, vec_array_count>& dest_vec_array, const vec_type& vec_to_insert) const
            {
                dest_vec_array[lower_index] = vec_to_insert;
            }
        };
    }
    // internal

    //
    // This is new functionality: "v_insert".
    //
    namespace cv400
    {
        template <int lane_start, typename vec_type, int vec_array_count>
        inline void v_insert(vec_type(&dest_vec_array)[vec_array_count], const vec_type& vec_to_insert)
        {
            using internal::v_insert_array_helper;
            const v_insert_array_helper<lane_start, vec_type, vec_array_count> helper;
            return helper.write_to_array(dest_vec_array, vec_to_insert);
        }

        template <int lane_start, typename vec_type, size_t vec_array_count>
        inline void v_insert(std::array<vec_type, vec_array_count>& dest_vec_array, const vec_type& vec_to_insert)
        {
            using internal::v_insert_array_helper;
            const v_insert_array_helper<lane_start, vec_type, vec_array_count> helper;
            return helper.write_to_array(dest_vec_array, vec_to_insert);
        }
    }
    // cv400

    CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
