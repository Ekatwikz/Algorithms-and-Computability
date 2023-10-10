# Algorithms and Computability Labs
Initial boilerplate for our AaC groupwork

## Prerequisites
- [make](https://www.gnu.org/software/make/)
- some c++ compiler (just a reminder for myself :skull:)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)  
    (Ubuntu: `sudo apt install clang-tidy`)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)  
    (Ubuntu: `sudo apt install clang-format`)
- [graphviz](https://www.graphviz.org/download/#executable-packages) (optional)
- [doxygen](https://www.doxygen.nl/manual/install.html) (optional)  
    (Ubuntu: `sudo apt install doxygen`)  
    (Ubuntu: `sudo apt install doxygen-latex`)
- [texlive](https://tug.org/texlive/) (optional)  
    (Ubuntu: `sudo apt install texlive`)

## Build
- `make` :thumbsup:

## Lint
- `make check-format` and `make check-lint`  
    If they return 0 (doesn't have to be clean), the CI checks will be green too :thumbsup:  
    You can also `make auto-lint` to try automatically fix formatting and linting issues  
    beware: this will modify actual files + I haven't tested much

## Documentation
- `make docs`  
    will generate refman.pdf in ./docs/latex and index.html in ./docs/html  
- the CI also hosts the generated docs on [gh pages](https://ekatwikz.github.io/Algorithms-and-Computability)  
    it also uploads the generated refman.pdf artifact, in the summary of the latest [Documentation action](https://github.com/Ekatwikz/Algorithms-and-Computability/actions/workflows/documentation.yml)


