#include "vulkan.hpp"

// recreation of methods from msdfgen/ext/import-font.cpp/.h
namespace msdfgen {
    #define MSDFGEN_LEGACY_FONT_COORDINATE_SCALE (1/64.)

    enum class FontCoordinateScaling {
        eFontScalingNone,
        eFontScalingEmNormalized,
        eFontScalingLegacy,
    };

    struct FtContext {
        double scale;
        Point2 position;
        Shape *shape;
        Contour *contour;
    };

    static Point2 ftPoint2(const ft::FT_Vector &vector, double scale) {
        return Point2(scale*vector.x, scale*vector.y);
    }

    constexpr double getFontCoordinateScale(const ft::FT_Face &face, FontCoordinateScaling coordinateScaling) {
        switch (coordinateScaling) {
            case FontCoordinateScaling::eFontScalingNone:
                return 1;
            case FontCoordinateScaling::eFontScalingEmNormalized:
                return 1./(face->units_per_EM ? face->units_per_EM : 1);
            case FontCoordinateScaling::eFontScalingLegacy:
                return MSDFGEN_LEGACY_FONT_COORDINATE_SCALE;
        }
        return 1;
    }

    static int ftMoveTo(const ft::FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        if (!(context->contour && context->contour->edges.empty()))
            context->contour = &context->shape->addContour();
        context->position = ftPoint2(*to, context->scale);
        return 0;
    }

    static int ftLineTo(const ft::FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        Point2 endpoint = ftPoint2(*to, context->scale);
        if (endpoint != context->position) {
            context->contour->addEdge(EdgeHolder(context->position, endpoint));
            context->position = endpoint;
        }
        return 0;
    }

    static int ftConicTo(const ft::FT_Vector *control, const ft::FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        Point2 endpoint = ftPoint2(*to, context->scale);
        if (endpoint != context->position) {
            context->contour->addEdge(EdgeHolder(context->position, ftPoint2(*control, context->scale), endpoint));
            context->position = endpoint;
        }
        return 0;
    }

    static int ftCubicTo(const ft::FT_Vector *control1, const ft::FT_Vector *control2, const ft::FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        Point2 endpoint = ftPoint2(*to, context->scale);
        if (endpoint != context->position || crossProduct(ftPoint2(*control1, context->scale)-endpoint, ftPoint2(*control2, context->scale)-endpoint)) {
            context->contour->addEdge(EdgeHolder(context->position, ftPoint2(*control1, context->scale), ftPoint2(*control2, context->scale), endpoint));
            context->position = endpoint;
        }
        return 0;
    }

    ft::FT_Error readFreetypeOutline(Shape &output, ft::FT_Outline *outline, double scale) {
        output.contours.clear();
        output.inverseYAxis = false;
        FtContext context = { };
        context.scale = scale;
        context.shape = &output;
        ft::FT_Outline_Funcs ftFunctions;
        ftFunctions.move_to = &ftMoveTo;
        ftFunctions.line_to = &ftLineTo;
        ftFunctions.conic_to = &ftConicTo;
        ftFunctions.cubic_to = &ftCubicTo;
        ftFunctions.shift = 0;
        ftFunctions.delta = 0;
        ft::FT_Error error = ft::FT_Outline_Decompose(outline, &ftFunctions, &context);
        if (!output.contours.empty() && output.contours.back().edges.empty())
            output.contours.pop_back();
        return error;
    }
}