
#include "sol.h"
#include "soltoken.h"
#include "soltypes.h"
#include "sollist.h"
#include "solfunc.h"
#include "solop.h"

SolObject Object;
SolToken Token;
SolFunction Function;
SolList List;
SolNumber Number;
SolString String;
SolBoolean Boolean;

SolObject nil;

static inline void sol_runtime_init_operators();

void sol_runtime_init() {
    local_token_pool = calloc(1, sizeof(*local_token_pool));
    
    // initialize object types
    Object = malloc(sizeof(*Object));
    memcpy(Object, &DEFAULT_OBJECT, sizeof(*Object));
    sol_token_register("Object", Object);
    
    // create token as object, then switch to token to avoid early resolution when binding in pool
    Token = (SolToken) sol_obj_create_global(Object, TYPE_SOL_OBJ, &(struct sol_token_raw){ NULL }, sizeof(*Token), "Token");
    Token->super.type_id = TYPE_SOL_TOKEN;
    
    Function = (SolFunction) sol_obj_create_global(Object, TYPE_SOL_FUNC, &(struct sol_func_raw){ NULL, NULL, NULL }, sizeof(*Function), "Function");
    List = (SolList) sol_obj_create_global(Object, TYPE_SOL_LIST, &(struct sol_list_raw){ false, NULL, NULL, 0, NULL, 0 }, sizeof(*List), "List");
    Number = (SolNumber) sol_obj_create_global(Object, TYPE_SOL_DATATYPE, &(struct sol_num_raw){ DATA_TYPE_NUM, 0 }, sizeof(*Number), "Number");
    String = (SolString) sol_obj_create_global(Object, TYPE_SOL_DATATYPE, &(struct sol_string_raw){ DATA_TYPE_STR, NULL }, sizeof(*String), "String");
    Boolean = (SolBoolean) sol_obj_create_global(Object, TYPE_SOL_DATATYPE, &(struct sol_bool_raw){ DATA_TYPE_BOOL, 1 }, sizeof(*Boolean), "Boolean");
    
    nil = (SolObject) sol_list_create();
    sol_token_register("nil", nil);
    
    sol_runtime_init_operators();
}

#define REGISTER_OP(token, name) sol_token_register(#token, (SolObject) sol_operator_create(OP_ ## name))
static inline void sol_runtime_init_operators() {
    REGISTER_OP(+, ADD);
    REGISTER_OP(-, SUBTRACT);
    REGISTER_OP(*, MULTIPLY);
    REGISTER_OP(/, DIVIDE);
    REGISTER_OP(mod, MOD);
    REGISTER_OP(bind, BIND);
    REGISTER_OP(set, SET);
    REGISTER_OP(evaluate, EVALUATE);
    REGISTER_OP(^, LAMBDA);
    REGISTER_OP(print, PRINT);
    REGISTER_OP(not, NOT);
    REGISTER_OP(and, AND);
    REGISTER_OP(or, OR);
    REGISTER_OP(=, EQUALITY);
    REGISTER_OP(<, LESS_THAN);
    REGISTER_OP(>, GREATER_THAN);
    REGISTER_OP(<=, LESS_THAN_EQUALITY);
    REGISTER_OP(>=, GREATER_THAN_EQUALITY);
    REGISTER_OP(if, IF);
    REGISTER_OP(loop, LOOP);
}

void sol_runtime_destroy() {
}
