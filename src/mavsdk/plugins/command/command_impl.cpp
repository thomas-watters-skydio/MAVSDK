#include "command_impl.h"

namespace mavsdk {

CommandImpl::CommandImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

CommandImpl::CommandImpl(std::shared_ptr<System> system) : PluginImplBase(std::move(system))
{
    _parent->register_plugin(this);
}

CommandImpl::~CommandImpl()
{
    _parent->unregister_plugin(this);
}

void CommandImpl::init() {}

void CommandImpl::deinit() {}

void CommandImpl::enable() {}

void CommandImpl::disable() {}



void CommandImpl::send_command_long_async(Command::CommandLong command, const Command::ResultCallback callback)
{
    MavlinkCommandSender::CommandLong cmd{};

    cmd.target_system_id = command.target_system;
    cmd.target_component_id = command.target_component;
    cmd.command = command.command;
    cmd.confirmation = command.confirmation;
    cmd.params.maybe_param1 = command.param1;
    cmd.params.maybe_param2 = command.param2;
    cmd.params.maybe_param3 = command.param3;
    cmd.params.maybe_param4 = command.param4;
    cmd.params.maybe_param5 = command.param5;
    cmd.params.maybe_param6 = command.param6;
    cmd.params.maybe_param7 = command.param7;

    _parent->send_command_async(
        cmd, [this, callback](MavlinkCommandSender::Result result, float) {
            callback(command_result_to_mission_result(result));
        });
}



Command::Result CommandImpl::send_command_long(Command::CommandLong command)
{
    MavlinkCommandSender::CommandLong cmd{};

    cmd.target_system_id = command.target_system;
    cmd.target_component_id = command.target_component;
    cmd.command = command.command;
    cmd.confirmation = command.confirmation;
    cmd.params.maybe_param1 = command.param1;
    cmd.params.maybe_param2 = command.param2;
    cmd.params.maybe_param3 = command.param3;
    cmd.params.maybe_param4 = command.param4;
    cmd.params.maybe_param5 = command.param5;
    cmd.params.maybe_param6 = command.param6;
    cmd.params.maybe_param7 = command.param7;

    auto prom = std::promise<Command::Result>();
    auto fut = prom.get_future();

    _parent->send_command_async(
        cmd, [&prom, this](MavlinkCommandSender::Result result, float) {
            prom.set_value(command_result_to_mission_result(result));
        });
    return fut.get();
}

Command::Result
CommandImpl::command_result_to_mission_result(MavlinkCommandSender::Result result)
{
    Command::Result ret;
    switch (result) {
        case MavlinkCommandSender::Result::Success:
            ret = Command::Result::Accepted;
            break;
        case MavlinkCommandSender::Result::InProgress:
            ret = Command::Result::InProgress;
            break;
        default:
            ret = Command::Result::Unsupported;
            break;
    }

    return ret;
}




} // namespace mavsdk