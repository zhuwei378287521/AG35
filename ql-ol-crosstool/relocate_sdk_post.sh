#!/bin/sh

# replace {oldprefix} with the new prefix in all text files: configs/scripts/etc.
# replace the host perl with SDK perl.
target_sdk_dir="$1"
native_sysroot="$1/sysroots/x86_64-oesdk-linux"

OLD_INSTALL_DIR="$2"


for replace in "$target_sdk_dir -maxdepth 1" "$native_sysroot"; do
    find $replace -type f
done | xargs -n100 file | grep ":.*\(ASCII\|script\|source\).*text" | \
    awk -F':' '{printf "\"%s\"\n", $1}' | \
    grep -v "$target_sdk_dir/ql-ol-crosstool-env-init" | \
    xargs -n100 sed -i \
        -e "s:$OLD_INSTALL_DIR:$target_sdk_dir:g" \
        -e "s:^#! */usr/bin/perl.*:#! /usr/bin/env perl:g" \
        -e "s: /usr/bin/env perl: /usr/bin/env perl:g"
# change all symlinks pointing to {oldprefix}
for l in $(find $native_sysroot -type l); do
	ln -sfn $(readlink $l| sed -e "s:$OLD_INSTALL_DIR:$target_sdk_dir:") $l
done

