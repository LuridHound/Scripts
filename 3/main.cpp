#include <iostream>
#include <fstream>


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
       std::cout << "Incorrect number of arguments";
       return -1;
    }


    system(("mkdir " + std::string(argv[1])).data());

    std::ofstream file;

    file.open(std::string(argv[1]).append("/").append(argv[1]).append(".h"));

    file << std::string("#ifdef ").append(argv[2]);
    file << '\n';
    file << std::string("#define ").append(argv[2]);

    file << '\n' << '\n' << '\n';

    file << std::string("#endif ").append("//").append(argv[2]);

    file.close();

    return 0;
}