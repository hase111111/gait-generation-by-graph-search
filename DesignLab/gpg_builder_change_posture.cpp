
#include "gpg_builder_change_posture.h"

#include "cassert_define.h"


namespace designlab {

GpgBuilderChangePosture::GpgBuilderChangePosture(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr)
    , presenter_ptr_(presenter_ptr)
    , checker_ptr_(checker_ptr) {
    assert(converter_ptr_);
    assert(presenter_ptr_);
    assert(checker_ptr_);
}


std::unique_ptr<IGaitPatternGenerator>
GpgBuilderChangePosture::Build() const {
    return std::unique_ptr<IGaitPatternGenerator>();
}

}  // namespace designlab
