#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>
int main();

int main()
{
    std::string command("ssh -Y pi@10.0.0.179 '/home/pi/Documents/github/2427_radio/build/Chip_ID' 2>&1");

    std::array<char, 128> buffer;
    std::string result;

    std::cout << "Opening reading pipe" << std::endl;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "Couldn't start command." << std::endl;
        return 0;
    }
    while (fgets(buffer.data(), 128, pipe) != NULL) {
        std::cout << "Reading..." << buffer.data();
        result += buffer.data();
    }
    auto returnCode = pclose(pipe);

    std::cout << result << std::endl;
    bool isFound = result.find("43447");
    if (isFound) std::cout << "The chip ID is correct" << std::endl;
    std::cout << returnCode << std::endl;

    return 0;
}
