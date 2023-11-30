#include "hexapod_renderer_builder.h"

#include "phantomx_renderer_model.h"
#include "phantomx_renderer_simple.h"
#include "phantomx_mk2.h"

std::tuple<std::shared_ptr<IDxlib3dRenderer>, std::shared_ptr<IDxlibNodeSetter> > HexapodRendererBuilder::Build(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const DisplayQuality display_quality
)
{
	//! @todo IHexapodのtypeを見て、適切なrendererを返す

	if (display_quality == DisplayQuality::kHigh)
	{
		const auto renderer = std::make_shared<PhantomXRendererModel>(converter_ptr, calculator_ptr);

		return { renderer , renderer };
	}
	else
	{
		const auto renderer = std::make_shared<PhantomXRendererSimple>(converter_ptr, calculator_ptr, display_quality);

		return { renderer , renderer };
	}
}
