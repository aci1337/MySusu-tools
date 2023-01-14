#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: file.exe target.exe [flags]\n";
        return 0;
    }

    bool flagU = false;
    bool flagA = false;
    bool flagL = false;
    bool flagP = false;
    bool flagAll = false;

    for (int i = 2; i < argc; i++)
    {
        std::string flag = argv[i];
        if (flag == "-u")
        {
            flagU = true;
        }
        else if (flag == "-a")
        {
            flagA = true;
        }
        else if (flag == "-l")
        {
            flagL = true;
        }
        else if (flag == "-p")
        {
            flagP = true;
        }
        else if (flag == "-all")
        {
            flagAll = true;
        }
    }

    std::ifstream target(argv[1], std::ios::binary);
    std::ofstream output("output.txt");
    if (!target.is_open())
    {
        std::cout << "Unable to open target.exe\n";
        return 0;
    }

    std::unordered_map<long, int> lines;
    long i = 0;
    char c;
    std::string line;
    while (target.get(c))
    {
        if (isprint(c) && (flagU || flagA))
        {
            line += c;
        }
        else
        {
            if (!line.empty())
            {
                if (isprint(c))
                {
                    line += c;
                }
                else
                {
                    if (!line.empty())
                    {
                        output << " " << lines[i] << " - " << line << std::endl;
                        line.clear();
                    }
                    i++;
                    lines[i] = lines[i - 1] + 1;
                }

            
                if (flagU && line.find("\\u") != std::string::npos)
                {
                    output << line << std::endl;
                }
                else if (flagA && line.find("\\u") == std::string::npos)
                {
                    output << line << std::endl;
                }
                else if (flagL && (line.find("http") != std::string::npos || line.find("ftp") != std::string::npos))
                {
                    output << line << std::endl;
                }
                else if (flagP && line.find("\\") != std::string::npos)
                {
                    output << line << std::endl;
                }
                line.clear();
            }
        }
    }
    if (flagAll)
    {
        std::string folderName = std::filesystem::path(argv[1]).stem().string();
        std::filesystem::create_directory(folderName);
        std::filesystem::rename("output.txt", folderName + "/output.txt");
        std::filesystem::rename("unicode.txt", folderName + "/unicode.txt");
        std::filesystem::rename("ascii.txt", folderName + "/ascii.txt");
        std::filesystem::rename("links.txt", folderName + "/links.txt");
        std::filesystem::rename("paths.txt", folderName + "/paths.txt");

    }
    output.close();
    target.close();
    std::cout << "Output saved to output.txt\n";
    return 0;
    return 0;
}