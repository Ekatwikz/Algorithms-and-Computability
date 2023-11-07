SOURCEDIR:=./src
DOCSDIR:=./docs
OUTPUTDIR:=./build
OBJECTDIR:=$(OUTPUTDIR)/obj
INCLUDEDIR:=./include
LIBDIR:=./lib

WARNINGS:=all extra pedantic

# TODO: Add an option to toggle these:
DEBUGFLAGS:=-g3 -O0

FFLAGS:=no-omit-frame-pointer
STANDARD:=c++20

ifeq ($(OS), Windows_NT)
	EXTENSION=.exe
else
	# Gucci stuff onli on linix
	FFLAGS+=sanitize=address,undefined
	EXTENSION=
endif

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
DEFAULTTARGETS:=$(patsubst $(SOURCEDIR)/%.cpp, $(OUTPUTDIR)/%$(EXTENSION), $(SOURCES))
LIBSOURCES:=$(wildcard $(LIBDIR)/*.cpp)
LIBHEADERS:=$(wildcard $(INCLUDEDIR)/*.hpp)
OBJECTS:=$(patsubst $(LIBDIR)/%.cpp, $(OBJECTDIR)/%.o, $(LIBSOURCES))

CFLAGS=-I$(INCLUDEDIR) $(WARNINGS:%=-W%) $(FFLAGS:%=-f%) $(DEBUGFLAGS) -std=$(STANDARD)

.PHONY: all clean docs docs-clean

$(OBJECTDIR)/%.o: $(LIBDIR)/%.cpp $(LIBHEADERS)
	@mkdir -pv $(OBJECTDIR)
	@printf "=== %s -> %s ===\n" "$<" "$@"
	@$(CXX) $(CFLAGS) -c $< -o $@

all: $(DEFAULTTARGETS)
$(OUTPUTDIR)/%$(EXTENSION): $(SOURCEDIR)/%.cpp $(OBJECTS) $(LIBHEADERS)
	@mkdir -pv $(OUTPUTDIR)
	@printf "=== %s -> %s ===\n" "$<" "$@"
	@$(CXX) $(CFLAGS) $(OBJECTS) $< -o $@

docs:
	@doxygen
	@make -C $(DOCSDIR)/latex
	@printf "\n\033[0;30;42mOKAY!\033[0m\n"

# nukes are simpler than surgeries:
clean:
	@rm -rfv $(OUTPUTDIR)

docs-clean:
	@rm -rfv $(DOCSDIR)

