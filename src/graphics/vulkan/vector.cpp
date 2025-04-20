#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<VectorEngineRef> VulkanInterface::instanceVectorEngine(SceneRef& ref) {

        VectorEngineRef vEngineRef;
        vEngineRef.instanceNumber = vectorEngineInstances.size();

        vectorEngineInstances.push_back({});

        vectorEngineInstances.back().scene = ref;

        createVectorEngineDescriptorPool(vectorEngineInstances.back());
        TRYR(transformRes, createVectorEngineTransformPipeline(vectorEngineInstances.back()));
        TRYR(renderRes, createVectorEngineRenderPipeline(vectorEngineInstances.back()));
        TRYR(outputRes, createVectorEngineOutputResources(vectorEngineInstances.back()));
        createVectorEngineDescriptorSet(vectorEngineInstances.back());
        //TRYR(edgeRes, submitVectorEngineEdgeResources(vectorEngineInstances.back()));
        updateVectorEngineDescriptorSet(vectorEngineInstances.back());

        return vEngineRef;
    }
}