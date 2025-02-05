
#include <splitgui/events.hpp>
#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>

#include <vector>

int event(int a, int b) {
    printf("event: %d, %d\n", a, b);
    return a + b;
}

int main() {
    SplitGui::EventHandler handler;
    handler.bindFunction(event, "event");

    SplitGui::Event* event = handler.fetchEvent("event");
    int ret = event->callRet<int>(10, 20).back();

    printf("return: %d\n", ret);

    return 0;
}
