#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace stream
{

/**
 * Stream object used to write data to the a given target directory and filename. 
 * To prevent corruption of a possibly existing target file, it will open a stream
 * to a temporary file for writing first. On calling close(), the temporary stream 
 * will be finalised and the temporary file will be moved over to the target file,
 * which in turn will be renamed to .bak first.
 */
class ExportStream
{
private:
    std::filesystem::path _tempFile;
    std::ofstream _tempStream;
    std::string _outputDirectory;
    std::string _filename;

public:
    // Output stream mode
    enum class Mode
    {
        Text, 
        Binary,
    };

    ExportStream(const std::string& outputDirectory, const std::string& filename, Mode mode = Mode::Text) :
        ExportStream(outputDirectory, filename, mode == Mode::Binary ? std::ios::out | std::ios::binary : std::ios::out)
    {}

    ExportStream(const std::string& outputDirectory, const std::string& filename, std::ios::openmode mode) :
        _outputDirectory(outputDirectory),
        _filename(filename)
    {
        std::filesystem::path targetPath = _outputDirectory;

        if (!std::filesystem::exists(targetPath))
        {
            std::filesystem::create_directories(targetPath);
        }

        // Open a temporary file (leading underscore)
        _tempFile = targetPath / ("_" + _filename);

        _tempStream = std::ofstream(_tempFile.string(), mode);

        if (!_tempStream.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + _tempFile.string());
        }
    }

    // Returns the stream for writing the export data
    std::ofstream& getStream()
    {
        return _tempStream;
    }

    void close()
    {
        _tempStream.close();

        // The full OS path to the output file
        std::filesystem::path targetPath = _outputDirectory;
        targetPath /= _filename;

        if (std::filesystem::exists(targetPath))
        {
            try
            {
                // Move the old target file to .bak (overwriting any existing .bak file)
                std::filesystem::remove(targetPath);
            }
            catch (std::filesystem::filesystem_error& ex)
            {
                throw std::runtime_error("Could not remove the existing file " + targetPath.string() + ": " + ex.what());
            }
        }

        try
        {
            std::filesystem::rename(_tempFile, targetPath);
        }
        catch (std::filesystem::filesystem_error&)
        {
            throw std::runtime_error("Could not rename the temporary file: " + _tempFile.string());
        }
    }
};

}
