#include <iostream>

#include <filesystem>

#include <set>
#include <map>


#include "AnalyzeFunctions/AnalyzeFunctions.h"
#include "ThreadPool/ThreadPool.h"

#include "Statistics/Statistics.h"


int main(int argc, char *argv[])
{
    Statistics directoryInfo;

    AnalyzeFunctions::invitationMessage();

    std::cout << "Input the number of files to ignore.\n";

    int numberIgnoreFiles;
    std::cin >> numberIgnoreFiles;

    if (numberIgnoreFiles != 0)
    {
        std::cout << "Input the path to files, which must be ignored.\n";
    }

    std::set<std::filesystem::path> ignoreFiles;

    for (int i = 0; i < numberIgnoreFiles; ++i)
    {
        std::string temp;
        std::cin >> temp;

        std::filesystem::path dir{temp};
        if (!std::filesystem::exists(temp))
        {
            std::cout << "The path " << dir << " is invalid.\n";
            continue;
        }
        dir = std::filesystem::canonical(dir);

        auto it = ignoreFiles.find(dir);
        if (it != ignoreFiles.end())
        {
            std::cout << "You have already recorded this file" << '\n';
            continue;
        }
        else
        {
            ignoreFiles.insert(dir);
        }
    }

    if (!ignoreFiles.empty())
    {
        std::cout << "The following files will be ignored :" << '\n';
        for (const auto& file : ignoreFiles)
        {
            std::cout << file << '\n';
        }
    }

    std::string scriptName = argv[0];

    std::map<std::string, std::string> languages = {};               // Find according files.
    std::map<int, std::string> sortedByLines = {};                   // The previous one, but the first field is the amount of lines.

    AnalyzeFunctions::initializeLanguages(languages);

    std::filesystem::path directory = std::filesystem::current_path();

    bool atLeastOneFile = false;

    std::queue<std::filesystem::path> files;
    files.push(directory);

    ThreadPool pool;

    while (!files.empty())
    {
        for (const auto& file : std::filesystem::directory_iterator{files.front()})
        {
            // Ignore the script itself.
            if (scriptName == std::string(file.path().c_str()))
            {
                continue;
            }

            auto it = ignoreFiles.find(std::string(file.path().c_str()));
            if (it != ignoreFiles.end())
            {
                directoryInfo.addFileInfo(Statistics::File::IGNORE);
                continue;
            }

            if (std::filesystem::is_directory(status(file)))
            {
                files.push(file);
                continue;
            }

            auto [language, isKnownFileExtension] = AnalyzeFunctions::getLanguageByExtension(languages,
                                                                           std::string(file.path().extension().c_str()));
            if (!isKnownFileExtension)
            {
                directoryInfo.addFileInfo(Statistics::File::UNKNOWN);
                continue;
            }

            if (std::filesystem::is_regular_file(status(file)))
            {
                directoryInfo.addFileInfo(Statistics::File::ADD);
                atLeastOneFile = true;
            }

            pool.submitTask(file, language);
        }

        files.pop();
    }

    while (!pool.isFinished())
    {
        std::this_thread::yield();
    }

    pool.stop();

    while (!pool.allFinished())
    {
        std::this_thread::yield();
    }

    int totalLinesAmount = 0;
    for (const auto& languageInfo : pool.getInfoByLanguage())
    {
        totalLinesAmount += languageInfo.second;
        sortedByLines[languageInfo.second] = languageInfo.first;
    }

    directoryInfo.printDirectoryStatistics();

    if (atLeastOneFile)
    {
        AnalyzeFunctions::filesLanguageStatistics(sortedByLines, totalLinesAmount);
    }

    return 0;
}
