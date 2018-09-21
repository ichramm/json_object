#!/bin/bash

#########################################
# File  : symbol-package.sh
# Author: ichramm
# Date  : Tue 6 Sep 2016 19:27:54 PM UYT
#########################################


#
#Search paths for ls (https://sourceware.org/gdb/current/onlinedocs/gdb/Separate-Debug-Files.html)
#(note: binary=ls build-id=abcdef1234)
#/usr/lib/debug/.build-id/ab/cdef1234.debug
#/usr/bin/ls.debug
#/usr/bin/.debug/ls.debug
#/usr/lib/debug/usr/bin/ls.debug.

BINFILE=$1
PKGDBGDIR=${PKGDBGDIR:-$2}

function extract_build_id() {
    eu-readelf -n $1 2>/dev/null | grep 'Build ID' | sed 's/ *Build ID: \(.*\)/\1/' || true
}

function process_elf() {
    local bin_file=$1
    local target_dir="$2/usr/lib/debug/.build-id"

    local build_id=$(extract_build_id $bin_file)
    if [ -n "$build_id" ]; then # some files may not be ELFs
        local dbgdir="${build_id:0:2}"
        local dbgfile="${build_id:2}.debug"

        rm -f "$dbgfile"

        # Create dbg file in the current directory and then move it to the final directory
        objcopy --only-keep-debug "$bin_file" "$dbgfile"
        objcopy --strip-debug "$bin_file"
        objcopy --add-gnu-debuglink="$dbgfile" "$bin_file"

        mkdir -p "$target_dir/$dbgdir"
        mv "$dbgfile" "$target_dir/$dbgdir"
    fi
}

# does not fail if the file does not exist
if [ -f "$BINFILE" ]; then
    process_elf $BINFILE $PKGDBGDIR
fi
