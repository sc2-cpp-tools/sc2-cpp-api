#!/bin/bash
#############################################################################
# Bootstrap Ubuntu distribution for sc2-cpp-api development
#############################################################################
# Tested for: focal
SUITE=$(lsb_release -cs)

function essential_packages {
    sudo apt-get update -qq && \
        sudo apt-get install -y --no-install-recommends \
            software-properties-common wget apt-utils file zip gpg-agent \
            make git lsb-release \
            python3 python3-pip
}

essential_packages

#############################################################################
# GCC
# Supports up to gcc-11
#############################################################################

function install_gcc {
    GCC_VERSION="11"
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
        sudo apt-get update -qq && \
        sudo apt-get install -y --no-install-recommends \
        gcc-${GCC_VERSION} g++-${GCC_VERSION} gdb

    # Set gcc-13 as the default gcc
    sudo update-alternatives --install /usr/bin/gcc gcc \
        "$(which gcc-${GCC_VERSION})" 100
    sudo update-alternatives --install /usr/bin/g++ g++ \
        "$(which g++-${GCC_VERSION})" 100
}

install_gcc

#############################################################################
# LLVM
# Supports up to clang-15 (clang-14 cutoff for IWYU)
#############################################################################

function install_llvm {
    LLVM_VERSION="14"
    LLVM_URL="http://apt.llvm.org/${SUITE}/"
    LLVM_PKG="llvm-toolchain-${SUITE}-${LLVM_VERSION}"

    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | \
        sudo apt-key add && \
        sudo add-apt-repository -y "deb ${LLVM_URL} ${LLVM_PKG} main" && \
        sudo apt-get update -qq && \
        sudo apt-get install -y --no-install-recommends \
            clang-${LLVM_VERSION} \
            lldb-${LLVM_VERSION} \
            lld-${LLVM_VERSION} \
            clangd-${LLVM_VERSION} \
            llvm-${LLVM_VERSION}-dev \
            libclang-${LLVM_VERSION}-dev \
            clang-tidy-${LLVM_VERSION} \
            clang-format-${LLVM_VERSION}

    # Set required tools as the default
    # Note this does not --slave the entire toolchain, so swapping between
    # different llvm installs is not supported from this script
    sudo update-alternatives \
        --install /usr/bin/clang clang \
            "$(which clang-${LLVM_VERSION})" 100 \
        --slave /usr/bin/clang++ clang++ \
            "$(which clang++-${LLVM_VERSION})" \
        --slave /usr/bin/clang-tidy clang-tidy \
            "$(which clang-tidy-${LLVM_VERSION})" \
        --slave /usr/bin/clang-format clang-format \
            "$(which clang-format-${LLVM_VERSION})"
}

install_llvm

#############################################################################
# CMake
#############################################################################

function install_latest_cmake {
    CMAKE_URL="https://apt.kitware.com/ubuntu/"

    wget -O https://apt.kitware.com/keys/kitware-archive-latest.asc \
        2>/dev/null | \
        gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg \
        >/dev/null && \
        sudo apt-add-repository -y "deb ${CMAKE_URL} ${SUITE} main"

    sudo apt-key adv --keyserver keyserver.ubuntu.com \
        --recv-keys 6AF7F09730B3F0A4
    sudo apt install kitware-archive-keyring
    sudo rm /etc/apt/trusted.gpg.d/kitware.gpg

    sudo apt-get update -qq && \
        sudo apt-get install -y --no-install-recommends cmake cmake-curses-gui
}

install_latest_cmake

# cmake-format and cmake-lint
pip install cmakelang

# cmakelang tooling is installed in ~/.local/bin
sudo ln -s "${HOME}/.local/bin/cmake-format" /usr/bin/cmake-format
sudo ln -s "${HOME}/.local/bin/cmake-lint" /usr/bin/cmake-lint

#############################################################################
# Optional dependencies
#############################################################################

function install_optional {
    sudo apt-get update -qq && \
        sudo apt-get install -y --no-install-recommends \
            doxygen graphviz ccache cppcheck
}

install_optional

#############################################################################
# IWYU
# Supports up to clang-14
#############################################################################

function install_include_what_you_use {
    IWYU="/home/iwyu"
    IWYU_BUILD="${IWYU}/build"
    IWYU_SRC="${IWYU}/include-what-you-use"
    sudo mkdir -p "${IWYU_BUILD}" && \
        sudo git clone --branch clang_${LLVM_VERSION} \
            https://github.com/include-what-you-use/include-what-you-use.git \
            "${IWYU_SRC}"

    sudo CC=clang-${LLVM_VERSION} CXX=clang++-${LLVM_VERSION} \
        cmake -S "${IWYU_SRC}" \
        -B "${IWYU_BUILD}" \
        -G "Unix Makefiles" \
        -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${LLVM_VERSION} && \
        sudo cmake --build "${IWYU_BUILD}" -j && \
        sudo cmake --install "${IWYU_BUILD}"

    sudo mkdir -p "$(include-what-you-use -print-resource-dir 2>/dev/null)"
    sudo ln -s "$(readlink -f /usr/lib/clang/${LLVM_VERSION}/include)" \
        "$(include-what-you-use -print-resource-dir 2>/dev/null)/include"
}

install_include_what_you_use

#############################################################################
# Cleanup
#############################################################################

sudo apt-get autoremove -y && sudo apt-get clean && \
    sudo rm -rf /var/lib/apt/lists/*
