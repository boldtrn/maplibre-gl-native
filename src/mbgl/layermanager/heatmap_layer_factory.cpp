#include <mbgl/layermanager/heatmap_layer_factory.hpp>

#include <mbgl/renderer/buckets/heatmap_bucket.hpp>
#include <mbgl/renderer/layers/render_heatmap_layer.hpp>
#include <mbgl/style/layers/heatmap_layer.hpp>
#include <mbgl/style/layers/heatmap_layer_impl.hpp>

namespace mbgl {

const style::LayerTypeInfo* HeatmapLayerFactory::getTypeInfo() const noexcept {
    return style::HeatmapLayer::Impl::staticTypeInfo();
}

std::unique_ptr<style::Layer> HeatmapLayerFactory::createLayer(const std::string& id, const style::conversion::Convertible& value) noexcept {
    auto const source = getSource(value);
    if (!source) {
        return nullptr;
    }

    return std::unique_ptr<style::Layer>(new style::HeatmapLayer(id, *source));
}

std::unique_ptr<Bucket> HeatmapLayerFactory::createBucket(const BucketParameters& parameters, const std::vector<Immutable<style::LayerProperties>>& layers) noexcept {
    return std::make_unique<HeatmapBucket>(parameters, layers);
}

std::unique_ptr<RenderLayer> HeatmapLayerFactory::createRenderLayer(Immutable<style::Layer::Impl> impl) noexcept {
    assert(impl->getTypeInfo() == getTypeInfo());
    return std::make_unique<RenderHeatmapLayer>(staticImmutableCast<style::HeatmapLayer::Impl>(impl));
}

} // namespace mbgl
