#pragma once
#include "AbstractGraphicMain.h"
#include "MapState.h"

class GraphicMainBasic final : public AbstractGraphicMain
{
public:
	GraphicMainBasic(const GraphicDataBroker* _broker);
	~GraphicMainBasic() = default;

	bool update() override;

	void draw() const override;

private:

	MapState m_Map;	//マップを表示する．
};
