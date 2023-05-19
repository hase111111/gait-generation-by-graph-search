#include "MapState.h"

MapState::MapState(const EMapCreateMode _mode, const int _option, const bool _do_output)
{
}

MapState::MapState(const MapState& _other)
{
	m_map_data.clear();

	for (const auto &i : _other.m_map_data)
	{
		m_map_data.push_back(i);
	}
}
