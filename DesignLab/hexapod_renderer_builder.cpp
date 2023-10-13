#include "hexapod_renderer_builder.h"

#include "phantomx_renderer_model.h"
#include "phantomx_renderer_simple.h"
#include "phantomx_state_calculator.h"
#include "phantomx_state_calculator_hato.h"

std::unique_ptr<IHexapodRenderer> HexapodRendererBuilder::Build(
    const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator,
    const DisplayQuality display_quality
    )
{
    //AbstractHexapodStateCalculator‚Ìtype‚ğŒ©‚ÄA“KØ‚Èrenderer‚ğ•Ô‚·
    bool is_phantomx = (
        typeid(*calculator) == typeid(const std::shared_ptr<const PhantomXStateCalclator>&) ||
        typeid(*calculator) == typeid(PhantomXStateCalclator_Hato)
    );

    if (is_phantomx)
    {
        if (display_quality == DisplayQuality::kHigh) 
        {
            return std::make_unique<PhantomXRendererModel>(calculator);
        }
        else 
        {
            return std::make_unique<PhantomXRendererSimple>(calculator, display_quality);
        }
	}


    return std::make_unique<PhantomXRendererSimple>(calculator, display_quality);
}
