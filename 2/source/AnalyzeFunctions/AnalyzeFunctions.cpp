#include "AnalyzeFunctions.h"


#include <iostream>
#include <fstream>
#include <iomanip>


void AnalyzeFunctions::invitationMessage()
{
    std::cout << '\n' << '\n'
              << "Don't delete, rename or move files in the current directory."
              << '\n' << "Otherwise program can crash."
              << '\n' << '\n';
}



//  It's guaranteed that file exists.
//
[[nodiscard]]
int AnalyzeFunctions::getLinesInFile(std::string&& pathToFile)
{
    static constexpr int MAX_EXPECTED_LENGTH = 100;
    //  "static thread_local" the same is "thread_local".
    //  https://stackoverflow.com/questions/22794382/are-c11-thread-local-variables-automatically-static/22794640
    thread_local std::string temp;

    // For reserving memory only.
    if (pathToFile.empty())
    {
        temp.reserve(MAX_EXPECTED_LENGTH);
        return 0;
    }


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
void AnalyzeFunctions::initializeLanguages(std::map<std::string, std::string>& languages)
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
}


//
//
[[nodiscard]]
std::pair<std::string, bool> AnalyzeFunctions::getLanguageByExtension(const std::map<std::string, std::string>& languages, const std::string&& extension)
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
void AnalyzeFunctions::filesLanguageStatistics(const std::map<int, std::string>& sortedByLines, const int lineAmountInFiles)
{

    std::cout << "=======================================";
    std::cout << '\n'
              << "Language statistics:"
              << '\n' << '\n';

    const int maxLength = 12;   // "ActionScript"

    for (const auto& line : sortedByLines)
    {
        if (line.first == 0)
        {
            continue;
        }

        std::cout << std::setw(maxLength) << std::left << line.second;

        float temp = static_cast<float>(line.first) / lineAmountInFiles;
        temp *= 100;

        std::cout << '\t' << '\t' << std::setprecision(4) << temp << "%" << '\t' << '\t' << line.first << '\n';
    }

    std::cout << "======================================="
              << '\n';
}

