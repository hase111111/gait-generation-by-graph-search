
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "simulation_map_parameter_validator.h"


namespace designlab
{

std::tuple<bool, std::string> SimulationMapParameterValidator::Validate(
    const SimulationMapParameter& toml_data) const
{
    if (toml_data.hole_rate < 0 || 100 < toml_data.hole_rate)
    {
        return { false, kErrMesPerforatedRange };
    }

    if (toml_data.rough_min_height > toml_data.rough_max_height)
    {
        return { false, kErrMesRoughnessMaxMin };
    }

    if (toml_data.stripe_interval < 0)
    {
        return { false, kErrMesStripeGtZero };
    }

    if (toml_data.step_length < 0)
    {
        return { false, kErrMesStepLengthGtZero };
    }

    return { true, "" };
}

}  // namespace designlab
