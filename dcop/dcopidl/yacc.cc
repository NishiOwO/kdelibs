
/*  A Bison parser, made from yacc.yy
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	T_CHARACTER_LITERAL	258
#define	T_DOUBLE_LITERAL	259
#define	T_IDENTIFIER	260
#define	T_INTEGER_LITERAL	261
#define	T_STRING_LITERAL	262
#define	T_INCLUDE	263
#define	T_CLASS	264
#define	T_LEFT_CURLY_BRACKET	265
#define	T_LEFT_PARANTHESIS	266
#define	T_RIGHT_CURLY_BRACKET	267
#define	T_RIGHT_PARANTHESIS	268
#define	T_COLON	269
#define	T_SEMICOLON	270
#define	T_PUBLIC	271
#define	T_VIRTUAL	272
#define	T_CONST	273
#define	T_TYPEDEF	274
#define	T_COMMA	275
#define	T_LESS	276
#define	T_GREATER	277
#define	T_AMPERSAND	278
#define	T_ENUM	279
#define	T_UNKNOWN	280
#define	T_EQUAL	281
#define	T_SCOPE	282
#define	T_NULL	283
#define	T_DCOP	284
#define	T_DCOP_AREA	285

#line 1 "yacc.yy"


#include <stdlib.h>
#include <stdio.h>

#include <qstring.h>

extern int yylex();

// extern QString idl_lexFile;
extern int idl_line_no;

void kidlInitFlex( const char *_code );

void yyerror( const char *s )
{
	qDebug( "In line %i : %s", idl_line_no, s );
        exit(1);
	//   theParser->parse_error( idl_lexFile, s, idl_line_no );
}


#line 25 "yacc.yy"
typedef union
{
  long   _int;
  QString        *_str;
  ushort          _char;
  double _float;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		101
#define	YYFLAG		-32768
#define	YYNTBASE	31

#define YYTRANSLATE(x) ((unsigned)(x) <= 285 ? yytranslate[x] : 46)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     6,     8,    11,    14,    18,    23,    27,
    29,    36,    41,    43,    46,    49,    53,    61,    63,    67,
    68,    70,    72,    76,    78,    82,    87,    94,    95,    97,
   101,   107,   111,   112,   115,   118,   121,   124,   127,   132,
   143,   153,   162
};

static const short yyrhs[] = {    -1,
    32,    35,     0,    35,     0,     8,     0,     8,    32,     0,
     5,    10,     0,     5,    20,    33,     0,    14,    17,    16,
    33,     0,    14,    16,    33,     0,    10,     0,     9,     5,
    34,    29,    36,    15,     0,     9,     5,    15,    35,     0,
    12,     0,    37,    36,     0,    45,    36,     0,    30,    14,
    36,     0,    19,     5,    21,    38,    22,     5,    15,     0,
     5,     0,     5,    20,    38,     0,     0,    18,     0,     5,
     0,     5,    20,    38,     0,     5,     0,    18,     5,    23,
     0,     5,    21,    40,    22,     0,    18,     5,    21,    40,
    22,    23,     0,     0,    43,     0,    42,    20,    43,     0,
    18,     5,    23,     5,    44,     0,     5,     5,    44,     0,
     0,    26,     7,     0,    26,     3,     0,    26,     4,     0,
    26,     6,     0,    26,    28,     0,    26,     5,    27,     5,
     0,    17,    41,     5,    11,    42,    13,    39,    26,    28,
    15,     0,    41,     5,    11,    42,    13,    39,    26,    28,
    15,     0,    17,    41,     5,    11,    42,    13,    39,    15,
     0,    41,     5,    11,    42,    13,    39,    15,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    78,    81,    84,    90,    94,   101,   107,   116,   120,   126,
   134,   138,   144,   148,   152,   156,   163,   172,   178,   187,
   191,   198,   202,   209,   215,   221,   227,   236,   240,   244,
   251,   257,   266,   269,   272,   275,   278,   281,   284,   290,
   296,   302,   308
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","T_CHARACTER_LITERAL",
"T_DOUBLE_LITERAL","T_IDENTIFIER","T_INTEGER_LITERAL","T_STRING_LITERAL","T_INCLUDE",
"T_CLASS","T_LEFT_CURLY_BRACKET","T_LEFT_PARANTHESIS","T_RIGHT_CURLY_BRACKET",
"T_RIGHT_PARANTHESIS","T_COLON","T_SEMICOLON","T_PUBLIC","T_VIRTUAL","T_CONST",
"T_TYPEDEF","T_COMMA","T_LESS","T_GREATER","T_AMPERSAND","T_ENUM","T_UNKNOWN",
"T_EQUAL","T_SCOPE","T_NULL","T_DCOP","T_DCOP_AREA","main","includes","super_classes",
"class_header","class","body","typedef","typedef_params","qualifier","return_params",
"return","params","param","default","function", NULL
};
#endif

static const short yyr1[] = {     0,
    31,    31,    31,    32,    32,    33,    33,    34,    34,    34,
    35,    35,    36,    36,    36,    36,    37,    38,    38,    39,
    39,    40,    40,    41,    41,    41,    41,    42,    42,    42,
    43,    43,    44,    44,    44,    44,    44,    44,    44,    45,
    45,    45,    45
};

static const short yyr2[] = {     0,
     0,     2,     1,     1,     2,     2,     3,     4,     3,     1,
     6,     4,     1,     2,     2,     3,     7,     1,     3,     0,
     1,     1,     3,     1,     3,     4,     6,     0,     1,     3,
     5,     3,     0,     2,     2,     2,     2,     2,     4,    10,
     9,     8,     7
};

static const short yydefact[] = {     1,
     4,     0,     0,     3,     5,     0,     2,    10,     0,     0,
     0,     0,     0,    12,     0,     0,     9,     0,    24,    13,
     0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
     8,     0,     0,     0,     0,     0,    11,    14,     0,    15,
     7,    22,     0,     0,     0,    25,     0,    16,    28,     0,
    26,    28,     0,    18,     0,     0,     0,     0,    29,    23,
     0,     0,     0,     0,    33,     0,    20,     0,    20,    27,
    19,     0,     0,    32,     0,    21,     0,    30,     0,    17,
    35,    36,     0,    37,    34,    38,    33,    43,     0,    42,
     0,     0,    31,     0,     0,    39,    41,    40,     0,     0,
     0
};

static const short yydefgoto[] = {    99,
     3,    17,    11,     4,    25,    26,    55,    77,    43,    27,
    58,    59,    74,    28
};

static const short yypact[] = {     7,
    10,    28,    18,-32768,-32768,    31,-32768,-32768,    35,    18,
   -19,    33,    23,-32768,    -5,    14,-32768,    33,    27,-32768,
     1,    38,    45,    39,    40,    -5,    49,    -5,-32768,    33,
-32768,    51,    52,    26,    37,    -5,-32768,-32768,    48,-32768,
-32768,    41,    42,    54,    51,-32768,    55,-32768,     3,    55,
-32768,     3,    44,    43,    46,    57,    62,    16,-32768,-32768,
    22,    47,    55,    64,    50,    56,    53,     3,    53,-32768,
-32768,    58,    -2,-32768,    67,-32768,     2,-32768,     5,-32768,
-32768,-32768,    59,-32768,-32768,-32768,    50,-32768,    60,-32768,
    61,    69,-32768,    63,    65,-32768,-32768,-32768,    75,    77,
-32768
};

static const short yypgoto[] = {-32768,
    80,    -7,-32768,    34,     4,-32768,   -41,    13,    66,    70,
    32,    15,     0,-32768
};


#define	YYLAST		111


static const short yytable[] = {    19,
    81,    82,    83,    84,    85,    19,    20,    56,    60,    15,
    31,    21,    22,    23,     1,     2,    88,     1,    22,    90,
    57,    71,    41,    29,    24,    86,     2,    89,    67,    38,
    91,    40,     6,    30,    69,    68,     7,    16,    18,    48,
     8,    68,    34,    14,     9,    10,    45,    32,    46,    35,
    12,    13,    36,    39,    37,    42,    44,    47,    49,    54,
    50,    65,    63,    51,    52,    62,    66,    64,    72,    70,
    76,    87,    80,    96,   100,    73,   101,    97,    75,    98,
     5,    79,    78,    61,     0,    92,    93,    94,    95,     0,
    33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    53
};

static const short yycheck[] = {     5,
     3,     4,     5,     6,     7,     5,    12,     5,    50,    29,
    18,    17,    18,    19,     8,     9,    15,     8,    18,    15,
    18,    63,    30,    10,    30,    28,     9,    26,    13,    26,
    26,    28,     5,    20,    13,    20,     3,     5,    16,    36,
    10,    20,     5,    10,    14,    15,    21,    21,    23,     5,
    16,    17,    14,     5,    15,     5,     5,    21,    11,     5,
    20,     5,    20,    22,    11,    22,     5,    22,     5,    23,
    18,     5,    15,     5,     0,    26,     0,    15,    23,    15,
     1,    69,    68,    52,    -1,    27,    87,    28,    28,    -1,
    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    45
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/share/bison/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 79 "yacc.yy"
{
	  ;
    break;}
case 2:
#line 82 "yacc.yy"
{
	  ;
    break;}
case 3:
#line 85 "yacc.yy"
{
	  ;
    break;}
case 4:
#line 91 "yacc.yy"
{
		printf("<INCLUDE file=\"%s\"/>\n", yyvsp[0]._str->latin1() );
	  ;
    break;}
case 5:
#line 95 "yacc.yy"
{
		printf("<INCLUDE file=\"%s\"/>\n", yyvsp[-1]._str->latin1() );
	  ;
    break;}
case 6:
#line 102 "yacc.yy"
{
		QString* tmp = new QString( "<SUPER name=\"%1\"/>" );
		*tmp = tmp->arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 7:
#line 108 "yacc.yy"
{
		QString* tmp = new QString( "<SUPER name=\"%1\"/>%2" );
		*tmp = tmp->arg( *(yyvsp[-2]._str) ).arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 8:
#line 117 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 9:
#line 121 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
		qDebug("You must inherit virtual");
		exit(1);
	  ;
    break;}
case 10:
#line 127 "yacc.yy"
{
		qDebug("You must inherit from DCOPObject");
		exit(1);
	  ;
    break;}
case 11:
#line 135 "yacc.yy"
{
		printf("<CLASS name=\"%s\">\n%s\n%s</CLASS>\n", yyvsp[-4]._str->latin1(), yyvsp[-3]._str->latin1(), yyvsp[-1]._str->latin1() );
	  ;
    break;}
case 12:
#line 139 "yacc.yy"
{
	  ;
    break;}
case 13:
#line 145 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 14:
#line 149 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-1]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 15:
#line 153 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-1]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 16:
#line 157 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 17:
#line 164 "yacc.yy"
{
		QString* tmp = new QString("<TYPEDEF name=\"%1\" template=\"%2\">%3</TYPEDEF>\n");
		*tmp = tmp->arg( *(yyvsp[-1]._str) ).arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-3]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 18:
#line 173 "yacc.yy"
{
		QString* tmp = new QString("<PARAM type=\"%1\"/>");
		*tmp = tmp->arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 19:
#line 179 "yacc.yy"
{
		QString* tmp = new QString("<PARAM type=\"%1\"/>%2");
		*tmp = tmp->arg( *(yyvsp[-2]._str) ).arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 20:
#line 188 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 21:
#line 192 "yacc.yy"
{
		yyval._str = new QString( "const" );
	  ;
    break;}
case 22:
#line 199 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 23:
#line 203 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-2]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 24:
#line 210 "yacc.yy"
{
		QString* tmp = new QString("<RET type=\"%1\"/>");
		*tmp = tmp->arg( *(yyvsp[0]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 25:
#line 216 "yacc.yy"
{
		QString* tmp = new QString("<RET type=\"%1\" qleft=\"const\" qright=\"&\"/>");
		*tmp = tmp->arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 26:
#line 222 "yacc.yy"
{
		QString* tmp = new QString("<RET type=\"%1<%2>\"/>");
		*tmp = tmp->arg( *(yyvsp[-3]._str) ).arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 27:
#line 228 "yacc.yy"
{
		QString* tmp = new QString("<RET type=\"%1<%2>\" qleft=\"const\" qright=\"&\"/>");
		*tmp = tmp->arg( *(yyvsp[-4]._str) ).arg( *(yyvsp[-2]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 28:
#line 237 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 29:
#line 241 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 30:
#line 245 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-2]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 31:
#line 252 "yacc.yy"
{
		QString* tmp = new QString("<ARG name=\"%1\" type=\"%2\" qleft=\"const\" qright=\"&\"/>");
		*tmp = tmp->arg( *(yyvsp[-1]._str) ).arg( *(yyvsp[-3]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 32:
#line 258 "yacc.yy"
{
		QString* tmp = new QString("<ARG name=\"%1\" type=\"%2\"/>");
		*tmp = tmp->arg( *(yyvsp[-1]._str) ).arg( *(yyvsp[-2]._str) );
		yyval._str = tmp;		
	  ;
    break;}
case 33:
#line 267 "yacc.yy"
{
	  ;
    break;}
case 34:
#line 270 "yacc.yy"
{
	  ;
    break;}
case 35:
#line 273 "yacc.yy"
{
	  ;
    break;}
case 36:
#line 276 "yacc.yy"
{
	  ;
    break;}
case 37:
#line 279 "yacc.yy"
{
	  ;
    break;}
case 38:
#line 282 "yacc.yy"
{
	  ;
    break;}
case 39:
#line 285 "yacc.yy"
{
	  ;
    break;}
case 40:
#line 291 "yacc.yy"
{
		QString* tmp = new QString("<FUNC name=\"%1\" qual=\"%4\">%2%3</FUNC>\n");
		*tmp = tmp->arg( *(yyvsp[-7]._str) ).arg( *(yyvsp[-8]._str) ).arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-3]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 41:
#line 297 "yacc.yy"
{
		QString* tmp = new QString("<FUNC name=\"%1\" qual=\"%4\">%2%3</FUNC>\n");
		*tmp = tmp->arg( *(yyvsp[-7]._str) ).arg( *(yyvsp[-8]._str) ).arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-3]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 42:
#line 303 "yacc.yy"
{
		QString* tmp = new QString("<FUNC name=\"%1\" qual=\"%4\">%2%3</FUNC>\n");
		*tmp = tmp->arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-6]._str) ).arg( *(yyvsp[-3]._str) ).arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 43:
#line 309 "yacc.yy"
{
		QString* tmp = new QString("<FUNC name=\"%1\" qual=\"%4\">%2%3</FUNC>\n");
		*tmp = tmp->arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-6]._str) ).arg( *(yyvsp[-3]._str) ).arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;
	  ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/share/bison/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 316 "yacc.yy"


void kidlParse( const char *_code )
{
    printf("<!DOCTYPE DCOP-IDL><DCOP-IDL>\n");
    kidlInitFlex( _code );
    yyparse();
    printf("</DCOP-IDL>\n");
}
