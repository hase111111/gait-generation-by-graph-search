#ifndef DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_H
#define DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_H

#include <string>

#include "interface_toml_data_validator.h"
#include "simulation_map_parameter.h"


class SimulationMapParameterValidator final : public ITomlDataValidator<SimulationMapParameter>
{
public:
	std::tuple<bool, std::string> Validate(const SimulationMapParameter& toml_data) const override;

private:
	const std::string kErrMesPerforatedRange{ "確率は0 ～ 100 %の範囲にする必要があります．" };
	const std::string kErrMesRoughnessMaxMin{ "粗さの最大値は最小値以上にする必要があります．" };
	const std::string kErrMesStripeGtZero{ "ストライプの数は0より大きい必要があります．" };
	const std::string kErrMesStepLengthGtZero{ "階段の奥行は0より大きい必要があります．" };
};


#endif //DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_H