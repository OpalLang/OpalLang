# Contributing Guidelines

Thank you for considering contributing to OpalLang! We welcome contributions from the community to help improve and grow the project. Please follow these guidelines to ensure a smooth contribution process.

## Getting Started

1. Fork the repository on GitHub.
2. Clone your forked repository to your local machine.
3. Create a new branch for your feature or bugfix.

```sh
$ git checkout -b my-feature-branch
```

## Making Changes

1. Ensure your code follows the project's coding style and conventions. You can format your code using the provided script:

```sh
$ ./scripts/format_code.sh -f
```

2. Write tests for your changes to ensure they are working correctly.
3. Run the existing tests to make sure your changes do not break any existing functionality.

```sh
$ ./scripts/run_tests.sh
```

## Submitting a Pull Request

1. Push your changes to your forked repository.

```sh
$ git push origin my-feature-branch
```

2. Open a pull request on GitHub against the `staging` branch of the main repository.
3. Provide a clear and descriptive title and description for your pull request.
4. Ensure your pull request passes all continuous integration checks.

## Code Review Process

1. One of the project maintainers will review your pull request.
2. You may be asked to make changes or provide additional information.
3. Once your pull request is approved, it will be merged into the `staging` branch.

## Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project, you agree to abide by its terms.

## Contact

If you have any questions or need further assistance, feel free to email us at [opallang.contributors@gmail.com](mailto:opallang.contributors@gmail.com).

Thank you for your contributions!