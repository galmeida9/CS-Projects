%option c++ prefix="og_scanner_" outfile="og_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "og_parser.tab.h"

inline char hex2char(std::string s) {
    s.erase(0, 1); // erase '\' from string
    return (char) stoi(s, nullptr, 16);
}

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT
%%
    {set_debug(0);}

    /* Comments */

"//".*      ;

"/*"                    yy_push_state(X_COMMENT);
<X_COMMENT>"/*"         yy_push_state(X_COMMENT);
<X_COMMENT>"*/"         yy_pop_state();
<X_COMMENT>.|\n         ;

    /* Keywords */

"auto"      return tAUTO;
"int"       return tINT;
"real"      return tREAL;
"string"    return tSTR;
"ptr"       return tPTR;
"public"    return tPUBLIC;
"require"   return tREQUIRE;
"sizeof"    return tSIZEOF;
"input"     return tINPUT;
"nullptr"   return tNULLPTR;
"procedure" return tPROC;
"break"     return tBREAK;
"continue"  return tCONT;
"return"    return tRETURN;
"if"        return tIF; 
"then"      return tTHEN;
"elif"      return tELIF; 
"else"      return tELSE; 
"for"       return tFOR;
"do"        return tDO;
"write"     return tWRITE;
"writeln"   return tWRITELN;

    /* Identifiers */

[A-Za-z][A-Za-z0-9_]*   yylval.s = new std::string(yytext); return tIDENTIFIER;

    /* Strings */

\"                              yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"                    yy_pop_state(); return tSTRING;
<X_STRING>\\\"                  *yylval.s += yytext + 1;
<X_STRING>\\n                   *yylval.s += "\n";
<X_STRING>\\t|\\r               ;
<X_STRING>\\[0-9a-fA-F]{1,2}    *yylval.s += hex2char(yytext);
<X_STRING>.                     *yylval.s += yytext;

    /* Integers */

[0-9]+                  { 
                            try { yylval.i = std::stoi(yytext, nullptr, 10); return tINTEGER; } 
                            catch (const std::invalid_argument& e) { yyerror(e.what()); } 
                            catch (const std::out_of_range& e) { yyerror(e.what()); }
                        }
0x[0-9a-fA-F]+          {   
                            try { yylval.i = std::stoi(yytext, nullptr, 16); return tINTEGER; } 
                            catch (const std::invalid_argument& e) { yyerror(e.what()); } 
                            catch (const std::out_of_range& e) { yyerror(e.what()); }
                        }

    /* Reals */

[0-9]*\.[0-9]+([Ee][-+]?[0-9]+)?    {
                                        try { yylval.d = std::stod(yytext); return tDOUBLE; } 
                                        catch (const std::invalid_argument& e) { yyerror(e.what()); } 
                                        catch (const std::out_of_range& e) { yyerror(e.what()); }
                                    }
[0-9]+\.[0-9]*([Ee][-+]?[0-9]+)?    {
                                        try { yylval.d = std::stod(yytext); return tDOUBLE; } 
                                        catch (const std::invalid_argument& e) { yyerror(e.what()); } 
                                        catch (const std::out_of_range& e) { yyerror(e.what()); }
                                    }
[0-9]+([Ee][-+]?[0-9]+)             {
                                        try { yylval.d = std::stod(yytext); return tDOUBLE; } 
                                        catch (const std::invalid_argument& e) { yyerror(e.what()); } 
                                        catch (const std::out_of_range& e) { yyerror(e.what()); }
                                    }

    /* Operators and other characters */
    
[-()<>=+*\/%,;{}~\.\[\]@\?]        return *yytext;

"||"        return tOR; 
"&&"        return tAND;
">="        return tGE; 
"<="        return tLE; 
"=="        return tEQ; 
"!="        return tNE; 

    /* Whitespaces  */

[ \t\n]+    ;

    /* Unknown characters */

.           yyerror("Unknown character");

%%
