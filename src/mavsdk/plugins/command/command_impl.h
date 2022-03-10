#pragma once

#include "plugins/command/command.h"
#include "plugin_impl_base.h"

namespace mavsdk {

class CommandImpl : public PluginImplBase {
public:
    explicit CommandImpl(System& system);
    explicit CommandImpl(std::shared_ptr<System> system);
    ~CommandImpl() override;

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;



    void send_command_long_async(Command::CommandLong command, const Command::ResultCallback callback);



    Command::Result send_command_long(Command::CommandLong command);


    Command::Result command_result_to_mission_result(MavlinkCommandSender::Result result);

private:
};

} // namespace mavsdk