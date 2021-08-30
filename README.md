# FbxToLwo - Convert Meshes from FBX to Lightwave LWO

Command-line utility to convert FBX meshes to Lightwave's LWO2 file format, based on the [OpenFBX library](https://github.com/nem0/OpenFBX) and the LWO2 exporter code as used in the [DarkRadiant Level Editor](https://github.com/codereader/DarkRadiant).

Keeps material names, normals and vertex colours intact. Will merge vertices sharing the same set of attributes.

No guarantees whatsover, I just hope it's useful - contributions welcome!

## Single File Usage
> **FbxToLwo** <file1.fbx> <file2.fbx> <...>

Single specified FBX files will be converted to LWO, which will be placed right next to the FBX files.
> Example: **FbxToLwo** c:\temp\model.fbx c:\temp\model2.fbx

## Batch Folder Conversion Usage
> **FbxToLwo** -input path -output path

Every FBX in the input folder and all its child folders will be converted to LWO, which will be placed in the same relative path in the output folder. *Existing files will be overwritten!*
> **FbxToLwo** -input c:\temp\fbx_files -output c:\temp\lwo_files

## Compiling

Open the FbxToLwo.sln (Visual Studio 2019) solution file in the root folder,
compile and run. There are no third-party dependencies except for those already
included in this repository. 

There's no CMake file present yet at the time of writing.

## License

All code is published under GPLv2, except for the OpenFBX libraries which are licensed under the MIT License. See the [LICENSE](https://raw.githubusercontent.com/codereader/FbxToLwo/master/LICENSE) file in this repository.
