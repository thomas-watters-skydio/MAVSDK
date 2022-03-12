#include "command_impl.h"

namespace mavsdk {

static Command::Result command_result_to_command_result(MavlinkCommandSender::Result result)
{
    switch (result) {
        case MavlinkCommandSender::Result::Success:
            return Command::Result::Success;
        case MavlinkCommandSender::Result::NoSystem:
            return Command::Result::NoSystem;
        case MavlinkCommandSender::Result::ConnectionError:
            return Command::Result::ConnectionError;
        case MavlinkCommandSender::Result::Busy:
            return Command::Result::Busy;
        case MavlinkCommandSender::Result::CommandDenied:
            return Command::Result::CommandDenied;
        case MavlinkCommandSender::Result::Timeout:
            return Command::Result::Timeout;
        case MavlinkCommandSender::Result::Unsupported:
            return Command::Result::Unsupported;
        default:
            return Command::Result::Unknown;
    }
}

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

void CommandImpl::send_command_long_async(
    Command::CommandLong command, const Command::ResultCallback callback)
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

    _parent->send_command_async(cmd, [this, callback](MavlinkCommandSender::Result result, float) {
        command_result_callback(result, callback);
    });
}

Command::Result CommandImpl::send_command_long(Command::CommandLong command)
{
    auto prom = std::promise<Command::Result>();
    auto fut = prom.get_future();

    send_command_long_async(command, [&prom](Command::Result result) { prom.set_value(result); });

    return fut.get();
}

void CommandImpl::command_result_callback(
    MavlinkCommandSender::Result command_result, const Command::ResultCallback& callback) const
{
    Command::Result cmd_result = command_result_to_command_result(command_result);

    if (callback) {
        auto temp_callback = callback;
        _parent->call_user_callback([temp_callback, cmd_result]() { temp_callback(cmd_result); });
    }
}

} // namespace mavsdk