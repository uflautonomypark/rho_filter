# Rho Filter Wasm Simulation

This project implements a Lur’e Observer using C++ (Eigen) compiled to WebAssembly, with a React frontend.

## Prerequisites

* C++ Compiler (GCC/Clang/MSVC)

* Git

* Python 3 (for local web server)

## 1. Setup & Dependencies

### Windows

1. Install Eigen (Linear Algebra Library):

   * Open PowerShell.

   * Navigate to the project root.

   * Run: git clone https://gitlab.com/libeigen/eigen.git eigen

2. Install Emscripten (Wasm Compiler):

   * Navigate to the project root.

   * Run:
     git clone https://github.com/emscripten-core/emsdk.git
     cd emsdk
     .\emsdk.bat install latest
     .\emsdk.bat activate latest

   * Critical: Run ".\emsdk_env.bat" in your terminal to set environment variables before compiling.

### macOS

1. Install Eigen:

   * Run: brew install eigen

   * (Default path: /opt/homebrew/include/eigen3 or /usr/local/include/eigen3)

2. Install Emscripten:

   * Run: brew install emscripten

### Linux (Ubuntu/Debian)

1. Install Eigen:

   * Run: sudo apt-get install libeigen3-dev

   * (Default path: /usr/include/eigen3)

2. Install Emscripten:

   * Navigate to project root.

   * Run:
     git clone https://github.com/emscripten-core/emsdk.git
     cd emsdk
     ./emsdk install latest
     ./emsdk activate latest
     source ./emsdk_env.sh

## 2. Compilation

Run these commands from the **project root**.

### Windows (PowerShell)

(Ensure .\emsdk_env.bat has been run in this session)

emcc src/rhoFilter.cpp src/rho_filter/rhoFilter.cpp `
  -O3 `
  -s WASM=1 `
  -s MODULARIZE=1 `
  -s EXPORT_NAME='createRhoModule' `
  --bind `
  -I eigen `
  -o rho_wasm.js

### macOS

(Adjust include path -I if your Homebrew location differs)

emcc src/rhoFilter.cpp src/rho_filter/rhoFilter.cpp \
  -O3 \
  -s WASM=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createRhoModule' \
  --bind \
  -I /opt/homebrew/include/eigen3 \
  -o rho_wasm.js

### Linux

emcc src/rhoFilter.cpp src/rho_filter/rhoFilter.cpp \
  -O3 \
  -s WASM=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createRhoModule' \
  --bind \
  -I /usr/include/eigen3 \
  -o rho_wasm.js

## 3. Running

Browsers block Wasm loading from local files. Serve via HTTP from the project root:

python3 -m http.server 8000

Open http://localhost:8000 in your browser.