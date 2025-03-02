
//! @file hexapod_renderer_builder.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "hexapod_renderer_builder.h"

#include "phantomx_mk2.h"
#include "phantomx_mk2_renderer_model.h"
#include "phantomx_renderer_simple.h"


namespace designlab
{

std::tuple<std::shared_ptr<IDxlib3dRenderer>, std::shared_ptr<IDxlibNodeSetter> > HexapodRendererBuilder::Build(
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
  const DisplayQuality display_quality
)
{
    //! @todo IHexapodの type を見て、適切な renderer を返す．

    if (display_quality == DisplayQuality::kHigh)
    {
        const auto renderer =
            std::make_shared<PhantomXMkIIRendererModel>(converter_ptr, calculator_ptr);

        return { renderer , renderer };
    }
    else
    {
        const auto renderer =
            std::make_shared<PhantomXRendererSimple>(converter_ptr,
                                                     calculator_ptr,
                                                     display_quality);

        return { renderer , renderer };
    }
}

}  // namespace designlab
