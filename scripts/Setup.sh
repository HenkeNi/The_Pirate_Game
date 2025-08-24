#!/bin/bash
# setup.sh - initialize submodules and bootstrap vcpkg on Linux/macOS
# Note: untested if you don’t have a mac/Linux environment yet

set -e  # exit immediately if any command fails

# Go to repo root (assumes script is in Scripts/)
cd "$(dirname "$0")/.."

echo "Updating submodules..."
git submodule update --init --recursive
echo "Submodules updated successfully!"

# Bootstrap vcpkg
if [ -d "extern/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    cd extern/vcpkg
    ./bootstrap-vcpkg.sh
    echo "vcpkg bootstrapped successfully!"
else
    echo "vcpkg submodule not found at extern/vcpkg"
    exit 1
fi

echo "Setup complete!"
