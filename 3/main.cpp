#include <iostream>
#include <fstream>

#include <cstring>
#include <algorithm>

#include <map>

#include <filesystem>


//
//
enum class Key : size_t
                {
                    // Keys

                    // Type of guards in file.
                    // Mutually exclusive.
                    PRAGMA = 1 << 0,
                    INCLUDE_GUARDS = 1 << 1,    //  default

                    FOLDER = 1 << 2,            //  default

                    // Files
                 
                    CPP = 1 << 3,
                    H = 1 << 4,
                    INL = 1 << 5,

                    // Modes
                 
                    EMPTY_MODE = 0,
                    DEFAULT_MODE = Key::INCLUDE_GUARDS | Key::FOLDER | Key::INL,

                };


//
//
enum class Return
                    {
                        // When a user wants to see help page.
                        HELP = 0,
                        NORMAL = 0,

                        NO_KEY = 1,
                    };

enum class Error
                {
                    OK = 0,
                    CANNOT_OPEN_FILE = 1,
                    FILE_EXISTS = 2
                };


//  For files, that you can forget to close.
//
template<class T>
class SafeFile
{
    public :

        SafeFile() = default;
        ~SafeFile();
        
        T& getFile();

    private :

        T file;
};


template<class T>
static constexpr auto to_integral(T enumValue) -> std::underlying_type_t<T>;

void toUpper(std::string& str);

void printHelp();

[[nodiscard]]
bool haveKey(const Key key, Key extension);

void create(const Key state, std::string_view name);
[[nodiscard]]
Error createFile(std::string_view fileName, std::string_view extension, bool isGuard, bool isFolder, bool isCpp);

void processKey(Key& state, std::string_view keys_);

void validateKey(Key& state);


int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        if (std::strncmp(argv[1], "--help", 6) == 0)
        {
            printHelp();
            return static_cast<int>(Return::HELP);
        }
    }

    Key state;

    for (int i = 1; i < argc; ++i)
    {
        state = Key::EMPTY_MODE;

        while (argv[i][0] == '-')
        {
            // Doesn't include '-' to keys
            processKey(state, argv[i] + 1);

            ++i;
        }

        validateKey(state);

        // No more strings from the command line.
        if (i >= argc)
        {
           break;
        }

        create(state, argv[i]);
    }

    return static_cast<int>(Return::NORMAL);
}


template<class T>
SafeFile<T>::~SafeFile()
{
    if (file.is_open())
    {
        file.close();
    }
};


template<class T>
T& SafeFile<T>::getFile()
{
    return file;
}


//  Convert enum value to it's underlying type.
//
template<class T>
static constexpr auto to_integral(T enumValue) -> std::underlying_type_t<T>
{
    return static_cast<std::underlying_type_t<T>>(enumValue);
}


//
//
void toUpper(std::string& str)
{
    std::for_each
            (str.begin(), str.end(),
             [](char& symbol)
             {
                 symbol = toupper(symbol);
             }
            );
}


//
//
void printHelp()
{
    std::cout << "Usage: "
              << "crilf [OPTIONS] FILENAME ..." << '\n'
              << "Example: "
              << "crilf -pf temp -p temp_1 temp_2" << '\n'

              << '\n'

              << "Default Mode : -ifI" << '\n'

              << '\n'

              << "Pattern protection:" << '\n'
              << "-i Include guards." << '\n'
              << "-p Pragma once." << '\n'

              << '\n'

              << "Miscellaneous:\n"
              << "-f create file in the folder with the same name." << '\n'

              << "Files:" << '\n'
              << "-C create .cpp file." << '\n'
              << "-H create .h file." << '\n'
              << "-I create .inl file." << '\n'

              << '\n'

              << "Do nothing, if file exists." << '\n'

              << "If your key is incomplete, then default keys will be added(if they are necessary)." << '\n'
              << "If no key was printed, then the default one will be chosen. " << '\n';
}


//
//
[[nodiscard]]
bool haveKey(const Key key, Key extension)
{
    return to_integral(extension) & to_integral(key);
}


//  If file exists, delete it's content.
//  Supposed, that extension is without dot.
[[nodiscard]]
Error createFile(std::string_view fileName, std::string_view extension, bool isGuard, bool isFolder, bool isCpp)
{
    SafeFile<std::ofstream> file;
    
    if (std::string currentFileNameWithPath; true)
    {
        if (isFolder)
        {
            currentFileNameWithPath = std::string(fileName).append("/").append(fileName).append(".").append(extension);
        }
        else
        {
            currentFileNameWithPath = std::string(fileName).append(".").append(extension);
        }
        
        if (std::filesystem::exists(currentFileNameWithPath))
        {
            return Error::FILE_EXISTS;
        }
        
        file.getFile().open(currentFileNameWithPath);
    }

    if (!file.getFile().is_open())
    {
       return Error::CANNOT_OPEN_FILE;
    }

    std::string fileNameString{fileName};
    std::string extensionString{extension};

    if (isCpp)
    {
        return Error::OK;
    }

    toUpper(fileNameString);
    toUpper(extensionString);

    if (isGuard)
    {

        file.getFile() << std::string("#ifdef ").append(fileNameString).append("_").append(extensionString)

                       << '\n'

                       << std::string("#define ").append(fileNameString).append("_").append(extensionString)

                       << "\n\n\n"

                       << std::string("#endif ").append("//").append(fileNameString).append("_").append(extensionString);
    }
    else
    {
        file.getFile() << "#pragma once";
    }

    return Error::OK;
}


//
//
void create(const Key state, std::string_view name)
{
    bool includeGuards = haveKey(state, Key::INCLUDE_GUARDS);
    bool isFolder = haveKey(state, Key::FOLDER);

    if (isFolder)
    {
        system(("mkdir " + std::string(name)).data());
    }

    static constexpr std::array<std::pair<std::string_view, Key>, 3> extensionsKeys = {std::make_pair("cpp", Key::CPP),
                                                                                       std::make_pair("h", Key::H),
                                                                                       std::make_pair("inl", Key::INL)};

    static constexpr std::array<std::string_view, 3> errorsLogs = {"OK", "Cannot create file", "File already exists"};

    for (auto&& key = extensionsKeys.begin(); key != extensionsKeys.end(); ++key)
    {
        Error error = Error::OK;

        if (haveKey(state, key->second))
        {
            error = createFile(name, key->first, includeGuards, isFolder, key->second == Key::CPP);
        }

        if (error != Error::OK)
        {
           std::cout << errorsLogs[to_integral(error)] << ':' << ' ' << name << '.' << key->first << '\n';
        }
    }
}


//
//
void processKey(Key& state, std::string_view keys)
{
    static  std::map<const char, Key> programKeys =
    {
            {'p', Key::PRAGMA},
            {'i', Key::INCLUDE_GUARDS},
            {'f', Key::FOLDER},
            {'H', Key::H},
            {'C', Key::CPP},
            {'I', Key::INL}
    };
    
    for (const auto& key : keys)
    {
        auto it = programKeys.find(key);

        if (it != programKeys.end())
        {
            state = static_cast<Key>(to_integral(state) | to_integral((*it).second));
        }
    }
}


//
//
void validateKey(Key& state) 
{
    bool noState = state == Key::EMPTY_MODE;
    bool noExtensions = !(haveKey(state, Key::CPP) || haveKey(state, Key::H) ||
                          haveKey(state, Key::INL));

    bool noGuard = !(haveKey(state, Key::PRAGMA) || haveKey(state, Key::INCLUDE_GUARDS)) &&
                   !haveKey(state, Key::CPP);

    if (noState)
    {
       state = Key::DEFAULT_MODE;
    }

    if (noExtensions)
    {
        state = static_cast<Key>(to_integral(state) | to_integral(Key::INL));
    }

    if (noGuard)
    {
       state = static_cast<Key>(to_integral(state) | to_integral(Key::INCLUDE_GUARDS));
    }

    //  Pragma has a lower priority to be chosen.
    if (haveKey(state, Key::INCLUDE_GUARDS) && haveKey(state, Key::PRAGMA))
    {
        state = static_cast<Key>(to_integral(state) ^ to_integral(Key::PRAGMA));
    }
}