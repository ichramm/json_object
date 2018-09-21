#!/bin/bash

ROOT=.

#
# Version the changes apply to
version=""

#
# Commit from which to start considering changes (not included)
first_commit=""

#
# Whether to be quite or to be verbose
quiet="no"

#
# Last commit on which to include changes
last_commit=""

#
# Path to the Changelog file
changelog_file=""

#
# List of changes to look for
change_types="Internal Added Changed Deprecated Fixed Removed"

#
# List of changes that will be parsed but not written to Changelog
ignored_change_types="Internal"

# Testing
if [ -n "$ENABLE_INTERNAL" ]; then
    change_types="Internal $change_types"
fi

#
# Git version
declare -i git_version

#
# Count the total number of changes
declare -i num_changes=0

#
# List of changes indexed by type
declare -A changes_read;

#
# Prints debug information to stderr
function log() {
    if [ "$quiet" = "no" ]; then
        echo "$@" > /dev/stderr
    fi
}

#
# Just prints usage information
function usage() {
    echo "Usage: $0 -v <version> -f <changelog-file> -s <first-commit> [-e <last-commit>] [-h]" > /dev/stderr
}

function parse_args() {
    while getopts "v:f:s:e:hq" FLAG;
    do
        case "$FLAG" in
            v)
                version="$OPTARG";
                ;;
            f)
                changelog_file="$OPTARG";
                ;;
            s)
                first_commit="$OPTARG";
                ;;
            e)
                last_commit="$OPTARG";
                ;;
            q)
                quiet="yes";
                ;;
            h)
                usage
                exit 0
                ;;
        esac
    done

    if [ -z "$first_commit" -o -z "$version" ]; then
        usage && exit 1
    fi

    if [ -z "$last_commit" ]; then
        last_commit="HEAD"
    fi

    [ -n "$changelog_file" ] && \
        [ -f "$changelog_file" ] &&  \
        [ -n "$(grep -o "Version $version" "$changelog_file")" ] && \
        log "ChangeLog: Version $version exists" && \
        exit 1

    log "-------------------------------"
    log "version       : $version"
    log "first_commit  : $first_commit"
    log "last_commit   : $last_commit"
    log "changelog_file: $changelog_file"
    log "-------------------------------"
}

#
# Loads git version into git_version
function compute_git_version() {
    local git_version_string=($(git --version | awk '{print $3}' | sed 's/\./ /g'))

    git_version=0

    for (( i=0; i<3; i++ ));
    do
        ((git_version=git_version*100+${git_version_string[i]}))
    done
}

#
# Outputs the issue number
# Parameters: <commit>
function parse_issue() {
    local commit=$1
    local issue=$(git log --format="%s%n%b" -n 1 $commit | sed -n 's/.*@issue \([^ ]*\).*/\1/p' | sed 's/^#//' | sed "s/'//g")

    # support for Fastrack-style issue number
    if [ "$issue" -eq "$issue" 2> /dev/null ]; then
        issue=$(printf "%07d" $issue)
    fi

    echo $issue
}

#
# Parses one or more changes from a single commit
# Parameters: <commit> <issue>
# Pre: <issue> is not empty
function parse_changes() {
    local commit=$1
    local issue=$2
    local tmpfile=$(mktemp)

    local log_format="%s%n%b"
    if [ $git_version -ge 10701 ]; then
        log_format="%B"
    fi

    local changes=0
    for change_type in $change_types; do

        git log --format="$log_format" -n 1 $commit | \
            grep "^$change_type:" | \
            sed "s/^$change_type: \(.*\)/\1/g" > $tmpfile

        test -s $tmpfile || continue;

        while IFS='' read -r line || [[ -n "$line" ]]; do
            line="- ($issue): $line"
            if [ -z "${changes_read[$change_type]}" ]; then
                changes_read[$change_type]="$line";
            else
                changes_read[$change_type]="${changes_read[$change_type]}\n$line";
            fi
            ((changes+=1))
        done < "$tmpfile"
    done

    if [ $changes -eq 0 ]; then
        log "Commit $commit ignored, commit message is not compliant"
    else
        ((num_changes+=changes))
    fi

    rm -f $tmpfile
}

#
# Prints final changelog
function output() {
    if [ $num_changes -eq 0 ]; then
        log "No commit is worthy of being added to the Changelog, sorry"
        return 0
    fi

    echo -e "================================================================================"
    echo -e "Version $version\n" # adds extra LF
    for change_type in $change_types; do
        if [[ $ignored_change_types != *$change_type* && -n "${changes_read[$change_type]}" ]]; then
            echo "$change_type"
            echo "$change_type" | sed 's/./~/g'
            echo -e "${changes_read[$change_type]}" | sort | uniq
            echo # extra LF
        elif [ -n "${changes_read[$change_type]}" ]; then
            log "Ignoring $(echo -e ${changes_read[$change_type]} | uniq | wc -l) $change_type changes"
        fi
    done
    echo
}

function main() {
    parse_args $@
    compute_git_version

    local commits=($(git log --format="%h" $first_commit..$last_commit))
    local num_commits=${#commits[@]}
    log "Processing $num_commits commits..."

    for (( i=0; i<$num_commits; i++ ));
    do
        local commit=${commits[$i]}
        local issue=$(parse_issue $commit)

        if [ -z "$issue" ]; then
            log "Commit $commit ignored, mentions no issue"
        else
            log "Commit $commit mentions issue $issue"
            parse_changes $commit $issue
        fi
    done

    if [ -n "$changelog_file" ]; then
        if [ -f "$changelog_file" ]; then
            local tmpfile=$(mktemp)
            output | cat - "$changelog_file" > $tmpfile
            mv "$changelog_file" "$changelog_file.old"
            mv "$tmpfile" "$changelog_file"
        else
            output > "$changelog_file"
        fi
    else
        output
    fi
}

main $@
