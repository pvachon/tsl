# The Standard Library

This is the repository for the The Standard Library, or TSL.

# Building

You will need at least CMake version 3.7 installed. Most distributions have some version
of CMake available. Once you have CMake available, create a `build` directory and invoke
CMake in there:

```
mkdir tsl/build
cd tsl/build
cmake ..
```
This creates the necessary Makefiles. Next, invoke make to build the project:
```
make
```

## Installation

Install the TSL somewhere convenient using the `make install` command, after successfully
building the distribution.

## Dependencies

The TSL currently depends on:
 * Linux
 * ConcurrencyKit (most distributions have a package, look for libck)
 * Jansson (most distributions have a package, look for libjansson)
 * CMake (most distributions have a package, look for cmake)

### On Debian/Raspbian/Ubuntu
To perform the minimal build:

1. Install Build Dependencies from `apt`:
   ```
   sudo apt-get install build-essential git libjansson-dev \
       libck-dev cmake\
       python
   ```

2. Check out tsl, build it, and install it. In short:
   ```
   git clone https://github.com/pvachon/tsl.git
   mkdir tsl/build
   cd tsl/build
   cmake ..
   make
   make install
   ```

# Packaging
You can create a package using `cpack`. You'll likely want to package a release build, so
do something like the following:
```
mkdir tsl/release
cd tsl/release
cmake -DCMAKE_BUILD_TYPE=Release ..
cpack
```
This will leave a `dpkg` file in the release directory, ready for installation.

# Getting Help

If you think you've found a bug (hey, it happens), open a [Github issue](https://github.com/pvachon/tsl/issues) for the project.

# License

The TSL is provided under two licenses - the GPLv2 and the MIT/X license. You
can pick whichever license works best for you.

# Author

Most of this code was written by Phil Vachon (phil@security-embedded.com).

