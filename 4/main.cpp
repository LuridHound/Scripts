#include <iostream>
#include <fstream>

#include <algorithm>

#include <iterator>
#include <set>
#include <map>
#include <queue>

#include <filesystem>

#include <type_traits>


template<typename Iterator, typename T, typename = typename std::enable_if_t<std::is_same_v<T,std::ofstream>>>
void writeToFile(Iterator begin, Iterator end, T& filename);

template<typename FirstContainer>
void analyze(std::filesystem::path&& pathToDirectory, FirstContainer& container);

template<typename Container, typename File>
void addRecord(Container& container, File file, std::string);


int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "You need to input at least one extension.";
        return -1;
    }

    std::map<std::string, std::set<std::string>> filenames;

    for (int i = 1; i < argc; ++i)
    {
        filenames[argv[i]];
    }

    analyze(std::filesystem::current_path(), filenames);

    std::ofstream result;

    result.open("result.txt");

    if (!result.is_open())
    {
        std::cout << "Cannot create a file for the result." << '\n';

        result.close();

        return -1;
    }

    for (const auto& extension : filenames)
    {
        result << '\t' << extension.first << '\n';

        if (extension.second.empty())
        {
           continue;
        }
        else
        {
            writeToFile(extension.second.begin(), extension.second.end(), result);
        }

        result << "\n\n";
    }

    result.close();

    return 0;
}


template<typename Iterator, typename T, typename>
void writeToFile(Iterator begin, Iterator end, T& filename)
{
    auto writeRecord = [number = 0, &filename](auto key) mutable
    {
        filename << std::string(key + " = " + std::to_string(number++) + ",\n");
    };

    std::for_each(begin, end, writeRecord);
}


template<typename FirstContainer>
void analyze(std::filesystem::path&& pathToDirectory, FirstContainer& container)
{
    for (const auto& file :std::filesystem::directory_iterator{pathToDirectory})
    {
        auto currentExtension = std::string(file.path().extension());

        auto it = std::find_if(container.begin(), container.end(), [&currentExtension](auto key)
        {
            return key.first == currentExtension;
        });

        if (it != container.end())
        {
            addRecord(container, file, (*it).first);
        }
    }
}


template<typename Container, typename File>
void addRecord(Container& container, File file, std::string extension)
{
    auto upperCase = [](std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](auto symbol){
            return toupper(symbol);
        });
    };

    std::string name = file.path().stem().filename().c_str();

    upperCase(name);

    container[extension].insert(name);
}