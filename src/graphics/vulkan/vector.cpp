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

    Result VulkanInterface::deleteEdges(VectorEngineRef& vEngineRef, EdgeRef& edgeRef) {
        SPLITGUI_PROFILE;

        VectorEngineObject& vEng = vectorEngineInstances[vEngineRef.instanceNumber];

        vEng.edges.erase(edgeRef.edgesStart, edgeRef.edgesEnd);

        TRYR(submitRes, submitVectorEngineEdgeResources(vEng));

        return Result::eSuccess;
    }

    std::array<VectorEdgeBufferObject, VECTOR_RES> proccessLinearEdge(LinearEdge& edge, Vec3& color, unsigned int modelNumber) {
        std::array<VectorEdgeBufferObject, VECTOR_RES> ret;

        Vec3 prev = lerp(edge.from, edge.to, 0);

        for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

            Vec3 curr = lerp(edge.from, edge.to, (float)i / (float)VECTOR_RES);

            VectorEdgeBufferObject edgeObj;
            edgeObj.start       = prev;
            edgeObj.end         = curr;
            edgeObj.modelNumber = modelNumber;
            edgeObj.color       = color;

            ret[i - 1] = edgeObj;

            prev = curr;
        }

        return ret;
    }

    std::array<VectorEdgeBufferObject, VECTOR_RES> proccessQuadraticEdge(QuadraticEdge& edge, Vec3& color, unsigned int modelNumber) {
        std::array<VectorEdgeBufferObject, VECTOR_RES> ret;

        Vec3 prev = quadraticBezier(edge.from, edge.control, edge.to, 0);

        for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

            Vec3 curr = quadraticBezier(edge.from, edge.control, edge.to, (float)i / (float)VECTOR_RES);

            VectorEdgeBufferObject edgeObj;
            edgeObj.start       = prev;
            edgeObj.end         = curr;
            edgeObj.modelNumber = modelNumber;
            edgeObj.color       = color;

            ret[i - 1] = edgeObj;

            prev = curr;
        }

        return ret;
    }

    std::array<VectorEdgeBufferObject, VECTOR_RES> proccessCubicEdge(CubicEdge& edge, Vec3& color, unsigned int modelNumber) {
        std::array<VectorEdgeBufferObject, VECTOR_RES> ret;

        Vec3 prev = cubicBezier(edge.from, edge.control1, edge.control2, edge.to, 0);

        for (unsigned int i = 1; i < VECTOR_RES + 1; i++) {

            Vec3 curr = cubicBezier(edge.from, edge.control1, edge.control2, edge.to, (float)i / (float)VECTOR_RES);

            VectorEdgeBufferObject edgeObj;
            edgeObj.start       = prev;
            edgeObj.end         = curr;
            edgeObj.modelNumber = modelNumber;
            edgeObj.color       = color;

            ret[i - 1] = edgeObj;

            prev = curr;
        }

        return ret;
    }

    std::array<VectorEdgeBufferObject, VECTOR_RES> proccessEdge(Edge& edge, Vec3& color, unsigned int modelNumber) {
        if (std::holds_alternative<LinearEdge>(edge)) {

            return proccessLinearEdge(std::get<LinearEdge>(edge), color, modelNumber);
        }

        if (std::holds_alternative<QuadraticEdge>(edge)) {

            return proccessQuadraticEdge(std::get<QuadraticEdge>(edge), color, modelNumber);
        }

        if (std::holds_alternative<CubicEdge>(edge)) {

            return proccessCubicEdge(std::get<CubicEdge>(edge), color, modelNumber);
        }

        return {};
    }

    ResultValue<EdgeRef> VulkanInterface::submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges, ModelRef model, Vec3 color) {
        SPLITGUI_PROFILE;

        EdgeRef edgeRef;

        VectorEngineObject& vEng = vectorEngineInstances[ref.instanceNumber];
        SceneObject& scene = scenes[vEng.scene.sceneNumber];

        std::optional<unsigned int> modelNumber = scene.models.offset(model.model);

        if (!modelNumber.has_value()) {
            return Result::eCouldNotFindElementInList;
        }

        for (auto& edge : edges) {

            std::array<VectorEdgeBufferObject, VECTOR_RES> edgeObjs = proccessEdge(edge, color, modelNumber.value());

            for (auto& edgeObj : edgeObjs) {
                LinkElement<VectorEdgeBufferObject>* element = vEng.edges.push(edgeObj);

                if (!edgeRef.edgesStart) 
                    edgeRef.edgesStart = element;
                
                edgeRef.edgesEnd = element;
            }
        }

        if (!(vEng.edges.size() > 0)) {
            return edgeRef;
        }
        
        TRYR(submitRes, submitVectorEngineEdgeResources(vEng));
        updateVectorEngineEdges(vEng);

        return edgeRef;
    }

    Result VulkanInterface::resubmitEdgeData(VectorEngineRef& ref, EdgeRef edgeRef, std::vector<Edge>& edges, ModelRef model, Vec3 color) {
        SPLITGUI_PROFILE;

        VectorEngineObject& vEng = vectorEngineInstances[ref.instanceNumber];
        SceneObject& scene = scenes[vEng.scene.sceneNumber];

        std::optional<unsigned int> modelNumber = scene.models.offset(model.model);

        if (!modelNumber.has_value()) {
            return Result::eCouldNotFindElementInList;
        }

        LinkElement<VectorEdgeBufferObject>* current = edgeRef.edgesStart;

        for (auto& edge : edges) {

            if (current == edgeRef.edgesEnd->next) 
                return Result::eSizeMismatch;

            std::array<VectorEdgeBufferObject, VECTOR_RES> edgeObjs = proccessEdge(edge, color, modelNumber.value());

            for (auto& edgeObj : edgeObjs) {
                current->data = edgeObj;

                current = current->next;
            }
        }

        if (!(vEng.edges.size() > 0)) {
            return Result::eSuccess;
        }
        
        TRYR(submitRes, submitVectorEngineEdgeResources(vEng));
        updateVectorEngineEdges(vEng);

        return Result::eSuccess;
    }
}