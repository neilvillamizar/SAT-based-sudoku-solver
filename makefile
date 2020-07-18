ODIR = obj
TSRC = src/Translators
SATSRC = src/SAT
SATHDIR = $(SATSRC)/headers
CFLAGS = -std=c++17 -Wall -O2
ZCDIR = src/zChaff/zchaff64
ZCHAFF = $(ZCDIR)/zchaff

all: zchaff sat-sud sud-sat sat_solver

sat-sud: $(TSRC)/retrieve_sudoku_from_SAT.cpp   
	@g++ $(CFLAGS) $< -o $@

sud-sat: $(TSRC)/reduce_sudoku_to_SAT.cpp 
	@g++ $(CFLAGS) $< -o $@

zchaff: $(ZCHAFF)
	@cp $< zchaff

sat_solver: $(ODIR)/sat_solver.o $(ODIR)/main_solver.o
	@g++ $(CFLAGS) $^ -o $@

$(ODIR)/main_solver.o: $(SATSRC)/main_solver.cpp objdir
	@g++ $(CFLAGS) -I $(SATHDIR) -c $< -o $@

$(ODIR)/sat_solver.o: $(SATSRC)/sat_solver.cpp objdir
	@g++ $(CFLAGS) -I $(SATHDIR) -c $< -o $@

objdir: 
	@mkdir -p obj

$(ZCHAFF):
	@(cd $(ZCDIR) && $(MAKE))

clean:
	if test -d "$(ODIR)"; then rm -rd $(ODIR); fi
	if test -f "sat_solver"; then rm sat_solver; fi
	if test -f "zchaff"; then rm zchaff; fi
	if test -f "sud-sat"; then rm sud-sat; fi
	if test -f "sat-sud"; then rm sat-sud; fi
	if test -d "log"; then rm -r log; fi
