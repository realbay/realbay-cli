
## What is RealBay?

RealBay is a tool for browsing torrents.

## What is RealBay CLI?

These are the Command Line Interface (CLI) to the tools. Maybe
one day there will be a `librealbay` but for now this small C
toolset can be used to build and search realbay indexes.

## Building from Source

You can build this from source simply by using `make`, which the
`Makefile` was made by a human should be simple. If none of that makes
sense to you, try this:

    git clone https://github.com/realbay/realbay-cli
    cd realbay-cli
    make

You only need `gcc` to make this from source code. It can be compiled
most anywhere with most any C compiler and has no external dependencies
besides `libc` (for `fopen()` etc.)

## How do I create a new index?

Making a new index is done using the `createindex` command:

    realbay createindex myfile.csv myindex.tordex

Here there are two files:

 * `myfile.csv` is a CSV file that should be separated by pipes
 * `myindex.tordex` is where to write the index

The generated index file is binary and will be 64 bytes for every
line in the CSV file.
