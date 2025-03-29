#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

#include <vector>

int main() {

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "vector test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, graphics.submitWindow(window));
    graphics.attachEventHandler(eventHandler);

    std::vector<SplitGui::Contour> contours;

    contours.resize(5);

    SplitGui::MoveTo        contour1;
    SplitGui::LinearContour contour2;
    SplitGui::LinearContour contour3;
    SplitGui::LinearContour contour4;
    SplitGui::LinearContour contour5;

    contour1.from = {0.5, 0.75};
    contour2.to   = {0.75, 0.5};
    contour3.to   = {0.5, 0.25};
    contour4.to   = {0.25, 0.5};
    contour5.to   = {0.5, 0.75};

    contours[0] = contour1;
    contours[1] = contour2;
    contours[2] = contour3;
    contours[3] = contour4;
    contours[4] = contour5;

    SplitGui::ResultValue<unsigned int> textureIndex = graphics.createContourImage(contours);

    TRYDC(textureIndex);  

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0xFFFFFF,
        0,
        SplitGui::VertexFlagsBits::eTextureMsdf | SplitGui::VertexFlagsBits::eUseTexture,
        textureIndex.value
    );

    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}