#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<SceneRef> VulkanInterface::instanceScene(IVec2 x1, IVec2 x2) {

        SceneRef reference;
        reference.sceneNumber = scenes.size();

        scenes.push_back({});
        
        TRYR(pipelineRes, createScenePipeline(scenes.back()));
        TRYR(depthRes, createSceneDepthResources(scenes.back()));
        TRYR(outputRes, createSceneOutputResources(scenes.back()));
        createSceneFramebuffers(scenes.back());
        createSceneDescriptorSet(scenes.back());
        updateSceneDescriptorSet(scenes.back());
        
        SPLITGUI_LOG("Created Scene: %ld", scenes.size());

        return reference;
    }

    void VulkanInterface::submitTriangleData(SceneRef& ref, std::vector<Vertex>& newVertices, std::vector<uint16_t>& newIndices, int flags, int textureNumber) {
        

        SPLITGUI_LOG("Submitted Triangles");
    }

    Result VulkanInterface::updateScene(SceneRef& ref, IVec2 x1, IVec2 x2) {
        

        SPLITGUI_LOG("Updated Scene: %d", ref.sceneNumber);

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraView(SceneRef& ref, Mat4& view) {

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraProjection(SceneRef& ref, Mat4& projection) {

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraPosition(SceneRef& ref, Vec3& position) {

        return Result::eSuccess;
    }
}