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

void PoiImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_POI_REPORT,
        [this](const mavlink_message_t& message) { process_poi_report(message); },
        this);
}

void PoiImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void PoiImpl::enable() {}

void PoiImpl::disable() {}

void PoiImpl::subscribe_poi_report(Poi::PoiReportCallback callback)
{
    std::lock_guard<std::mutex> lock(_subscription_mutex);
    _poi_report_subscription = callback;
}

Poi::PoiReport PoiImpl::poi_report() const
{
    return Poi::PoiReport{};
}

void PoiImpl::process_poi_report(const mavlink_message_t& message)
{
    mavlink_poi_report_t poi_report;
    mavlink_msg_poi_report_decode(&message, &poi_report);

    Poi::PoiReport poi_report_out;

    poi_report_out.uid = poi_report.uid;
    poi_report_out.time_utc_detected = poi_report.time_utc_detected;
    poi_report_out.time_utc_updated = poi_report.time_utc_updated;
    poi_report_out.time_boot_ms = poi_report.time_boot_ms;
    poi_report_out.latitude = poi_report.latitude;
    poi_report_out.longitude = poi_report.longitude;
    poi_report_out.alt_msl = poi_report.alt_msl;
    poi_report_out.alt_ellip = poi_report.alt_ellip;
    poi_report_out.alt_ground = poi_report.alt_ground;
    poi_report_out.classification = poi_report.classification;
    poi_report_out.x = poi_report.x;
    poi_report_out.y = poi_report.y;
    poi_report_out.z = poi_report.z;
    poi_report_out.q = decltype(poi_report_out.q)(poi_report.q, poi_report.q + 4);
    poi_report_out.dist = poi_report.dist;
    poi_report_out.vel_n = poi_report.vel_n;
    poi_report_out.vel_e = poi_report.vel_e;
    poi_report_out.vel_d = poi_report.vel_d;
    poi_report_out.hdg = poi_report.hdg;
    poi_report_out.height = poi_report.height;
    poi_report_out.width = poi_report.width;
    poi_report_out.depth = poi_report.depth;
    poi_report_out.approach_vector_start = decltype(poi_report_out.approach_vector_start)(
        poi_report.approach_vector_start, poi_report.approach_vector_start + 3);
    poi_report_out.approach_vector_end = decltype(poi_report_out.approach_vector_end)(
        poi_report.approach_vector_end, poi_report.approach_vector_end + 3);
    poi_report_out.approach_velocity = decltype(poi_report_out.approach_velocity)(
        poi_report.approach_velocity, poi_report.approach_velocity + 3);
    poi_report_out.ttl = poi_report.ttl;
    poi_report_out.confidence_overall = poi_report.confidence_overall;
    poi_report_out.confidence_detection = poi_report.confidence_detection;
    poi_report_out.confidence_classification = poi_report.confidence_classification;
    poi_report_out.confidence_localization = poi_report.confidence_localization;
    poi_report_out.status_flags = poi_report.status_flags;
    poi_report_out.geometry = poi_report.geometry;
    poi_report_out.name = poi_report.name;
    poi_report_out.app6_symbol = poi_report.app6_symbol;

    std::lock_guard<std::mutex> lock(_subscription_mutex);
    if (_poi_report_subscription) {
        auto callback = _poi_report_subscription;
        _parent->call_user_callback([callback, poi_report_out]() { callback(poi_report_out); });
    }
}

} // namespace mavsdk