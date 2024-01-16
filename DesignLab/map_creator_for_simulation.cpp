
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "map_creator_for_simulation.h"

#include <algorithm>
#include <random>

#include "cassert_define.h"
#include "math_util.h"


namespace designlab
{

MapCreatorForSimulation::MapCreatorForSimulation(const SimulationMapParameter& messenger) :
    messenger_(messenger)
{
    // map_min_x が map_max_x より小さいことを確認する．
    assert(messenger_.map_min_x < messenger_.map_max_x);

    // map_min_y が map_max_y より小さいことを確認する．
    assert(messenger_.map_min_y < messenger_.map_max_y);
}

MapState MapCreatorForSimulation::InitMap()
{
    std::vector<Vector3> map_data;

    switch (messenger_.mode)
    {
        case enums::SimulationMapMode::kFlat:
        {
            CreateFlatMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kVerticalStripe:
        {
            CreateVerticalStripeMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kHorizontalStripe:
        {
            CreateHorizontalStripeMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kDiagonalStripe:
        {
            CreateDiagonalStripeMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kMesh:
        {
            CreateMeshMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kLatticePoint:
        {
            CreateLatticePointMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kCircle:
        {
            CreateCircleMap(&map_data);
            break;
        }
        case enums::SimulationMapMode::kDonut:
        {
            CreateDonutMap(&map_data);
            break;
        }
        default:
        {
            // 異常な値が入力されたら，平面のマップを生成する．
            CreateFlatMap(&map_data);
            break;
        }
    }

    // オプション指定に基づき，Z座標を変更する．

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kPerforated))
    {
        // 穴あき地形にする．
        ChangeMapToPerforated(&map_data);
    }

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kStep))
    {
        // 階段状にする．
        ChangeMapToStep(&map_data);
    }

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kSlope))
    {
        // 坂道にする．
        ChangeMapToSlope(&map_data);
    }

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kTilt))
    {
        // 坂道にする．
        ChangeMapToTilt(&map_data);
    }

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kRough))
    {
        // デコボコにする．
        ChangeMapToRough(&map_data);
    }

    if (messenger_.option & static_cast<unsigned int>(enums::SimulationMapOption::kRadiation))
    {
        // 放射状に穴をあける．
        ChangeMapToRadial(&map_data);
    }

    return MapState(map_data);
}

void MapCreatorForSimulation::UpdateMap([[maybe_unused]] MapState* current_map)
{
    // マップを更新する必要がないので，何もしない．
}


void MapCreatorForSimulation::CreateFlatMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める．
    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // ロボットの正面方向．
            const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

            // ロボットの側面方向．
            const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

            map->push_back({ x_pos, y_pos, messenger_.base_z });  // 脚設置可能点を追加する．
        }
    }
}

void MapCreatorForSimulation::CreateVerticalStripeMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める．
    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // 縦じまをつくるために，一定間隔ごとに追加する．最初の待機場所の座標ならば無条件に追加する．
            const float x_rough = (messenger_.map_start_rough_x - messenger_.map_min_x) /
                MapState::kMapPointDistance;

            if (y % (messenger_.stripe_interval * 2) < messenger_.stripe_interval || x < x_rough)
            {
                // ロボットの正面方向．
                const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

                // ロボットの側面方向．
                const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

                map->push_back({ x_pos, y_pos, messenger_.base_z });  // 脚設置可能点を追加する．
            }
        }
    }
}

void MapCreatorForSimulation::CreateHorizontalStripeMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める．
    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // 縦じまをつくるために，一定間隔ごとに追加する．最初の待機場所の座標ならば無条件に追加する．
            const float x_rough = (messenger_.map_start_rough_x - messenger_.map_min_x) /
                MapState::kMapPointDistance;

            if (x % (messenger_.stripe_interval * 2) < messenger_.stripe_interval || x < x_rough)
            {
                // ロボットの正面方向．
                const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

                // ロボットの側面方向．
                const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

                map->push_back({ x_pos, y_pos, messenger_.base_z });  // 脚設置可能点を追加する．
            }
        }
    }
}

void MapCreatorForSimulation::CreateDiagonalStripeMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める．
    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // 斜めじまをつくるために，一定間隔ごとに追加する．最初の待機場所の座標ならば無条件に追加する．
            const bool x_in_stripe = x % (messenger_.stripe_interval * 2) <
                messenger_.stripe_interval;

            const bool y_in_stripe = y % (messenger_.stripe_interval * 2) <
                messenger_.stripe_interval;

            const bool do_create_map = x_in_stripe == y_in_stripe;

            const float x_rough = (messenger_.map_start_rough_x - messenger_.map_min_x) /
                MapState::kMapPointDistance;

            if (do_create_map || x < x_rough)
            {
                // ロボットの正面方向．
                const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

                // ロボットの側面方向．
                const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

                map->push_back({ x_pos, y_pos, messenger_.base_z });  // 脚設置可能点を追加する．
            }
        }
    }
}

void MapCreatorForSimulation::CreateMeshMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの xとyの存在範囲全体に脚設置可能点を敷き詰める．
    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // 網目模様をつくるために，一定間隔ごとに追加する．
            // 最初の待機場所の座標ならば無条件に追加する．
            bool do_create_map;

            if ((x % (messenger_.stripe_interval * 2) < messenger_.stripe_interval))
            {
                if ((y % (messenger_.stripe_interval * 2) < messenger_.stripe_interval))
                {
                    do_create_map = true;
                }
                else
                {
                    do_create_map = false;
                }
            }
            else
            {
                do_create_map = true;
            }

            const float x_rough = (messenger_.map_start_rough_x - messenger_.map_min_x) /
                MapState::kMapPointDistance;

            if (do_create_map || x < x_rough)
            {
                // ロボットの正面方向．
                const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

                // ロボットの側面方向．
                const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

                map->push_back({ x_pos, y_pos, messenger_.base_z });  // 脚設置可能点を追加する．
            }
        }
    }
}

void MapCreatorForSimulation::CreateLatticePointMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．
    assert(map->empty());    // map が空であることを確認する．

    // マップの x と y の存在範囲全体に脚設置可能点を敷き詰める．

    const float x_max = (messenger_.map_max_x - messenger_.map_min_x) / MapState::kMapPointDistance;
    const float y_max = (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance;

    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            // 網目模様をつくるために，一定間隔ごとに追加する．
            // 最初の待機場所の座標ならば無条件に追加する．
            bool do_create_map = false;

            if ((x % (messenger_.stripe_interval * 2) < messenger_.stripe_interval))
            {
                if ((y % (messenger_.stripe_interval * 2) >= messenger_.stripe_interval))
                {
                    do_create_map = false;
                }
                else
                {
                    do_create_map = true;
                }
            }
            else
            {
                do_create_map = false;
            }

            float x_rough = (messenger_.map_start_rough_x - messenger_.map_min_x) /
                MapState::kMapPointDistance;

            if (do_create_map || x < x_rough)
            {
                // ロボットの正面方向．
                const float x_pos = messenger_.map_min_x + x * MapState::kMapPointDistance;

                // ロボットの側面方向．
                const float y_pos = messenger_.map_min_y + y * MapState::kMapPointDistance;

                // 脚設置可能点を追加する．
                map->push_back({ x_pos, y_pos, messenger_.base_z });
            }
        }
    }
}

void MapCreatorForSimulation::CreateCircleMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    // 円が存在する範囲に脚設置可能点を敷き詰める．
    // その後，円の外側の脚設置可能点を削除する．

    const float x_min = messenger_.circle_center.x - messenger_.circle_radius;
    const float x_max = messenger_.circle_center.x + messenger_.circle_radius;
    const float y_min = messenger_.circle_center.y - messenger_.circle_radius;
    const float y_max = messenger_.circle_center.y + messenger_.circle_radius;

    for (int x = 0; x < (x_max - x_min) / MapState::kMapPointDistance; ++x)
    {
        for (int y = 0; y < (y_max - y_min) / MapState::kMapPointDistance; ++y)
        {
            // ロボットの正面方向．
            const float x_pos = x_min + x * MapState::kMapPointDistance;

            // ロボットの側面方向．
            const float y_pos = y_min + y * MapState::kMapPointDistance;

            // 脚設置可能点を追加する．
            const float distance = Vector2(x_pos, y_pos).GetDistanceFrom(messenger_.circle_center.ProjectedXY());

            if (distance <= messenger_.circle_radius)
            {
                map->push_back({ x_pos, y_pos, messenger_.base_z });
            }
        }
    }
}

void MapCreatorForSimulation::CreateDonutMap(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    // ドーナツが存在する範囲に脚設置可能点を敷き詰める．
    // その後，ドーナツの外側の脚設置可能点を削除する．

    const float x_min = messenger_.circle_center.x - messenger_.circle_radius;
    const float x_max = messenger_.circle_center.x + messenger_.circle_radius;
    const float y_min = messenger_.circle_center.y - messenger_.circle_radius;
    const float y_max = messenger_.circle_center.y + messenger_.circle_radius;

    for (int x = 0; x < (x_max - x_min) / MapState::kMapPointDistance; ++x)
    {
        for (int y = 0; y < (y_max - y_min) / MapState::kMapPointDistance; ++y)
        {
            // ロボットの正面方向．
            const float x_pos = x_min + x * MapState::kMapPointDistance;

            // ロボットの側面方向．
            const float y_pos = y_min + y * MapState::kMapPointDistance;

            // 脚設置可能点を追加する．
            const float distance = Vector2(x_pos, y_pos).GetDistanceFrom(messenger_.circle_center.ProjectedXY());

            if (messenger_.donut_radius <= distance && distance <= messenger_.circle_radius)
            {
                map->push_back({ x_pos, y_pos, messenger_.base_z });
            }
        }
    }
}

void MapCreatorForSimulation::ChangeMapToPerforated(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    // 厳密にホール率に合わせるために，まずはマップを stripe_interval に合わせて区切って，
    // 全部で何マスあるか調べる．
    const int cell_num_x = static_cast<int>(
        (messenger_.map_max_x - messenger_.map_start_rough_x) / MapState::kMapPointDistance) /
        messenger_.stripe_interval;

    const int cell_num_y = static_cast<int>(
        (messenger_.map_max_y - messenger_.map_min_y) / MapState::kMapPointDistance) /
        messenger_.stripe_interval;

    const int cell_sum = cell_num_x * cell_num_y;

    // マスの数だけ要素を持つ vector を用意する．値は全て false で初期化する．
    std::vector<bool> do_perforated(cell_sum, false);

    // ホール率に合わせて，値を true に変更する．
    const int hole_num = cell_sum * messenger_.hole_rate / 100;

    for (int i = 0; i < hole_num; i++)
    {
        do_perforated.at(i) = true;
    }

    // ランダムなホールにするために要素の順番をシャッフルする．
    std::shuffle(std::begin(do_perforated), std::end(do_perforated),
                 std::default_random_engine());

    // マップに穴をあける．
    for (auto itr = (*map).begin(); itr != (*map).end();)
    {
        // 待機場所の外に対してのみ作業をする．
        if ((*itr).x < messenger_.map_start_rough_x)
        {
            itr++;
            continue;
        }

        // マスで区切るとどこに位置するかを調べる．
        const int cell_pos_x = static_cast<int>(
            ((*itr).x - messenger_.map_start_rough_x) / MapState::kMapPointDistance) /
            messenger_.stripe_interval;

        const int cell_pos_y = static_cast<int>(
            ((*itr).y - messenger_.map_min_y) / MapState::kMapPointDistance) /
            messenger_.stripe_interval;

        const int cell_index = cell_pos_x * cell_num_y + cell_pos_y;

        // cell_pos の値がおかしくないかチェックする．
        if (0 <= cell_index && cell_index < do_perforated.size())
        {
            // 穴あけをする場所ならば．
            if (do_perforated[cell_index])
            {
                // 脚設置可能点を消してイテレータを更新する．
                itr = (*map).erase(itr);
            }
            else
            {
                // 消さないならば次へ移動する．
                itr++;
            }
        }
        else
        {
            // 消さずに次へ移動する．
            itr++;
        }
    }
}

void MapCreatorForSimulation::ChangeMapToStep(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    for (auto& i : *map)
    {
        // 待機場所の外に対してのみ作業をする．
        if (i.x > messenger_.map_start_rough_x)
        {
            // 階段の何段目かを計算する．待機場所のすぐ上が1段目なので1を足している．
            const int step_count = 1 + static_cast<int>((i.x - messenger_.map_start_rough_x) /
                                                        messenger_.step_length);

            // 階段状にZ座標を変更する．
            i.z += messenger_.step_height * step_count;
        }
    }
}

void MapCreatorForSimulation::ChangeMapToSlope(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    for (auto& i : *map)
    {
        // 待機場所の外に対してのみ作業をする．
        if (i.x > messenger_.map_start_rough_x)
        {
            // 階段状にZ座標を変更する．
            i.z += (i.x - messenger_.map_start_rough_x) *
                tan(math_util::ConvertDegToRad(messenger_.slope_angle));
        }
    }
}

void MapCreatorForSimulation::ChangeMapToTilt(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    for (auto& i : *map)
    {
        // 待機場所の外に対してのみ作業をする
        if (i.x > messenger_.map_start_rough_x)
        {
            // 階段状にZ座標を変更する．
            i.z += i.y * tan(math_util::ConvertDegToRad(messenger_.tilt_angle));
        }
    }
}

void MapCreatorForSimulation::ChangeMapToRough(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    // まずはマップを STRIPE_INTERVAL にあわせて区切って，全部で何マスあるか調べる．
    const int cell_num_x = static_cast<int>(
        (messenger_.map_start_rough_x - messenger_.map_min_x) /
        MapState::kMapPointDistance) / messenger_.stripe_interval;

    const int cell_num_y = static_cast<int>(
        (messenger_.map_max_y - messenger_.map_min_y) /
        MapState::kMapPointDistance) / messenger_.stripe_interval;

    const int cell_sum = cell_num_x * cell_num_y;

    // マスの数だけ要素を持つ vector を用意する．
    std::vector<float> change_z_length;

    for (int i = 0; i < cell_sum; i++)
    {
        // ランダムなZ座標を入れる．
        change_z_length.push_back(math_util::GenerateRandomNumber(
            messenger_.rough_min_height,
            messenger_.rough_max_height));
    }

    for (auto& i : *map)
    {
        if (i.x <= messenger_.map_start_rough_x)
        {
            continue;
        }

        // マスで区切るとどこに位置するかを調べる．
        const int cell_pos_x = static_cast<int>((i.x - messenger_.map_start_rough_x) /
            MapState::kMapPointDistance) /
            messenger_.stripe_interval;

        const int cell_pos_y = static_cast<int>((i.y - messenger_.map_min_y) /
            MapState::kMapPointDistance) /
            messenger_.stripe_interval;

        const int cell_index = cell_pos_x * cell_num_y + cell_pos_y;

        // cell_index の値がおかしくないかチェックする．
        if (0 <= cell_index && cell_index < change_z_length.size())
        {
            i.z += change_z_length[cell_index];
        }
    }
}

void MapCreatorForSimulation::ChangeMapToRadial(std::vector<Vector3>* map) const
{
    assert(map != nullptr);  // map が nullptr でないことを確認する．

    const float divided_angle = std::numbers::pi_v<float> / messenger_.radial_division;

    for (auto itr = (*map).begin(); itr != (*map).end();)
    {
        // 放射状の穴あけの中心からの角度を計算する．
        const float angle = atan2((*itr).y - messenger_.radial_center.y, (*itr).x - messenger_.radial_center.x) +
            std::numbers::pi_v<float> +math_util::ConvertDegToRad(messenger_.radial_angle_offset);

        if (static_cast<int>(angle / divided_angle) % 2 == 1)
        {
            const int i = static_cast<int>(angle / divided_angle);  // 何番目の角度かを計算する．
            const float angle_dif = angle - i * divided_angle;      // 何番目の角度からの差を計算する．

            // 角度の差がホール率より小さい場合は消す．
            if (angle_dif < divided_angle * messenger_.radial_hole_rate / 100)
            {
                // 脚設置可能点を消してイテレータを更新する．
                itr = (*map).erase(itr);
            }
            else
            {
                // 消さないならば次へ移動する．
                itr++;
            }
        }
        else
        {
            // 消さないならば次へ移動する．
            itr++;
        }
    }
}

}  // namespace designlab
