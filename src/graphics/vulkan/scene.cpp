#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<SceneRef> VulkanInterface::instanceScene(Vec2 x1, Vec2 x2, float depth) {

        SceneRef reference;
        reference.sceneNumber = scenes.size();

        IVec2 windowSize = pWindow->getSize();
        Vec2 delta = x1 - x2;

        scenes.push_back({});

        scenes.back().sceneSize = { (int)(windowSize.x * std::abs(delta.x) / 2.0f), (int)(windowSize.y * std::abs(delta.y) / 2.0f) };
        
        createSceneDescriptorPool(scenes.back());
        TRYR(pipelineRes, createScenePipeline(scenes.back()));
        TRYR(depthRes, createSceneDepthResources(scenes.back()));
        TRYR(outputRes, createSceneOutputResources(scenes.back()));
        createSceneFramebuffers(scenes.back());
        createSceneDescriptorSet(scenes.back());
        TRYR(dataRes, createSceneDataUniform(scenes.back()));
        updateSceneDescriptorSet(scenes.back());

        reference.rect = drawRect(x1, x2, HexColor(0xFF00FF).normalize(), depth, VertexFlagsBits::eScene, reference.sceneNumber);
        
        SPLITGUI_LOG("Created Scene: %ld, (%d, %d)", scenes.size(), scenes.back().sceneSize.x, scenes.back().sceneSize.y);

        return reference;
    }

    inline Result VulkanInterface::createScenesImageArray() {

        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType     = vk::ImageType::e2D;
        imageInfo.format        = vk_surfaceFormat.format;
        imageInfo.extent.width  = vk_swapchainExtent.width;
        imageInfo.extent.height = vk_swapchainExtent.height;
        imageInfo.extent.depth  = 1;
        imageInfo.mipLevels     = 1;
        imageInfo.arrayLayers   = MAX_SCENES;
        imageInfo.samples       = vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = vk::ImageTiling::eOptimal;
        imageInfo.usage         = vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
        imageInfo.sharingMode   = vk::SharingMode::eExclusive;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_scenesImageArrayImages = vk_device.createImage(imageInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getImageMemoryRequirements(vk_scenesImageArrayImages);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType);

        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memType.value;

        vk_scenesImageArrayImageMemory = vk_device.allocateMemory(allocateInfo);
        vk_device.bindImageMemory(vk_scenesImageArrayImages, vk_scenesImageArrayImageMemory, 0);

        vk::SamplerCreateInfo samplerInfo;
        samplerInfo.magFilter               = vk::Filter::eNearest;
        samplerInfo.minFilter               = vk::Filter::eNearest;
        samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eNearest;
        samplerInfo.addressModeU            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.addressModeV            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.addressModeW            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.mipLodBias              = 0.0f;
        samplerInfo.anisotropyEnable        = vk::False;
        samplerInfo.maxAnisotropy           = 1.0f;
        samplerInfo.compareEnable           = vk::False;
        samplerInfo.compareOp               = vk::CompareOp::eNever;
        samplerInfo.borderColor             = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = vk::False;
        samplerInfo.minLod                  = 0.0f;
        samplerInfo.maxLod                  = 1.0f;

        vk_scenesImageArraySampler = vk_device.createSampler(samplerInfo);

        vk::ImageViewCreateInfo imageViewInfo;
        imageViewInfo.image                           = vk_scenesImageArrayImages;
        imageViewInfo.viewType                        = vk::ImageViewType::e2DArray;
        imageViewInfo.format                          = imageInfo.format;
        imageViewInfo.components.r                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.g                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.b                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.a                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        imageViewInfo.subresourceRange.baseMipLevel   = 0;
        imageViewInfo.subresourceRange.levelCount     = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount     = imageInfo.arrayLayers;

        vk_scenesImageArrayImageView = vk_device.createImageView(imageViewInfo);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = vk_scenesImageArrayImages;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = imageInfo.arrayLayers;
        barrier.srcAccessMask                   = vk::AccessFlagBits(0);
        barrier.dstAccessMask                   = vk::AccessFlagBits::eMemoryWrite;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe, 
            vk::PipelineStageFlagBits::eTransfer, 
            vk::DependencyFlagBits(0), 
            0, nullptr, 0, nullptr, 1, 
            &barrier
        );

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        SPLITGUI_LOG("Created Scenes Image Array");

        return Result::eSuccess;
    }

    Result VulkanInterface::submitTriangleData(SceneRef& ref, std::vector<Vertex>& newVertices, std::vector<uint16_t>& newIndices, int flags, int textureNumber) {

        unsigned int oldVerticesSize = scenes[ref.sceneNumber].vertices.size();
        unsigned int oldIndicesSize  = scenes[ref.sceneNumber].indices.size();

        scenes[ref.sceneNumber].vertices.resize(oldVerticesSize + newIndices.size());
        scenes[ref.sceneNumber].indices.resize(oldIndicesSize + newIndices.size());

        for (unsigned int i = 0; i < newIndices.size(); i += 3) { 
            // TODO: FIX THIS, THIS IS BAD LIKE REALLY BAD
            // to solve: create a uniform containing surface normals and use like a geometry shader to assign normals

            Vec3 points[3];

            for (unsigned int j = 0; j < 3; j++) {

                SceneVertexBufferObject vbo;
                vbo.vertex        = newVertices[newIndices[i + j]];
                vbo.flags         = VertexFlagsBits::eScene ^ flags;
                vbo.modelNumber   = 0;
                vbo.textureNumber = textureNumber;

                scenes[ref.sceneNumber].vertices[oldVerticesSize + i + j] = vbo;

                scenes[ref.sceneNumber].indices[oldIndicesSize + i + j] = oldVerticesSize + i + j;

                points[j] = vbo.vertex.pos;
            }

            Vec3 edge1 = points[1] - points[0];
            Vec3 edge2 = points[2] - points[0];

            Vec3 normal = edge1.cross(edge2);

            normal.normalize();
            
            scenes[ref.sceneNumber].vertices[oldVerticesSize + i + 0].normal = normal;
            scenes[ref.sceneNumber].vertices[oldVerticesSize + i + 1].normal = normal;
            scenes[ref.sceneNumber].vertices[oldVerticesSize + i + 2].normal = normal;
        }

        vk::DeviceSize   indexBufferSize;
        vk::Buffer       stagingIndexBuffer;
        vk::DeviceMemory stagingIndexBufferMemory;

        vk::DeviceSize   vertexBufferSize;
        vk::Buffer       stagingVertexBuffer;
        vk::DeviceMemory stagingVertexBufferMemory;

        TRYR(stagingRes1, InstanceStagingBuffer(scenes[ref.sceneNumber].indices,  stagingIndexBuffer,  stagingIndexBufferMemory,  indexBufferSize ));
        TRYR(stagingRes2, InstanceStagingBuffer(scenes[ref.sceneNumber].vertices, stagingVertexBuffer, stagingVertexBufferMemory, vertexBufferSize));

        vk::Buffer       tempIndexBuffer;
        vk::DeviceMemory tempIndexBufferMemory;

        vk::Buffer       tempVertexBuffer;
        vk::DeviceMemory tempVertexBufferMemory;

        TRYR(bufferRes1, createBuffer(
            indexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempIndexBuffer,
            tempIndexBufferMemory
        ));

        TRYR(bufferRes2, createBuffer(
            vertexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempVertexBuffer,
            tempVertexBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        vk::BufferCopy copyRegionIndex;
        vk::BufferCopy copyRegionVertex;

        copyBuffer(stagingIndexBuffer,  tempIndexBuffer, indexBufferSize,  commandBuffer, copyRegionIndex);
        copyBuffer(stagingVertexBuffer, tempVertexBuffer, vertexBufferSize, commandBuffer, copyRegionVertex);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.waitIdle();
        
        cleanupSceneVertexAndIndexBuffers(ref);

        scenes[ref.sceneNumber].vertexBuffer       = tempVertexBuffer;
        scenes[ref.sceneNumber].vertexBufferMemory = tempVertexBufferMemory;

        scenes[ref.sceneNumber].indexBuffer        = tempIndexBuffer;
        scenes[ref.sceneNumber].indexBufferMemory  = tempIndexBufferMemory;

        vk_device.destroyBuffer(stagingVertexBuffer);
        vk_device.freeMemory(stagingVertexBufferMemory);

        vk_device.destroyBuffer(stagingIndexBuffer);
        vk_device.freeMemory(stagingIndexBufferMemory);
        
        scenes[ref.sceneNumber].knownIndicesSize = scenes[ref.sceneNumber].indices.size();
        
        SPLITGUI_LOG("Submitted Triangles: %d", scenes[ref.sceneNumber].knownIndicesSize);

        return Result::eSuccess;
    }

    ModelRef VulkanInterface::createModel(SceneRef& ref, Mat4& model) { // TODO:
        ModelRef outRef;
        outRef.modelNumber = scenes[ref.sceneNumber].models.size();

        scenes[ref.sceneNumber].models.push_back(model);

        return outRef;
    }

    Result VulkanInterface::updateScene(SceneRef& ref, Vec2 x1, Vec2 x2) {

        IVec2 windowSize = pWindow->getSize();
        Vec2 delta = x1 - x2;

        vk_device.waitIdle();

        scenes[ref.sceneNumber].sceneSize = { (int)(windowSize.x * std::abs(delta.x) / 2.0f), (int)(windowSize.y * std::abs(delta.y) / 2.0f) };

        cleanupSceneFrameBuffers(ref);
        cleanupSceneOutputImages(ref);
        cleanupSceneDepthImages(ref);

        TRYR(depthRes, createSceneDepthResources(scenes[ref.sceneNumber]));
        TRYR(outputRes, createSceneOutputResources(scenes[ref.sceneNumber]));
        createSceneFramebuffers(scenes[ref.sceneNumber]);

        updateRect(ref.rect, x1, x2, HexColor(0xFF00FF).normalize(), 0);

        SPLITGUI_LOG("Updated Scene: %d", ref.sceneNumber);

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraView(SceneRef& ref, Mat4& view) {

        scenes[ref.sceneNumber].sceneData.cameraView = view;

        vk::DeviceSize   bufferSize = sizeof(SceneObj);
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, bufferSize);

        std::memcpy(memory, &scenes[ref.sceneNumber].sceneData, bufferSize);

        vk_device.unmapMemory(stagingBufferMemory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        
        vk::BufferCopy copyRegion;
        
        copyBuffer(stagingBuffer,  scenes[ref.sceneNumber].dataUniformBuffer, bufferSize,  commandBuffer, copyRegion);
        
        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);
        
        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraProjection(SceneRef& ref, Mat4& projection) {

        scenes[ref.sceneNumber].sceneData.cameraProjection = projection;

        vk::DeviceSize   bufferSize = sizeof(SceneObj);
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, bufferSize);

        std::memcpy(memory, &scenes[ref.sceneNumber].sceneData, bufferSize);

        vk_device.unmapMemory(stagingBufferMemory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        
        vk::BufferCopy copyRegion;
        
        copyBuffer(stagingBuffer,  scenes[ref.sceneNumber].dataUniformBuffer, bufferSize,  commandBuffer, copyRegion);
        
        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraPosition(SceneRef& ref, Vec3& position) {

        scenes[ref.sceneNumber].sceneData.cameraPosition = position;

        vk::DeviceSize   bufferSize = sizeof(SceneObj);
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, bufferSize);

        std::memcpy(memory, &scenes[ref.sceneNumber].sceneData, bufferSize);

        vk_device.unmapMemory(stagingBufferMemory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        
        vk::BufferCopy copyRegion;
        
        copyBuffer(stagingBuffer,  scenes[ref.sceneNumber].dataUniformBuffer, bufferSize,  commandBuffer, copyRegion);
        
        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        return Result::eSuccess;
    }
}