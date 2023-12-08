#ifndef DESIGNLAB_MAP_CREATOR_FACTORY_H_
#define DESIGNLAB_MAP_CREATOR_FACTORY_H_

#include "interface_map_creator.h"
#include "application_setting_record.h"

#include <memory>


class MapCreatorFactory final
{
public:

	static std::unique_ptr<IMapCreator> Create();
};

#endif  // DESIGNLAB_MAP_CREATOR_FACTORY_H_