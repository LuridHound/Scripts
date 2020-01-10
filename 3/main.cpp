#include <iostream>
#include <fstream>

#include <cstring>

#include <map>

#include <algorithm>


//
//
enum class Key : size_t
                {
                    // Type of guards in file.
                    // Mutually exclusive.
                    PRAGMA = 1 << 0,
                    INCLUDE_GUARDS = 1 << 1,    //  default

                    FOLDER = 1 << 2,            //  default

                };


//
//
enum class Return : int
                    {
                        // When a user wants to see help page.
                        HELP = 0,
                        NORMAL = 0,

                        NO_KEY = 1,

                    };


void printHelp();

void setDefault(size_t& key);

[[nodiscard]]
bool haveExtension(const size_t key, Key extension);

void create(const size_t state, std::string_view name);
void createFile(std::string_view fileName, bool isGuard = true, bool isFolder = true);

void processKey(size_t& state, std::string_view keys_);


int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        if (strncmp(argv[1], "--help", 6) == 0)
        {
            printHelp();
            return static_cast<int>(Return::HELP);
        }
    }

    size_t state;

    for (int i = 1; i < argc; ++i)
    {
        state = 0u;

        if (argv[i][0] == '-')
        {
            if (i + 1 >= argc)
            {
                std::cout << "No file name after key. "
                             "Return.\n";

                return static_cast<int>(Return::NO_KEY);
            }

            // Doesn't include '-' to keys
            processKey(state, argv[i] + 1);

            ++i;
        }
        else
        {
           setDefault(state);
        }

        create(state, argv[i]);
    }

    return static_cast<int>(Return::NORMAL);
}


//
//
void printHelp()
{
    std::cout << "Usage: ";
    std::cout << "crilf [OPTION] FILENAME ...\n";
    std::cout << "Example: ";
    std::cout << "crilf -pf temp -p temp_1 temp_2\n";

    std::cout << '\n';

    std::cout << "Pattern protection:\n";
    std::cout << "-i Include guards.\n";
    std::cout << "-p Pragma once.\n";

    std::cout << '\n';

    std::cout << "Miscellaneous:\n";
    std::cout << "-f create file in the folder with the same name.\n";

    std::cout << '\n';

    std::cout << "If file already exists, delete files content.\n";
    std::cout << "Report bugs to: cursedseraph@mail.ru\n";

    return;
}


//
//
void setDefault(size_t& key)
{
    key |= static_cast<size_t>(Key::FOLDER) |
           static_cast<size_t>(Key::INCLUDE_GUARDS);

    return;
}


//
//
[[nodiscard]]
bool haveExtension(const size_t key, Key extension)
{
    return static_cast<size_t>(extension) & key;
}


//  If file exists, delete it's content.
//
void createFile(std::string_view fileName, bool isGuard, bool isFolder)
{
    std::ofstream file;

    if (isFolder)
    {
        file.open(std::string(fileName).append("/").append(fileName).append(".inl"), std::ios_base::trunc);
    }
    else
    {
        file.open(std::string(fileName).append(".inl"), std::ios_base::trunc);
    }

    if (isGuard)
    {
        std::string name{fileName};

        std::for_each
        (name.begin(), name.end(),
                [](char& symbol)
                {
                    symbol = toupper(symbol);
                }
        );

        file << std::string("#ifdef ").append(name).append("_INL");

        file << '\n';

        file << std::string("#define ").append(name).append("_INL");

        file << "\n\n\n";

        file << std::string("#endif ").append("//").append(name).append("_INL");
    }
    else
    {
        file << std::string("#pragma once");
    }

    file.close();

    return;
}


//
//
void create(const size_t state, std::string_view name)
{
    if (haveExtension(state, Key::FOLDER))
    {
        system(("mkdir " + std::string(name)).data());
    }

    createFile(name, haveExtension(state, Key::INCLUDE_GUARDS), haveExtension(state, Key::FOLDER));

    return;
}


//
//
void processKey(size_t& state, std::string_view keys)
{
    static  std::map<const char, Key> programmKeys =
    {
            {'p', Key::PRAGMA},
            {'i', Key::INCLUDE_GUARDS},
            {'f', Key::FOLDER}
    };

    for (const auto& key : keys)
    {
        auto it = programmKeys.find(key);

        if (it != programmKeys.end())
        {
            state |= static_cast<size_t>((*it).second);
            std::cout << static_cast<size_t>((*it).second) << '\n';
        }
    }

    if (haveExtension(state, Key::INCLUDE_GUARDS) && haveExtension(state, Key::PRAGMA))
    {
        setDefault(state);
    }

    return;
}
