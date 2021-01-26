#define tINTEGER 257
#define tDOUBLE 258
#define tIDENTIFIER 259
#define tSTRING 260
#define tPUBLIC 261
#define tREQUIRE 262
#define tPRIVATE 263
#define tSIZEOF 264
#define tINPUT 265
#define tAUTO 266
#define tINT 267
#define tREAL 268
#define tSTR 269
#define tPTR 270
#define tNULLPTR 271
#define tPROC 272
#define tBREAK 273
#define tCONT 274
#define tRETURN 275
#define tIF 276
#define tTHEN 277
#define tELIF 278
#define tELSE 279
#define tFOR 280
#define tDO 281
#define tWRITE 282
#define tWRITELN 283
#define tAND 284
#define tOR 285
#define tIFX 286
#define tGE 287
#define tLE 288
#define tEQ 289
#define tNE 290
#define tNEG 291
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
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
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
