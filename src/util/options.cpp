#include "util/option.hpp"

#include "util/type.hpp"

namespace sc2::util {

ProgramOptionsManager::ProgramOptionsManager(const int argc, const char** const argv) :
    cli(argc, argv), app("Starcraft 2 C++ API") {
    app.get_formatter()->column_width(cli_help_column_width);
}

auto ProgramOptionsManager::parseOptions() -> void {
    std::string temp_server_address = ProgramOptions::default_server_address;

    app.add_option(options::map::option, options.sc2_map, options::map::description)->required();
    app.add_option(options::executable::option, options.sc2_path, options::executable::description)->required();
    app.add_option(options::server::option, temp_server_address, options::server::description);
    app.add_option(options::port::option, options.sc2_server_port, options::port::description);

    try {
        app.parse(cli.argc, cli.argv);
        // boost::asio::ip::make_address can throw
        options.sc2_server_address = boost::asio::ip::make_address(temp_server_address);
    } catch (const CLI::CallForHelp& help) {
        app.exit(help);
        throw std::runtime_error("User call for help at command line, terminate program.");
    } catch (const CLI::RequiredError& error) {
        app.exit(error);
        throw std::runtime_error("Missing required option");
    } catch (const CLI::ParseError& error) {
        // Notify the user of unexpected options, but do not exit the program
        app.exit(error);
    } catch (const boost::exception& error) {
        throw std::runtime_error("Invalid server address");
    }
}

} // namespace sc2::util