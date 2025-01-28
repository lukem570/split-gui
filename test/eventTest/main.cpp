
#include <splitgui/events.hpp>

#include <vector>

int event(int a, int b) {
    printf("event: %d, %d\n", a, b);
    return a + b;
}

int main() {
    SplitGui::EventHandler handler;
    handler.bindFunction(event, "event");

    SplitGui::Event* event = handler.fetchEvent("event");
    int ret = event->call<int>(10, 20);

    printf("return: %d\n", ret);

    return 0;
}
