#!/bin/bash

error_exit() {
    echo "$1" >&2
    exit 1
}

install_on_linux() {
    if ! pkg-config --libs check &>/dev/null; then
        sudo apt-get update && sudo apt-get install -y check || error_exit "check.h install failure"
    fi
}

install_on_darwin() {
    if ! pkg-config --libs check &>/dev/null; then
        brew install check || error_exit "check.h install failure"
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
        error_exit "This script support only Linux or darwin"
esac

echo " libcheck.h install successfully!"