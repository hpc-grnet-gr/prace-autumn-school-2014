FC=g77
FCFLAGS=-O0
TARGETS=mm

all: $(TARGETS)

mm: mm.f
	$(FC) $(FCFLAGS) $^ -o $@ -lblas

clean:
	rm -rf $(TARGETS)
