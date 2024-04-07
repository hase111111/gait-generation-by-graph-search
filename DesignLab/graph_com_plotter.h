
/// @file      graph_com_plotter.h
/// @author    hasegawa
/// @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPH_COM_PLOTTER_H_
#define DESIGNLAB_GRAPH_COM_PLOTTER_H_

#include "interface_dxlib_3d_renderer.h"

namespace designlab
{

class GraphComPlotter : public IDxlib3dRenderer
{
public:
    GraphComPlotter() = default;
    ~GraphComPlotter() = default;

    void Draw() const override;
};

}  // namespace designlab

#endif  // DESIGNLAB_GRAPH_COM_PLOTTER_H_
