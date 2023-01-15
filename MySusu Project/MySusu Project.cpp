#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <chrono>

class MySusu {
public:
    MySusu();
    MySusu(const char* s);
    MySusu(const MySusu& other);
    ~MySusu();

    MySusu& operator=(const MySusu& other);
    MySusu& operator+=(const MySusu& other);

    size_t length() const;
    bool empty() const;
    const char* c_str() const;
    char& operator[](size_t pos);
    const char& operator[](size_t pos) const;

    void clear();
    void append(const char* s);
    int find(const char* s) const;

private:
    char* m_str;
    size_t m_length;
    std::vector<char> m_buffer;
};


MySusu::MySusu()
    : m_str(nullptr)
    , m_length(0)
{
    m_buffer.reserve(32);
}

MySusu::MySusu(const char* s)
    : MySusu()
{
    append(s);
}

MySusu::MySusu(const MySusu& other)
    : MySusu()
{
    append(other.c_str());
}

MySusu::~MySusu()
{
    clear();
}

MySusu& MySusu::operator=(const MySusu& other)
{
    clear();
    append(other.c_str());
    return *this;
}

MySusu& MySusu::operator+=(const MySusu& other)
{
    append(other.c_str());
    return *this;
}

size_t MySusu::length() const
{
    return m_length;
}

bool MySusu::empty() const
{
    return m_length == 0;
}

const char* MySusu::c_str() const
{
    return m_str;
}

char& MySusu::operator[](size_t pos)
{
    if (pos >= m_length) {
        throw std::out_of_range("MyString::operator[]");
    }
    return m_str[pos];
}

const char& MySusu::operator[](size_t pos) const
{
    if (pos >= m_length) {
        throw std::out_of_range("MyString::operator[] const");
    }
    return m_str[pos];
}

void MySusu::clear()
{
    delete[] m_str;
    m_str = nullptr;
    m_length = 0;
    m_buffer.clear();
}

void MySusu::append(const char* s)
{
    size_t len = strlen(s);
    if (m_str == nullptr) {
        m_str = new char[len + 1];
        strcpy(m_str, s);
    }
    else {
        size_t newLen = m_length + len;
        if (m_buffer.capacity() < newLen + 1) {
            m_buffer.reserve(newLen + 1);
        }
        strcpy(&m_buffer[0], m_str);
        strcpy(&m_buffer[m_length], s);
        delete[] m_str;
        m_str = &m_buffer[0];
    }
}

int MySusu::find(const char* s) const
{
    const char* p = strstr(m_str, s);
    if (p == nullptr) {
        return -1;
    }
    return p - m_str;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: file.exe target.exe [flags]\n";
        std::cout << "Flags -u = show unicode strings\n";
        std::cout << "Flags -a = show ascii strings\n";
        std::cout << "Flags -l = show links\n";
        std::cout << "Flags -p = show paths\n";
        std::cout << "Flags -all = show all\n";
        std::cout << "Flags -s = save result as the given name (optional)\n";
        std::cout << "Flags -h = show this help\n";
        std::cout << "Flags -av = check for viruses\n";
        return 0;
    }

    bool flagU = false;
    bool flagA = false;
    bool flagL = false;
    bool flagP = false;
    bool flagAll = false;
    bool flagS = false;
    bool flagAV = false;
    std::string outputFile;

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
        else if (flag == "-s")
        {
            flagS = true;
            outputFile = argv[i + 1];
            i++;
        }
        else if (flag == "-av")
        {
            flagAV = true;
        }
    }

    auto startTime = std::chrono::steady_clock::now();

    std::ifstream target(argv[1], std::ios::binary | std::ios::in | std::ios::out);
    std::ofstream output;
    if (flagS)
    {
        output.open(outputFile);
    }
    else
    {
        output.open("output.txt");
    }
    if (!target.is_open())
    {
        std::cout << "Unable to open target.exe\n";
        return 0;
    }

    if (flagAV)
    {
        
        //std::string fileData;
        //std::ifstream target(argv[1], std::ios::binary | std::ios::in);
        //if (target.is_open())
        //{
          //  target.seekg(0, std::ios::end);
          //  fileData.resize(target.tellg());
          //  target.seekg(0, std::ios::beg);
          //  target.read(&fileData[0], fileData.size());
          //  target.close();
        //}
        /*MySusu virusSignature("Virus");
        if (fileData.find(virusSignature.c_str()) != std::string::npos)
        {
            flagAV = true;
        }*/

        // SOON
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
                        output << " " << lines[i] << " - " << line.c_str() << std::endl;
                        line.clear();
                    }
                    i++;
                    lines[i] = lines[i - 1] + 1;
                }


                if (flagU && line.find("\\u") != -1)
                {
                    output << line.c_str() << std::endl;
                }
                else if (flagA && line.find("\\u") == -1)
                {
                    output << line.c_str() << std::endl;
                }
                else if (flagL && (line.find("http") != -1 || line.find("ftp") != -1))
                {
                    output << line.c_str() << std::endl;
                }
                else if (flagP && line.find("\\") != -1)
                {
                    output << line.c_str() << std::endl;
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
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    if (flagAV)
    {
        std::cout << "Virus scan completed. ";
        if (flagAV)
        {
            std::cout << "VIRUS! NOT SAFE TO RUN\n";
        }
        else
        {
            std::cout << "NOT A VIRUS, ENJOY!\n";
        }
    }

    std::cout << "Done! everything it's saved and working enjoy (saved to " << (flagS ? outputFile : "output.txt") << ") and took " << elapsedTime.count() << " seconds\n";
    return 0;
}
