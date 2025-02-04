FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    git \
    git-lfs \
    curl \
    jq \
    bash \
    build-essential \
    python3 \
    valgrind \
    cppcheck \
    libpcre3-dev \
    clang-tidy \
    check \
    pkg-config


COPY . /project
COPY materials/build/scripts /project/ci-scripts
COPY materials/build/scripts /project/tests
COPY materials/linters /project/tests/linters

WORKDIR "/project"

ENTRYPOINT ["/bin/bash", "src/scripts/test_in_linux.sh"]
# ENTRYPOINT ["/bin/bash"]