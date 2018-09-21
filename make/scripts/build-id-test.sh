#!/bin/sh

##
## Tests linker suport for build-id

CXX=$1

TMPDIR="$(mktemp -d)"
CFILE="$TMPDIR/test.c"
BINFILE="$TMPDIR/test"

echo '
int main() {
    return 0;
}' >> "$CFILE"

if $CXX -Wl,--build-id=sha1 "$CFILE" -o "$BINFILE" 2> /dev/null; then
    echo "yes"
else
    echo "no"
fi

if [ -d "$TMPDIR" ]; then
    rm -fr $TMPDIR 2>&1 > /dev/null
fi
