#pragma once

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "boost/asio.hpp"

namespace sc2::util {

namespace options::map {
inline const std::string short_flag = "-m";
inline const std::string full_flag = "--map";
inline const std::string option = short_flag + "," + full_flag;
inline const std::string description = "Path to the SC2Map to launch";
}; // namespace options::map

namespace options::executable {
inline const std::string short_flag = "-e";
inline const std::string full_flag = "--executable";
inline const std::string option = short_flag + "," + full_flag;
inline const std::string description = "Path to the SC2 executable to launch";
}; // namespace options::executable

namespace options::server {
inline const std::string short_flag = "-s";
inline const std::string full_flag = "--server";
inline const std::string option = short_flag + "," + full_flag;
inline const std::string description = "Server address for websocket connection";
}; // namespace options::server

namespace options::port {
inline const std::string short_flag = "-p";
inline const std::string full_flag = "--port";
inline const std::string option = short_flag + "," + full_flag;
inline const std::string description = "Server port for websocket to listen on";
}; // namespace options::port

/// @brief Stores command line arguments from main() for parsing by CLI::App
class CliArguments {
    friend class ProgramOptionsManager;

public:
    // CliArguments should be initialized with command line arguments from main()
    CliArguments() = delete;

    /// @brief Construct CliArguments from cli arguments in main()
    /// @param argc argc from main()
    /// @param argv argv from main()
    CliArguments(const int argc, const char** const argv) : argc(argc), argv(argv) {
    }

    // CliArguments should not be copied
    CliArguments(const CliArguments& rhs) = delete;
    auto operator=(const CliArguments& rhs) -> CliArguments& = delete;

private:
    const int argc;
    const char** const argv;
};

/// @brief Data class for storing parsed and defaulted command line arguments
class ProgramOptions {
    friend class ProgramOptionsManager;

public:
    // Expose defaults
    inline static const std::string default_server_address = "127.0.0.1";
    inline static constexpr boost::asio::ip::port_type default_server_port = 8500;

    ProgramOptions() = default;

    ~ProgramOptions() = default;

    ProgramOptions(const ProgramOptions& rhs) = default;
    auto operator=(const ProgramOptions& rhs) -> ProgramOptions& = default;

    [[nodiscard]] auto getServerAddress() const noexcept -> const boost::asio::ip::address& {
        return sc2_server_address;
    }

    [[nodiscard]] auto getServerPort() const noexcept -> const boost::asio::ip::port_type& {
        return sc2_server_port;
    }

    [[nodiscard]] auto getSc2Map() const noexcept -> const std::filesystem::path& {
        return sc2_map;
    }

    [[nodiscard]] auto getSc2Path() const noexcept -> const std::filesystem::path& {
        return sc2_path;
    }

private:
    boost::asio::ip::address sc2_server_address = boost::asio::ip::make_address(default_server_address);
    boost::asio::ip::port_type sc2_server_port = default_server_port;
    std::filesystem::path sc2_map;
    std::filesystem::path sc2_path;
};

class ProgramOptionsManager {
public:
    // ProgramOptionsManager should be initialized with command line arguments
    ProgramOptionsManager() = delete;

    ~ProgramOptionsManager() = default;

    ProgramOptionsManager(const int argc, const char** const argv);

    // ProgramOptionsManager should not be copied
    ProgramOptionsManager(const ProgramOptionsManager& rhs) = delete;
    auto operator=(const ProgramOptionsManager& rhs) -> ProgramOptionsManager& = delete;

    auto parseOptions() -> void;

    auto getOptions() const -> const ProgramOptions& {
        return options;
    }

private:
    // Formatting CLI::CallForHelp output
    static constexpr int cli_help_column_width = 40;

    const CliArguments cli;
    CLI::App app;
    ProgramOptions options;
};

} // namespace sc2::util