# OpalLang

> Unleash the power of elegant simplicity in programming

> ⚠️ **Development Status**: This language is currently under active development and is in its early stages. At present, only the lexer component has been implemented. Many features described below are planned but not yet available. Stay tuned for exciting updates! ✨

## Overview 

Opal is more than just a programming language – it's a fresh perspective on how code should feel. Designed with simplicity and performance in mind, Opal brings together the best aspects of modern programming while eliminating unnecessary complexity. Whether you're a beginner taking your first steps into programming or a seasoned developer looking for a more elegant solution, Opal offers something special.

## Why Opal?

- **Crystal Clear Syntax**: Write code that reads like poetry – clean, intuitive, and beautiful
- **Lightning-Fast Performance**: Built from the ground up with speed in mind
- **Developer Joy**: Experience the satisfaction of writing code that just feels right
- **Future-Ready**: Modern features wrapped in an elegant package
- **Type Safety Without Tears**: Strong typing that works with you, not against you
- **Learning Curve? More Like a Learning Glide**: Perfect for beginners, powerful for experts
- **Flexible Memory Management**: Choose your own path - automatic or manual, you decide!

## Taste the Magic 🚀

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
    // Clean, expressive syntax that speaks for itself, with untyped variables
    x = 5
    valid = x > 0 and x < 10  // Python-style elegance
    also_valid = x >= 1 && x <= 9  // C-style familiarity

    // String interpolation
    calc = Calculator(x)
    calc.power(2)
    result = "x^2 = ${calc.value}, ${x # 3} is x XOR 3"  // # is XOR

    spoil.print("Peek behind the curtain... more magic awaits! 🎩✨")
}
```

## Quick Start

### Prerequisites

Before diving into the world of Opal, you'll need:
- CMake (version 3.22 or higher)
- C++ compiler with C++20 support
- Git

One command to set up your magical workspace:
```bash
./scripts/install_deps.sh  # Let us handle the setup for you
```

### Conjure Your Build

```bash
# Clone your gateway to Opal
git clone https://github.com/yourusername/OpalLang.git
cd OpalLang

# Summon the build
./scripts/build.sh
```

Your wand (compiled binary) will appear in the `bin` directory.

### Start Casting Spells

Jump into the interactive playground:
```bash
./bin/opal
```

Or run your masterpiece:
```bash
./bin/opal path/to/your/script.op
./bin/opal path/to/your/script.opal
```

## Learn More

Dive deeper into the magic - explore our comprehensive documentation in the `docs` directory. Each feature is carefully documented, with examples and best practices to help you make the most of Opal.

## Join the Journey

Opal is more than a project - it's a vision of what programming can be. We're actively developing and expanding its capabilities, and every update brings new exciting features. Want to be part of something special? Keep watching this space!

## License

This magical creation is licensed under the terms found in the LICENSE file. Feel free to explore, experiment, and create!

---
*"Simplicity is the ultimate sophistication" - Leonardo da Vinci*