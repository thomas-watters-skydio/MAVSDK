#pragma once

#include "plugins/poi/poi.h"
#include "plugin_impl_base.h"

namespace mavsdk {

class PoiImpl : public PluginImplBase {
public:
    explicit PoiImpl(System& system);
    explicit PoiImpl(std::shared_ptr<System> system);
    ~PoiImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void subscribe_poi_report(Poi::PoiReportCallback callback);

    Poi::PoiReport poi_report() const;

private:

    void process_poi_report(const mavlink_message_t& message);

    std::mutex _subscription_mutex{};
    Poi::PoiReportCallback _poi_report_subscription{nullptr};
};

} // namespace mavsdk