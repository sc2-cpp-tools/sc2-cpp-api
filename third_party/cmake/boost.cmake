# Boost::process:v2 is an experimental release - but header only
set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_RELEASE_LIBS ON)

find_package(
    Boost
    1.80.0
    REQUIRED)

find_package(
    Threads
    REQUIRED)
