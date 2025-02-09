#ifndef SPLITGUI_RESULT_HPP
#define SPLITGUI_RESULT_HPP

#include "lib.hpp"

#include <string>

namespace SplitGui {

    #define TRY(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { return name.result; }
    #define TRYD(resultValue) if (resultValue.result != SplitGui::Result::eSuccess) { return resultValue.result; }
    #define TRYC(type, name, resultValue) SplitGui::ResultValue<type> name = resultValue; if (name.result != SplitGui::Result::eSuccess) { SPLITGUI_LOG("Failed with code %d", (int)name.result); return (int)name.result; }
    #define TRYR(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { return name; }
    #define TRYRC(name, result) SplitGui::Result name = result; if (name != SplitGui::Result::eSuccess) { SPLITGUI_LOG("Failed with code %d", (int)name); return (int)name; }

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
        eFailedToCreateGraphicsPipeline,
        eFailedToSubmitQueue,
        eFailedToUseFont,
        eFailedToLoadFont,
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
        eInvalidExpression,
        eInvalidPrematureClosure,
        eInvalidXml,
        eInvalidTag,
    };

    // Similar to vulkan's result values
    template <typename T>
    class SPLITGUI_EXPORT ResultValue {
        public:
            T value;
            std::string message;
            Result result;

            ResultValue() : value(), message(), result(Result::eSuccess) {}

            ResultValue(T value) :                           value(value), message(),        result(Result::eSuccess) {}
            ResultValue(Result error) :                      value(),      message(),        result(error) {}
            ResultValue(Result error, std::string message) : value(),      message(message), result(error) {}

            ResultValue<T> operator=(const Result inputResult) { return ResultValue<T>(inputResult); }
    
    };
}

#endif