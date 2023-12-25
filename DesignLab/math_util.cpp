
#include "math_util.h"

#include <iomanip>
#include <random>
#include <sstream>

#include "cassert_define.h"


namespace designlab::math_util
{

float limitRangeAngle(const float angle)
{
    // atan2に合わせるため，角度を -π < angle < π にする (例えば 340°→-20°,-340°→20°)
    float res = angle + MathConst<float>::kPi;

    res = std::fmodf(res, 2.0 * MathConst<float>::kPi);

    if (res < 0)
    {
        res += MathConst<float>::kPi;
    }
    else
    {
        res -= MathConst<float>::kPi;
    }

    return res;
}


}  // namespace designlab::math_util
