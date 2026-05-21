#!/bin/bash
# Brute force numeric passwords against a crackme binary

BINARY="$1"
START="${2:-0}"
END="${3:-99999}"

for (( i=START; i<=END; i++ )); do
    if echo "$i" | "$BINARY" 2>&1 | grep -qi "OK"; then
        echo "Password: $i"
        exit 0
    fi
done

echo "Not found"
