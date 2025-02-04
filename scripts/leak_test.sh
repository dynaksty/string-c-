#!/bin/bash

TEST_NAME="unittest"

if [[ "$(uname)" == "Linux" ]]; then
    valgrind --tool=memcheck --leak-check=yes ./$TEST_NAME
elif [[ "$(uname)" == "Darwin" ]]; then
    leaks -atExit -- ./$TEST_NAME
fi