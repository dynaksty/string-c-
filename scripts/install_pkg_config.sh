#!/bin/bash

error_exit() {
    echo "$1" >&2
    exit 1
}

install_on_linux() {
    if ! command -v pkg-config &>/dev/null; then
        sudo apt-get update && sudo apt-get install -y pkg-config || error_exit "pkg-config. install failure"
    fi
}

install_on_darwin() {
    if ! command -v brew &>/dev/null; then
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)" || error_exit "Homebrew install faliure"
    fi
    if ! command -v pkg-config &>/dev/null; then
        brew install pkg-config || error_exit "pkg-config. install failure"
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

echo "pkg-config. install successfully!"