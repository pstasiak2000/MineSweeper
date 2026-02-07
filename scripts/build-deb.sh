#!/usr/bin/env bash

### LAUNCH THIS SCRIPT FROM ROOT
set -e

#ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ROOT="."

rm -rf "$ROOT/debian"
cp -r "$ROOT/packaging/debian" "$ROOT/debian"


### --- VERSIONING ---
# Make sure you have dch installed: sudo apt install devscripts
# Step 1: Read latest git tag and strip the 'v'

TAG=$(git describe --tags --abbrev=0 | sed 's/^v//')
echo "Building Debian package for version: $TAG"

# Step 2: Automatically create/update debian/changelog
# If you don't have a changelog yet, this creates it
dch --create -v ${TAG}-1 --package minesweeper "Automatic build from git tag ${TAG}" --force-distribution

# Step 3: Build the package without signing
debuild -us -uc -b
