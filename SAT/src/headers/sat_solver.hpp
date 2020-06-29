#include <vector>
#include <stack>
#include <utility>
#include <queue>

typedef struct Decision {

    int var;
    bool value;
    bool tried_both;
    std::vector<int> implications;

    Decision(int var, bool value) : var(var), value(value), tried_both(false){}

} Decision;

typedef std::pair<int, bool> Assignment;

class SATSolver{

    private:

        typedef struct Clause {

            int idx;
            int size;
            std::vector<int> vars;
            // watched literals
            int fwatch, swatch;
            Clause() : fwatch(0), swatch(1) {}
        } Clause;
        
        std::stack<Decision> decisions; 
        std::vector<Clause*> clauses;
        std::vector<bool> decided;
        std::vector<bool> value;
        std::vector<std::vector<int>> watched; 
        int current_var;
        int NVar;
        int NClauses;
        bool SAT;

    public:

        bool IsSAT();

        void Initialize();

        bool SolveExpression();

        void PrintResult();

        void ReadExpression();

        bool RemoveSingleVars();

        bool Decide();

        void DeduceImplications(std::queue<Assignment>& implications, int literal);

        bool PropagateDecision();

        bool ChangeDecision();
        
        std::vector<std::pair<int, bool>> DeduceImplications(int var);

        Assignment GetImplication(Clause *clause);

        int ReplaceWatchedLiteral(Clause *clause, int var);

        inline int mapIdx(int var) { return var + NVar; }
};  