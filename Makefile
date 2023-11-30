SOURCEDIR:=./src
DOCSDIR:=./docs
OUTPUTDIR:=./build
OBJECTDIR:=$(OUTPUTDIR)/obj
INCLUDEDIR:=./include
LIBDIR:=./lib

# blgh
TESTSOURCEDIR:=./test
TESTOUTPUTDIR:=./test/build
TESTOBJECTDIR:=$(TESTOUTPUTDIR)/obj
TESTLIBDIR:=./test/catch2

WARNINGS:=all extra pedantic

# A little hacky but what can we dooo
override DEBUGFLAGS:=-g3 -O0 $(DEBUGFLAGS)

FFLAGS:=no-omit-frame-pointer
STANDARD:=c++20

ifeq ($(OS), Windows_NT)
	EXTENSION:=.exe
	CXXFLAGS+=-static-libstdc++
else
	# Gucci stuff onli on linix
	FFLAGS+=sanitize=address,undefined
	EXTENSION:=
endif

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
DEFAULTTARGETS:=$(patsubst $(SOURCEDIR)/%.cpp, $(OUTPUTDIR)/%$(EXTENSION), $(SOURCES))
LIBSOURCES:=$(wildcard $(LIBDIR)/*.cpp)
LIBHEADERS:=$(wildcard $(INCLUDEDIR)/*.hpp)
OBJECTS:=$(patsubst $(LIBDIR)/%.cpp, $(OBJECTDIR)/%.o, $(LIBSOURCES))

# kinda repetitve...
TESTSOURCES:=$(wildcard $(TESTSOURCEDIR)/*.cpp)
TESTTARGETS:=$(patsubst $(TESTSOURCEDIR)/%.cpp, $(TESTOUTPUTDIR)/%$(EXTENSION), $(TESTSOURCES))
TESTLIBSOURCES:=$(wildcard $(TESTLIBDIR)/*.cpp)
TESTLIBHEADERS:=$(wildcard $(TESTLIBDIR)/*.hpp)
TESTOBJECTS:=$(patsubst $(TESTLIBDIR)/%.cpp, $(TESTOBJECTDIR)/%.o, $(TESTLIBSOURCES))

CXXFLAGS+=-I$(INCLUDEDIR) -I$(TESTLIBDIR) $(WARNINGS:%=-W%) $(FFLAGS:%=-f%) $(DEBUGFLAGS) -std=$(STANDARD)

.PHONY: all clean docs docs-clean tests run-tests
.SECONDARY: $(TESTOBJECTS)
.DELETE_ON_ERROR:

ifndef VERBOSE
.SILENT:
endif

$(OBJECTDIR)/%.o: $(LIBDIR)/%.cpp $(LIBHEADERS)
	mkdir -pv $(OBJECTDIR)
	printf "=== %s -> %s ===\n" "$<" "$@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(DEFAULTTARGETS)
$(OUTPUTDIR)/%$(EXTENSION): $(SOURCEDIR)/%.cpp $(OBJECTS) $(LIBHEADERS)
	mkdir -pv $(OUTPUTDIR)
	printf "=== %s -> %s ===\n" "$<" "$@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $@

$(TESTOBJECTDIR)/%.o: $(TESTLIBDIR)/%.cpp $(TESTLIBHEADERS)
	mkdir -pv $(TESTOBJECTDIR)
	printf "=== %s -> %s ===\n" "$<" "$@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(TESTTARGETS)
$(TESTOUTPUTDIR)/%$(EXTENSION): $(TESTSOURCEDIR)/%.cpp $(OBJECTS) $(TESTOBJECTS)
	mkdir -pv $(TESTOUTPUTDIR)
	printf "=== %s -> %s ===\n" "$<" "$@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(TESTOBJECTS) $< -o $@

# slightly goofy
run-tests: tests
	RESULT=0 ; \
	for test in $(TESTTARGETS) ; do \
		printf "===\n\033[0;30;46mTESTING\033[0m %s\n===\n\n" "$$test" ; \
		command ./$$test $(TESTFLAGS) ; \
		RESULT=$$((RESULT | $$?)) ; \
	done ; \
	exit $$RESULT

docs:
	doxygen
	make -C $(DOCSDIR)/latex
	printf "\n\033[0;30;42mOKAY!\033[0m\n"

# nukes are simpler than surgeries:
clean:
	rm -rfv $(OUTPUTDIR) $(TESTOUTPUTDIR)

docs-clean:
	rm -rfv $(DOCSDIR)

.PHONY: clean
# Yoinked+edited from: https://tex.stackexchange.com/a/40759

# NB: For now this has to be manually synced with the latexmkrc
OUTPUTDIR:=./build
SOURCEDIR:=.
SOURCES:=$(wildcard $(SOURCEDIR)/*.tex)
DEFAULTTARGETS:=$(patsubst $(SOURCEDIR)/%.tex, $(OUTPUTDIR)/%.pdf, $(SOURCES))

.PHONY: all clean

ifndef VERBOSE
.SILENT:
endif

# MAIN LATEXMK RULE
# -pdf tells latexmk to generate PDF directly (instead of DVI).
# -pdflatex="" tells latexmk to call a specific backend with specific options.
# -use-make tells latexmk to call make for generating missing files.

# -interaction=nonstopmode keeps the pdflatex backend from stopping at a
# missing file reference and interactively asking you for an alternative.
all: $(DEFAULTTARGETS)
$(OUTPUTDIR)/%.pdf: %.tex
	mkdir -pv $(OUTPUTDIR)
	latexmk -use-make $<

# CUSTOM BUILD RULES
%.tex: %.raw
	./raw2tex $< > $@

%.tex: %.dat
	./dat2tex $< > $@

clean:
	#rm -rfv $(OUTPUTDIR)
	latexmk -CA
