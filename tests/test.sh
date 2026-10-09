#!/bin/sh
# Integration smoke tests; execute with: make test
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
TMP=$(mktemp -d "${TMPDIR:-/tmp}/myls-test.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
mkdir -p "$TMP/sub"
printf a > "$TMP/alpha"
printf bbb > "$TMP/bravo"
: > "$TMP/.hidden"
: > "$TMP/sub/child"
ln -s alpha "$TMP/link"
chmod +x "$TMP/bravo"
cd "$ROOT"
./myls "$TMP" > "$TMP/out"
grep -qx alpha "$TMP/out"
grep -qx bravo "$TMP/out"
! grep -q hidden "$TMP/out"
./myls -a "$TMP" > "$TMP/out"
grep -qx . "$TMP/out"
grep -qx .. "$TMP/out"
grep -qx .hidden "$TMP/out"
./myls -A "$TMP" > "$TMP/out"
grep -qx .hidden "$TMP/out"
! grep -qx . "$TMP/out"
./myls -d "$TMP" > "$TMP/out"
grep -qx "$TMP" "$TMP/out"
./myls -F "$TMP" > "$TMP/out"
grep -qx 'sub/' "$TMP/out"
grep -qx 'link@' "$TMP/out"
grep -qx 'bravo\*' "$TMP/out"
./myls -S "$TMP/alpha" "$TMP/bravo" > "$TMP/out"
# Largest regular file in our fixture appears first.
[ "$(head -1 "$TMP/out")" = "$TMP/bravo" ] || { echo 'size-sort test failed'; exit 1; }
./myls -R "$TMP" > "$TMP/out"
grep -qx 'child' "$TMP/out"
./myls -l "$TMP" > "$TMP/out"
grep -q -- 'link -> alpha' "$TMP/out"
./myls -n "$TMP" > "$TMP/out"
grep -q 'alpha' "$TMP/out"
./myls -i "$TMP" > "$TMP/out"
grep -q alpha "$TMP/out"
./myls -s "$TMP" > "$TMP/out"
grep -q alpha "$TMP/out"
./myls -sh "$TMP" > "$TMP/out"
grep -q bravo "$TMP/out"
./myls -sk "$TMP" > "$TMP/out"
grep -q bravo "$TMP/out"
./myls -tr "$TMP" > "$TMP/out"
grep -q alpha "$TMP/out"
./myls -ctu "$TMP" > "$TMP/out"
grep -q bravo "$TMP/out"
./myls -fq "$TMP" > "$TMP/out"
grep -q alpha "$TMP/out"
./myls -w "$TMP" > "$TMP/out"
grep -q bravo "$TMP/out"
./myls -z "$TMP" >/dev/null 2>"$TMP/err" && { echo 'invalid option accepted'; exit 1; }
./myls "$TMP/nonexistent" >/dev/null 2>"$TMP/err" && { echo 'nonexistent file accepted'; exit 1; }
echo 'PASS: myls smoke tests'
