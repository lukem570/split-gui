#ifndef SPLITGUI_RESULT_HPP
#define SPLITGUI_RESULT_HPP

#include "lib.hpp"

#include <string>

namespace SplitGui {

    #define TRY(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { return name.result; }
    #define TRYD(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { return resultValue.result; }
    #define TRYC(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { printf("Failed with code %d\n", (int)name.result); return (int)name.result; }
    #define TRYR(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { return name; }
    #define TRYRC(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { printf("Failed with code %d\n", (int)name); return (int)name; }
    #define TRYDC(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { printf("Failed with code %d\n", (int)resultValue.result); return (int)resultValue.result; }

    enum class Result {
        eSuccess,

        // system
        eFailedToOpenFile,
        eHeapAllocFailed,

        // vulkan
        eVulkanNotSupported,
        eVulkanNotFound, 
        eDeviceNotFound,
        eFailedToInstanceFreeType,
        eFailedToFindSurfaceExtension,
        eFailedToFindSwapchainExtension,
        eFailedToFindQueueFamily,
        eFailedToFindSuitableMemoryType,
        eFailedToFindSuitableFormat,
        eFailedToCreateGraphicsPipeline,
        eFailedToCreateComputePipeline,
        eFailedToSubmitQueue,
        eFailedToUseFont,
        eFailedToLoadFont,
        eFailedToLoadGlyph,
        eFailedToSetFontSize,
        eFailedToWaitForFences,
        eFailedToResetFences, 
        eFailedToGetNextSwapchainImage,
        eFailedToGetLayer,
        eFailedToCreateSurface,
        eInvalidRectRefUsed,
        eInvalidTrianglesRefUsed,
        eSceneAlreadyHasVectorEngine,
        eCouldNotFindElementInList,
        eFailedToParseSvg,
        eInvalidCropRegionRefUsed,

        // interface
        eInvalidNumberOfChildren,
        eInvalidToken,
        eInvalidType,
        eInvalidDirection,
        eInvalidOrigin,
        eInvalidSetting,
        eInvalidExpression,
        eInvalidPrematureClosure,
        eInvalidXml,
        eInvalidTag,
        eMissingGraphics,
    };

    // Similar to vulkan's result values
    template <typename T>
    class SPLITGUI_EXPORT ResultValue {
        public:
            T value;
            Result result;

            ResultValue() : value(), result(Result::eSuccess) {}

            ResultValue(T value)      : value(value), result(Result::eSuccess) {}
            ResultValue(Result error) : value(),      result(error) {}

            ResultValue<T> operator=(const Result inputResult) { return ResultValue<T>(inputResult); }
    
    };
}

#endif