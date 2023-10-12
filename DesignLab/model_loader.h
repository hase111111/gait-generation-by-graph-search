//! @file model_loader.h
//! @brief Dxlibの3Dモデルを読み込むクラス

#ifndef DESIGNLAB_MODEL_LOADER_H_
#define DESIGNLAB_MODEL_LOADER_H_

#include <string>
#include <map>

#include "singleton.h"


//! @class ModelLoader
//! @brief Dxlibの3Dモデルを読み込むクラス．呼び出すときはModelLoader::GetIns()->LoadModel(file_path)とする．
class ModelLoader final : public Singleton<ModelLoader>
{
public:
	
	//! @brief モデルを読み込む．すでに読み込みずみのモデルを読み込んだ場合は，ハンドル番号を返す．
	//! @param [in] file_path モデルのファイルパス
	//! @return int モデルのハンドル番号．失敗したら-1を返す．成功値は正の値．
	int LoadModel(const std::string file_path);

private:

	friend Singleton<ModelLoader>;
	ModelLoader() = default;
	~ModelLoader() = default;
	ModelLoader(const ModelLoader& r) = default;
	ModelLoader& operator=(const ModelLoader& r) = default;

	std::map<std::string , int> model_handle_map_;	//!< 読み込み済みのモデルのファイルパスとハンドル番号の対応表
};


#endif