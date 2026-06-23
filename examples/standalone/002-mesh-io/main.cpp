/*****************************************************************************
 * VCLib Standalone Example 002: Mesh I/O
 *
 * This standalone example demonstrates how to load and save meshes using
 * VCLib when included via FetchContent.
 *
 * It shows:
 * - Loading meshes from different file formats (OBJ, PLY)
 * - Saving meshes in various formats
 * - Querying mesh information
 * - Using LoadSettings and SaveSettings
 *
 * Build instructions:
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_BUILD_TYPE=Release
 *   cmake --build . --config Release
 ****************************************************************************/

#include <iostream>
#include <filesystem>
#include <vclib/meshes.h>
#include <vclib/io.h>
#include <vclib/version.h>

namespace fs = std::filesystem;

int main()
{
    std::cout << "=== VCLib Standalone Example 002: Mesh I/O ===" << std::endl;
    std::cout << "VCLib Version: " << VCLIB_VERSION_STRING << std::endl;
    std::cout << std::endl;

    // Create output directory
    fs::path outputDir = fs::current_path() / "standalone_002_output";
    fs::create_directories(outputDir);

    std::cout << "--- Creating a Test Mesh ---" << std::endl;

    // Create a simple cube mesh for testing
    vcl::TriMesh cube = vcl::createCube<vcl::TriMesh>();

    std::cout << "  Cube vertices: " << cube.vertexCount() << std::endl;
    std::cout << "  Cube faces: " << cube.faceCount() << std::endl;
    std::cout << std::endl;

    // Save in different formats
    std::cout << "--- Saving Mesh in Various Formats ---" << std::endl;

    fs::path objPath = outputDir / "cube.obj";
    fs::path plyPath = outputDir / "cube.ply";
    fs::path stlPath = outputDir / "cube.stl";
    fs::path offPath = outputDir / "cube.off";
    try {
        // Save as OBJ (ASCII)
        vcl::saveMesh(cube, objPath.string());
        std::cout << "  Saved: " << objPath.string() << std::endl;

        // Save as PLY (binary)

        vcl::MeshInfo saveInfo;
        saveInfo.setPerVertexPosition();
        saveInfo.setPerVertexNormal();
        saveInfo.setFaces(true);

        vcl::SaveSettings saveSettings;
        saveSettings.info = saveInfo;
        saveSettings.binary = true;
        vcl::saveMesh(cube, plyPath.string(), saveSettings);
        std::cout << "  Saved: " << plyPath.string() << " (binary PLY)" << std::endl;

        // Save as STL (binary)
        vcl::saveMesh(cube, stlPath.string());
        std::cout << "  Saved: " << stlPath.string() << std::endl;

        // Save as OFF
        vcl::saveMesh(cube, offPath.string());
        std::cout << "  Saved: " << offPath.string() << std::endl;

        std::cout << "  ✓ All formats saved successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  Error saving mesh: " << e.what() << std::endl;
        return 1;
    }
    std::cout << std::endl;

    // Load the saved meshes back
    std::cout << "--- Loading Saved Meshes ---" << std::endl;

    try {
        // Load OBJ
        vcl::TriMesh loadedObj;
        vcl::MeshInfo objInfo;
        vcl::loadMesh(loadedObj, objPath.string(), objInfo);
        std::cout << "  Loaded OBJ:" << std::endl;
        std::cout << "    Vertices: " << loadedObj.vertexCount() << std::endl;
        std::cout << "    Faces: " << loadedObj.faceCount() << std::endl;
        std::cout << "    Has positions: " << objInfo.hasPerVertexPosition() << std::endl;

        // Load PLY
        vcl::TriMesh loadedPly;
        vcl::MeshInfo plyInfo;
        vcl::loadMesh(loadedPly, plyPath.string(), plyInfo);
        std::cout << "  Loaded PLY:" << std::endl;
        std::cout << "    Vertices: " << loadedPly.vertexCount() << std::endl;
        std::cout << "    Faces: " << loadedPly.faceCount() << std::endl;

        // Load STL
        vcl::TriMesh loadedStl;
        vcl::loadMesh(loadedStl, stlPath.string());
        std::cout << "  Loaded STL:" << std::endl;
        std::cout << "    Vertices: " << loadedStl.vertexCount() << std::endl;
        std::cout << "    Faces: " << loadedStl.faceCount() << std::endl;

        std::cout << "  ✓ All meshes loaded successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  Error loading mesh: " << e.what() << std::endl;
        return 1;
    }
    std::cout << std::endl;

    // Clean up output directory
    std::cout << "--- Cleanup ---" << std::endl;
    fs::remove_all(outputDir);
    std::cout << "  Output directory removed." << std::endl;
    std::cout << std::endl;

    std::cout << "=== Example completed successfully! ===" << std::endl;

    return 0;
}
