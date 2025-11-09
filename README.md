# PostgreSQL MVCC Visualizer

A live, interactive visualizer application that demonstrates how PostgreSQL's Multi-Version Concurrency Control (MVCC) system works.

## Prerequisites

- CMake 3.15 or higher
- C++20 compatible compiler (Clang 12+ recommended for macOS)
- macOS 10.14+ (for AppKit support)
- Xcode Command Line Tools (for Objective-C++ support)

### Installing Xcode Command Line Tools on macOS

```bash
xcode-select --install
```

## Building

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure CMake with BUILD_EXECUTABLE enabled:
```bash
cmake .. -DPostgresMvccVisualizer_BUILD_EXECUTABLE=ON
```

3. Build the project:
```bash
cmake --build .
```

## Running

After building, run the executable:

```bash
./bin/Debug/PostgresMvccVisualizer
```

Or on Linux:
```bash
./bin/Release/PostgresMvccVisualizer
```

## Project Structure

The project uses a C++ core with AppKit UI via Objective-C++ bridge:

- `src/main.mm` - Application entry point (Objective-C++)
- `src/main_window_core.cpp` - Pure C++ core with business logic
- `include/project/main_window_core.hpp` - C++ core header
- `src/app_delegate.mm` - NSApplication delegate (Objective-C++)
- `include/project/app_delegate.hpp` - App delegate header
- `src/main_window_controller.mm` - Window controller bridging C++ to AppKit (Objective-C++)
- `include/project/main_window_controller.hpp` - Window controller header
- `POSTGRES_MVCC_VISUALIZER.md` - Detailed project documentation

## Architecture

This application uses a **C++ core + AppKit UI via Objective-C++** architecture:

- **Pure C++ Core**: Business logic and models are written in pure C++ (`.hpp`/`.cpp` files)
- **Objective-C++ Bridge**: UI layer uses Objective-C++ (`.mm` files) to bridge between C++ and AppKit
- **Native AppKit**: Uses native macOS UI frameworks (NSWindow, NSViewController, NSToolbar, etc.)

This approach provides:
- 100% native macOS look and feel
- Full control over menus and macOS-specific patterns
- Clean separation between business logic (C++) and UI (AppKit)

## Development

The project uses modern C++20 features with Objective-C++ for the macOS UI layer. See `POSTGRES_MVCC_VISUALIZER.md` for detailed architecture and implementation plans.

