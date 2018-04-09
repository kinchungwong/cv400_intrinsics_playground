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
    template <int start_lane, typename vec_type, 
        bool aligns_lower = (start_lane == 0),
        bool aligns_middle = (start_lane * 2 == vec_type::nlanes),
        bool aligns_upper = (start_lane == vec_type::nlanes)>
    class v_insert_helper;

    template <int start_lane, typename vec_type>
    class v_insert_helper<start_lane, vec_type, true, false, false>
    {
    public:
        void write_to_lower_upper(vec_type& dest_lower, vec_type& /*dest_upper*/, const vec_type& new_value) const
        {
            dest_lower = new_value;
        }
    };

    template <int start_lane, typename vec_type>
    class v_insert_helper<start_lane, vec_type, false, false, true>
    {
    public:
        void write_to_lower_upper(vec_type& /*dest_lower*/, vec_type& dest_upper, const vec_type& new_value) const
        {
            dest_upper = new_value;
        }
    };

    template <int start_lane, typename vec_type>
    class v_insert_helper<start_lane, vec_type, false, true, false>
    {
    public:
        void write_to_lower_upper(vec_type& dest_lower, vec_type& dest_upper, const vec_type& new_value) const
        {
            using sse_type = typename internal::v_sse_traits<vec_type>::sse_type;
            __m128i new_value_cast = internal::v_sse_reinterpret<__m128i>(new_value.val);
            __m128i orig_lower_cast = internal::v_sse_reinterpret<__m128i>(dest_lower.val);
            __m128i orig_upper_cast = internal::v_sse_reinterpret<__m128i>(dest_upper.val);
            __m128i combined_lower = _mm_unpacklo_epi64(orig_lower_cast, new_value_cast);
            __m128i combined_upper = _mm_unpackhi_epi64(new_value_cast, orig_upper_cast);
            dest_lower.val = internal::v_sse_reinterpret<sse_type>(combined_lower);
            dest_upper.val = internal::v_sse_reinterpret<sse_type>(combined_upper);
        }
    };

    template <int start_lane, typename vec_type>
    class v_insert_helper<start_lane, vec_type, false, false, false>
    {
        static_assert((start_lane > 0), "");
        static_assert((start_lane * 2 != vec_type::nlanes), "");
        static_assert((start_lane < vec_type::nlanes), "");
        enum
        {
            bytes_per_lane = sizeof(vec_type::lane_type),
            lower_bytes_to_replace = ((vec_type::nlanes - start_lane) * bytes_per_lane),
            upper_bytes_to_replace = (start_lane * bytes_per_lane)
        };
    public:
        void write_to_lower_upper(vec_type& dest_lower, vec_type& dest_upper, const vec_type& new_value) const
        {
            // ====== TODO ======
            // Need more optimization.
            // General case needs 10 instructions, 
            // compared to 2 instructions for half-vector case,
            // and zero instructions (pure move) for vector-aligned case.
            // ======
            using sse_type = typename internal::v_sse_traits<vec_type>::sse_type;
            const __m128i allzero = _mm_setzero_si128();
            const __m128i allset = _mm_cmpeq_epi8(allzero, allzero);
            __m128i cast_new_value = v_sse_reinterpret<__m128i>(new_value.val);
            __m128i lower_keep_mask = _mm_srli_si128(allset, lower_bytes_to_replace);
            __m128i upper_keep_mask = _mm_slli_si128(allset, upper_bytes_to_replace);
            __m128i orig_lower = v_sse_reinterpret<__m128i>(dest_lower.val);
            __m128i orig_upper = v_sse_reinterpret<__m128i>(dest_upper.val);
            __m128i masked_lower = _mm_and_si128(orig_lower, lower_keep_mask);
            __m128i masked_upper = _mm_and_si128(orig_upper, upper_keep_mask);
            __m128i combined_lower = _mm_or_si128(masked_lower, _mm_slli_si128(cast_new_value, upper_bytes_to_replace));
            __m128i combined_upper = _mm_or_si128(masked_upper, _mm_srli_si128(cast_new_value, lower_bytes_to_replace));
            dest_lower.val = v_sse_reinterpret<sse_type>(combined_lower);
            dest_upper.val = v_sse_reinterpret<sse_type>(combined_upper);
        }
    };
}

//
// This is new functionality: "v_insert".
//
namespace cv400
{
    template <int start_lane, typename vec_type>
    inline void v_insert(vec_type& dest_lower, vec_type& dest_upper, const vec_type& new_value)
    {
        using internal::v_insert_helper;
        const v_insert_helper<start_lane, vec_type> helper;
        helper.write_to_lower_upper(dest_lower, dest_upper, new_value);
    }
}
// cv400

CV_CPU_OPTIMIZATION_HAL_NAMESPACE_END

}
// cv
