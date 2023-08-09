#pragma once

#include <array>

#include "singleton.h"


//! @class Keyboard
//! @date 2023/08/07
//! @author 長谷川
//! @brief Dxlibのキー入力を取得するクラス．
//! @details 詳しい処理の内容はこちら https://dixq.net/rp2/08.html @n
//! @n Keyboardはシングルトンクラスなので実体を生成することができない．
//! @n getIns()を用いて関数を呼び出す必要がある．@n
//! @n 例) @n @t Keyboard::getIns()->update() @n
//! @n また注意点としては，あくまでDxlibの処理を行うため，コマンドラインでは呼び出せない．
//! @n というか呼び出せるけれどうまく動作するかは保証されない．致命的なバグやエラーを誘発する可能性があるので呼ぶべきでない．
//! @n 素直に std::cin を使うべき．( C言語の関数の scanf は使用すべきでない)
class Keyboard final : public Singleton<Keyboard>
{
public:

	//! @brief キー入力を更新する．これを毎フレーム実行しないと，キー入力を取得できない．
	void update();

	//! @brief keyCodeのキーが押されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 押されているフレーム数．
	int getPressingCount(const int key_code) const;

	//! @brief keyCodeのキーが離されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 離されているフレーム数．
	int getReleasingCount(const int key_code) const;

private:

	Keyboard();
	friend Singleton< Keyboard >;

	static const int KEY_NUM = 256;				//キー総数
	std::array<int, KEY_NUM> m_pressing_count;	//押されカウンタ
	std::array<int, KEY_NUM> m_releasing_count;	//離されカウンタ

	bool isAvailableCode(const int key_code) const;			//keyCodeが有効なキー番号か問う
};


//! @file keyboard.h
//! @date 2023/08/07
//! @author 長谷川
//! @brief Dxlibのキー入力を取得するクラスの実装．
//! @n 行数 : @lineinfo
