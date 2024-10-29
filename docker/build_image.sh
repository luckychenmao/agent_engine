#!/bin/bash

rootdir=$(dirname "$(readlink -f "\$0")")
cd "$rootdir"

docker build -t agent_dev -f ./Dockerfile  .
