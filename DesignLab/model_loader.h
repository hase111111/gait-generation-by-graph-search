
//! @file      model_loader.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MODEL_LOADER_H_
#define DESIGNLAB_MODEL_LOADER_H_

#include <string>
#include <map>

#include "singleton.h"


namespace designlab
{

//! @class ModelLoader
//! @brief Dxlibの3Dモデルを読み込むクラス．
//! @details シングルトンであり，ModelLoader::GetIns()でインスタンスを取得する．
//! @n 呼び出すときはModelLoader::GetIns()->GetModelHandle(file_path)とする．
//! @code
//! // 例
//!
//! // モデルのファイルパス
//! std::string file_path = "data/model/robot/robot.x";
//!
//! // モデルのハンドル番号
//! int model_handle = ModelLoader::GetIns()->GetModelHandle(file_path);
//! @endcode
//! @see Singleton
//! @attention このクラスを使用するときは，必ず DxLib_Init() の後に使用すること．
//! また，DxLib_End() の前に使用を終了すること．
//! これらを守らないと，アプリがクラッシュする．
class ModelLoader final : public Singleton<ModelLoader>
{
public:
    //! @brief Dxlibは3Dモデルを描画する際に，モデルのハンドルを指定する．
    //! @n モデルがまだ読み込まれていない場合は，モデルを読み込んでから，ハンドル番号を返す．
    //! @n すでに読み込みずみのモデルを読み込んだ場合は，ハンドル番号を返す．
    //! @param[in] file_path モデルのファイルパス.
    //! @return モデルのハンドル番号．失敗したら-1を返す．成功値は正の値．
    [[nodiscard]] int GetModelHandle(const std::string& file_path);

private:
    friend Singleton<ModelLoader>;
    ModelLoader() = default;
    ~ModelLoader() = default;
    ModelLoader(const ModelLoader& r) = default;
    ModelLoader& operator=(const ModelLoader& r) = default;

    //! 1フレーム当たりにかかる時間(ミリ秒)
    std::map<std::string, int> model_handle_map_;
};

}  // namespace designlab


#endif  // DESIGNLAB_MODEL_LOADER_H_
