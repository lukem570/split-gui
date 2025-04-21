#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<VectorEngineRef> VulkanInterface::instanceVectorEngine(SceneRef& ref) {

        if (scenes[ref.sceneNumber].vEngineRef.has_value()) {
            return Result::eSceneAlreadyHasVectorEngine;
        }

        VectorEngineRef vEngineRef;
        vEngineRef.instanceNumber = vectorEngineInstances.size();

        scenes[ref.sceneNumber].vEngineRef = vEngineRef;

        vectorEngineInstances.push_back({});
        vectorEngineInstances.back().scene = ref;

        createVectorEngineDescriptorPool(vectorEngineInstances.back());
        TRYR(transformRes, createVectorEngineTransformPipeline(vectorEngineInstances.back()));
        TRYR(renderRes, createVectorEngineRenderPipeline(vectorEngineInstances.back()));
        createVectorEngineDescriptorSet(vectorEngineInstances.back());
        updateVectorEngineDescriptorSet(vectorEngineInstances.back());

        return vEngineRef;
    }

    ResultValue<EdgeRef> VulkanInterface::submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges) {

        EdgeRef edgeRef;

        for (unsigned int i = 0; i < edges.size(); i++) {

            LinkElement<VectorEdgeBufferObject>* element;

            if (std::holds_alternative<LinearEdge>(edges[i])) {
                LinearEdge& edge = std::get<LinearEdge>(edges[i]);

                VectorEdgeBufferObject edgeObj;
                edgeObj.start    = edge.from;
                edgeObj.control1 = edge.from + Vec3(1.0f/3.0f) * (edge.to - edge.from);
                edgeObj.control2 = edge.from + Vec3(2.0f/3.0f) * (edge.to - edge.from);
                edgeObj.end      = edge.to;

                element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);
            }

            if (std::holds_alternative<QuadraticEdge>(edges[i])) {
                QuadraticEdge& edge = std::get<QuadraticEdge>(edges[i]);

                VectorEdgeBufferObject edgeObj;
                edgeObj.start    = edge.from;
                edgeObj.control1 = edge.from + Vec3(2.0f/3.0f) * (edge.control - edge.from);
                edgeObj.control2 = edge.to + Vec3(2.0f/3.0f) * (edge.control - edge.to);
                edgeObj.end      = edge.to;

                element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);
            }

            if (std::holds_alternative<CubicEdge>(edges[i])) {
                
                CubicEdge& edge = std::get<CubicEdge>(edges[i]);

                VectorEdgeBufferObject edgeObj;
                edgeObj.start    = edge.from;
                edgeObj.control1 = edge.control1;
                edgeObj.control2 = edge.control2;
                edgeObj.end      = edge.to;

                element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);
            }

            if (i == 0) {
                edgeRef.edgesStart = element;
            } else if (i + 1 == edges.size()) {
                edgeRef.edgesEnd = element;
            }
        }

        if (!(vectorEngineInstances[ref.instanceNumber].edges.size() > 0)) {
            return edgeRef;
        }
        
        TRYR(submitRes, submitVectorEngineEdgeResources(vectorEngineInstances[ref.instanceNumber]));
        updateVectorEngineEdges(vectorEngineInstances[ref.instanceNumber]);

        return edgeRef;
    }
}