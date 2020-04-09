#include <iostream>
#include <filesystem>


void rename_if(const std::filesystem::path& pathToFile, std::string& packName);


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect input. Input must contain the name of the pack.";
        return 0;
    }

    std::string packName = argv[1];

    std::filesystem::path directory = std::filesystem::current_path();

    for (const auto& file : std::filesystem::recursive_directory_iterator{directory} )
    {
        if (is_regular_file(file.path()))
        {
            rename_if(file.path(), packName);
        }
    }

    return 0;
}


void rename_if(const std::filesystem::path& pathToFile, std::string& packName)
{
    auto offset = std::string(pathToFile.stem().filename().c_str()).find(packName);

    if (offset == std::string::npos)
    {
        return;
    }

    std::string filename = pathToFile.stem().filename().c_str();

    std::string right = filename.substr(offset + packName.length());

    std::string left = filename.substr(0, offset);

    std::filesystem::path ho = (pathToFile.parent_path() / ( left.append(right.append(pathToFile.extension().filename().c_str()))));

    std::filesystem::rename(pathToFile, ho);
}