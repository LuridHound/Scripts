#ifndef SCRIPT_ANALYZEFUNCTIONS_H
#define SCRIPT_ANALYZEFUNCTIONS_H


#include <map>


namespace AnalyzeFunctions
{

    [[nodiscard]]
    std::pair<std::string, bool> getLanguageByExtension(const std::map<std::string, std::string>& languages, const std::string&& extension);

    [[nodiscard]]
    int getLinesInFile(std::string&& pathToFile);

    void initializeLanguages(std::map<std::string, std::string>& languages);

    void filesLanguageStatistics(const std::map<int, std::string>& sortedByLines, const int lineAmountInFiles);

    void invitationMessage();

};


#endif //SCRIPT_ANALYZEFUNCTIONS_H
