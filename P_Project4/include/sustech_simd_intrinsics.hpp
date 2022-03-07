#pragma once
//#if __ARM_NEON_FP==1
//#include <arm64intr.h>
//#include <arm_neon.h>
//#endif
//#if (defined (_M_IX86) || defined (_M_X64)) && !defined(_CHPE_ONLY_) && (!defined(_M_ARM64EC) || !defined(_DISABLE_SOFTINTRIN_))
//        #include <immintrin.h>
//        #include <ammintrin.h>
//#endif
#include <intrin.h>
//TODO: add support for matmul core function using simd. add support for vector encapsulating different simd and different depth