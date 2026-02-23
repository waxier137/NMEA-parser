#NMEA 0183 Message Parser
A high-performance C++ tool designed to process raw GNSS NMEA logs, validate data integrity via XOR checksums, and visualize spatial paths in Google Earth.

##Project Overview

This project bridges the gap between raw serial data and GIS visualization. It was developed to handle diverse NMEA message types (GGA, RMC, DTM, etc.) through a scalable, polymorphic architecture.

###Key Features
- **Polymorphic Architecture**: Uses a base-class dispatcher to handle multiple NMEA message types through a single interface.
- **Data Integrity**: Implements XOR checksum validation to filter corrupted serial data before processing.
- **GIS Integration**: Transforms raw latitude/longitude into 3D tracklogs using the `libkml` library.
- **Memory Management**: Optimized heap allocation with zero memory leaks, demonstrated via strict lifecycle management.

##Demo
###Spatial Visualization in Calgary
Below is a tracklog generated from campus (University of Calgary) through downtown Calgary.

<img width="672" height="485" alt="Screenshot 2026-02-21 030152" src="https://github.com/user-attachments/assets/250876a3-c99d-4068-a671-c738ef88376b" />

##Installation and Build
This project uses **vcpkg** for dependency management and **CMake** for build automation.

```bash
# Clone the repository
git clone [https://github.com/waxier137/NMEA-parser.git](https://github.com/waxier137/NMEA-parser.git)

# Configure with vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg_path]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build
