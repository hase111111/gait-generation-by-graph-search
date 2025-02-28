
# GaitPatternGeneratorの作り方

## はじめに

GaitPatternGenerator（歩容生成器，GPG）クラスの作り方について説明します．
本プロジェクトでは`GaitPatternGeneratorBasic`や，`GaitPatternGeneratorSwitchMove`などさまざまなクラスが実装されています．
ある条件によって，生成する歩容（＋ロボットの動作，回転，姿勢変更など）を変えたい場合は，新しいGPGクラスを作成することが必要です．
まず，どのような仕組みによってGPGで歩容が生成されるのかを説明し，その後にGPGクラスの作り方について説明します．

<details>
<summary>その前に...</summary>

以下の用語の意味を理解したうえで，このページを読むことをオススメします．

- **クラス**: C言語で言うところの構造体（struct）です．しかし，内部に関数を持つことができます．（ちなみにC++の構造体はクラスとほとんど機能が同じで内部に関数を持てます，ややこしいですね）
- **継承**: あるクラスを元にして新しいクラスを作ることです．新しいクラスは元のクラスの変数や関数を引き継ぎます．
- **純粋仮想関数**: 継承元のクラスが持つ関数で，継承先のクラスで必ず実装（override）しなければならない関数です．
- **実装**: ある関数の処理の内容を書くことです．多くの場合，宣言はhファイルに書き，実装はcppファイルに書きます．
- **インクルードガード**: ヘッダファイルの先頭に記述する，同じファイルを複数回インクルードしないための記述です．書式としては以下のようになります．

```cpp
#ifndef ファイル名_H_
#define ファイル名_H_

// ここにヘッダファイルの内容を書く

#endif  // ファイル名_H_
```

- **コピー渡し**: 関数の引数に変数を渡すことです．関数内で引数の値を変更しても，元の変数の値は変わりません．通常C++ではすべてコピー渡しです．値を渡したいだけならばこれで十分ですが，関数内で変数の値を変更したい場合や，巨大な変数を渡す場合はポインタ渡しや参照渡しを使います．

```cpp
#include <iostream>

void AddOne(int x)
{
    x += 1;
}

int main()
{
    int a = 0;
    AddOne(a);
    std::cout << a << std::endl;  // 0
}
```

- **ポインタ渡し**: 関数の引数にポインタを渡すことです．ポインタは変数のアドレスを示すもので，関数内でそのアドレスにアクセスできます．次のような例で使用します．

```cpp
#include <iostream>

void AddOne(int* x)
{
    *x += 1;
}

int main()
{
    int a = 0;
    AddOne(&a);
    std::cout << a << std::endl;  // 1
}
```

- **参照渡し**: 関数の引数に参照を渡すことです．参照はポインタと同じように変数のアドレスを示しますが，ポインタと違い，アドレス演算子（`&`）を使わずにアクセスできます．ポインタ渡しと比べて安全なので多用します．次のような例で使用します．

```cpp
#include <iostream>

void AddOne(int& x)
{
    x += 1;
}

int main()
{
    int a = 0;
    AddOne(a);
    std::cout << a << std::endl;  // 1
}
```

</details>

## GPGの仕組み

サンプルコードとして，一番シンプルな機能を持っている`GaitPatternGeneratorBasic`クラスを説明します．
不要なコメントは省略しています．

<details>
<summary>gait_pattern_generator_basic.hのコード</summary>

```cpp:gait_pattern_generator_basic.h
#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_

#include <memory>
#include <vector>

#include "gait_pattern_graph_tree.h"
#include "graph_tree_creator.h"
#include "interface_gait_pattern_generator.h"
#include "interface_graph_searcher.h"
#include "interpolated_node_creator.h"
#include "robot_state_node.h"


namespace designlab
{

class GaitPatternGeneratorBasic final : public IGaitPatternGenerator
{
public:
    GaitPatternGeneratorBasic(
      std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
      std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
      int max_depth,
      int max_node_num);

    ~GaitPatternGeneratorBasic() = default;


    GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map_ref,
      const RobotOperation& operation,
      RobotStateNode* output_node) override;

private:
    const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;
    const std::unique_ptr<const IGraphSearcher> graph_searcher_ptr_;
    GaitPatternGraphTree graph_tree_;
    const int max_depth_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_
```

</details>
<br>

このように，`GaitPatternGeneratorBasic`クラスは`IGaitPatternGenerator`クラスを継承しています．
`GaitPatternGeneratorBasic`クラスはメンバ変数として，歩容パターングラフを作成するクラスの`GraphTreeCreator`，
グラフ探索を行うクラスの`IGraphSearcher`，作成したグラフを格納する`GaitPatternGraphTree`，最大探索深さ`max_depth`を持っています．



ちなみに，`IGaitPatternGenerator`クラスは以下のようになっています．

<details>
<summary>interface_gait_pattern_generator.hのコード</summary>

```cpp:interface_gait_pattern_generator.h
#ifndef DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_
#define DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_

#include <vector>

#include "graph_search_result_record.h"
#include "map_state.h"
#include "robot_state_node.h"
#include "robot_operation.h"


namespace designlab
{

class IGaitPatternGenerator
{
public:
    IGaitPatternGenerator() = default;
    virtual ~IGaitPatternGenerator() = default;

    virtual GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map,
      const RobotOperation& operation,
      RobotStateNode* output_node_ptr) = 0;
};

}  // namespace designlab

#endif  // DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_
```

</details>

`IGaitPatternGenerator`クラスは純粋仮想関数`GetNextNodeByGraphSearch`を持っています．
そのため，`GaitPatternGeneratorBasic`クラスは`GetNextNodeByGraphSearch`関数を必ず実装しなければなりません．

この関数は，現在のノード（`current_node`），地図情報（`map`），ロボットの操作情報（`operation`）を受け取り，次のノードを返す関数です．
次のノードは引数の`output_node_ptr`に格納され，ポインタ渡しします．
関数の戻り値は`GraphSearchResult`型で，次のノードが見つかった場合は成功，見つからなかった場合は失敗を返します．

## 新しいファイルの作り方

