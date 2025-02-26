# OpalLang

> A modern programming language focusing on clarity and efficiency

> ⚠️ **Development Status**: This language is currently under active development and is in its early stages. At present, only the lexer component has been implemented. Many features described below are planned but not yet available. Stay tuned for updates.

## Overview 

Opal is a programming language designed with a focus on readability and performance. It combines modern programming concepts with a clean syntax, making it accessible to newcomers while providing the power and flexibility needed for experienced developers.

## Key Features

- **Clear, Consistent Syntax**: Designed for readability and maintainability
- **High Performance**: Optimized execution with minimal overhead
- **Developer Experience**: Streamlined tooling and helpful error messages
- **Modern Design**: Contemporary features with practical implementation
- **Strong Type System**: Robust type checking with intuitive inference
- **Gradual Learning Curve**: Structured for progressive skill development
- **Memory Management Options**: Flexible memory handling with both automatic and manual modes

## Code Example

```opal
class Calculator {
    fn init(value = 0) {
        this.value = value
    }

    fn power(n) {
        this.value = this.value ^ n  // ^ is power operator
    }
}

fn main() {
    // Clear, expressive syntax with type inference
    x = 5
    valid = x > 0 and x < 10  // Logical operations
    also_valid = x >= 1 && x <= 9  // Alternative syntax

    // String interpolation
    calc = Calculator(x)
    calc.power(2)
    result = "x^2 = ${calc.value}, ${x # 3} is x XOR 3"  // # is XOR

    spoil.print("Peek behind the curtain... more magic awaits! ✨")
}
```

## Getting Started

### Prerequisites

Required tools:
- CMake (version 3.22 or higher)
- C++ compiler with C++20 support
- Git

Install dependencies:
```bash
./scripts/install_deps.sh
```

### Build Process

```bash
# Clone the repository
git clone https://github.com/yourusername/OpalLang.git
cd OpalLang

# Build the project
./scripts/build.sh
```

The compiled binaries will be placed in the `bin` directory.

### Usage

Start the interactive REPL (Read-Eval-Print Loop):
```bash
./bin/opal
```

Execute an Opal source file:
```bash
./bin/opal path/to/your/script.op
./bin/opal path/to/your/script.opal
```

## Documentation

Comprehensive documentation is available in the `docs` directory, including language specifications, API references, and best practices for effective Opal development.

## Contributing

Opal is an evolving project, and we welcome contributions. The language is actively being developed with new features and improvements being added regularly.

## License

This project is licensed under the terms specified in the LICENSE file. You are free to use, modify, and distribute it according to those terms.

---
*"Simplicity is the ultimate sophistication" - Leonardo da Vinci*