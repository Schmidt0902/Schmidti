#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

const std::string original_content = "Dies ist der originale Dateiinhalts.";

void check_and_repair_file(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (content != original_content) {
        std::cout << "Die Datei wurde verändert. Sie wird wiederhergestellt." << std::endl;
        std::ofstream repair_file(filepath);
        repair_file << original_content;
        repair_file.close();
    } else {
        std::cout << "Die Datei ist unverändert." << std::endl;
    }
}

int main() {
    const std::string filepath = "self_repairing_file.txt";

    if (!std::filesystem::exists(filepath)) {
        std::ofstream create_file(filepath);
        create_file << original_content;
        create_file.close();
        std::cout << "Originaldatei erstellt." << std::endl;
    }

    check_and_repair_file(filepath);
    return 0;
}