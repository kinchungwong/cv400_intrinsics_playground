# cv400_intrinsics_playground

Idea playground for OpenCV 4.0 intrinsics

----

## Improving ```v_extract``` (OpenCV issue 11242)

Date: 2018-04-09

### Files (header-only)

```cpp
// ======
// These headers must be included (or combined) in this ordering.
// ======
#include "v_sse_traits.h"
#include "v_sse_palignr.h"
#include "v_sse_reinterpret.h"
#include "v_reinterpret_as.h"
#include "v_extract_cv400.h"
#include "v_extract_array_cv400.h"
```

### Demo: extracting a vector sub-sequence of 4 elements from a concatenation of 2 vectors (total 8 elements).

The previous implementation had a bug which makes ```v_extract``` unusable on floating-point vectors.

```cpp
template <int lanes_to_skip>
cv::v_float32x4 test_two_vecs()
{
    cv::v_float32x4 a = { 11, 12, 13, 14 };
    cv::v_float32x4 b = { 21, 22, 23, 24 };
    cv::v_float32x4 c = cv::cv400::v_extract<lanes_to_skip>(a, b);
    return c;
}
```

### Demo: extracting a vector sub-sequence from an array ("raw", or "c-style") of vectors

```cpp
template <int lanes_to_skip>
cv::v_float32x4 test_raw_array()
{
    cv::v_float32x4 raa[3] =
    {
        cv::v_float32x4{ 11, 12, 13, 14 },
        cv::v_float32x4{ 21, 22, 23, 24 },
        cv::v_float32x4{ 31, 32, 33, 34 },
    };
    cv::v_float32x4 c = cv::cv400::v_extract<lanes_to_skip>(raa);
    return c;
}
```

### Demo: extracting a vector sub-sequence from an array (using "std::array") of vectors

```cpp
template <int lanes_to_skip>
cv::v_float32x4 test_std_array()
{
    std::array<cv::v_float32x4, 3> saa =
    {
        cv::v_float32x4{ 11, 12, 13, 14 },
        cv::v_float32x4{ 21, 22, 23, 24 },
        cv::v_float32x4{ 31, 32, 33, 34 },
    };
    cv::v_float32x4 c = cv::cv400::v_extract<lanes_to_skip>(saa);
    return c;
}
```

### Demo: compile-time checking of offset parameters ("lanes_to_skip")

```cpp
void demo()
{
    // ======
    // "test_two_vecs" uses two vectors as input (total 8 lanes).
    // ======
    // "test_raw_array" and "test_std_array" both use an array of 
    // three vectors as input (total 12 lanes).
    // ======
    // Commented-out lines are intended to trigger 
    // compile-time error (static_assert).
    // ======
    //dump(test_two_vecs<-1>()); // error
    dump(test_two_vecs<0>());
    dump(test_two_vecs<4>());
    //dump(test_two_vecs<5>()); // error
    //dump(test_raw_array<-1>()); // error
    dump(test_raw_array<0>());
    dump(test_raw_array<1>());
    dump(test_raw_array<8>());
    //dump(test_raw_array<9>()); // error
    //dump(test_std_array<-1>()); // error
    dump(test_std_array<0>());
    dump(test_std_array<1>());
    dump(test_std_array<8>());
    //dump(test_std_array<9>()); // error
}
```

----

***End of README.md***

----
