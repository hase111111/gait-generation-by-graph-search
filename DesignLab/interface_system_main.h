//! @file interface_system_main.h
//! @brief システムのインターフェース．


#ifndef DESIGNLAB_INTERFACE_SYSTEM_MAIN_H_
#define DESIGNLAB_INTERFACE_SYSTEM_MAIN_H_


class ISystemMain
{
public:

	virtual ~ISystemMain() = default;

	//! @brief 主要な処理を行う関数
	virtual void Main()	= 0;
};


#endif