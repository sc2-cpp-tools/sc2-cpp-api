#include "util/option.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

class BaseProgramOptionsTestFixture {
public:
protected:
    const std::string test_executable = "/test/executable";
    const std::string test_map = "/path/to/sc2/map.SC2Map";
    const std::string test_sc2 = "/path/to/sc2/executable";
    const std::string non_default_server_address = "127.0.0.2";
    const std::string non_default_server_port = "8877";

private:
};

TEST_CASE_METHOD(BaseProgramOptionsTestFixture, "Validate base fixture assumptions", "[options]") {
    REQUIRE(non_default_server_address != sc2::util::ProgramOptions::default_server_address);

    auto port = static_cast<boost::asio::ip::port_type>(stoi(non_default_server_port));
    REQUIRE(port != sc2::util::ProgramOptions::default_server_port);
}

class RequiredOptionsImplicitDefaults : public BaseProgramOptionsTestFixture {
public:
protected:
    static constexpr int argc = 5;
    // NOLINTBEGIN: simulate command line arguments
    // --map --executable
    const char* argv[argc] = {
        test_executable.c_str(),
        sc2::util::options::map::full_flag.c_str(),
        this->test_map.c_str(),
        sc2::util::options::executable::full_flag.c_str(),
        this->test_sc2.c_str()};
    // NOLINTEND simulate command line arguments

private:
};

TEST_CASE_METHOD(
    RequiredOptionsImplicitDefaults,
    "Test required options with implicit defaults, fully qualified flags",
    "[options]") {
    sc2::util::ProgramOptionsManager manager(argc, static_cast<const char**>(argv));

    REQUIRE_NOTHROW(manager.parseOptions());

    const sc2::util::ProgramOptions options = manager.getOptions();

    REQUIRE(
        options.getServerAddress() == boost::asio::ip::make_address(sc2::util::ProgramOptions::default_server_address));
    REQUIRE(options.getServerPort() == sc2::util::ProgramOptions::default_server_port);
}

class RequiredOptionsExplicitDefaults : public BaseProgramOptionsTestFixture {
public:
protected:
    static constexpr int argc = 9;
    // NOLINTBEGIN: simulate command line arguments
    // --map --executable --server --port
    const char* argv[argc] = {
        test_executable.c_str(),
        sc2::util::options::map::full_flag.c_str(),
        this->test_map.c_str(),
        sc2::util::options::executable::full_flag.c_str(),
        this->test_sc2.c_str(),
        sc2::util::options::server::full_flag.c_str(),
        this->non_default_server_address.c_str(),
        sc2::util::options::port::full_flag.c_str(),
        this->non_default_server_port.c_str()};
    // NOLINTEND simulate command line arguments

private:
};

TEST_CASE_METHOD(
    RequiredOptionsExplicitDefaults,
    "Test required options with explicit defaults, fully qualified flags",
    "[options]") {
    sc2::util::ProgramOptionsManager manager(argc, static_cast<const char**>(argv));

    REQUIRE_NOTHROW(manager.parseOptions());

    const sc2::util::ProgramOptions options = manager.getOptions();

    REQUIRE(options.getServerAddress() == boost::asio::ip::make_address(this->non_default_server_address));
    REQUIRE(options.getServerPort() == static_cast<boost::asio::ip::port_type>(stoi(this->non_default_server_port)));
    REQUIRE(options.getSc2Map() == static_cast<std::filesystem::path>(this->test_map));
    REQUIRE(options.getSc2Path() == static_cast<std::filesystem::path>(this->test_sc2));
}

class MissingRequiredOptions : public BaseProgramOptionsTestFixture {
public:
protected:
    static constexpr int argc = 1;
    // NOLINTBEGIN: simulate command line arguments
    const char* argv[argc] = {test_executable.c_str()};
    // NOLINTEND simulate command line arguments

private:
};

TEST_CASE_METHOD(MissingRequiredOptions, "No required cli options, should throw exception and exit", "[option]") {
    sc2::util::ProgramOptionsManager manager(argc, static_cast<const char**>(argv));
    REQUIRE_THROWS(manager.parseOptions());
}

class MixedFlags : public BaseProgramOptionsTestFixture {
public:
protected:
    static constexpr int argc = 9;
    // NOLINTBEGIN: simulate command line arguments
    // -m --executable -s --port
    const char* argv[argc] = {
        test_executable.c_str(),
        sc2::util::options::map::short_flag.c_str(),
        this->test_map.c_str(),
        sc2::util::options::executable::full_flag.c_str(),
        this->test_sc2.c_str(),
        sc2::util::options::server::short_flag.c_str(),
        this->non_default_server_address.c_str(),
        sc2::util::options::port::full_flag.c_str(),
        this->non_default_server_port.c_str()};
    // NOLINTEND simulate command line arguments

private:
};

TEST_CASE_METHOD(MixedFlags, "Test all options with a mixture of full and short flags", "[options]") {
    sc2::util::ProgramOptionsManager manager(argc, static_cast<const char**>(argv));

    REQUIRE_NOTHROW(manager.parseOptions());

    const sc2::util::ProgramOptions options = manager.getOptions();

    REQUIRE(options.getServerAddress() == boost::asio::ip::make_address(this->non_default_server_address));
    REQUIRE(options.getServerPort() == static_cast<boost::asio::ip::port_type>(stoi(this->non_default_server_port)));
    REQUIRE(options.getSc2Map() == static_cast<std::filesystem::path>(this->test_map));
    REQUIRE(options.getSc2Path() == static_cast<std::filesystem::path>(this->test_sc2));
}

class InvalidServerAddress : public BaseProgramOptionsTestFixture {
public:
protected:
    const std::string bad_server_address;
    static constexpr int argc = 5;
    // NOLINTBEGIN: simulate command line arguments
    // --map --executable
    const char* argv[argc] = {
        test_executable.c_str(),
        sc2::util::options::map::full_flag.c_str(),
        this->test_map.c_str(),
        sc2::util::options::executable::full_flag.c_str(),
        bad_server_address.c_str()};
    // NOLINTEND simulate command line arguments

private:
};

TEST_CASE_METHOD(
    InvalidServerAddress, "Malformed server addresses should throw on boost::asio::ip::address", "[options]") {
    sc2::util::ProgramOptionsManager manager(argc, static_cast<const char**>(argv));

    REQUIRE_THROWS(manager.parseOptions());
}