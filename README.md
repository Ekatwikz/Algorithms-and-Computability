# Algorithms and Computability Labs
Our AaC groupwork repo

## Prerequisites
- [make](https://www.gnu.org/software/make/)
- some c++ compiler (just a reminder for myself :skull:)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)  
(Ubuntu: `sudo apt install clang-tidy`)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)  
(Ubuntu: `sudo apt install clang-format`)
- [GNU parallel](https://www.gnu.org/software/parallel) (optional)  
(Ubuntu: `sudo apt install parallel`)  
(NB: Not the moreutils version!)
- [graphviz](https://www.graphviz.org/download/#executable-packages) (optional, but very recommended)
- [doxygen](https://www.doxygen.nl/manual/install.html) (optional)  
(Ubuntu: `sudo apt install doxygen doxygen-latex`)
- [latexmk](https://www.cantab.net/users/johncollins/latexmk) (optional)  
(Ubuntu: `sudo apt install latexmk texlive-science texlive-bibtex-extra texlive-latex-extra`)
- [TeXstudio](https://www.texstudio.org/#download) (optional)

## Build
- `make` :thumbsup:

- If you prefer to see the guts of the build:  
`make VERBOSE=1`

## Tests
- `make run-tests`  
If they pass, the CI checks will accept too :thumbsup:  

- Again, if you prefer verbosity, you can do:  
`make run-tests VERBOSE=1 TESTFLAGS="-sv high"`

## Lint
- Run `./lint-helper.sh -c format` and `./lint-helper.sh -c lint`  
If they all say `OKAY!`, the CI checks will accept too :thumbsup:  

- You can also do `./lint-helper.sh -a all` to try automatically fix formatting and linting issues  
beware: this will modify your code!

- Sprinkle in some `-j` for a bit of parallelism
if things start to feel a little sluggish

![Deployment Script demo](./extra_stuff/lint-helper-demo.gif)

## Documentation
- `make docs`  
will automatically generate ./docs/latex/refman.pdf and ./docs/html/index.html
from the doxy comments  
It'd be nice if you could keep `make docs` final output as `OKAY!` as well,
you only have to write some brief documentation for new function definitions
- the CI also hosts the generated docs on [gh pages](https://ekatwikz.github.io/Algorithms-and-Computability)  
it also uploads some other useful generated files, in the summary of the latest
[Build action](https://github.com/Ekatwikz/Algorithms-and-Computability/actions/workflows/build.yml)
