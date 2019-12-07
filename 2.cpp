#include <iostream>

#include <iomanip>

#include <fstream>

#include <experimental/filesystem>

#include <set>
#include <map>
#include <queue>

enum class File
{
    IGNORE = 0,
    UNKNOWN = 1,
    ADD = 2
};


class Statistics final
        {
        public :

            Statistics();
            void addFileInfo(const File TYPE);
            void printDirectoryStatistics();
            
        private :

                    int files;
                    int unknown_files;
                    int ignored_files;

        };

[[nodiscard]]
std::pair<std::string, bool> getLanguageByExtension(const std::map<std::string, std::string>& languages, const std::string&& extension);

[[nodiscard]]
int getLinesInFile(std::string pathToFile);

void initializeLanguages(std::map<std::string, std::string>& languages);

void filesLanguageStatistics(const std::map<int, std::string>& sortedByLines, const int lineAmountInFiles);

int main(int argc, char *argv[])
{
    Statistics directoryInfo;

    std::cout << '\n' << '\n';
    std::cout << "Don't delete, rename or move files in the current directory."
              << '\n' << "Otherwise program can crash.";
    std::cout << '\n' << '\n';

    std::cout << "Input the number of files to ignore.\n";

    int numberIgnoreFiles;
    std::cin >> numberIgnoreFiles;

    if (numberIgnoreFiles != 0)
    {
        std::cout << "Input the path to files, which must be ignored.\n";
    }
    std::set<std::experimental::filesystem::path> ignoreFiles;

    for (int i = 0; i < numberIgnoreFiles; ++i)
    {
        std::string temp;
        std::cin >> temp;

        std::experimental::filesystem::path dir{temp};
        if (!std::experimental::filesystem::exists(temp))
        {
            std::cout << "The path " << dir << " is invalid.\n";
            continue;
        }

        dir = std::experimental::filesystem::canonical(dir, std::experimental::filesystem::path{});

        auto it = ignoreFiles.find(dir);
        if (it != ignoreFiles.end())
        {
            std::cout << "You have already inputed this file.\n";
            continue;
        }
        else
        {
            ignoreFiles.insert(dir);
        }
    }

    if (ignoreFiles.size() != 0)
    {
        std::cout << "The following files will be ignored :" << '\n';
        for (const auto& file : ignoreFiles)
        {
            std::cout << file << '\n';
        }
    }

    std::string scriptName = argv[0];

    std::map<std::string, std::string> languages = {};    // find according files
    std::map<std::string, int> infoByLanguage = {};                  //  have an information about how many lines each language have
    std::map<int, std::string> sortedByLines = {};                   // the previous one, but the first field is the amount of lines.

    initializeLanguages(languages);

    std::experimental::filesystem::path directory = std::experimental::filesystem::current_path();

    bool atLeastOneFile = false;
    int lines_amount = 0;

    std::queue<std::experimental::filesystem::path> files;
    files.push(directory);
    while (files.size() != 0)
    {
        for (const auto& file : std::experimental::filesystem::directory_iterator{files.front()})
        {
            // Ignore the script itself.
            if (scriptName == std::string(file.path().c_str()))
            {
                continue;
            }

            auto it = ignoreFiles.find(std::string(file.path().c_str()));
            if (it != ignoreFiles.end())
            {
                directoryInfo.addFileInfo(File::IGNORE);
                continue;
            }

            if (std::experimental::filesystem::is_directory(status(file)))
            {
                files.push(file);
            }

            auto [language, isKnownFileExtension] = getLanguageByExtension(languages,
                                                                           std::string(file.path().extension().c_str()));
            if (!isKnownFileExtension)
            {
                directoryInfo.addFileInfo(File::UNKNOWN);
                continue;
            }

            if (std::experimental::filesystem::is_regular_file(status(file)))
            {
                directoryInfo.addFileInfo(File::ADD);
                atLeastOneFile = true;
            }

            int currentLines = getLinesInFile( file.path().c_str());

            infoByLanguage[language] += currentLines;

            lines_amount += currentLines;
        }

        files.pop();
    }

    for (auto it = infoByLanguage.begin(); it != infoByLanguage.end(); ++it)
    {
        sortedByLines[it->second] = it->first;
    }

    directoryInfo.printDirectoryStatistics();
    if (atLeastOneFile)
    {
        filesLanguageStatistics(sortedByLines, lines_amount);
    }

    return 0;
}


//  It's guaranteed that file exists.
//
[[nodiscard]]
int getLinesInFile(std::string pathToFile)
{
    static constexpr int MAX_EXPECTED_LENGTH = 100;
    static std::string temp;

    static bool initialized = false;
    if (!initialized)
    {
        temp.reserve(MAX_EXPECTED_LENGTH);
    }
    initialized = true;


    int lines = 0;

    std::ifstream file;
    file.open(pathToFile);

    while (getline(file, temp))
    {
        ++lines;
    }

    file.close();

    return lines;
}


//
//
void initializeLanguages(std::map<std::string, std::string>& languages)
{
    languages[".as"]            = "ActionScript";
    languages[".adb"]           = "Ada";
    languages["ads"]            = "Ada";
    languages[".asm"]           = "Assembler";
    languages[".masm"]          = "Assembler";
    languages[".nasm"]          = "Assembler";
    languages[".bas"]           = "Basic";
    languages[".c"]             = "C";
    languages[".h"]             = "C";
    languages[".cpp"]           = "C++";
    languages[".cxx"]           = "C++";
    languages[".c++"]           = "C++";
    languages[".cc"]            = "C++";
    languages[".hpp"]           = "C++";
    languages[".hxx"]           = "C++";
    languages[".h++"]           = "C++";
    languages[".hh"]            = "C++";
    languages[".inl"]           = "C++";
    languages[".cs"]            = "C#";
    languages[".css"]           = "CSS";
    languages[".cg"]            = "Cg";
    languages[".cbl"]           = "Cobol";
    languages[".cob"]           = "Cobol";
    languages[".cpy"]           = "Cobol";
    languages[".coffee"]        = "CoffeeScript";
    languages[".litcoffee"]     = "CoffeeScript";
    languages[".v"]             = "Coq";
    languages[".cu"]            = "CUDA";
    languages[".cuh"]           = "CUDA";
    languages[".cuda"]          = "CUDA";
    languages[".d"]             = "D";
    languages[".dart"]          = "Dart";
    languages[".es"]            = "ECMAScript";
    languages[".erl"]           = "Erlang";
    languages[".hrl"]           = "Erlang";
    languages[".fs"]            = "F#";
    languages[".fsi"]           = "F#";
    languages[".fsx"]           = "F#";
    languages["fsscript"]       = "F#";
    languages[".f"]             = "Fortran";
    languages[".for"]           = "Fortran";
    languages[".f90"]           = "Fortran";
    languages[".go"]            = "Go";
    languages[".glsl"]          = "GLSL";
    languages[".groovy"]        = "Groovy";
    languages[".hs"]            = "Haskell";
    languages[".lhs"]           = "Haskell";
    languages[".hlsl"]          = "HLSL";
    languages[".html"]          = "HTML";
    languages[".java"]          = "Java";
    languages[".js"]            = "JavaScript";
    languages[".mjs"]           = "JavaScript";
    languages[".json"]          = "JSON";
    languages[".kt"]            = "Kotlin";
    languages[".kts"]           = "Kotlin";
    languages[".tex"]           = "LaTeX";
    languages[".lsp"]           = "Lisp";
    languages[".lua"]           = "Lua";
    languages[".m"]             = "Objective-C";
    languages[".mm"]            = "Objective-C";
    languages[".ml"]            = "OCaml";
    languages[".mli"]           = "OCaml";
    languages[".cl"]            = "OpenCL";
    languages[".pp"]            = "Pascal";
    languages[".pas"]           = "Pascal";
    languages[".pl"]            = "Perl";
    languages[".pm"]            = "Perl";
    languages[".t"]             = "Perl";
    languages[".pod"]           = "Perl";
    languages[".p6"]            = "Perl 6";
    languages[".pl6"]           = "Perl 6";
    languages[".pm6"]           = "Perl 6";
    languages[".php"]           = "PHP";
    languages[".phtml"]         = "PHP";
    languages[".php3"]          = "PHP";
    languages[".php4"]          = "PHP";
    languages[".php5"]          = "PHP";
    languages[".php7"]          = "PHP";
    languages[".phps"]          = "PHP";
    languages[".php-s"]         = "PHP";
    languages[".pht"]           = "PHP";
    languages[".ps"]            = "PostScript";
    languages[".py"]            = "Python";
    languages[".q"]             = "Q";
    languages[".qs"]            = "Q#";
    languages[".rb"]            = "Ruby";
    languages[".rs"]            = "Rust";
    languages[".rlib"]          = "Rust";
    languages[".sc"]            = "Scala";
    languages[".scala"]         = "Scala";
    languages[".swift"]         = "Swift";
    languages[".ts"]            = "TypeScript";
    languages[".tsx"]           = "TypeScript";

    return;
}


//
//
[[nodiscard]]
std::pair<std::string, bool> getLanguageByExtension(const std::map<std::string, std::string>& languages, const std::string&& extension)
{
    auto it = languages.find(extension);

    if (it != languages.end())
    {
        return std::make_pair(it->second, true);
    }

    return std::make_pair("", false);
}


//
//
void filesLanguageStatistics(const std::map<int, std::string>& sortedByLines, const int lineAmountInFiles)
{

    std::cout << "=======================================";
    std::cout << '\n'
              << "Language statistics:"
              << '\n';

    for (const auto& lines : sortedByLines)
    {
        if (lines.first == 0)
        {
            continue;
        }

        std::cout << '\t' << lines.second;

        float temp = static_cast<float>(lines.first) / lineAmountInFiles;
        temp *= 100;

        std::cout << '\t' << '\t' << std::setprecision(4) << temp << '\t' << '\t' << lines.first << '\n';
    }

    std::cout << "=======================================";
    std::cout << '\n';

    return;
}


//
//
void Statistics::addFileInfo(const File TYPE)
{
    ++files;
    switch (TYPE)
    {
        case File::IGNORE :
        {
            ++ignored_files;
            break;
        }

        case File::UNKNOWN :
        {
            ++unknown_files;
            break;
        }

        case File::ADD :
        {
            break;
        }
        
    }

    return;
}


//
//
void Statistics::printDirectoryStatistics()
{
    std::cout << "=======================================";
    std::cout << '\n'
              << "Directory statistics :"
              << '\n';

    std::cout << '\t' << "Files in the directory: " << files << '.' << '\n' << '\n';
    std::cout << '\t' << "Ignored files : " << ignored_files << '.' << '\n' << '\n';
    std::cout << '\t' << "Unknown files : " << unknown_files << '.' << '\n' << '\n';

    std::cout << "=======================================";
    std::cout << '\n';

    return;
}


Statistics::Statistics():
    files(0),
    unknown_files(0),
    ignored_files(0)
{

}
