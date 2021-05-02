#!/bin/bash

set -e

if [[ $# -ne 2 ]]; then
    echo "Error: Expected exactly two arguments but got $#" > /dev/stderr
    exit 1
fi

TARGET="$1"
RESOURCE="$2"

if ! command -v 'valgrind'; then
    echo "Error: Could not find valgrind" > /dev/stderr
    exit 1
fi

if [[ ! -f "$TARGET" ]] || [[ ! -x "$TARGET" ]]; then
    echo "Error: Could not find target at '$TARGET'" > /dev/stderr
    exit 1
fi

if [[ ! -f "$RESOURCE" ]]; then
    echo "Error: Could not find resource at '$RESOURCE'" > /dev/stderr
    exit 1
fi

echo "Solving \"$RESOURCE\" with \"$TARGET\"..."

valgrind --leak-check=full --show-leak-kinds=all \
"$TARGET" --dimacs < "$RESOURCE" \
2>&1 1> /dev/null | grep 'no leaks are possible'

exit $?
