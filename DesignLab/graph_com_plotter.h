
//! @file graph_com_plotter.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

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
