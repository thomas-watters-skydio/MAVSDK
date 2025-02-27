#include <iostream>
#include <cmath>
#include "integration_test_helper.h"
#include "mavsdk.h"
#include "plugins/action/action.h"
#include "plugins/telemetry/telemetry.h"

using namespace mavsdk;

TEST_F(SitlTest, PX4ActionGoto)
{
    Mavsdk mavsdk;

    ConnectionResult ret = mavsdk.add_udp_connection();
    ASSERT_EQ(ret, ConnectionResult::Success);

    // Wait for system to connect via heartbeat.
    ASSERT_TRUE(poll_condition_with_timeout(
        [&mavsdk]() {
            const auto systems = mavsdk.systems();

            if (systems.size() == 0) {
                return false;
            }

            const auto system = mavsdk.systems().at(0);
            return system->is_connected();
        },
        std::chrono::seconds(10)));

    auto system = mavsdk.systems().at(0);
    ASSERT_TRUE(system->has_autopilot());
    auto telemetry = std::make_shared<Telemetry>(system);

    int iteration = 0;
    while (!telemetry->health_all_ok()) {
        LogInfo() << "waiting for system to be ready";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ASSERT_LT(++iteration, 10);
    }

    auto action = std::make_shared<Action>(system);
    Action::Result action_ret = action->arm();
    EXPECT_EQ(action_ret, Action::Result::Success);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    action_ret = action->takeoff();
    EXPECT_EQ(action_ret, Action::Result::Success);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Go somewhere
    action->goto_location(47.398000, 8.545592, NAN, NAN);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // And back
    action->goto_location(47.3977233, 8.545592, NAN, NAN);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    action_ret = action->land();
    EXPECT_EQ(action_ret, Action::Result::Success);

    iteration = 0;
    while (telemetry->in_air()) {
        LogInfo() << "waiting for system to be landed";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // TODO: currently we need to wait a long time until landed is detected.
        ASSERT_LT(++iteration, 20);
    }

    action_ret = action->disarm();
    EXPECT_EQ(action_ret, Action::Result::Success);
}
