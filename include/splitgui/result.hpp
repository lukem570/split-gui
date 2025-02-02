#ifndef SPLITGUI_RESULT_HPP
#define SPLITGUI_RESULT_HPP

#include <string>

namespace SplitGui {

    #define TRY(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { return name.result; }
    #define TRYD(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { return resultValue.result; }
    #define TRYC(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { printf("Failed with code %d\n", (int)name.result); return (int)name.result; }
    #define TRYR(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { return name; }
    #define TRYRC(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { printf("Failed with code %d\n", (int)name); return (int)name; }

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