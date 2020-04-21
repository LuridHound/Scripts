#ifndef SCRIPT_STATISTICS_H
#define SCRIPT_STATISTICS_H


class Statistics final
{
public :

    enum class File
    {
        IGNORE = 0,
        UNKNOWN = 1,
        ADD = 2
    };

    Statistics();
    void addFileInfo(const File TYPE);
    void printDirectoryStatistics();

private :

    int files;
    int unknown_files;
    int ignored_files;
};


#endif //SCRIPT_STATISTICS_H
