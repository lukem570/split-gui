#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<VectorEngineRef> VulkanInterface::instanceVectorEngine(SceneRef& ref) {
        SPLITGUI_PROFILE;

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

    Vec3 cubicBezier(Vec3& p0, Vec3& p1, Vec3& p2, Vec3& p3, float t) {
        float u = 1.0 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
    
        return Vec3(uuu) * p0 + Vec3(3.0 * uu * t) * p1 + Vec3(3.0 * u * tt) * p2 + Vec3(ttt) * p3;
    }

    Vec3 quadraticBezier(Vec3& p0, Vec3& p1, Vec3& p2, float t) {
        float u = 1.0 - t;
        float tt = t * t;
        float uu = u * u;
    
        return Vec3(uu) * p0 + Vec3(2.0 * u * t) * p1 + Vec3(tt) * p2;
    }

    Vec3 lerp(Vec3& p0, Vec3& p1, float t) {
        float u = 1.0 - t;
        return Vec3(u) * p0 + Vec3(t) * p1;
    }

    void VulkanInterface::deleteEdges(VectorEngineRef& vEngineRef, EdgeRef& edgeRef) {
        SPLITGUI_PROFILE;

        vectorEngineInstances[vEngineRef.instanceNumber].edges.erase(edgeRef.edgesStart, edgeRef.edgesEnd);
    }

    ResultValue<EdgeRef> VulkanInterface::submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges, ModelRef model) {
        SPLITGUI_PROFILE;

        EdgeRef edgeRef;

        std::optional<unsigned int> modelNumber = scenes[vectorEngineInstances[ref.instanceNumber].scene.sceneNumber].models.offset(model.model);

        if (!modelNumber.has_value()) {
            return Result::eCouldNotFindElementInList;
        }

        for (unsigned int i = 0; i < edges.size(); i++) {

            LinkElement<VectorEdgeBufferObject>* element;

            if (std::holds_alternative<LinearEdge>(edges[i])) {
                LinearEdge& edge = std::get<LinearEdge>(edges[i]);

                Vec3 prev = lerp(edge.from, edge.to, 0);

                for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

                    Vec3 curr = lerp(edge.from, edge.to, (float)i / (float)VECTOR_RES);

                    VectorEdgeBufferObject edgeObj;
                    edgeObj.start       = prev;
                    edgeObj.end         = curr;
                    edgeObj.modelNumber = modelNumber.value();

                    element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);

                    prev = curr;
                }
            }

            if (std::holds_alternative<QuadraticEdge>(edges[i])) {
                QuadraticEdge& edge = std::get<QuadraticEdge>(edges[i]);

                Vec3 prev = quadraticBezier(edge.from, edge.control, edge.to, 0);

                for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

                    Vec3 curr = quadraticBezier(edge.from, edge.control, edge.to, (float)i / (float)VECTOR_RES);

                    VectorEdgeBufferObject edgeObj;
                    edgeObj.start       = prev;
                    edgeObj.end         = curr;
                    edgeObj.modelNumber = modelNumber.value();

                    element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);

                    prev = curr;
                }
            }

            if (std::holds_alternative<CubicEdge>(edges[i])) {
                
                CubicEdge& edge = std::get<CubicEdge>(edges[i]);

                Vec3 prev = cubicBezier(edge.from, edge.control1, edge.control2, edge.to, 0);

                for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

                    Vec3 curr = cubicBezier(edge.from, edge.control1, edge.control2, edge.to, (float)i / (float)VECTOR_RES);

                    VectorEdgeBufferObject edgeObj;
                    edgeObj.start       = prev;
                    edgeObj.end         = curr;
                    edgeObj.modelNumber = modelNumber.value();

                    element = vectorEngineInstances[ref.instanceNumber].edges.push(edgeObj);

                    prev = curr;
                }
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