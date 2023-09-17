//! @file keyboard.h
//! @brief Dxlibのキー入力を取得するクラス

#ifndef DESIGNLAB_KEYBOARD_H_
#define DESIGNLAB_KEYBOARD_H_


#include <array>

#include "singleton.h"


//! @class Keyboard
//! @brief Dxlibのキー入力を取得するクラス．
//! @details 詳しい処理の内容はこちら https://dixq.net/rp2/08.html 
//! @n
//! @n Keyboardはシングルトンクラスなので実体を生成することができない．
//! @n getIns()を用いて関数を呼び出す必要がある．
//! @n
//! @n 例) 
//! @n @t Keyboard::getIns()->Update() 
//! @n
//! @n また注意点としては，あくまでDxlibの処理を行うため，コマンドラインでは呼び出せない．
//! @n というか呼び出せるけれどうまく動作するかは保証されない．致命的なバグやエラーを誘発する可能性があるので呼ぶべきでない．
//! @n 素直に std::cin を使うべき．( C言語の関数の scanf は使用すべきでない)

class Keyboard final : public Singleton<Keyboard>
{
public:

	//! @brief キー入力を更新する．これを毎フレーム実行しないと，キー入力を取得できない．
	void Update();

	//! @brief keyCodeのキーが押されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 押されているフレーム数．異常なkey_codeが渡された場合は-1を返す．
	int GetPressingCount(const int key_code) const;

	//! @brief keyCodeのキーが離されているフレーム数を取得する．
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return int 離されているフレーム数．異常なkey_codeが渡された場合は-1を返す．
	int GetReleasingCount(const int key_code) const;

private:

	Keyboard();
	friend Singleton< Keyboard >;

	static const int kKeyNum = 256;						//!< Dxlibにおけるキー総数

	//! @brief keyCodeが有効なキー番号か返す
	//! @param [in] keyCode どのキーを調べたいか，例えばZキーだったら，KEY_INPUT_Zとなる．
	//! @return bool 有効なキー番号ならtrue，そうでなければfalse．
	bool IsAvailableCode(const int key_code) const;

	std::array<int, kKeyNum> key_pressing_counter_;	//押されカウンタ
	std::array<int, kKeyNum> key_releasing_counter_;	//離されカウンタ
};


#endif // !DESIGNLAB_KEYBOARD_H_