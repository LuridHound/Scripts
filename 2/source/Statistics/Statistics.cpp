#include "Statistics.h"

#include <iostream>


Statistics::Statistics():
        files(0),
        unknown_files(0),
        ignored_files(0)
{

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
}


//
//
void Statistics::printDirectoryStatistics()
{
    std::cout << "=======================================";
    std::cout << '\n'
              << "Directory statistics :"
              << '\n' << '\n';

    std::cout << '\t' << "Files in the directory: " << files << '.' << '\n' << '\n';
    std::cout << '\t' << "Ignored files : " << ignored_files << '.' << '\n' << '\n';
    std::cout << '\t' << "Unknown files : " << unknown_files << '.' << '\n' << '\n';

    std::cout << "=======================================";
    std::cout << '\n';
}