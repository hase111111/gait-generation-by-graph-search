#include "result_viewer_system_main.h"

#include <filesystem>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "file_tree.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


ResultViewerSystemMain::ResultViewerSystemMain(
	std::unique_ptr<IGraphicMain>&& graphic_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr
) :
	kDirectoryName("result"),
	kFileTreePath(std::filesystem::current_path().string() + "/" + kDirectoryName),
	graphic_system_(std::move(graphic_ptr), setting_ptr)
{
}

void ResultViewerSystemMain::Main()
{
	dlio::OutputTitle("Result Viewer System");

	// GUIを表示する
	boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system_);

	while (true)
	{
		dlio::Output("ファイルを選択してください．", OutputDetail::kSystem);


		// 終了するかどうかを選択

		if (dlio::InputYesNo("終了しますか？"))
		{
			dlio::OutputNewLine(1, OutputDetail::kSystem);

			break;
		}

		dlio::OutputNewLine(1, OutputDetail::kSystem);
	}


	// GUIの終了を待つ
	dlio::OutputHorizontalLine(true, OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("GUIの終了を待っています", OutputDetail::kSystem);

	graphic_thread.join();
}

void ResultViewerSystemMain::Read()
{

}
