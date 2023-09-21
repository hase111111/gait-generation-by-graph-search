#pragma once

#include <array>

#include "phantomx_state_calculator.h"


class PhantomXStateCalclator_Hato final : public PhantomXStateCalclator
{
public:
	PhantomXStateCalclator_Hato();

	bool isLegInRange(const int leg_index, const designlab::Vector3& leg_pos) const override;

private:

	static constexpr int kMaxDifZ = 200;

	static std::array <float, kMaxDifZ> initLegMaxR();

	static std::array <float,kMaxDifZ> leg_max_r_;
};
