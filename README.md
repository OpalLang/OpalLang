# OpalLang

## Overview

Opal is a modern programming language designed with simplicity and performance in mind. It offers a clean, intuitive syntax that makes it easy to learn for beginners while providing the power and flexibility needed for experienced developers.

## Key Features

- **Simple and Intuitive Syntax**: Opal's syntax is straightforward and easy to understand, reducing the learning curve for new programmers.
- **Performance-Oriented**: Despite its simplicity, Opal is designed to deliver high performance for a wide range of applications.
- **Clean Code Structure**: The language encourages clean, readable code with a focus on maintainability.
- **Modern Language Features**: Includes support for classes, functions, control structures, and other modern programming paradigms.
- **Strong Type System**: Provides type safety while maintaining a clean syntax without excessive verbosity.

## Example

```
fn main() {
    print("Hello, World!")
}
```

A more complex example:

```
fn main() {
    secret_number = 29
    guess = 0

    while guess != secret_number {
        guess = input("Enter a guess: ")
    }
    print("You guessed the number!")
}
```

## Getting Started

### Prerequisites

Before building Opal, make sure you have the following dependencies installed:
- CMake (version 3.22 or higher)
- C++ compiler with C++20 support
- Git

You can install all required dependencies using the provided script:

```bash
# Install dependencies
./scripts/install_deps.sh
```

### Building the Project

To build the Opal language interpreter, follow these steps:

```bash
# Clone the repository
git clone https://github.com/yourusername/OpalLang.git
cd OpalLang

# Build the project
./scripts/build.sh
```

The compiled binary will be available in the `bin` directory.

### Running Opal

After building, you can run Opal in REPL (interactive) mode:

```bash
./bin/opal
```

Or execute an Opal script:

```bash
./bin/opal path/to/your/script.op
./bin/opal path/to/your/script.opal
```

## Documentation

For more detailed information about the language, please refer to the documentation in the `docs` directory.

## About

This project is a programming language under active development. Features may change and new capabilities are being added regularly.

## License

This project is licensed under the terms found in the LICENSE file.