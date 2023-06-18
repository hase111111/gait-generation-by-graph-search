#pragma once
#include "Singleton.h"
#include <array>


class Keyboard final : public Singleton<Keyboard> 
{
	Keyboard();
	friend Singleton< Keyboard >;

public:

	//! @brief キー入力を更新する．これを毎フレーム実行しないと，キー入力を取得できない．
	void update();	
	
	//! @brief keyCodeのキーが押されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 押されているフレーム数．
	int getPressingCount(int keyCode);

	//! @brief keyCodeのキーが離されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 離されているフレーム数．
	int getReleasingCount(int keyCode);

private:
	static const int KEY_NUM = 256;				//キー総数
    std::array<int, KEY_NUM> _pressingCount;	//押されカウンタ
    std::array<int, KEY_NUM> _releasingCount;	//離されカウンタ

	bool isAvailableCode(int keyCode);			//keyCodeが有効なキー番号か問う
};


//! @file Keyboard.h
//! @brief Dxlibのキー入力を取得するクラスの実装．
//! @author 長谷川

//! @class Keyboard
//! @brief Dxlibのキー入力を取得するクラス．
//! @details 詳しい処理の内容はこちら https://dixq.net/rp2/08.html <br>
//! <br>
//! Keyboardはシングルトンクラスなので実体を生成することができない．<br>
//! getIns()を用いて関数を呼び出す必要がある．<br>
//! <br>
//! 注意点としては，あくまでDxlibの処理を行うため，コマンドラインでは呼び出せない．<br>
//! というか呼び出せるけれどうまく動作するかは保証されない．致命的なバグやエラーを誘発する可能性があるので呼ぶべきでない．<br>
//! std::cinとかを使うべき．
//! @author 長谷川
