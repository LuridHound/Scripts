#include <iostream>
#include <filesystem>


//
//
void rename_if(const std::filesystem::path& path_1, std::string& pattern)
{
    auto ar = std::string(path_1.stem().filename().c_str()).find(pattern);

    if(ar == std::string::npos)
    {
        return;
    }

    std::string temp = std::string(path_1.stem().filename().c_str()).substr(ar + pattern.length());

    std::filesystem::path ho = (path_1.parent_path() / ( temp + std::string(path_1.extension().filename().c_str())));

    std::filesystem::rename(path_1, ho);

    return;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect input. Input must contain the name of the game.";
        return 0;
    }

    std::string pattern = argv[1];

    std::filesystem::path directory = std::filesystem::current_path();

    for (const auto& file : std::filesystem::recursive_directory_iterator{directory} )
    {
        if (is_regular_file(file.path()))
        {
            rename_if(file.path(), pattern);
        }
    }

    return 0;
}
