#include <iostream>
#include <fstream>

int main() {
    // Open the file for reading
    std::ifstream file("example.txt");

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    char ch;

    // Read characters until the end of file
    while (file >> std::noskipws >> ch) {
        // Process the character, for example, print it
        std::cout << ch;
    }

    // Close the file
    file.close();

    return 0;
}
