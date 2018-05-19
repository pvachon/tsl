# The Standard Library

This is the repository for the The Standard Library, or TSL.

# Building

You will need Python installed. Invoke `./waf configure` to configure the build,
and `./waf build` to build the tools.

The applications will end up in `build/release/bin`. Just invoke them in the usual way.

## Installation

Install the TSL somewhere convenient using the `./waf install` command.

## Dependencies

The TSL currently depends on:
 * Linux
 * ConcurrencyKit (https://github.com/concurrencykit/ck)
 * libjansson (most distributions have a package)

### On Debian/Raspbian/Ubuntu
To perform the minimal build:

1. Install Build Dependencies from `apt`:
   ```
   sudo apt-get install build-essential git libjansson-dev
   ```

2. Build ConcurrencyKit:
   ```
   git clone https://github.com/concurrencykit/ck.git
   cd ck
   ./configure && make && sudo make install
   ```

3. Check out tsl, build it, and install it:
   ```
   git clone https://github.com/pvachon/tsl.git
   cd tsl-sdr
   ./waf configure && ./waf build install
   ```

# Getting Help

Be sure to check the [project wiki](https://github.com/pvachon/tsl-sdr/wiki) for
use cases, documentation and other details.

If you think you've found a bug (hey, it happens), open a [Github issue](https://github.com/pvachon/tsl-sdr/issues) for the project.

# License

The TSL is provided under two licenses - the GPLv2 and the MIT/X license. You
can pick whichever license works best for you.

# Author

Most of this code was written by Phil Vachon (phil@security-embedded.com).

