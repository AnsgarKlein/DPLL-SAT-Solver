#!/bin/bash

set -e

if [[ $# -ne 4 ]]; then
    echo "Error: Expected exactly four arguments but got $#" > /dev/stderr
    exit 1
fi

TARGET="$1"
RESOURCE="$2"
CONVERTER="$3"
CHECKER="$4"

if [[ ! -f "$TARGET" ]] || [[ ! -x "$TARGET" ]]; then
    echo "Error: Could not find target at '$TARGET'" > /dev/stderr
    exit 1
fi

if [[ ! -f "$RESOURCE" ]]; then
    echo "Error: Could not find resource at '$RESOURCE'" > /dev/stderr
    exit 1
fi

if [[ ! -f "$CONVERTER" ]] || [[ ! -x "$CONVERTER" ]]; then
    echo "Error: Could not find converter at '$CONVERTER'" > /dev/stderr
    exit 1
fi

if [[ ! -f "$CHECKER" ]] || [[ ! -x "$CHECKER" ]]; then
    echo "Error: Could not find checker at '$CHECKER'" > /dev/stderr
    exit 1
fi

echo "Solving \"$RESOURCE\" with \"$TARGET\"..."
echo "Output wil be converted to solution using \"$CONVERTER\"."
echo "Solution will be checked for correctness with \"$CHECKER\"."

"$TARGET" --dimacs < "$RESOURCE" | "$CONVERTER" | "$CHECKER" --problem "$RESOURCE" -s '-'
exit $?
