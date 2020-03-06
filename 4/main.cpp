#include <algorithm>
#include <iterator>
#include <map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <type_traits>

template<typename Iterator, typename T, typename = typename std::enable_if_t<std::is_convertible_v<T,std::string>>>
void writeToFile(Iterator begin, Iterator end, T&& filename);

template<typename FirstContainer, typename SecondContainer>
void analyze(std::filesystem::path&& pathToDirectory, FirstContainer& container, SecondContainer& extensions);

template<typename Container, typename File>
void addRecord(Container& container, File file);

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "You need to input at least one extension.";
        return -1;
    }

    std::vector<std::string> extensions;
    extensions.reserve(argc - 1);

    for (int i = 1; i < argc; ++i)
    {
        extensions.emplace_back(argv[i]);
    }
    
    std::map<std::string, size_t> filenames;

    analyze(std::filesystem::current_path(), filenames, extensions);

    writeToFile(filenames.begin(), filenames.end(), "result.txt");

    return 0;
}


template<typename Iterator, typename T, typename>
void writeToFile(Iterator begin, Iterator end, T&& filename)
{
    std::ofstream file(filename);
    auto writeRecord = [number = 0](auto key) mutable
    {
        return std::string(key.first + " = " + std::to_string(number++) + ",\n");
    };
    std::transform(begin, end, std::ostream_iterator<std::string>(file), writeRecord);

    file.close();
}


template<typename FirstContainer, typename SecondContainer>
void analyze(std::filesystem::path&& pathToDirectory, FirstContainer& container, SecondContainer& extensions)
{
    for (const auto& file :std::filesystem::recursive_directory_iterator{pathToDirectory})
    {
        auto currentExtension = std::string(file.path().extension());

        auto it = std::find(extensions.begin(), extensions.end(), currentExtension);

        if (it != extensions.end())
        {
            addRecord(container, file);
        }
    }
}


template<typename Container, typename File>
void addRecord(Container& container, File file)
{
    auto upperCase = [](std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](auto symbol){
            return toupper(symbol);
        });
    };

    std::string name = file.path().stem().filename().c_str();

    upperCase(name);

    container[name];
}