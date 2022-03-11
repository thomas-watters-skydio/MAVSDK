// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/command/command.proto)

#include <iomanip>

#include "command_impl.h"
#include "plugins/command/command.h"

namespace mavsdk {

using CommandLong = Command::CommandLong;

Command::Command(System& system) : PluginBase(), _impl{std::make_unique<CommandImpl>(system)} {}

Command::Command(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{std::make_unique<CommandImpl>(system)}
{}

Command::~Command() {}

void Command::send_command_long_async(CommandLong command, const ResultCallback callback)
{
    _impl->send_command_long_async(command, callback);
}

Command::Result Command::send_command_long(CommandLong command) const
{
    return _impl->send_command_long(command);
}

bool operator==(const Command::CommandLong& lhs, const Command::CommandLong& rhs)
{
    return (rhs.target_system == lhs.target_system) &&
           (rhs.target_component == lhs.target_component) && (rhs.command == lhs.command) &&
           (rhs.confirmation == lhs.confirmation) &&
           ((std::isnan(rhs.param1) && std::isnan(lhs.param1)) || rhs.param1 == lhs.param1) &&
           ((std::isnan(rhs.param2) && std::isnan(lhs.param2)) || rhs.param2 == lhs.param2) &&
           ((std::isnan(rhs.param3) && std::isnan(lhs.param3)) || rhs.param3 == lhs.param3) &&
           ((std::isnan(rhs.param4) && std::isnan(lhs.param4)) || rhs.param4 == lhs.param4) &&
           ((std::isnan(rhs.param5) && std::isnan(lhs.param5)) || rhs.param5 == lhs.param5) &&
           ((std::isnan(rhs.param6) && std::isnan(lhs.param6)) || rhs.param6 == lhs.param6) &&
           ((std::isnan(rhs.param7) && std::isnan(lhs.param7)) || rhs.param7 == lhs.param7);
}

std::ostream& operator<<(std::ostream& str, Command::CommandLong const& command_long)
{
    str << std::setprecision(15);
    str << "command_long:" << '\n' << "{\n";
    str << "    target_system: " << command_long.target_system << '\n';
    str << "    target_component: " << command_long.target_component << '\n';
    str << "    command: " << command_long.command << '\n';
    str << "    confirmation: " << command_long.confirmation << '\n';
    str << "    param1: " << command_long.param1 << '\n';
    str << "    param2: " << command_long.param2 << '\n';
    str << "    param3: " << command_long.param3 << '\n';
    str << "    param4: " << command_long.param4 << '\n';
    str << "    param5: " << command_long.param5 << '\n';
    str << "    param6: " << command_long.param6 << '\n';
    str << "    param7: " << command_long.param7 << '\n';
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, Command::Result const& result)
{
    switch (result) {
        case Command::Result::Accepted:
            return str << "Accepted";
        case Command::Result::TemporarilyRejected:
            return str << "Temporarily Rejected";
        case Command::Result::Denied:
            return str << "Denied";
        case Command::Result::Unsupported:
            return str << "Unsupported";
        case Command::Result::Failed:
            return str << "Failed";
        case Command::Result::InProgress:
            return str << "In Progress";
        case Command::Result::Cancelled:
            return str << "Cancelled";
        case Command::Result::NoSystem:
            return str << "No System";
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk