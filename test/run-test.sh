#!/usr/bin/env bash

set +e

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
TMPF=$(mktemp)
trap 'rm -rf ${TMPF}' 0 1 2 15

cp "${SCRIPT_DIR}/dos_file.txt" "${TMPF}"

echo Before
xxd "${TMPF}"
echo
"${SCRIPT_DIR}/../dos2unix" "${TMPF}"
echo After
xxd "${TMPF}"


