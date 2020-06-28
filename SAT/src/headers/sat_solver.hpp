

    typedef struct Decision{

        int var;
        bool value;
        bool tried_both;
        std::vector<int> implications;
        std::vector<int> deleted_clauses; 

        Decision(int var, bool value) : var(var), value(value), tried_both(false){}

    };

    typedef struct Clause {

        int size;
        bool active;
        std::vector<int> variables;
        
        // watched literals implementation
        int fvar, svar;
        size_t idx;

        bool IsWatchedVar(int var) {
            return var == fvar || var == svar;
        }

        std::pair<int, bool> GetImplication(int var) {
            
            std::pair<int, bool> imp;
            imp.first = (var == fvar ? svar : fvar);
            imp.second = imp.first > 0;
            return imp;
        }

    };

    class SATSolver{

        private:
        
            std::vector<int> vars;            // 0..N
            std::vector<bool> value;          // 0..2*N
            std::stack<Decision> decisions;         


        public:

            SATSolver(){
                
            }

            bool IsSAT();

            bool RemoveSingleVars();

            bool Decide();

            bool PropagateDecision();

            bool SatSolver::ChangeDecision();
            
            std::vector<std::pair<int, bool>> DeduceImplications(int var);
            
            void EliminateClauses(int var, std::vector<int> &deleted_clauses);

            inline bool CheckContradiction(std::pair<int, bool> &f, std::pair<int, bool> &s);

            inline bool CheckContradiction(std::pair<int, bool> &imp);

            bool CheckForConflicts(sd::vector<std::pair<int, bool>> &implications);

    }