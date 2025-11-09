#!/bin/bash

# Build and Run Script for PostgreSQL MVCC Visualizer
# This script builds the project and runs the executable
# Uses AppKit via Objective-C++ for native macOS UI

set -e  # Exit on error

# Check if running on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "Error: This application requires macOS (AppKit)"
    exit 1
fi

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Get the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
EXECUTABLE="${BUILD_DIR}/bin/Debug/PostgresMvccVisualizer"

echo -e "${GREEN}=== PostgreSQL MVCC Visualizer Build & Run ===${NC}\n"

# Step 1: Create build directory if it doesn't exist
if [ ! -d "${BUILD_DIR}" ]; then
    echo -e "${YELLOW}Creating build directory...${NC}"
    mkdir -p "${BUILD_DIR}"
fi

# Step 2: Configure CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
cd "${BUILD_DIR}"
cmake .. -DPostgresMvccVisualizer_BUILD_EXECUTABLE=ON \
         -DCMAKE_BUILD_TYPE=Debug

# Step 3: Build the project
echo -e "${YELLOW}Building project...${NC}"
cmake --build . --target PostgresMvccVisualizer

# Step 4: Check if executable exists
if [ ! -f "${EXECUTABLE}" ]; then
    echo -e "${RED}Error: Executable not found at ${EXECUTABLE}${NC}"
    exit 1
fi

# Step 5: Run the executable
echo -e "${GREEN}Build successful! Running application...${NC}\n"
"${EXECUTABLE}"

