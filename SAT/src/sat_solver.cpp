#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include "sat_solver.hpp"

bool SatSolver::IsSAT() { 

    // First, we reduce every clause of size 1 and
    // implicate values from them. This is done so
    // the logic of algorithm below holds
    if (!RemoveSingleVars()) {

        // conflict found
        return false;
    }

    while (true) { 
        
        if (!Decide()) {            
            // no more vars to decide, 
            // then expresion is satisfiable
            return true;
        }

        while (!PropagateDecision()) {

            // conflict has been found, then we have to undo
            // lat decision that we made and try the other way

            if (!ChangeDecision()) {

                // no more decisions to change, still conflict
                // happened. Then, expresion is not satisfiable
                return false
            }
        }
    }    
}

bool SatSolver::RemoveSingleVars() {

    for(Clause &clause : clauses) {

        if (clause.size == 1) {

            int p = clause.variables[0];
            bool val = p > 0;

            if (decided[p] && value[p] != val) {

                // conflict found
                return false;
            }

            value[p] =  val;
            decided[p] = true;

            return true;
        
        }
    } 
}

bool SatSolver::Decide() {

    while (!cur_var < vars.size()) {
    
        // retrieve next element form queue of variables yet to decide
        int var = vars[currrent_var++]

        if (decided[var]) {
            // var value was previously implicated
            continue;
        }

        bool val = (init_count[0][var] < init_count[1][var]);
        // stacking the new decision
        Decision new_decision(var,val)
        decisions.push(new_decision);
        // storing the value
        decided[var] = true;
        value[var] = val;
        
    }

    // no more vars to decide
    return false;
}

void SatSolver::EliminateClauses(int var, std::vector<int> &deleted_clauses) {
    
    for(int idx : clauses_map[var]) {

        Clause &clause = clauses[idx];

        if(clause.active) {

            clause.active = false;
            deleted_clauses.push_back(idx)
        }
    }
}

std::vector<std::pair<int, bool>> SatSolver::DeduceImplications(int var) {

    std::vector<std::pair<int, bool>> implications;

    for(int idx : clauses_map[var]) {

        Clause &clause = clauses[idx];

        if (cluse.IsWatchedVar(var)) {
            if (!clause.ReplaceWatchedVar(var)) {
                std::pair<int, bool> imp = clause.GetImplication(var);
                implications.push_back(imp);
            }
        }

    }

    return implications;
    
}

inline bool SatSolver::CheckContradiction(std::pair<int, bool> &f, std::pair<int, bool> &s) {
    return f.first == s.first && f.second != s.second;
}

inline bool SatSolver::CheckContradiction(std::pair<int, bool> &imp) {
    return value[imp.first] != imp.second;
}

bool SatSolver::CheckForConflicts(sd::vector<std::pair<int, bool>> &implications) {

    std::sort(implications.begin(), implications.end());
    for(size_t i = 1; i < implications.size(); i++) {

        if (CheckContradiction(implications[i], implications[i-1]) 
        ||  CheckContradiction(implications[i])) {
            return false;
        }
    }

    return true;
}

bool SatSolver::PropagateDecision() {

    Decision & last_decision = stack.top();

    std::queue<std::pair<int, bool>> assignments;
    assignments.push( {last_dec.var, last_decision.value} );

    while (!assignments.empty()) {
        
        
        std::pair<int, bool> assignment = assignments.front();
        assignment.pop();
        
        // checking which state (negated or not) we are using
        // for deriving implications and eliminating clauses
        int t, f;

        if (assignment.value) {
            
            t = assignment.var; 
            f = -t;

        } else {

            f = assignment.var;
            t = -f;        
        } 

        // The state (negated or not) of var that holds
        // the value 'true' will be used for eliminating clauses
        EliminateClauses(t, last_decision.deleted_clauses);

        // The state (negated or not) of var that holds
        // the value 'false' will be used for deducing implications
        std::vector<std::pair<int,bool>> implications = DeduceImplications(f);

        // Check if there is a contradiction within the implications
        if (CheckForConflicts(implications)) {

            // if conflict is found, report failure
            return false;

        } else {
            
            // no conflict found, then we can store implications
            for(std::pair<int,bool> &imp : implications) { 

                // assigning the value to the variable
                value[imp.first] = imp.second;
                decided[imp.first] = true;
                // remebering the change we did for this state
                assignment.implications.push_back(imp);
                // adding it to the queue for propagating
                assignments.push(imp);

            }
        }
    }

    return true;

}

void SatSolver::InitWatchedLiterals (CLause &clause) {

    int fvar = -1, svar = -1;
    size_t idx = 0;

    for(; idx < clause.size; idx++) {

        if (!decided[clause.variables[idx]]) {

            if (fvar != -1) {
                svar = idx;
                break;
            } else {
                fvar = idx;
            }
        }
    }

    clause.fvar = fvar;
    clause.svar = svar;
}

bool SatSolver::ChangeDecision () {

    while (!decisions.empty()) {

        Decision &last_decision = decisions.top();

        // Discard all implications and restore
        // all clauses affected by this decision
        for(std::pair<int, bool>& imp : implications) {

            decided[imp.first] = false;
        }
        
        for(int idx : deleted_clauses) {
            // restaurar clausulas
        }    


        if (last_decision.tried_both) {

            decided[decision.var] = false;
            decisions.pop();
            current_var--;

        } else {
            
            value[desicion.var] ^= true;
            decision.tried_both = true;
            implications.clear();
            deleted_clauses.clear();
        }
    }

    return false;
}