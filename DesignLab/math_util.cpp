
#include "math_util.h"

#include <iomanip>
#include <numbers>
#include <random>
#include <sstream>

#include "cassert_define.h"


namespace designlab::math_util
{

float limitRangeAngle(const float angle)
{
    // atan2に合わせるため，角度を -π < angle < π にする (例えば 340°→-20°,-340°→20°)
    float res = angle + std::numbers::pi_v<float>;

    res = std::fmodf(res, 2.0 * std::numbers::pi_v<float>);

    if (res < 0)
    {
        res += std::numbers::pi_v<float>;
    }
    else
    {
        res -= std::numbers::pi_v<float>;
    }

    return res;
}


}  // namespace designlab::math_util
