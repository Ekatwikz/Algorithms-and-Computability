# Algorithms and Computability Labs
Initial boilerplate for our AaC groupwork

## Prerequisites
- [make](https://www.gnu.org/software/make/)
- some c++ compiler (just a reminder for myself :skull:)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)  
(Ubuntu: `sudo apt install clang-tidy`)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)  
(Ubuntu: `sudo apt install clang-format`)
- [graphviz](https://www.graphviz.org/download/#executable-packages) (optional, but very recommended)
- [doxygen](https://www.doxygen.nl/manual/install.html) (optional)  
(Ubuntu: `sudo apt install doxygen doxygen-latex`)
- [texlive](https://tug.org/texlive/) (optional)  
(Ubuntu: `sudo apt install texlive`)

## Build
- `make` :thumbsup:

## Tests
- `make run-tests`  
If they pass, the CI checks will accept too :thumbsup:  

## Lint
- Run `./lint-helper.sh -c format` and `./lint-helper -c lint`  
If they all say `OKAY!`, the CI checks will accept too :thumbsup:  
You can also do `./lint-helper -a all` to try automatically fix formatting and linting issues  
beware: this will modify your code!  

![Deployment Script demo](./extra_stuff/lint-helper-demo.gif)

## Documentation
- `make docs`  
will automatically generate ./docs/latex/refman.pdf and ./docs/html/index.html
from the doxy comments  
It'd be nice if you could keep `make docs` final output as `OKAY!` as well,
you only have to write some brief documentation for new function definitions
- the CI also hosts the generated docs on [gh pages](https://ekatwikz.github.io/Algorithms-and-Computability)  
it also uploads the generated refman.pdf artifact, in the summary of the latest [Documentation action](https://github.com/Ekatwikz/Algorithms-and-Computability/actions/workflows/documentation.yml)


