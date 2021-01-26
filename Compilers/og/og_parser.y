%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                      i;	          /* integer value */
  double                   d;            /* real value */
  std::string              *s;	          /* symbol name or string literal */
  std::vector<std::string> *v;           /* vector */
  cdk::basic_node          *node;	      /* node pointer */
  cdk::sequence_node       *sequence;    /* sequences */
  cdk::expression_node     *expression;  /* expression nodes */
  cdk::lvalue_node         *lvalue;      /* left-values */
  cdk::basic_type          *type;        /* expression, function and variable type */
  og::block_node           *block;       /* block nodes */
} 

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tPUBLIC tREQUIRE tPRIVATE tSIZEOF tINPUT
%token <type> tAUTO tINT tREAL tSTR tPTR
%token tNULLPTR tPROC tBREAK tCONT tRETURN
%token tIF tTHEN tELIF tELSE tFOR tDO
%token tWRITE tWRITELN
%token tAND tOR

%nonassoc tIFX
%nonassoc tELIF
%nonassoc tELSE

%right '='
%left tOR
%left tAND
%left tGE tLE '>' '<'
%left tEQ tNE
%nonassoc tNEG
%left '+' '-'
%left '*' '/' '%'
%nonassoc '~'
%nonassoc '[' '@'

%type<i> public require
%type <node> stmt program var ivar tuplevar vardecl typevar iter cond elif decl idecl func proc
%type <sequence> stmts exprs decls idecls stmts vardecls forvars typevars
%type <expression> expr
%type <type> expr_type all_type auto
%type <lvalue> lval
%type <block> block
%type <s> strs id
%type <v> ids

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program   : decls         { compiler->ast($1); }
          |               { compiler->ast($$ = new cdk::sequence_node(LINE)); }
          ;

decls     : decl          { $$ = new cdk::sequence_node(LINE, $1); }
          | decls decl    { $$ = new cdk::sequence_node(LINE, $2 ,$1);}

decl      : var ';'       { $$ = $1;}
          | func          { $$ = $1;}
          | proc          { $$ = $1;}
          ;

idecls    : idecl         { $$ = new cdk::sequence_node(LINE, $1);}
          | idecls idecl  { $$ = new cdk::sequence_node(LINE, $2 ,$1);}
          ;

idecl     : ivar ';'      { $$ = $1;}
          ;
          
block     : '{' '}'                     { $$ = new og::block_node(LINE, nullptr, nullptr);}
          | '{' idecls stmts '}'        { $$ = new og::block_node(LINE, $2, $3);}
          | '{' idecls '}'              { $$ = new og::block_node(LINE, $2, nullptr);}
          | '{' stmts '}'               { $$ = new og::block_node(LINE, nullptr, $2);}
          ;

stmts     : stmt                { $$ = new cdk::sequence_node(LINE, $1); }
          | stmts stmt          { $$ = new cdk::sequence_node(LINE, $2, $1); }
          ;

stmt      : expr ';'           { $$ = new og::evaluation_node(LINE, $1);    }
          | tWRITE exprs ';'   { $$ = new og::print_node(LINE, $2, false);  }
          | tWRITELN exprs ';' { $$ = new og::print_node(LINE, $2, true);   }
          | tBREAK             { $$ = new og::break_node(LINE);             }
          | tRETURN exprs ';'  { $$ = new og::return_node(LINE, new og::tuple_node(LINE, $2, tPUBLIC)); }
          | tRETURN ';'        { $$ = new og::return_node(LINE);            }
          | tCONT              { $$ = new og::continue_node(LINE);          }
          | iter               { $$ = $1;                                   }
          | cond               { $$ = $1;                                   }
          | block              { $$ = $1;                                   }
          ;

func      : expr_type tIDENTIFIER '(' ')'                         { $$ = new og::function_declaration_node(LINE, $1, *$2, tPRIVATE); delete $2;       }
          | auto      tIDENTIFIER '(' ')'                         { $$ = new og::function_declaration_node(LINE, $1, *$2, tPRIVATE); delete $2;       }
          | expr_type tIDENTIFIER '(' vardecls ')'                { $$ = new og::function_declaration_node(LINE, $1, *$2, $4, tPRIVATE); delete $2;   }
          | auto      tIDENTIFIER '(' vardecls ')'                { $$ = new og::function_declaration_node(LINE, $1, *$2, $4, tPRIVATE); delete $2;   }
          | expr_type tIDENTIFIER '(' ')' block                   { $$ = new og::function_definition_node(LINE, $1, *$2, $5, tPRIVATE); delete $2;    }
          | auto      tIDENTIFIER '(' ')' block                   { $$ = new og::function_definition_node(LINE, $1, *$2, $5, tPRIVATE); delete $2;    }
          | expr_type tIDENTIFIER '(' vardecls ')' block          { $$ = new og::function_definition_node(LINE, $1, *$2, $4, $6, tPRIVATE); delete $2;}
          | auto      tIDENTIFIER '(' vardecls ')' block          { $$ = new og::function_definition_node(LINE, $1, *$2, $4, $6, tPRIVATE); delete $2;}
          | public expr_type tIDENTIFIER '(' ')'                  { $$ = new og::function_declaration_node(LINE, $2, *$3, $1); delete $3;             }
          | require expr_type tIDENTIFIER '(' ')'                 { $$ = new og::function_declaration_node(LINE, $2, *$3, $1); delete $3;             }
          | public auto      tIDENTIFIER '(' ')'                  { $$ = new og::function_declaration_node(LINE, $2, *$3, $1); delete $3;             }
          | require auto      tIDENTIFIER '(' ')'                 { $$ = new og::function_declaration_node(LINE, $2, *$3, $1); delete $3;             }
          | public expr_type tIDENTIFIER '(' vardecls ')'         { $$ = new og::function_declaration_node(LINE, $2, *$3, $5, $1); delete $3;         }
          | require expr_type tIDENTIFIER '(' vardecls ')'        { $$ = new og::function_declaration_node(LINE, $2, *$3, $5, $1); delete $3;         }
          | public auto      tIDENTIFIER '(' vardecls ')'         { $$ = new og::function_declaration_node(LINE, $2, *$3, $5, $1); delete $3;         }
          | require auto      tIDENTIFIER '(' vardecls ')'        { $$ = new og::function_declaration_node(LINE, $2, *$3, $5, $1); delete $3;         }
          | public expr_type tIDENTIFIER '(' ')' block            { $$ = new og::function_definition_node(LINE, $2, *$3, $6, $1); delete $3;          }
          | require expr_type tIDENTIFIER '(' ')' block           { $$ = new og::function_definition_node(LINE, $2, *$3, $6, $1); delete $3;          }
          | public auto      tIDENTIFIER '(' ')' block            { $$ = new og::function_definition_node(LINE, $2, *$3, $6, $1); delete $3;          }
          | require auto      tIDENTIFIER '(' ')' block           { $$ = new og::function_definition_node(LINE, $2, *$3, $6, $1); delete $3;          }
          | public expr_type tIDENTIFIER '(' vardecls ')' block   { $$ = new og::function_definition_node(LINE, $2, *$3, $5, $7, $1); delete $3;      }
          | require expr_type tIDENTIFIER '(' vardecls ')' block  { $$ = new og::function_definition_node(LINE, $2, *$3, $5, $7, $1); delete $3;      }
          | public auto      tIDENTIFIER '(' vardecls ')' block   { $$ = new og::function_definition_node(LINE, $2, *$3, $5, $7, $1); delete $3;      }
          | require auto      tIDENTIFIER '(' vardecls ')' block  { $$ = new og::function_definition_node(LINE, $2, *$3, $5, $7, $1); delete $3;      }
          ; 

proc      : tPROC tIDENTIFIER '(' ')'                         { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$2, tPRIVATE); delete $2;         }
          | tPROC tIDENTIFIER '(' vardecls ')'                { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$2, $4, tPRIVATE); delete $2;     }
          | tPROC tIDENTIFIER '(' ')' block                   { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$2, $5, tPRIVATE); delete $2;      }
          | tPROC tIDENTIFIER '(' vardecls ')' block          { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$2, $4, $6, tPRIVATE); delete $2;  }
          | public tPROC tIDENTIFIER '(' ')'                  { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $1); delete $3;               }
          | require tPROC tIDENTIFIER '(' ')'                 { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $1); delete $3;               }
          | public tPROC tIDENTIFIER '(' vardecls ')'         { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $5, $1); delete $3;           }
          | require tPROC tIDENTIFIER '(' vardecls ')'        { $$ = new og::function_declaration_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $5, $1); delete $3;           }
          | public tPROC tIDENTIFIER '(' ')' block            { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $6, $1); delete $3;            }
          | require tPROC tIDENTIFIER '(' ')' block           { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $6, $1); delete $3;            }
          | public tPROC tIDENTIFIER '(' vardecls ')' block   { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $5, $7, $1); delete $3;        }
          | require tPROC tIDENTIFIER '(' vardecls ')' block  { $$ = new og::function_definition_node(LINE, new cdk::primitive_type(0, cdk::TYPE_VOID), *$3, $5, $7, $1); delete $3;        }
          ;

cond      : tIF expr tTHEN stmt %prec tIFX              { $$ = new og::if_node(LINE, $2, $4); }
          | tIF expr tTHEN stmt elif %prec tELIF        { $$ = new og::if_else_node(LINE, $2, $4, $5); }
          ;

elif      : tELSE stmt                                  { $$ = $2; }
          | tELIF expr tTHEN stmt %prec tIFX            { $$ = new og::if_node(LINE, $2, $4);}
          | tELIF expr tTHEN stmt elif                  { $$ = new og::if_else_node(LINE, $2, $4, $5); }
          ;

iter      : tFOR ';' ';' tDO stmt                       { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $5);   }
          | tFOR forvars ';' ';' tDO stmt               { $$ = new og::for_node(LINE, $2, nullptr, nullptr, $6);        }
          | tFOR ';' exprs ';' tDO stmt                 { $$ = new og::for_node(LINE, nullptr, $3, nullptr, $6);        }
          | tFOR ';' ';' exprs tDO stmt                 { $$ = new og::for_node(LINE, nullptr, nullptr, $4, $6);        }
          | tFOR forvars ';' exprs ';' tDO stmt         { $$ = new og::for_node(LINE, $2, $4, nullptr, $7);             }
          | tFOR forvars ';' ';' exprs tDO stmt         { $$ = new og::for_node(LINE, $2, nullptr, $5, $7);             }
          | tFOR ';' exprs ';' exprs tDO stmt           { $$ = new og::for_node(LINE, nullptr, $3, $5, $7);             }
          | tFOR forvars ';' exprs ';' exprs tDO stmt   { $$ = new og::for_node(LINE, $2, $4, $6, $8);                  }
          | tFOR exprs ';' ';' tDO stmt                 { $$ = new og::for_node(LINE, $2, nullptr, nullptr, $6);        }
          | tFOR exprs ';' exprs ';' tDO stmt           { $$ = new og::for_node(LINE, $2, $4, nullptr, $7);             }
          | tFOR exprs ';' ';' exprs tDO stmt           { $$ = new og::for_node(LINE, $2, nullptr, $5, $7);             }
          | tFOR exprs ';' exprs ';' exprs tDO stmt     { $$ = new og::for_node(LINE, $2, $4, $6, $8);                  }
          ;

exprs     : expr              { $$ = new cdk::sequence_node(LINE, $1);     }
          | exprs ',' expr    { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

expr      : tINTEGER                  	{ $$ = new cdk::integer_node(LINE, $1); }
          | tDOUBLE                     { $$ = new cdk::double_node(LINE, $1);  }
          | strs                        { $$ = new cdk::string_node(LINE, $1);  }
          | '-' expr                    { $$ = new cdk::neg_node(LINE, $2);     }
          | '+' expr                    { $$ = new og::identity_node(LINE, $2); }
          | lval '?'                    { $$ = new og::address_of_node(LINE, $1);}
          | expr '+' expr               { $$ = new cdk::add_node(LINE, $1, $3); }
          | expr '-' expr               { $$ = new cdk::sub_node(LINE, $1, $3); }
          | expr '*' expr               { $$ = new cdk::mul_node(LINE, $1, $3); }
          | expr '/' expr               { $$ = new cdk::div_node(LINE, $1, $3); }
          | expr '%' expr               { $$ = new cdk::mod_node(LINE, $1, $3); }
          | expr '<' expr               { $$ = new cdk::lt_node(LINE, $1, $3);  }
          | expr '>' expr               { $$ = new cdk::gt_node(LINE, $1, $3);  }
          | expr tGE expr               { $$ = new cdk::ge_node(LINE, $1, $3);  }
          | expr tLE expr               { $$ = new cdk::le_node(LINE, $1, $3);  }
          | expr tNE expr               { $$ = new cdk::ne_node(LINE, $1, $3);  }
          | expr tEQ expr               { $$ = new cdk::eq_node(LINE, $1, $3);  }
          | expr tAND expr              { $$ = new cdk::and_node(LINE, $1, $3); }
          | expr tOR expr               { $$ = new cdk::or_node(LINE, $1, $3);  }
          | '~' expr                    { $$ = new cdk::not_node(LINE, $2);     }
          | '(' expr ')'                { $$ = $2;                              }
          | tSIZEOF '(' exprs ')'       { $$ = new og::sizeof_node(LINE, $3) ;  }
          | lval                        { $$ = new cdk::rvalue_node(LINE, $1);  }
          | lval '=' expr               { $$ = new cdk::assignment_node(LINE, $1, $3);    }
          | '[' expr ']'                { $$ = new og::stack_alloc_node(LINE, $2);        }
          | tIDENTIFIER '(' ')'         { $$ = new og::function_call_node(LINE, *$1); delete $1;      }
          | tIDENTIFIER '(' exprs ')'   { $$ = new og::function_call_node(LINE, *$1, $3); delete $1;  }
          | tNULLPTR                    { $$ = new og::nullptr_node(LINE);      }
          | tINPUT                      { $$ = new og::read_node(LINE);         }
          ;

vardecls  : vardecl                   { $$ = new cdk::sequence_node(LINE, $1);      }
          | vardecls ',' vardecl      { $$ = new cdk::sequence_node(LINE, $3, $1);  }
          ;

vardecl   : expr_type tIDENTIFIER     { $$ = new og::variable_declaration_node(LINE, tPRIVATE, $1, new std::vector<std::string>(1, *$2)); delete $2;       }
          ;

forvars   : typevars                  { $$ = $1; }
          | tuplevar                  { $$ = new cdk::sequence_node(LINE, $1); }
          ;

ivar      : typevar                   { $$ = $1;}
          | tuplevar                  { $$ = $1;}
          ;

typevars  : typevar                   { $$ = new cdk::sequence_node(LINE, $1);      }
          | typevars ',' typevar      { $$ = new cdk::sequence_node(LINE, $3, $1);  }
          ;

typevar   : vardecl                                   { $$ = $1;}
          | expr_type tIDENTIFIER '=' expr            { $$ = new og::variable_declaration_node(LINE, tPRIVATE, $1, new std::vector<std::string>(1, *$2), $4); delete $2;   }
          ;

tuplevar  : auto ids '=' exprs                       { $$ = new og::variable_declaration_node(LINE, tPRIVATE, $1, $2, new og::tuple_node(LINE, $4, tPRIVATE)); }
          ;

var       : ivar                                        { $$ = $1;}
          | public expr_type tIDENTIFIER                { $$ = new og::variable_declaration_node(LINE, $1, $2, new std::vector<std::string>(1, *$3)); delete $3;             }
          | require expr_type tIDENTIFIER               { $$ = new og::variable_declaration_node(LINE, $1, $2, new std::vector<std::string>(1, *$3)); delete $3;             }
          | public expr_type tIDENTIFIER '=' expr       { $$ = new og::variable_declaration_node(LINE, $1, $2, new std::vector<std::string>(1, *$3), $5); delete $3;         }
          | require expr_type tIDENTIFIER '=' expr      { $$ = new og::variable_declaration_node(LINE, $1, $2, new std::vector<std::string>(1, *$3), $5); delete $3;         }
          | public auto ids '=' exprs                   { $$ = new og::variable_declaration_node(LINE, tPUBLIC, $2, $3, new og::tuple_node(LINE, $5, tPUBLIC));              }
          ;

ids       : id          { $$ = new std::vector<std::string>(1, *$1); delete $1;  }
          | ids ',' id  { $1->push_back(*$3); delete $3; $$ = $1;}
          ;

id        : tIDENTIFIER { $$ = $1; }
          ;

lval      : id                     { $$ = new cdk::variable_node(LINE, $1); }
          | expr '[' expr ']'      { $$ = new og::index_node(LINE, $1, $3); }
          | expr '@' tINTEGER      { $$ = new og::tuple_index_node(LINE, $1, $3); }
          ;
          

expr_type : tINT                        { $$ = new cdk::primitive_type(4, cdk::typename_type::TYPE_INT);        }
          | tREAL                       { $$ = new cdk::primitive_type(8, cdk::typename_type::TYPE_DOUBLE);     }
          | tSTR                        { $$ = new cdk::primitive_type(4, cdk::typename_type::TYPE_STRING);     }
          | tPTR '<' all_type '>'       { $$ = new cdk::reference_type(4, std::shared_ptr<cdk::basic_type>($3));}
          ;

all_type  : auto                        { $$ = $1; }
          | expr_type                   { $$ = $1; }
          ;

auto      : tAUTO                       { $$ = new cdk::primitive_type(0, cdk::TYPE_UNSPEC); }
          ;

public    : tPUBLIC   { $$ = tPUBLIC; }
          ;

require   : tREQUIRE   { $$ = tREQUIRE; }
          ;

strs      : tSTRING       { $$ = $1; }
          | strs tSTRING  { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }

%%


// Functions argumments should not use tuples