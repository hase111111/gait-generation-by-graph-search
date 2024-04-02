
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include <memory>

#include <boost/thread.hpp>

#include "test_main.h"

#if !defined (DESIGNLAB_USE_TEST)

#include "application_setting_record.h"
#include "application_setting_record_validator.h"
#include "boot_mode_selector.h"
#include "cmdio_util.h"
#include "gait_pattern_generator_basic.h"
#include "gait_pattern_generator_revaluation.h"
#include "gait_pattern_generator_switch_move.h"
#include "gait_pattern_generator_thread.h"
#include "graph_tree_creator.h"
#include "graph_searcher_straight_move.h"
#include "graph_searcher_spot_turn.h"
#include "graphic_main_basic.h"
#include "graphic_main_graph_viewer.h"
#include "graphic_main_display_model.h"
#include "graphic_main_robot_control.h"
#include "graphic_system.h"
#include "node_creator_builder_turn.h"
#include "node_creator_builder_turn_spot.h"
#include "node_creator_builder_straight_move.h"
#include "node_creator_builder_body_rot.h"
#include "map_creator_factory.h"
#include "robot_operator_factory.h"
#include "simulation_end_checker_factory.h"
#include "system_main_graph_viewer.h"
#include "system_main_result_viewer.h"
#include "system_main_robot_control.h"
#include "system_main_simulation.h"
#include "toml_file_importer.h"
#include "toml_directory_exporter.h"
#include "phantomx_mk2.h"



// このプロジェクトがコンパイルされない場合はソリューションエクスプローラーから
// DesignLabを右クリック →「スタートアッププロジェクトに設定」を選択．


int main()
{
    // int main内では designlab:: を省略できる．
    // 推奨されない書き方だが，可読性のため，ここでのみ使用する．
    using namespace designlab;
    using enum OutputDetail;

    // まずは，設定ファイルを読み込む
    CmdIOUtil::SetOutputLimit(kSystem);

    TomlDirectoryExporter toml_directory_exporter;
    toml_directory_exporter.Export();

    TomlFileImporter<ApplicationSettingRecord> application_setting_importer(
        std::make_unique<ApplicationSettingRecordValidator>());


    // 読み込んだ設定ファイルをクラスに記録する．
    const auto application_setting_record =
        std::make_shared<const ApplicationSettingRecord>(
            application_setting_importer.ImportOrUseDefault("./settings.toml"));


    // 次に，コマンドラインの出力を設定する．
    CmdIOUtil::DoOutput(application_setting_record->do_cmd_output);
    CmdIOUtil::SetOutputLimit(application_setting_record->cmd_output_detail);

    // タイトルを表示する．
    CmdIOUtil::OutputTitle("Free Gait Planning for a "
                           "Hexapod Walking Robot by Graph Search", true);


    // GUIを別のスレッドで実行する．
    // このスレッドへはGraphicDataBrokerを通してデータを渡す．
    GraphicSystem graphic_system(application_setting_record);

    // グラフィックシステムを別スレッドで実行する．
    boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system);


    // 処理を実行する．
    while (true)
    {
        // 起動モードを選択する．
        enums::BootMode boot_mode = application_setting_record->default_mode;

        if (application_setting_record->ask_about_modes)
        {
            BootModeSelector boot_mode_selector;

            // デフォルトの起動モードを設定する．
            boot_mode_selector.SetDefaultBootMode(application_setting_record->default_mode);

            // 起動モードを選択する．
            boot_mode = boot_mode_selector.SelectBootMode();
        }


        // 選択が終わったら，選択されたモードに応じてシステムを作成する．
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        TomlFileImporter<PhantomXMkIIParameterRecord> parameter_importer;
        const PhantomXMkIIParameterRecord parameter_record = parameter_importer.ImportOrUseDefault("./simulation_condition/phantomx_mk2.toml");

        auto phantomx_mk2 = std::make_shared<PhantomXMkII>(parameter_record);
        PhantomXMkIIParameterRecord parameter_record_revaluation = parameter_record;
        parameter_record_revaluation.min_leg_range += 10.0f;
        auto phantomx_mk2_revaluation = std::make_shared<PhantomXMkII>(parameter_record_revaluation);

        auto node_creator_builder_straight = std::make_unique<NodeCreatorBuilderStraightMove>(phantomx_mk2, phantomx_mk2, phantomx_mk2);
        auto node_creator_builder_straight_revaluation = std::make_unique<NodeCreatorBuilderStraightMove>(phantomx_mk2_revaluation, phantomx_mk2_revaluation, phantomx_mk2_revaluation);

        // auto node_creator_builder_straight = std::make_unique<NodeCreatorBuilderBodyRot>(phantomx_mk2, phantomx_mk2, phantomx_mk2);
        auto node_creator_builder_turn_spot = std::make_unique<NodeCreatorBuilderTurnSpot>(phantomx_mk2, phantomx_mk2, phantomx_mk2);

        auto graph_tree_creator_straight = std::make_unique<GraphTreeCreator>(std::move(node_creator_builder_straight));
        auto graph_tree_creator_straight_revaluation = std::make_unique<GraphTreeCreator>(std::move(node_creator_builder_straight_revaluation));
        auto graph_tree_creator_turn_spot = std::make_unique<GraphTreeCreator>(std::move(node_creator_builder_turn_spot));

        auto graph_searcher_straight = std::make_unique<GraphSearcherStraightMove>(phantomx_mk2);
        auto graph_searcher_straight_revaluation = std::make_unique<GraphSearcherStraightMove>(phantomx_mk2_revaluation);
        auto graph_searcher_turn_spot = std::make_unique<GraphSearcherSpotTurn>(phantomx_mk2, phantomx_mk2);

        std::unique_ptr<ISystemMain> system_main;

        switch (boot_mode)
        {
            case enums::BootMode::kSimulation:
            {
                // シミュレーションシステムクラスを作成する．

                auto gait_pattern_generator_straight =
                    std::make_unique<GaitPatternGeneratorBasic>(std::move(graph_tree_creator_straight), std::move(graph_searcher_straight), 4, 20000000);
                //auto gait_pattern_generator_straight_revaluation =
                //    std::make_unique<GaitPatternGeneratorBasic>(std::move(graph_tree_creator_straight_revaluation), std::move(graph_searcher_straight_revaluation), 4, 20000000);

                // auto gait_pattern_generator =
                //  std::make_unique<GaitPatternGeneratorRevaluation>(std::move(gait_pattern_generator_straight), std::move(gait_pattern_generator_straight_revaluation), phantomx_mk2, phantomx_mk2);
                auto gait_pattern_generator_turn_spot = std::make_unique<GaitPatternGeneratorBasic>(std::move(graph_tree_creator_turn_spot), std::move(graph_searcher_turn_spot), 4, 20000000);
                auto gait_pattern_generator_switch_move = std::make_unique<GaitPatternGeneratorSwitchMove>(std::move(gait_pattern_generator_straight), std::move(gait_pattern_generator_turn_spot));

                TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
                const SimulationSettingRecord simulation_setting_record = simulation_setting_importer.ImportOrUseDefault("./simulation_condition/simulation_setting.toml");

                auto map_creator = MapCreatorFactory::Create(simulation_setting_record);
                auto simulation_end_checker = SimulationEndCheckerFactory::Create(simulation_setting_record);
                auto robot_operator = RobotOperatorFactory::Create(simulation_setting_record);
                auto node_initializer = std::make_unique<NodeInitializer>(simulation_setting_record.initial_positions,
                                                                          simulation_setting_record.initial_posture,
                                                                          simulation_setting_record.initial_move);
                auto result_exporter = std::make_shared<ResultFileExporter>(phantomx_mk2);

                system_main = std::make_unique<SystemMainSimulation>(
                  std::move(gait_pattern_generator_switch_move),
                  std::move(map_creator),
                  std::move(simulation_end_checker),
                  std::move(robot_operator),
                  std::move(node_initializer),
                  graphic_data_broker,
                  application_setting_record,
                  result_exporter);

                auto graphic_main = std::make_unique<GraphicMainBasic>(
                  graphic_data_broker,
                  phantomx_mk2,
                  phantomx_mk2,
                  phantomx_mk2,
                  application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main));

                break;
            }
            case enums::BootMode::kViewer:
            {
                // グラフビューアシステムクラスを作成する．

                TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
                const SimulationSettingRecord simulation_setting_record =
                    simulation_setting_importer.ImportOrUseDefault("./simulation_condition/simulation_setting.toml");

                auto map_creator = MapCreatorFactory::Create(simulation_setting_record);

                system_main = std::make_unique<SystemMainGraphViewer>(
                    std::move(graph_tree_creator_straight),
                    std::move(map_creator),
                    graphic_data_broker,
                    application_setting_record);

                std::unique_ptr<IGraphicMain> graphic_main_viewer =
                    std::make_unique<GraphicMainGraphViewer>(
                        graphic_data_broker,
                        phantomx_mk2,
                        phantomx_mk2,
                        phantomx_mk2,
                        application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main_viewer));

                break;
            }
            case enums::BootMode::kDisplayModel:
            {
                std::unique_ptr<IGraphicMain> graphic_main_test =
                    std::make_unique<GraphicMainDisplayModel>(
                    phantomx_mk2,
                    phantomx_mk2,
                    phantomx_mk2,
                    application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main_test));

                break;
            }
            case enums::BootMode::kResultViewer:
            {
                // 結果表示システムクラスを作成する．
                system_main = std::make_unique<SystemMainResultViewer>(graphic_data_broker,
                                                                       application_setting_record,
                                                                       phantomx_mk2,
                                                                       phantomx_mk2);

                std::unique_ptr<IGraphicMain> graphic_main = std::make_unique<GraphicMainBasic>(
                  graphic_data_broker,
                  phantomx_mk2,
                  phantomx_mk2,
                  phantomx_mk2,
                  application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main));

                break;
            }
            case enums::BootMode::kRobotControl:
            {
                system_main = std::make_unique<SystemMainRobotControl>(graphic_data_broker);

                std::unique_ptr<IGraphicMain> graphic_main_robot_control =
                    std::make_unique<GraphicMainRobotControl>(graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2, application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main_robot_control));
                break;
            }
            default:
            {
                assert(false);  // 無効なモードが指定された．
                break;
            }
        }

        // システムを実行する．
        if (system_main)
        {
            system_main->Main();
        }
        else
        {
            CmdIOUtil::Output("SystemMainクラスがありません．"
                              "(GraphicSystemしか使用しない場合は"
                              "このメッセージが表示されることがあります．)", kSystem);
        }


        // もう一度実行するかどうかを選択する．
        CmdIOUtil::OutputHorizontalLine("=", kSystem);
        CmdIOUtil::OutputNewLine(1, kSystem);

        if (!CmdIOUtil::InputYesNo("アプリケーションを続行しますか？"))
        {
            break;
        }

        CmdIOUtil::OutputNewLine(1, kSystem);
        CmdIOUtil::OutputHorizontalLine("=", kSystem);
        CmdIOUtil::OutputNewLine(1, kSystem);
    }

    CmdIOUtil::Output("Dxlibの終了を待っています．GUIの×ボタンを押してください．", kSystem);
    graphic_thread.join();

    return 0;
}

#endif  // ! defined (DESIGNLAB_USE_TEST)



//! @mainpage
//! @details
//! 今後このソースファイルを使う人のためにC++で大きなプロジェクトを作成するときのルールをここに定義
//! しておく．
//! 基本的には google c++ style guide に従っているが，一部独自のルールを追加している．
//! @n
//! @n 独自ルールについて，
//! @li その他の機能について…filesystemは使用する．
//! @n
//!
//! @section ●目次
//! @ref anchor_naming "命名規則について"
//! @n
//! @ref anchor_class "クラスについて"
//! @n
//! @ref anchor_func "関数について"
//! @n
//! @ref anchor_const "定数について"
//! @n
//! @ref anchor_header "ヘッダーについて"
//! @n
//! @ref anchor_array "配列について"
//! @n
//! @ref anchor_for "for文について"
//! @n
//! @ref anchor_float "小数について"
//! @n
//! @ref anchor_enum "列挙体について"
//! @n
//! @ref anchor_struct "構造体について"
//! @n
//! @n
//!
//! @anchor anchor_naming
//! @section ●命名規則
//! 変数やクラスの名前を決めるときは下記のどれかを使って決めるのが基本となる．
//! 例えば int NumBer; とか class Sample_Class;だとかは避けること．
//! @li スネークケース snake_case
//! @li キャメルケース camelCase
//! @li パスカルケース PascalCase
//!
//! @n Google C++ Style Guideでは，以下のように命名することが推奨されている．
//! @li 変数 スネークケース (my_var)
//! @li クラスのメンバ変数 スネークケース + "_" (class_var_)
//! @li 定数 "k" + パスカルケース (kMyConstant)
//! @li クラス パスカルケース (MyClass) 名詞．
//! @li インターフェース "I" + パスカルケース (IMyInterface)
//! @li 構造体 パスカルケース (MyStruct) 名詞．
//! @li 関数名 パスカルケース (MyFunction) 動詞 + (目的語)
//! @li 列挙体 パスカルケース (MyEnum)
//! @li 列挙体の要素 "k" + パスカルケース (kMyEnumElement)
//! @li マクロ アッパースネークケース (MY_MACRO)
//!
//! また，変数名をローマ字で書いたり，過度に省略したりするのは控えるべき．
//! (例えば int sushi; とか std::string tkg;など．寿司は仕方ないが...)
//! 固有名詞は全部書く，逆に init (初期化)や num (ナンバー)などよく使うものは省略すべき．
//! func(関数)等は見ればわかるので書かないように CalcFunc() → Calc()
//! 多少長くなっても何しているか分かりやすいほうが見やすなる．(特に関数において)．
//! @n
//! @n よく使う省略語
//! @li add (addition 加算)
//! @li calc (calculate 計算)
//! @li cnt (count カウント)
//! @li init (initialize 初期化)
//! @li num (number 数)
//! @li pos (position 位置)
//! @li res (result 結果)
//! @li ret (return 戻り値)
//! @li rot (rotation 回転)
//! @li sub (subtraction 減算)
//!
//! 関数においては動詞→目的語となるように付けること．(例えば GetData() とか MakeMap() とか)
//! また，bool型の場合は質問形式で命名するとよい．
//! bool is_empty = false; とか bool IsOddNumber(const int num); など．
//! @n
//! @n なぜこのような命名規則を導入するのかというと，
//! 自分の書いたプログラムを他人に読んでもらうとき，
//! 命名方法が適当だと非常に読みづらくなるためである．
//! @n
//!
//! @anchor anchor_class
//! @section ●クラスについて
//! @n 基本的にメンバ変数は全て private: に入れておくこと．
//! 面倒だが，値をとったり設定したりしたいなら，Get??? や Set??? 関数を作って，それを使うこと．
//! @code
//! //sample_class.h
//!
//! #pragma once
//!
//! class SampleClass final
//! {
//! public:
//!     int GetNum() const
//!     {
//!         return num_;
//!     }
//!
//!     void SetNum(const int num)
//!     {
//!         num_ = num;
//!     }
//!
//! private:
//!     int num_;
//! };
//! @endcode
//! 生のメンバ変数の値を変更するのはやめるべき．
//! @n
//! @n また，一つのクラスに大量の機能を追加しないこと．
//! かつての PassFinding クラスはグラフ探索＋ファイル入出力＋コマンド送信等
//! 多量の機能を持っていたが，可読性が下がるのであまりよくない．
//! 機能を細かく分けてプログラムを書くべき．
//! @n
//!
//! @anchor anchor_func
//! @section ●関数について
//! 関数を宣言した場合，その関数の引数や機能についてコメントを付けること．
//! 後から見た時に何をすればよいか分かりやすくてプログラムが楽になる．
//! 注釈はヘッダーに書いておけば，他のファイルでも確認できるようになる．
//! @n
//! 値を返したいときは基本は戻り値を使うこと．
//! 不必要なポインタ渡しや参照渡しはプログラムを無駄に煩雑にするため，避けるべき．
//! 複数の値を返す場合は，ポインタ渡しを用いて値を返すようにし，
//! 簡単な処理の場合は tuple を使うこと．
//! @n
//! @nまた変更しない引数は const を付けておくこと．
//! @code
//! //sample.h
//!
//! #pragma once
//!
//! //値渡しの場合はhの引数には const をつけなくてよい．(実装を h と cpp に分ける場合のみ)
//! int AddNum(int num1, int num2);
//!
//! //参照渡しの場合はhの引数にも const をつけること．
//! int CalcAverage(const std::vector<int>& nums);
//! @endcode
//! @code
//! //sample.cpp
//!
//! #include "sample.h"
//!
//! int AddNum(const int num1, const int num2)
//! {
//!     //num1 += 5 //これはエラーになる．
//!
//!     return num1 + num2;
//! }
//!
//! int CalcAverage(const std::vector<int>& nums)
//! {
//!     //nums.push_back(5); //これはエラーになる．
//!
//!     int sum = 0;
//!     for(const auto& num : nums)
//!     {
//!         sum += num;
//!     }
//!
//!     return sum / nums.size();
//! }
//! @endcode
//! 同様にメンバ変数を変更しないメンバ関数にも const を付けること．
//! @n
//! @n関数の引数においてマジックナンバーは避けること．
//! マジックナンバーとは直に数値を使うことで，
//! 本人以外にその値が何を表しているのか理解できないものを指す．
//! 変数や列挙体やコメントなどを用いて変数の意味を説明するようにすること．
//! @n
//!
//! @anchor anchor_const
//! @section ●定数について
//! #define を使うのはやめること．
//! C++においては const static な変数(静的で定数のメンバ変数)を使うほうがよい．
//! @code
//! //sample_const.h
//!
//! #pragma once
//!
//! class SampleConst
//! {
//! public:
//!   static const float kPi;
//! }
//! @endcode
//! @code
//! //sample_const.cpp
//!
//! #include <iostream>
//!
//! #include "sample_const.h"
//!
//! const float SampleConst::kPi = 3.1415926535f;
//!
//! int main()
//! {
//!     std::cout << SampleConst::kPi << std::endl;
//!
//!     return 0;
//! }
//! @endcode
//! @code
//! //出力内容
//!
//! 3.14159.....
//! @endcode
//! C++には constexprという機能があり，これを使うとコンパイル時に定数を計算してくれるため，
//! 実行時の負荷が減る．
//! 使用可能であるならばこれを使うべき．
//! @see designlab::PhantomXMkIIConst
//!
//! @anchor anchor_array
//! @section ●配列について
//! C言語において通常使われる int _array[10]; のような配列の使用は最小限にしたほうが良い．
//! なぜなら，このプログラムでも過去にそれによるスタックオーバーフローが起きていたためである．
//! (time[1000]という配列に時間を記録していたため， 1000回以上のループができなくなっていた．)
//! とりあえず大きな配列を作っておいて，その中にデータを記録するのは拡張性が低いし，
//! 見つけづらいバグの温床になるため，
//! std::vector (動的な配列) を積極的に使うことを推奨する．
//! @n new/delete は個人的には絶対に使いたくない．なぜならば，メモリリークの温床になるからである．
//! 動的配列は vector や map などのコンテナを使うこと．
//! @code
//! //sample_vector.h
//!
//! #pragma once
//!
//! #include <vector>
//!
//! //int time[1000];  //これはやめること．
//!
//! //int time[];
//!
//! //void TimeArrayInit()
//! //{
//! //  time = new int[1000];  //これはやめること．
//! //}
//!
//! std::vector<int> time(1000);  //これを推奨する．
//! @endcode
//!
//! また，変数を int x1,x2,x3,x4;のように大量に宣言するのはやめること．
//! その場合は普通に配列で構わない．
//! @n あるいは，std::arrayを使うこと，std::array<int, 4> x;
//! @code
//! //sample_array.h
//!
//! #pragma once
//!
//! #include <array>
//!
//! //int x1,x2,x3,x4;  //これはやめること．
//!
//! int x[4];  //これは許容する．
//!
//! std::array<int, 4> x;  //これを推奨する．
//! @endcode
//!
//! @anchor anchor_for
//! @section ●for文について
//! for文に使用する変数は i → j → k とすること．通例そのような命名をすることが多いためである．
//! また，for文の中でサイズの大きなクラスや構造体を宣言するのはやめること．
//! 非常に負荷がかかるためである．
//!
//! @anchor anchor_header
//! @section ●ヘッダーについて
//! 必ず，インクルードガードを書くこと．@#ifndef ～ @#endifのこと．
//! @#pragma once と書くと，インクルードガードを書かなくてもインクルードガードの機能が働く．
//! しかし，これはC++の機能ではないので非推奨．(実際にはほとんどのコンパイラで使えるが，
//! 一部のコンパイラでは使えない．特にこだわりがないならばどちらを使ってもよい．)
//! @n
//! @n ヘッダーにはむやみに #include を書かないようにすること．
//! コンパイル時間が伸びるためである．過不足なく必要なものだけをインクルードするようにする．
//! またグローバル変数を使ったり，externを使ったりするのは避けること．
//! 多くの場合スパゲッティコードの原因になる．
//! 設計をよく考えれば使わずに済むはず．どうしても必要な場合は一つのヘッダにまとめておくこと．
//! 散らされると探すのが本当に大変になる．
//!
//! @anchor anchor_float
//! @section ●小数について
//! 小数をあらわす事のできる変数の形は float と double の２つがある．
//! float のほうがサイズが小さいので探索に向いている，floatで統一して書いていく．
//! @n float = 4byte，double = 8byte.
//!
//! @anchor anchor_enum
//! @section ●列挙体について
//! 整数型の引数を使って関数の機能を分けたいとき，例えば以下のような関数を使いたいとき，
//! @code
//! //enum_sample1.h
//!
//! #pragma once
//!
//! #include <cassert>
//!
//! namespace error_value
//! {
//! constexpr int kErrorValue = -1;
//! }
//!
//! int CalculateNumber(const int num1, const int num2, const int mode)
//! {
//!     if(mode == 1){ return num1 + num2;}
//!     else if(mode == 2){ return num1 - num2;}
//!     else if(mode == 3){ return num1 * num2;}
//!     else if(mode == 4){ return num1 / num2;}
//!     else{ assert(false); }
//!
//!     return error_value::kErrorValue;
//! }
//! @endcode
//!
//! このような場合は列挙体を使うことをお勧めする．
//! 何故ならば，読みやすくなり，不正な値を入力されづらくなるためである．
//! 例えば以下のように書き換えることができる．
//!
//! @code
//! //enum_sample2.h
//!
//! #pragma once
//!
//! #include <cassert>
//!
//! namespace error_value
//! {
//!     constexpr int kErrorValue = -1;
//! }
//!
//! enum CalculateMode
//! {
//!     kAdd = 1,
//!     kSub = 2,
//!     kMul = 3,
//!     kDiv = 4,
//! };
//!
//! int CalculateNumber(const int num1, const int num2, const CalculateMode mode)
//! {
//!     if(mode == CalculateMode::kAdd){ return num1 + num2;}
//!     else if(mode == CalculateMode::kSub){ return num1 - num2;}
//!     else if(mode == CalculateMode::kMul){ return num1 * num2;}
//!     else if(mode == CalculateMode::kDiv){ return num1 / num2;}
//!     else{ assert(false); }
//!
//!     return error_value::kErrorValue;
//! }
//! @endcode
//! @see designlab::enums::BootMode
//! @see designlab::BootModeSelector
//! @n
//!
//! @anchor anchor_struct
//! @section●構造体について
//! @n C++で書かれているのでわざわざ typedef しなくてもよい．
//! C++においては struct はクラスと同じ扱いになる．
//! (ただし，デフォルトのアクセス指定子が異なる．クラスは private，structは public )
//! @nそのため，structを継承したクラスを作ることもできる．(非推奨だが)
//! @n 構造体を使用する場合は，メンバ変数は全て public にしておき，
//! そのようなクラスを作成したい場合に構造体を使うとよい．
//! @see designlab::ApplicationSettingRecord
//! @see designlab::Vector3
//! @see designlab::Vector2
