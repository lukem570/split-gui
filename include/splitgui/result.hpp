#ifndef SPLITGUI_RESULT_HPP
#define SPLITGUI_RESULT_HPP

#include <string>

namespace SplitGui {

    #define TRY(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { return resultValue.result; }
    #define TRYC(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { return (int)resultValue.result; }
    #define TRYR(result) if (result != SplitGui::Result::eSuccess) { return result; }
    #define TRYRC(result) if (result != SplitGui::Result::eSuccess) { return (int)result; }

    enum class Result {
        eSuccess,

        // system
        eFailedToOpenFile,

        // vulkan
        eVulkanNotSupported,
        eVulkanNotFound, 
        eDeviceNotFound,
        eFailedToInstanceFreeType,
        eFailedToFindSurfaceExtension,
        eFailedToFindSwapchainExtension,
        eFailedToFindQueueFamily,
        eFailedToFindSuitableMemoryType,
        eFailedToCreateGraphicsPipeline,
        eFailedToSubmitQueue,
        eFailedToUseFont,
        eFailedToLoadGlyph,
        eFailedToWaitForFences,
        eFailedToResetFences, 
        eFailedToGetNextSwapchainImage,
        eFailedToGetLayer,
        eFailedToCreateSurface,

        // interface
        eInvalidNumberOfChildren,
        eInvalidToken,
        eInvalidType,
        eInvalidDirection,
        eInvalidSetting,
        eInvalidPrematureClosure,
        eInvalidXml,
        eInvalidTag,
    };

    // Similar to vulkan's result values
    template <typename T>
    class ResultValue {
        public:
            T value;
            std::string message;
            Result result;

            ResultValue() : value(), message(), result(Result::eSuccess) {}

            ResultValue(T value) :                           value(value), message(),        result(Result::eSuccess) {}
            ResultValue(Result error) :                      value(),      message(),        result(result) {}
            ResultValue(Result error, std::string message) : value(),      message(message), result(result) {}

            ResultValue<T> operator=(const Result inputResult) { return ResultValue<T>(inputResult); }
    
    };
}

#endif