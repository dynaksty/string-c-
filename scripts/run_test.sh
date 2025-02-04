#!/bin/bash

cd ../..
docker rmi -f school21/testlinux:1.0 && docker build . -t school21/testlinux:1.0 -f src/scripts/Dockerfile && docker run --rm school21/testlinux:1.0 