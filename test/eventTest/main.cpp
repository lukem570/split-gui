
#include <splitgui/events.hpp>

#include <vector>

void event(int a, int b) {
    printf("event: %d, %d\n", a, b);
}

int main() {
    SplitGui::EventHandler handler;

    handler.bindFunction(event, "event");
    std::vector<int> params = {10, 20};


    return 0;
}
