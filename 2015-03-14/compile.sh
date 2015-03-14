#!/usr/bin/env bash

set -eu
clang++ -std=c++11 $1 && ./a.out
