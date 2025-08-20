#!/bin/bash

# Simple installation script for fastdijkstra package
# This script installs the package in development mode for easy testing

echo "Installing fastdijkstra package..."

# Check if pybind11 is installed
python3 -c "import pybind11" 2>/dev/null || {
    echo "Installing pybind11..."
    pip install pybind11
}

# Install the package in development mode
echo "Installing fastdijkstra in development mode..."
pip install -e .

echo "Installation complete!"
echo ""
echo "Test the installation by running:"
echo "python3 -c 'import fastdijkstra; print(\"Success!\")'"
