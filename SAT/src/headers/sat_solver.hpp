#include <vector>
#include <stack>
#include <utility>
#include <queue>

typedef struct Decision {

    int var;                        // decided variable
    bool value;                     // assigned value
    bool tried_both;                // decision already was tried with true and false
    std::vector<int> implications;  // variables implicated from decision

    // constructor
    Decision(int var, bool value) : var(var), value(value), tried_both(false){}

} Decision;

typedef std::pair<int, bool> Assignment;        // assignment of a value to a variable

class SATSolver{

    private:

        typedef struct Clause {

            int idx;                                    // Index of clause
            int size;                                   // Number of literals in clause
            std::vector<int> vars;                      // literals list
            // watched literals
            int fwatch, swatch;                         // watchers
            Clause() : fwatch(0), swatch(1) {}          // Constructor
        } Clause;

        std::queue<Assignment> assignments;
        std::stack<Decision> decisions;              // stack of decisions made for backtraking
        std::vector<Clause*> clauses;                // list of clauses
        std::vector<bool> decided;                   // list to check if a variable value is set
        std::vector<bool> value;                     // value of a variable
        std::vector<std::vector<int>> watched;       // list of watched clauses for each variable
        int current_var;                             // variable which value is being decided
        int NVar;                                    // Number of variables in the expression
        int NClauses;                                // Number of clauses in the expression
        bool SAT;                                    // result of evaluation

    public:

        // function that will handle most of the work to check if an expression is satisfiable
        bool IsSAT();

        // function to initialize needed structures
        void Initialize();

        // function to start the process to solve the problem
        bool SolveExpression();

        // function that will print values stored
        void PrintResult();

        // functio to read an expression
        void ReadExpression();

        // remove clauses with single vars at the beginning of execution
        bool RemoveSingleVars();

        // functio to select the next variable to decide
        bool Decide();

        // function to check if any implication is derivated after last decision
        void DeduceImplications(int literal);

        // function to store decision made and check its implications
        bool PropagateDecision();

        // function to change a decision made to resolve a conflict
        bool ChangeDecision();
        
        // Get an implication from a clause
        Assignment GetImplication(Clause *clause);
        
        // function to replace current clause watched literals
        int ReplaceWatchedLiteral(Clause *clause, int var);

        // map literal to index
        inline int mapIdx(int var) { return var + NVar; }
};  