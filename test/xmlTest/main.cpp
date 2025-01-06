#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <fstream>
#include <sstream>
#include <string>

int main() {

    std::ifstream indexFile("test/xmlTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();

    SplitGui::Interface interface;

    std::string page = buffer.str();

    interface.parseXml(page);

    SplitGui::Window window;

    
    return 0;
}
