all:
	pdflatex seminar.tex
	bibtex seminar
	pdflatex seminar.tex
	pdflatex seminar.tex
