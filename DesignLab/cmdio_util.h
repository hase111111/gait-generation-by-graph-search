
//! @file      cmdio_util.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_CMDIO_UTIL_H_
#define DESIGNLAB_CMDIO_UTIL_H_

#include <format>
#include <string>
#include <utility>

#include "boot_mode.h"
#include "output_detail.h"


// ----------------------------------------------------------------
//
// 色付きで出力をしたくない場合，あるいはうまく動作しない場合は，
// 以下のマクロをコメントアウトすること．
//
// ----------------------------------------------------------------

#define DESIGNLAB_USE_COLOR_OUTPUT


namespace designlab
{

//! @class CmdIOUtil
//! @brief 標準入出力の std::cout，std::cinを使った
//! コマンドライン入出力を行うシングルトンクラス．
//! @details
//! シングルトンクラスのため，インスタンス化はできない．実行時には，以下のように使う．
//! @code
//! // 出力の許可範囲を設定
//! CmdIOUtil::SetOutputLimit(OutputDetail::kDebug);
//! // 出力を行うかどうかを設定
//! CmdIOUtil::DoOutput(true);
//! // 出力
//! CmdIOUtil::Output("Hello World!", OutputDetail::kInfo);
//! @endcode
class CmdIOUtil final
{
public:
    CmdIOUtil() = delete;                               //!< インスタンス化させない．
    CmdIOUtil(const CmdIOUtil&) = delete;               //!< コピーコンストラクタを禁止．
    CmdIOUtil& operator=(const CmdIOUtil&) = delete;    //!< コピー代入演算子を禁止．
    CmdIOUtil(CmdIOUtil&&) = delete;                    //!< ムーブコンストラクタを禁止．


    //! @brief 出力するメッセージをどこまで許可するかを設定する関数．
    //! @n この関数を呼び出してから出ないと，他の関数を使えない．
    //! @n 例えば kError に設定すると，kError 未満の出力( kInfo とか kDebug とかはされない．
    //! @n 逆に kDebug に設定すると，すべての出力がされる．
    //! @n 1度呼び出したら，プログラム終了まで設定は有効となる．
    //! @param[in] limit 出力するメッセージをどこまで許可するか
    static void SetOutputLimit(OutputDetail limit);

    //! @brief そもそも出力をするかを設定する関数．
    //! @n false に設定しても システムメッセージは出力される．
    //! @param[in] do_output 出力をするかどうか
    static void DoOutput(bool do_output);


    //! @brief コマンドラインに文字を出力する関数．
    //! @n SetOutputLimit() で設定した出力の許可範囲内であれば出力される．
    //! @n 必ず SetOutputLimit() を呼び出してから使うこと．
    //! @param[in] str 出力する文字列
    //! @param[in] detail 出力する文字列の詳細
    static void Output(const std::string& str, OutputDetail detail);

    //! @brief コマンドラインに文字を出力する関数．Debug用の出力．
    //! @param[in] str 出力する文字列
    static inline void DebugOutput(const std::string& str)
    {
        Output(str, OutputDetail::kDebug);
    }

    //! @brief コマンドラインに文字を出力する関数．Info用の出力．
    //! @param[in] str 出力する文字列
    static inline void InfoOutput(const std::string& str)
    {
        Output(str, OutputDetail::kInfo);
    }

    //! @brief コマンドラインに文字を出力する関数．Warning用の出力．
    //! @param[in] str 出力する文字列
    static inline void WarningOutput(const std::string& str)
    {
        Output(str, OutputDetail::kWarning);
    }

    //! @brief コマンドラインに文字を出力する関数．Error用の出力．
    //! @param[in] str 出力する文字列
    static inline void ErrorOutput(const std::string& str)
    {
        Output(str, OutputDetail::kError);
    }

    //! @brief コマンドラインに文字を出力する関数．System用の出力．
    //! @param[in] str 出力する文字列
    static inline void SystemOutput(const std::string& str)
    {
        Output(str, OutputDetail::kSystem);
    }

    //! @brief コマンドラインに文字を出力する関数．
    //! @n SetOutputLimit() で設定した出力の許可範囲内であれば出力される．
    //! @n 必ず SetOutputLimit() を呼び出してから使うこと．
    //! @param[in] str 出力する文字列
    //! @param[in] detail 出力する文字列の詳細
    //! @param[in] args 出力する文字列に埋め込む変数
    template <typename... Args>
    static void FormatOutput(OutputDetail detail, const std::format_string<Args...> str, Args&&... args)
    {
        const std::string formatted_str = std::format(str, std::forward<Args>(args)...);
        Output(formatted_str, detail);
    }

    //! @brief コマンドラインに文字を出力する関数．
    //! @n SetOutputLimit() で設定した出力の許可範囲内であれば出力される．
    //! @n 必ず SetOutputLimit() を呼び出してから使うこと．
    //! @param[in] str 出力する文字列
    //! @param[in] detail 出力する文字列の詳細
    //! @param[in] args 出力する文字列に埋め込む変数
    template <typename... Args>
    static void SpacedFormatOutput(OutputDetail detail, const std::format_string<Args...> str, Args&&... args)
    {
        const std::string formatted_str = std::format(str, std::forward<Args>(args)...);
        SpacedOutput(formatted_str, detail);
    }

    //! @brief コマンドラインに文字を出力する関数．
    //! @n 前と後ろに改行を挿入する．
    //! @param[in] str 出力する文字列
    //! @param[in] detail 出力する文字列の詳細
    static void SpacedOutput(const std::string& str, OutputDetail detail);

    //! @brief 中央に文字を出力する関数．
    //! 文字列が長すぎる場合は普通に左詰めで出力される．
    //! @param[in] str 出力する文字列．
    //! @param[in] detail 出力する文字列の詳細．
    static void OutputCenter(const std::string& str, OutputDetail detail);

    //! @brief 右端に文字を出力する関数．
    //! 文字列が長すぎる場合は普通に左詰めで出力される．
    //! @param[in] str 出力する文字列．
    //! @param[in] detail 出力する文字列の詳細．
    static void OutputRight(const std::string& str, OutputDetail detail);


    //! @brief コマンドラインで改行をする関数．
    //! @param[in] num 改行する回数．0以下の値を入れると何もしない．
    //! @param[in] detail 出力する文字列の詳細．
    static void OutputNewLine(int num, OutputDetail detail);

    //! @brief コマンドラインに水平線を出力する関数．
    //! @param[in] line_visual 水平線の見た目．'-'ならば水平線，'='ならば二重水平線．
    //! @n 2文字以上の文字列を入れると動作しない．
    //! @param[in] detail 出力する文字列の詳細．
    static void OutputHorizontalLine(
        const std::string& line_visual, OutputDetail detail);

    //! @brief コマンドラインにこのソフトのタイトルを出力する関数．
    //! @n 出力される文字列は，必ず OutputDetail::kSystem で出力される．
    //! @param[in] str 出力する文字列．
    //! @param[in] output_copy_right コピーライトを出力するか．
    //! (デフォルトでは false )
    static void OutputTitle(const std::string& title_name, bool output_copy_right = false);


    //! @brief 入力待ちをする関数．
    //! @n 出力される文字列は，必ず OutputDetail::kSystem で出力される．
    //! @param[in] str 入力待ちをする際に出力する文字列．
    static void WaitAnyKey(const std::string& str = "Waiting for input.");

    //! @brief 整数を入力させる関数．
    //! @n 出力される文字列は，必ず OutputDetail::kSystem で出力される．
    //! @param[in] min 入力する整数の最小値．
    //! @param[in] max 入力する整数の最大値．
    //! @param[in] default_num デフォルトで入力する整数．
    //! @param[in] str 入力待ちをする際に出力する文字列．
    //! @return 入力された整数．
    static int InputInt(int min, int max, int default_num, const std::string& str = "Please enter an integer.");

    //! @brief yesかnoを入力させる関数．返り値で yes なら true，noなら falseを返す．
    //! @n 出力される文字列は，必ず OutputDetail::kSystem で出力される．
    //! @param[in] str 入力待ちをする際に出力する文字列．
    //! @retval true 入力されたのが yes ならば true．
    //! @retval false 入力されたのが no ならば false．
    static bool InputYesNo(const std::string& str = "Are you sure?");

    //! @brief ディレクトリ名を入力させる関数．
    //! @n 出力される文字列は，必ず OutputDetail::kSystem で出力される．
    //! @n ディレクトリ名には次の文字は使えない．
    //! @n \ / : * ? " < > |
    //! @n ディレクトリ名は空白を含めることができない．
    //! @param[in] str 入力待ちをする際に出力する文字列．
    //! @return 入力されたディレクトリ名．
    static std::string InputDirName(const std::string& str = "Enter a directory name. (Japanese is not recommended).");

private:
    static constexpr int kHorizontalLineLength = 100;  //!< 水平線の長さ．

    //! 出力制限，この値未満のメッセージの出力は行われない．
    static OutputDetail output_limit;

    //! falseの場合，出力を行わない(システムメッセージは除く)．
    static bool do_output;

    //! 初期化を既に行ったかどうか．
    static bool is_initialized;
};

}  // namespace designlab


#endif  // DESIGNLAB_CMDIO_UTIL_H_
