#include <iostream>
#include <filesystem>
#include <algorithm>
#include "openfbx/ofbx.h"

#include "export/Lwo2Exporter.h"
#include "FbxSurface.h"

inline ArbitraryMeshVertex ConstructMeshVertex(const ofbx::Geometry& geometry, int index)
{
    auto vertices = geometry.getVertices();
    auto normals = geometry.getNormals();
    auto uvs = geometry.getUVs();
    auto colours = geometry.getColors();

    return ArbitraryMeshVertex(
        Vertex3f(vertices[index].x, vertices[index].y, vertices[index].z),
        normals != nullptr ? Normal3f(normals[index].x, normals[index].y, normals[index].z) : Normal3f(1, 0, 0),
        uvs != nullptr ? TexCoord2f(uvs[index].x, 1.0 - uvs[index].y) : TexCoord2f(0, 0), // invert v
        colours != nullptr ? Vector3(colours[index].x, colours[index].y, colours[index].z) : Vector3(1, 1, 1)
    );
}

void ExportFbxMesh(ofbx::IScene& scene, model::Lwo2Exporter& exporter)
{
    for (int meshIndex = 0; meshIndex < scene.getMeshCount(); ++meshIndex)
    {
        auto mesh = scene.getMesh(meshIndex);
        auto geometry = mesh->getGeometry();

        std::cout << "Exporting FBX Mesh with " << geometry->getVertexCount() << " vertices\n";

        std::vector<model::FbxSurface> surfacesByMaterial(mesh->getMaterialCount());

        if (mesh->getMaterialCount() == 0)
        {
            surfacesByMaterial.emplace_back().material = "Material"; // create at least one surface
        }

        // Assign the surface name for each material
        for (int m = 0; m < mesh->getMaterialCount(); ++m)
        {
            auto material = mesh->getMaterial(m);
            surfacesByMaterial[m].material = material->name;
        }

        auto materials = geometry->getMaterials();
        auto faceIndices = geometry->getFaceIndices();

        for (int i = 0; i < geometry->getIndexCount(); i += 3)
        {
            // Material index is assigned per triangle
            auto polyIndex = i / 3;
            auto materialIndex = materials ? materials[polyIndex] : 0; // put into first material by default

            // Reverse the poly indices to get the CCW order
            auto indexA = (faceIndices[i+2] * -1) - 1; // last index is negative and 1-based
            auto indexB = faceIndices[i+1];
            auto indexC = faceIndices[i+0];

            surfacesByMaterial[materialIndex].addVertex(ConstructMeshVertex(*geometry, indexA));
            surfacesByMaterial[materialIndex].addVertex(ConstructMeshVertex(*geometry, indexB));
            surfacesByMaterial[materialIndex].addVertex(ConstructMeshVertex(*geometry, indexC));
        }

        // Apply the global transformation matrix
        auto t = geometry->getGlobalTransform();
#if 0
        auto transform = Matrix4::byColumns(
            t.m[0], t.m[1], t.m[2], t.m[3],
            t.m[4], t.m[5], t.m[6], t.m[7],
            t.m[8], t.m[9], t.m[10], t.m[11],
            t.m[12], t.m[13], t.m[14], t.m[15]
        );
#endif
        // "Objects in the FBX SDK are always created in the right handed, Y-Up axis system"
        auto transform = Matrix4::getIdentity();
        
        if (scene.getGlobalSettings()->UpAxis == ofbx::UpVector_AxisY)
        {
            transform = transform.getPremultipliedBy(Matrix4::getRotationForEulerXYZDegrees(Vector3(90, 0, 0)));
        }

        std::cout << "Generated " << surfacesByMaterial.size() << " triangulated surfaces\n";

        for (const auto& surface : surfacesByMaterial)
        {
            std::cout << " - " << surface.material << std::endl;
            exporter.addSurface(surface, transform);
        }
    }
}

void ConvertFbxToLwo(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath)
{
    std::ifstream ifs(inputPath, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> content(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(content.data(), pos);

    auto scene = ofbx::load(reinterpret_cast<ofbx::u8*>(content.data()), 
        static_cast<int>(content.size()), (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);

    if (!scene)
    {
        std::cerr << ofbx::getError() << std::endl;
        return;
    }

    try
    {
        auto exporter = std::make_shared<model::Lwo2Exporter>();

        ExportFbxMesh(*scene, *exporter);

        // Ensure the folders exist
        std::filesystem::create_directories(outputPath.parent_path());

        std::cout << "Exporting LWO to " << outputPath.string() << std::endl;
        exporter->exportToPath(outputPath.parent_path().string(), outputPath.filename().string());

        scene->destroy();
    }
    catch (const std::exception&)
    {
        scene->destroy();
        throw;
    }
}

namespace string
{

    std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    }

}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Single File Usage: FbxToLwo <file1.fbx> <file2.fbx> <...>" << std::endl;
        std::cout << "  Single specified FBX files will be converted to LWO, which will be placed right next to the FBX files." << std::endl;
        std::cout << "  Example: FbxToLwo c:\\temp\\model.fbx c:\\temp\\model2.fbx" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Batch Folder Conversion Usage: FbxToLwo -input <path> -output <path>" << std::endl;
        std::cout << "  Every FBX in the input folder and all its child folders will be converted to LWO, which will be placed" << std::endl;
        std::cout << "  in the same relative path in the output folder." << std::endl;
        std::cout << "  Example: FbxToLwo -input c:\\temp\fbx_files -output c:\\temp\\lwo_files" << std::endl;
        return -1;
    }

    std::filesystem::path inputFolder;
    std::filesystem::path outputFolder;

    for (int i = 1; i < argc; ++i)
    {
        if (string::toLower(argv[i]) == "-input")
        {
            if (argc <= i + 1)
            {
                std::cerr << "No input folder specified";
                return -1;
            }

            inputFolder = argv[i + 1];
            ++i;
        }
        else if (string::toLower(argv[i]) == "-output")
        {
            if (argc <= i + 1)
            {
                std::cerr << "No output folder specified";
                return -1;
            }

            outputFolder = argv[i + 1];
            ++i;
        }
    }

    if (inputFolder.empty() ^ outputFolder.empty())
    {
        std::cerr << "Both input and output folders must be specified" << std::endl;
        return -1;
    }

    if (std::filesystem::is_directory(inputFolder) && !outputFolder.empty())
    {
        std::cout << "Batch-converting the FBX files in directory " << inputFolder.string() << " to " << outputFolder.string() << std::endl;

        for (auto i = std::filesystem::recursive_directory_iterator(inputFolder); i != std::filesystem::recursive_directory_iterator(); ++i)
        {
            if (string::toLower(i->path().extension().string()) == ".fbx")
            {
                auto outputPath = outputFolder / std::filesystem::relative(i->path(), inputFolder);
                outputPath.replace_extension("lwo");

                std::cout << "Converting: " << i->path().string() << " => " << outputPath.string() << std::endl;

                ConvertFbxToLwo(i->path(), outputPath);
            }
        }

        return 0;
    }

    for (int i = 1; i < argc; ++i)
    {
        std::filesystem::path inputPath = argv[i];

        try
        {
            if (!std::filesystem::exists(inputPath)) throw std::runtime_error("Path does not exist " + inputPath.string());

            if (std::filesystem::is_regular_file(inputPath))
            {
                std::cout << "Trying to convert file " << inputPath.string() << std::endl;

                std::filesystem::path outputPath = inputPath;
                outputPath.replace_extension("lwo");

                ConvertFbxToLwo(inputPath, outputPath);
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Failed to handle file " << inputPath << ": " << ex.what() << std::endl;
        }
    }

    return 0;
}
