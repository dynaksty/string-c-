#!/bin/bash

error_exit() {
    echo "$1" >&2
    exit 1
}

install_on_linux() {
    if ! command -v lcov &>/dev/null; then
        sudo apt-get update && \
        sudo apt-get install -y lcov || \
        error_exit "Installation of lcov failed"
    fi
}

install_on_darwin() {
    if ! command -v lcov &>/dev/null; then
        #brew install gcc lcov || \
        brew install lcov || \
        error_exit "Installation of gcov, lcov failed"
    fi
}

platform=$(uname)

case $platform in
    Linux)
        install_on_linux
        ;;
    Darwin)
        install_on_darwin
        ;;
    *)
        error_exit "This script supports only Linux or Darwin"
esac

echo "gcov, lcov, and genhtml installed successfully!"