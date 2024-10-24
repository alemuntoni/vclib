#ifndef LOAD_BIMBA_AND_BUNNY_H
#define LOAD_BIMBA_AND_BUNNY_H

#ifndef VCLIB_WITH_MODULES
#include <vclib/algorithms/mesh/update.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#else
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
import vclib;
#endif

void loadBimbaAndBunnyMeshes(vcl::TriMesh& m1, vcl::TriMesh& m2)
{
    vcl::ConsoleLogger log;
    vcl::MeshInfo      loadedInfo;

    log.startTimer();
    m1 = vcl::load<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bimba_bin.stl", loadedInfo, log);

    log.startTimer();
    m2 = vcl::load<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny_simplified.stl", loadedInfo, log);
}

#endif // LOAD_BIMBA_AND_BUNNY_H
