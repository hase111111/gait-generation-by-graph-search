#include "SystemTest.h"
#include "ComUpDownNodeCreator.h"
#include "CmdIO.h"

void SystemTest::main()
{
	SNode _first;
	_first.init(false);

	CmdIO _cmd;
	_cmd.outputNode(_first, 0);

	std::vector<SNode> _vec;

	MapState _map;
	_map.init(EMapCreateMode::Flat, MapCreator::OPTION_NONE, false);

	ComUpDownNodeCreator _creator;
	_creator.init(&_map);
	_creator.create(_first, 0, _vec);

	int _cnt = 0;

	for (auto& i : _vec)
	{
		_cnt++;
		_cmd.outputNode(i, _cnt);

	}
}
