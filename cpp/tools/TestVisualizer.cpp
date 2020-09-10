// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <thread>

#include "open3d/Open3D.h"
#include "tools/ManuallyAlignPointCloud/VisualizerForAlignment.h"

void PrintTransformation(const Eigen::Matrix4d &transformation) {
    using namespace open3d;
    utility::LogInfo("Current transformation is:");
    utility::LogInfo("\t{:.6f} {:.6f} {:.6f} {:.6f}", transformation(0, 0),
                     transformation(0, 1), transformation(0, 2),
                     transformation(0, 3));
    utility::LogInfo("\t{:.6f} {:.6f} {:.6f} {:.6f}", transformation(1, 0),
                     transformation(1, 1), transformation(1, 2),
                     transformation(1, 3));
    utility::LogInfo("\t{:.6f} {:.6f} {:.6f} {:.6f}", transformation(2, 0),
                     transformation(2, 1), transformation(2, 2),
                     transformation(2, 3));
    utility::LogInfo("\t{:.6f} {:.6f} {:.6f} {:.6f}", transformation(3, 0),
                     transformation(3, 1), transformation(3, 2),
                     transformation(3, 3));
}

void PrintHelp() {
    using namespace open3d;
    // PrintOpen3DVersion();
    // clang-format off
    utility::LogInfo("Usage:");
    utility::LogInfo("    > ManuallyAlignPointCloud source_file target_file [options]");
    utility::LogInfo("      Manually align point clouds in source_file and target_file.");
    utility::LogInfo("");
    utility::LogInfo("Options:");
    utility::LogInfo("    --help, -h                : Print help information.");
    utility::LogInfo("    --verbose n               : Set verbose level (0-4).");
    utility::LogInfo("    --voxel_size d            : Set downsample voxel size.");
    utility::LogInfo("    --max_corres_distance d   : Set max correspondence distance.");
    utility::LogInfo("    --without_scaling         : Disable scaling in transformations.");
    utility::LogInfo("    --without_dialog          : Disable dialogs. Default files will be used.");
    utility::LogInfo("    --without_gui_icp file    : The program runs as a console command. No window");
    utility::LogInfo("                                will be created. The program reads an alignment");
    utility::LogInfo("                                from file. It does cropping, downsample, ICP,");
    utility::LogInfo("                                then saves the alignment session into file.");
    utility::LogInfo("    --without_gui_eval file   : The program runs as a console command. No window");
    utility::LogInfo("                                will be created. The program reads an alignment");
    utility::LogInfo("                                from file. It does cropping, downsample,");
    utility::LogInfo("                                evaluation, then saves everything.");
    // clang-format on
}

int main(int argc, char **argv) {
    using namespace open3d;

    

    visualization::VisualizerWithEditingFoo visualizer;

    visualizer.CreateVisualizerWindow("Source Point Cloud", 1280, 720, 10, 100);
    visualizer.BuildUtilities();
    
    while (visualizer.PollEvents()){}

    visualizer.DestroyVisualizerWindow();
    return 0;
}
