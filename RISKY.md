# The "risky" section of the cv400_intrinsics_playground

This is the "risky" section of the idea playground for OpenCV 4.0 intrinsics.

## Criteria for labeling code as risky

Code in this section are labeled "risky" because they might be:

 * Too complicated,
 * Too much code needed to implement a particular functionality,
 * Too hard to understand,
 * Too hard to explain "how it works" to another C++ programmer,
 * Too difficult to troubleshoot,
 * Have known defects, shortcomings, caveats,
 * Don't work with all existing code,
 * Etc.

## Gallery of risky code

### Lifting all two-argument universal intrinsics to handle arrays

*Added: 2018-04-09*

#### Brief explanation

 * Given a function that accepts two vectors and return one vector, synthesize a function or template that will accept two arrays of vectors, and return one array of vector.
   * The k-th output vector is computed from applying the function to the pair of vectors taken from the k-th index from each of the two input arrays.
 * All vector types (input and output arguments) shall be the same.
 * Functions that accept multiple arguments of different vector types, or that returns a vector of different type, will be handled separately.
 * The input argument will be qualified as const-reference.
 * The original function needs to have its function pointer taken (i.e. it must be decayable to a function pointer, in C++ technical speak.)
 * If the original function is overloaded, or if it is a macro, the following code might not work.
 * It is already known that some functions in OpenCV intrinsics do not work with the following code, for the reason stated above.

#### Code

```cpp
namespace cv
{
    namespace cv400_risky
    {
#if 0
        template <typename vec_type, int n, int nlanes = vec_type::nlanes>
        std::array<vec_type, n> v_array_apply_arg2(
            const std::array<vec_type, n>& arg0,
            const std::array<vec_type, n>& arg1,
            vec_type(*functor)(const vec_type&, const vec_type&))
        {
            std::array<vec_type, n> retval;
            for (int k = 0; k < n; ++k)
            {
                retval[k] = functor(arg0[k], arg1[k]);
            }
            return retval;
        }
#else
        template <typename vec_type, int n, vec_type(*functor)(const vec_type&, const vec_type&), int nlanes = vec_type::nlanes>
        std::array<vec_type, n> v_array_apply_arg2(
            const std::array<vec_type, n>& arg0,
            const std::array<vec_type, n>& arg1)
        {
            std::array<vec_type, n> retval;
            for (int k = 0; k < n; ++k)
            {
                retval[k] = functor(arg0[k], arg1[k]);
            }
            return retval;
        }
#endif
    }

    namespace cv400_array_func
    {
#define OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(vec_type, new_functor_id, canonical_functor_id) \
        template <int n> std::array<vec_type, n> new_functor_id(const std::array<vec_type, n>& arg0, const std::array<vec_type, n>& arg1) \
        { constexpr vec_type(*canonical_functor_ptr)(const vec_type&, const vec_type&) = canonical_functor_id; \
        return ::cv::cv400_risky::v_array_apply_arg2<vec_type, n, canonical_functor_ptr>(arg0, arg1); }

        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_min, cv::hal_baseline::v_min);
        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_max, cv::hal_baseline::v_max);
        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_absdiff, cv::hal_baseline::v_absdiff);
        //OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_add_wrap, cv::hal_baseline::v_add_wrap); // wraparound is meaningless for float32.
        //OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_sub_wrap, cv::hal_baseline::v_sub_wrap);

        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_add, cv::hal_baseline::operator+);
        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_sub, cv::hal_baseline::operator-);
        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_mul, cv::hal_baseline::operator*);
        OPENCV_HAL_IMPL_ARRAY_FUNC_EXPAND_ARG2(v_float32x4, v_div, cv::hal_baseline::operator/);
    }

    using namespace cv400_array_func;
}
```
