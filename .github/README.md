# GitHub Actions Workflows for Opal

This folder contains GitHub Actions workflows to automate various tasks in the Opal project.

## Available Workflows

### CI/CD (`ci.yml`)

This workflow is triggered on pushes to main branches and pull requests. It performs the following tasks:
- Project compilation
- Test execution
- Code formatting verification
- Package creation during releases

### Documentation (`docs.yml`)

This workflow generates and deploys Doxygen documentation to GitHub Pages when changes are made to the source code or Doxygen configuration.

### Security Analysis (`codeql.yml`)

This workflow uses CodeQL to analyze code for security vulnerabilities. It runs:
- On pushes to main branches
- On pull requests to main/master
- Once per week (Sunday at midnight)

### Benchmarks (`benchmark.yml`)

This workflow runs performance tests and records results to track performance over time.

## Configuration

To ensure these workflows function properly, make sure that:

1. Your repository has GitHub Actions permissions enabled
2. For documentation deployment, enable GitHub Pages and configure it to use the `gh-pages` branch
3. For benchmarks, ensure your benchmark executable generates results in the expected format

## Customization

You can customize these workflows by modifying the corresponding YAML files. Refer to the [GitHub Actions documentation](https://docs.github.com/en/actions) for more information.