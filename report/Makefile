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
# -use-make tells latexmk to call make (See: CUSTOM BUILD RULES) for generating missing files.

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
	#rm -rfv $(OUTPUTDIR) # Might wanna just nuke everything
	latexmk -CA
	# There's also the files in the current directory that an editor might vomit out as intermediates
	rm -fv *.pdf *.aux **/*.aux *.bbl *.blg *.log *.out *.toc *.synctex.gz *.run.xml *.fdb_latexmk *-blx.bib
