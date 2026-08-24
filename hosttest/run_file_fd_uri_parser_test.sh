#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${TMPDIR:-/tmp}/file_fd_uri_parser_test"
g++ -std=c++17 -O0 -Wall -Wextra \
  -I"${ROOT}/engine/plugin/plugins/source/file_source" \
  "${ROOT}/hosttest/file_fd_uri_parser_test.cpp" \
  "${ROOT}/engine/plugin/plugins/source/file_source/file_fd_uri_parser.cpp" \
  -o "${OUT}"
"${OUT}"
