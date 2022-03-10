#include "poi_impl.h"

namespace mavsdk {

PoiImpl::PoiImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

PoiImpl::PoiImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

PoiImpl::~PoiImpl()
{
    _parent->unregister_plugin(this);
}

void PoiImpl::init() {}

void PoiImpl::deinit() {}

void PoiImpl::enable() {}

void PoiImpl::disable() {}

void PoiImpl::subscribe_poi_report(Poi::PoiReportCallback /*callback*/) {}

Poi::PoiReport PoiImpl::poi_report() const
{
    return {};
}

} // namespace mavsdk