#pragma once

#include <mbgl/text/quads.hpp>
#include <mbgl/text/glyph_atlas.hpp>
#include <mbgl/text/collision_feature.hpp>
#include <mbgl/style/layers/symbol_layer_properties.hpp>
#include <mbgl/util/bitmask_operations.hpp>

namespace mbgl {

class Anchor;
class IndexedSubfeature;

struct ShapedTextOrientations {
    Shaping horizontal;
    Shaping vertical;
    // The following are used with variable text placement on.
    Shaping& right = horizontal; 
    Shaping center;
    Shaping left;
    bool singleLine = false;
};

enum class SymbolContent : uint8_t { None = 0, Text = 1 << 0, IconRGBA = 1 << 1, IconSDF = 1 << 2 };

struct SymbolInstanceSharedData {
    SymbolInstanceSharedData(GeometryCoordinates line,
                             const ShapedTextOrientations& shapedTextOrientations,
                             const std::optional<PositionedIcon>& shapedIcon,
                             const std::optional<PositionedIcon>& verticallyShapedIcon,
                             const style::SymbolLayoutProperties::Evaluated& layout,
                             style::SymbolPlacementType textPlacement,
                             const std::array<float, 2>& textOffset,
                             const ImageMap& imageMap,
                             float iconRotation,
                             SymbolContent iconType,
                             bool hasIconTextFit,
                             bool allowVerticalPlacement);
    bool empty() const;
    GeometryCoordinates line;
    // Note: When singleLine == true, only `rightJustifiedGlyphQuads` is populated.
    SymbolQuads rightJustifiedGlyphQuads;
    SymbolQuads centerJustifiedGlyphQuads;
    SymbolQuads leftJustifiedGlyphQuads;
    SymbolQuads verticalGlyphQuads;
    std::optional<SymbolQuads> iconQuads;
    std::optional<SymbolQuads> verticalIconQuads;
};

class SymbolInstance {
public:
    SymbolInstance(Anchor& anchor_,
                   std::shared_ptr<SymbolInstanceSharedData> sharedData,
                   const ShapedTextOrientations& shapedTextOrientations,
                   const std::optional<PositionedIcon>& shapedIcon,
                   const std::optional<PositionedIcon>& verticallyShapedIcon,
                   float textBoxScale,
                   float textPadding,
                   style::SymbolPlacementType textPlacement,
                   const std::array<float, 2>& textOffset,
                   float iconBoxScale,
                   float iconPadding,
                   const std::array<float, 2>& iconOffset,
                   const IndexedSubfeature& indexedFeature,
                   std::size_t layoutFeatureIndex,
                   std::size_t dataFeatureIndex,
                   std::u16string key,
                   float overscaling,
                   float iconRotation,
                   float textRotation,
                   const std::array<float, 2>& variableTextOffset,
                   bool allowVerticalPlacement,
                   SymbolContent iconType = SymbolContent::None);

    std::optional<size_t> getDefaultHorizontalPlacedTextIndex() const;
    const GeometryCoordinates& line() const;
    const SymbolQuads& rightJustifiedGlyphQuads() const;
    const SymbolQuads& leftJustifiedGlyphQuads() const;
    const SymbolQuads& centerJustifiedGlyphQuads() const;
    const SymbolQuads& verticalGlyphQuads() const;
    bool hasText() const;
    bool hasIcon() const;
    bool hasSdfIcon() const;
    const std::optional<SymbolQuads>& iconQuads() const;
    const std::optional<SymbolQuads>& verticalIconQuads() const;
    void releaseSharedData();

private:
    std::shared_ptr<SymbolInstanceSharedData> sharedData;

public:
    Anchor anchor;
    SymbolContent symbolContent;

    std::size_t rightJustifiedGlyphQuadsSize;
    std::size_t centerJustifiedGlyphQuadsSize;
    std::size_t leftJustifiedGlyphQuadsSize;
    std::size_t verticalGlyphQuadsSize;
    std::size_t iconQuadsSize;

    CollisionFeature textCollisionFeature;
    CollisionFeature iconCollisionFeature;
    std::optional<CollisionFeature> verticalTextCollisionFeature = std::nullopt;
    std::optional<CollisionFeature> verticalIconCollisionFeature = std::nullopt;
    WritingModeType writingModes;
    std::size_t layoutFeatureIndex; // Index into the set of features included at layout time
    std::size_t dataFeatureIndex;   // Index into the underlying tile data feature set
    std::array<float, 2> textOffset;
    std::array<float, 2> iconOffset;
    std::u16string key;
    bool isDuplicate;
    std::optional<size_t> placedRightTextIndex;
    std::optional<size_t> placedCenterTextIndex;
    std::optional<size_t> placedLeftTextIndex;
    std::optional<size_t> placedVerticalTextIndex;
    std::optional<size_t> placedIconIndex;
    std::optional<size_t> placedVerticalIconIndex;
    float textBoxScale;
    std::array<float, 2> variableTextOffset;
    bool singleLine;
    uint32_t crossTileID = 0;

    static constexpr uint32_t invalidCrossTileID() { return std::numeric_limits<uint32_t>::max(); }
};

using SymbolInstanceReferences = std::vector<std::reference_wrapper<const SymbolInstance>>;

} // namespace mbgl
