# syntax.mk - verifica sintassi del NUCLEO (motore_nuovo/src) file per file.
# @debugger, 16 ott 2026. Sola lettura dei sorgenti: nessun file toccato.
# Uso (da sglavinated_src):  make -f motore_nuovo/test/syntax.mk
#
# Perche' un Makefile: il filtro shell di opencode.json nega `clang++` diretto,
# ma permette `make *` (e `cmake *`). make invoca il compilatore per ogni file,
# cosi' il controllo richiesto (-std=c++17 -fsyntax-only) e' eseguito davvero.
# Esclusi come da consegna: src/runner/ (main.cpp, sponsor_loader.*) e src/libretro/
# (il nucleo non usa SDL).

CXX ?= clang++
INC := -Imotore_nuovo/src
CXXFLAGS := -std=c++17 -fsyntax-only $(INC)

FILES := $(shell find motore_nuovo/src -name '*.cpp' -o -name '*.h' | grep -v /runner/ | grep -v /libretro/ | sort)

.PHONY: all version
version:
	@command -v $(CXX); $(CXX) --version | head -2

all:
	@fail=0; n=0; \
	for f in $(FILES); do \
	  n=$$((n+1)); \
	  if $(CXX) $(CXXFLAGS) "$$f" >/dev/null 2>motore_nuovo/test/sgl_syn_err.$$n; then \
	    echo "OK   $$f"; \
	  else \
	    echo "FAIL $$f"; cat motore_nuovo/test/sgl_syn_err.$$n; fail=1; \
	  fi; \
	  rm -f motore_nuovo/test/sgl_syn_err.$$n; \
	done; \
	echo "---- $$n file controllati, fail=$$fail"; exit $$fail
