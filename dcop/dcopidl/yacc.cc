
/*  A Bison parser, made from yacc.yy
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	T_CHARACTER_LITERAL	257
#define	T_DOUBLE_LITERAL	258
#define	T_IDENTIFIER	259
#define	T_INTEGER_LITERAL	260
#define	T_STRING_LITERAL	261
#define	T_INCLUDE	262
#define	T_CLASS	263
#define	T_STRUCT	264
#define	T_LEFT_CURLY_BRACKET	265
#define	T_LEFT_PARANTHESIS	266
#define	T_RIGHT_CURLY_BRACKET	267
#define	T_RIGHT_PARANTHESIS	268
#define	T_COLON	269
#define	T_SEMICOLON	270
#define	T_PUBLIC	271
#define	T_PROTECTED	272
#define	T_TRIPE_DOT	273
#define	T_PRIVATE	274
#define	T_VIRTUAL	275
#define	T_CONST	276
#define	T_INLINE	277
#define	T_FRIEND	278
#define	T_RETURN	279
#define	T_SIGNAL	280
#define	T_SLOT	281
#define	T_TYPEDEF	282
#define	T_PLUS	283
#define	T_MINUS	284
#define	T_COMMA	285
#define	T_ASTERISK	286
#define	T_TILDE	287
#define	T_LESS	288
#define	T_GREATER	289
#define	T_AMPERSAND	290
#define	T_EXTERN	291
#define	T_ACCESS	292
#define	T_ENUM	293
#define	T_NAMESPACE	294
#define	T_UNKNOWN	295
#define	T_TRIPLE_DOT	296
#define	T_TRUE	297
#define	T_FALSE	298
#define	T_STATIC	299
#define	T_EQUAL	300
#define	T_SCOPE	301
#define	T_NULL	302
#define	T_INT	303
#define	T_ARRAY_OPEN	304
#define	T_ARRAY_CLOSE	305
#define	T_CHAR	306
#define	T_DCOP	307
#define	T_DCOP_AREA	308
#define	T_SIGNED	309
#define	T_UNSIGNED	310
#define	T_LONG	311
#define	T_SHORT	312
#define	T_FUNOPERATOR	313
#define	T_MISCOPERATOR	314

#line 1 "yacc.yy"

/*****************************************************************
Copyright (c) 1999 Torben Weis <weis@kde.org>
Copyright (c) 2000 Matthias Ettrich <ettrich@kde.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <qstring.h>

#define AMP_ENTITY "&amp;"
#define YYERROR_VERBOSE

extern int yylex();

// extern QString idl_lexFile;
extern int idl_line_no;
extern int function_mode;

static int dcop_area = 0;

void dcopidlInitFlex( const char *_code );

void yyerror( const char *s )
{
	qDebug( "In line %i : %s", idl_line_no, s );
        exit(1);
	//   theParser->parse_error( idl_lexFile, s, idl_line_no );
}


#line 54 "yacc.yy"
typedef union
{
  long   _int;
  QString        *_str;
  unsigned short          _char;
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



#define	YYFINAL		298
#define	YYFLAG		-32768
#define	YYNTBASE	61

#define YYTRANSLATE(x) ((unsigned)(x) <= 314 ? yytranslate[x] : 109)

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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     5,     8,    13,    14,    16,    17,    24,    28,
    32,    39,    46,    51,    59,    68,    71,    73,    75,    77,
    80,    81,    83,    85,    87,    89,    91,    93,    95,    96,
   100,   103,   106,   108,   112,   114,   118,   120,   123,   127,
   130,   132,   134,   137,   140,   143,   146,   149,   152,   158,
   163,   171,   178,   185,   189,   191,   195,   197,   199,   201,
   204,   206,   208,   210,   214,   222,   232,   239,   240,   242,
   244,   246,   249,   252,   256,   259,   262,   266,   269,   271,
   273,   275,   277,   280,   283,   286,   288,   289,   291,   295,
   297,   299,   302,   305,   310,   317,   321,   323,   327,   331,
   334,   337,   339,   342,   346,   348,   352,   355,   357,   358,
   361,   367,   369,   371,   373,   375,   380,   381,   383,   385,
   387,   389,   391,   398,   406,   408,   410,   414,   419,   421,
   425,   428,   434,   438,   444,   452,   459,   463,   465,   467,
   471,   476,   479,   480,   482,   485,   486,   489,   493,   498
};

static const short yyrhs[] = {    62,
    64,    61,     0,     0,     8,    62,     0,    37,    11,    61,
    13,     0,     0,    53,     0,     0,     9,    71,    75,    63,
    76,    16,     0,     9,    71,    16,     0,    10,    71,    16,
     0,    10,    71,    11,    61,    13,    16,     0,    40,     5,
    11,    61,    13,    16,     0,    28,    89,    71,    16,     0,
    28,    10,    11,    65,    13,    71,    16,     0,    28,    10,
    71,    11,    65,    13,    71,    16,     0,    23,   101,     0,
   101,     0,   108,     0,    77,     0,   108,    65,     0,     0,
    43,     0,    44,     0,    20,     0,    18,     0,    17,     0,
    26,     0,    27,     0,     0,    67,    68,    15,     0,    68,
    15,     0,    54,    15,     0,     5,     0,     5,    47,    71,
     0,    71,     0,    94,    17,    72,     0,    72,     0,    73,
    11,     0,    73,    31,    74,     0,    15,    74,     0,    11,
     0,    13,     0,    82,    76,     0,   101,    76,     0,    77,
    76,     0,    70,    76,     0,    69,    76,     0,   108,    76,
     0,    24,     9,    71,    16,    76,     0,    24,    71,    16,
    76,     0,    39,     5,    11,    78,    13,     5,    16,     0,
    39,     5,    11,    78,    13,    16,     0,    39,    11,    78,
    13,     5,    16,     0,    79,    31,    78,     0,    79,     0,
     5,    46,    81,     0,     5,     0,     3,     0,     6,     0,
    30,     6,     0,    48,     0,    71,     0,    80,     0,    80,
    29,    80,     0,    28,    71,    34,    90,    35,    71,    16,
     0,    28,    71,    34,    90,    35,    47,     5,    71,    16,
     0,    10,    71,    11,    61,    13,    16,     0,     0,    22,
     0,    55,     0,    56,     0,    55,    58,     0,    55,    57,
     0,    55,    58,    49,     0,    56,    58,     0,    56,    57,
     0,    56,    58,    49,     0,    56,    49,     0,    49,     0,
    57,     0,    58,     0,    52,     0,    55,    52,     0,    56,
    52,     0,    32,    85,     0,    32,     0,     0,    91,     0,
    86,    31,    91,     0,    84,     0,    71,     0,    10,    71,
     0,     9,    71,     0,    71,    34,    88,    35,     0,    71,
    34,    88,    35,    47,    71,     0,    87,    31,    88,     0,
    87,     0,    22,    87,    85,     0,    22,    87,    36,     0,
    22,    87,     0,    87,    36,     0,    87,     0,    87,    85,
     0,    89,    31,    90,     0,    89,     0,    89,    71,    92,
     0,    89,    92,     0,    42,     0,     0,    46,    93,     0,
    46,    12,    89,    14,    93,     0,     7,     0,    81,     0,
     4,     0,    66,     0,    71,    12,    86,    14,     0,     0,
    21,     0,    60,     0,    35,     0,    34,     0,    46,     0,
    89,    71,    12,    86,    14,    83,     0,    89,    59,    95,
    12,    86,    14,    83,     0,    80,     0,    97,     0,    97,
    31,    98,     0,     5,    12,    98,    14,     0,    99,     0,
    99,    31,   100,     0,    96,   103,     0,    21,    96,    46,
    48,   103,     0,    21,    96,   103,     0,    71,    12,    86,
    14,   103,     0,    71,    12,    86,    14,    15,   100,   103,
     0,    94,    33,    71,    12,    14,   103,     0,    45,    96,
   103,     0,    11,     0,    16,     0,   102,   104,    13,     0,
   102,   104,    13,    16,     0,   105,   104,     0,     0,    16,
     0,    31,   107,     0,     0,     5,   106,     0,    89,   107,
    16,     0,    45,    89,     5,    16,     0,    89,     5,    50,
    81,    51,    16,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   145,   148,   151,   155,   158,   164,   165,   168,   173,   176,
   179,   182,   185,   188,   191,   194,   197,   200,   203,   208,
   209,   211,   211,   213,   213,   213,   215,   215,   215,   218,
   222,   228,   234,   237,   244,   252,   256,   263,   267,   275,
   279,   286,   290,   294,   298,   302,   306,   310,   314,   318,
   324,   325,   326,   329,   330,   333,   334,   337,   338,   339,
   340,   341,   344,   345,   348,   358,   363,   368,   372,   379,
   380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
   390,   391,   392,   393,   396,   397,   400,   404,   405,   412,
   413,   414,   415,   416,   422,   431,   435,   440,   445,   452,
   457,   462,   467,   475,   479,   485,   494,   502,   510,   513,
   516,   522,   525,   528,   531,   534,   540,   541,   544,   544,
   544,   544,   547,   572,   579,   582,   583,   586,   589,   590,
   593,   597,   601,   605,   611,   617,   623,   630,   636,   637,
   638,   641,   642,   645,   648,   649,   651,   654,   655,   656
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","T_CHARACTER_LITERAL",
"T_DOUBLE_LITERAL","T_IDENTIFIER","T_INTEGER_LITERAL","T_STRING_LITERAL","T_INCLUDE",
"T_CLASS","T_STRUCT","T_LEFT_CURLY_BRACKET","T_LEFT_PARANTHESIS","T_RIGHT_CURLY_BRACKET",
"T_RIGHT_PARANTHESIS","T_COLON","T_SEMICOLON","T_PUBLIC","T_PROTECTED","T_TRIPE_DOT",
"T_PRIVATE","T_VIRTUAL","T_CONST","T_INLINE","T_FRIEND","T_RETURN","T_SIGNAL",
"T_SLOT","T_TYPEDEF","T_PLUS","T_MINUS","T_COMMA","T_ASTERISK","T_TILDE","T_LESS",
"T_GREATER","T_AMPERSAND","T_EXTERN","T_ACCESS","T_ENUM","T_NAMESPACE","T_UNKNOWN",
"T_TRIPLE_DOT","T_TRUE","T_FALSE","T_STATIC","T_EQUAL","T_SCOPE","T_NULL","T_INT",
"T_ARRAY_OPEN","T_ARRAY_CLOSE","T_CHAR","T_DCOP","T_DCOP_AREA","T_SIGNED","T_UNSIGNED",
"T_LONG","T_SHORT","T_FUNOPERATOR","T_MISCOPERATOR","main","includes","dcoptag",
"declaration","member_list","bool_value","nodcop_area","sigslot","nodcop_area_begin",
"dcop_area_begin","Identifier","super_class_name","super_class","super_classes",
"class_header","body","enum","enum_list","enum_item","number","int_expression",
"typedef","const_qualifier","int_type","asterisks","params","type_name","type_name_list",
"type","type_list","param","default","default_value","virtual_qualifier","operator",
"function_header","argument","arguments","init_item","init_list","function",
"function_begin","function_body","function_lines","function_line","Identifier_list_rest",
"Identifier_list","member", NULL
};
#endif

static const short yyr1[] = {     0,
    61,    61,    62,    62,    62,    63,    63,    64,    64,    64,
    64,    64,    64,    64,    64,    64,    64,    64,    64,    65,
    65,    66,    66,    67,    67,    67,    68,    68,    68,    69,
    69,    70,    71,    71,    72,    73,    73,    74,    74,    75,
    75,    76,    76,    76,    76,    76,    76,    76,    76,    76,
    77,    77,    77,    78,    78,    79,    79,    80,    80,    80,
    80,    80,    81,    81,    82,    82,    82,    83,    83,    84,
    84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
    84,    84,    84,    84,    85,    85,    86,    86,    86,    87,
    87,    87,    87,    87,    87,    88,    88,    89,    89,    89,
    89,    89,    89,    90,    90,    91,    91,    91,    92,    92,
    92,    93,    93,    93,    93,    93,    94,    94,    95,    95,
    95,    95,    96,    96,    97,    98,    98,    99,   100,   100,
   101,   101,   101,   101,   101,   101,   101,   102,   103,   103,
   103,   104,   104,   105,   106,   106,   107,   108,   108,   108
};

static const short yyr2[] = {     0,
     3,     0,     2,     4,     0,     1,     0,     6,     3,     3,
     6,     6,     4,     7,     8,     2,     1,     1,     1,     2,
     0,     1,     1,     1,     1,     1,     1,     1,     0,     3,
     2,     2,     1,     3,     1,     3,     1,     2,     3,     2,
     1,     1,     2,     2,     2,     2,     2,     2,     5,     4,
     7,     6,     6,     3,     1,     3,     1,     1,     1,     2,
     1,     1,     1,     3,     7,     9,     6,     0,     1,     1,
     1,     2,     2,     3,     2,     2,     3,     2,     1,     1,
     1,     1,     2,     2,     2,     1,     0,     1,     3,     1,
     1,     2,     2,     4,     6,     3,     1,     3,     3,     2,
     2,     1,     2,     3,     1,     3,     2,     1,     0,     2,
     5,     1,     1,     1,     1,     4,     0,     1,     1,     1,
     1,     1,     6,     7,     1,     1,     3,     4,     1,     3,
     2,     5,     3,     5,     7,     6,     3,     1,     1,     3,
     4,     2,     0,     1,     2,     0,     2,     3,     4,     6
};

static const short yydefact[] = {     5,
     5,     0,   117,     3,     5,    33,     0,     0,   118,     0,
   117,     0,     0,     0,     0,    79,    82,    70,    71,    80,
    81,     5,    91,    19,    90,   102,     0,     0,     0,    17,
    18,     0,     0,    93,    92,     0,     0,    91,     0,     0,
   100,     0,    16,     0,     0,     0,     0,     0,     0,     0,
    83,    73,    72,    78,    84,    76,    75,     1,    87,     0,
    86,   101,   103,    33,     0,     0,     0,     0,   138,   139,
   143,   131,     4,    34,    41,   117,     9,     7,     5,    10,
    93,    92,     0,   133,    99,    98,    21,    92,     0,     0,
    57,     0,    55,     5,    33,   137,    74,    77,   108,     0,
   109,    88,    97,     0,    85,     0,     0,   147,   121,   120,
   122,   119,     0,    87,   148,     0,   144,     0,   143,   118,
    35,    37,     0,    40,     0,     6,    29,     0,     0,     0,
     0,     0,    21,    21,    13,     0,     0,     0,     0,     0,
   149,     0,     0,     0,   109,   107,     0,    94,   146,   145,
    58,    59,     0,    61,    62,    63,     0,    87,     0,     0,
   140,   142,    38,   117,     0,     0,    42,    26,    25,    24,
     0,    27,    28,     0,     0,    29,     0,    29,    29,     0,
    29,    29,    29,    29,     0,   132,     0,     0,   146,    20,
     0,     0,    56,     0,    54,     0,     0,   134,    89,   114,
   112,     0,    22,    23,   115,    62,   113,   110,   106,    96,
     0,    60,     0,     0,     0,    68,     0,   141,    39,    36,
    92,     0,     0,     0,    32,     0,    31,    47,    46,     8,
    45,    43,    44,    48,    11,     0,     0,     0,     0,    52,
    53,    12,     0,   129,     0,     0,    87,    95,    64,   150,
    68,    69,   123,   136,     5,     0,    29,     0,    30,    14,
     0,    51,     0,     0,   135,     0,     0,   124,     0,    29,
    50,   105,     0,    15,   125,   126,     0,   130,   111,   116,
     0,    49,     0,     0,     0,   128,    67,   104,     0,     0,
   127,     0,    65,     0,    66,     0,     0,     0
};

static const short yydefgoto[] = {    32,
     3,   127,    22,   131,   205,   176,   177,   178,   179,    38,
   122,   123,   124,    78,   180,   181,    92,    93,   156,   207,
   182,   253,    25,    63,   100,    26,   104,    27,   273,   102,
   146,   208,    28,   113,    29,   276,   277,   244,   245,   183,
    71,    72,   118,   119,   108,    67,   184
};

static const short yypact[] = {    29,
    86,    21,   281,-32768,    92,    25,    58,    58,    59,   319,
   137,   147,   132,   105,    59,-32768,-32768,   159,   157,-32768,
-32768,    89,   126,-32768,-32768,     3,    11,    97,    96,-32768,
-32768,   152,    58,   207,   222,    58,    58,   153,    15,   102,
   195,    59,-32768,   186,    58,   179,   202,   209,    16,    96,
-32768,-32768,   164,-32768,-32768,-32768,   201,-32768,    35,   319,
   212,-32768,-32768,    12,    85,   239,   237,    58,-32768,-32768,
   241,-32768,-32768,-32768,-32768,   130,-32768,   206,    92,-32768,
-32768,-32768,   214,-32768,-32768,-32768,   256,   244,   253,   202,
   224,   259,   248,    92,    38,-32768,-32768,-32768,-32768,    17,
    14,-32768,   249,   250,-32768,   278,    73,-32768,-32768,-32768,
-32768,-32768,   272,    35,-32768,   276,-32768,   279,   241,-32768,
-32768,-32768,    27,-32768,   277,-32768,   219,   280,    96,    59,
   282,   291,   256,   256,-32768,   284,    73,   294,   202,   287,
-32768,   210,    35,   337,   260,-32768,   319,   263,   285,-32768,
-32768,-32768,   301,-32768,-32768,   286,   266,    35,   122,   304,
   303,-32768,-32768,   130,    58,    58,-32768,-32768,-32768,-32768,
   225,-32768,-32768,    58,   307,   174,   310,   219,   219,   311,
   219,   219,   219,   219,   315,-32768,   327,    58,    94,-32768,
   321,   172,-32768,   329,-32768,   330,   342,-32768,-32768,-32768,
-32768,    59,-32768,-32768,-32768,   323,-32768,-32768,-32768,-32768,
    58,-32768,    73,   332,   135,   328,    96,-32768,-32768,-32768,
   345,    58,   341,   324,-32768,   344,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   346,   347,    58,   349,-32768,
-32768,-32768,   352,   335,    96,   355,    35,-32768,-32768,-32768,
   328,-32768,-32768,-32768,    92,   354,   219,    59,-32768,-32768,
   356,-32768,    73,   342,-32768,   348,   136,-32768,   360,   219,
-32768,   351,   325,-32768,-32768,   353,   365,-32768,-32768,-32768,
   367,-32768,    59,    20,    73,-32768,-32768,-32768,   381,   371,
-32768,    58,-32768,   372,-32768,   361,   389,-32768
};

static const short yypgoto[] = {     1,
   392,-32768,-32768,   115,-32768,-32768,   218,-32768,-32768,    -3,
   230,-32768,   226,-32768,  -169,   394,   -68,-32768,  -202,   -81,
-32768,   148,-32768,   -14,  -108,    -7,   251,    40,   117,   258,
   257,   138,   -58,-32768,   113,-32768,   118,-32768,   141,   161,
-32768,   -33,   288,-32768,-32768,   300,    -1
};


#define	YYLAST		407


static const short yytable[] = {    23,
   296,    31,    41,    34,    35,   159,    84,    23,   228,   229,
   249,   231,   232,   233,   234,    64,    96,   125,     6,     6,
    95,   136,    58,    66,     6,   157,    86,  -146,    -2,    74,
   142,     5,    81,    82,    61,    66,     1,   163,    62,     6,
    88,    89,   106,    36,    37,    66,   105,   143,    39,   215,
    39,    45,   103,   141,    49,   193,    10,   164,    33,   144,
   275,   107,     6,     6,   116,     2,   289,    36,    37,    65,
   195,    33,   121,    65,    65,   151,    99,     6,   152,   128,
    10,    39,   275,    16,    33,   133,    17,   271,    -2,    18,
    19,    20,    21,     1,   140,   186,     1,   145,   101,     1,
   282,    -2,   153,   155,    -2,   125,    69,    16,   198,    48,
    17,    70,    69,    18,    19,    20,    21,    70,   109,   110,
   154,    40,     2,    23,   106,     2,   132,    50,     2,    68,
   111,   133,   133,   155,     6,   216,    46,    59,   267,   103,
   206,     6,    47,   107,   112,    36,    37,    83,   251,   280,
   120,     6,   143,   101,    50,    36,    44,     9,    10,    60,
   121,   121,   221,    30,    73,   143,   143,   223,    10,   187,
   224,    43,   132,   132,    23,    23,   239,    23,    23,    23,
    23,    42,   101,   254,   237,    16,    60,   240,    17,    90,
     6,    18,    19,    20,    21,    16,    87,   101,    17,   172,
   173,    18,    19,    20,    21,    54,    91,   248,    55,   155,
    51,   265,    97,    56,    57,    52,    53,    75,   256,    94,
    69,    76,    77,     6,   197,    70,    61,    36,   166,     6,
    85,   167,    79,   222,   261,   168,   169,    80,   170,     9,
    10,   246,   171,    61,   172,   173,   174,   190,   191,    98,
   114,  -117,   115,    23,   134,   269,   117,    13,   126,   155,
     6,   129,   206,    15,    36,    37,    23,    16,   135,   137,
    17,   138,   175,    18,    19,    20,    21,    10,   139,   147,
   290,   155,   149,   158,   148,     6,   101,   160,   294,     7,
     8,   161,   185,   165,   188,   189,   192,   272,   194,   196,
   130,     9,    10,    11,    16,   144,   212,    17,    12,   211,
    18,    19,    20,    21,   213,   106,   214,   217,   218,    13,
    14,   225,   272,     6,   227,    15,   230,    36,    37,    16,
   235,   236,    17,   238,   247,    18,    19,    20,    21,   151,
   200,     6,   152,   201,   241,   242,   243,   250,   202,   252,
   151,   200,     6,   152,   201,   255,   257,   258,   259,   284,
   297,   141,   260,   263,   262,   264,   153,    16,   266,   270,
    17,   274,   281,    18,    19,    20,    21,   153,   286,   203,
   204,   283,   287,   285,   154,   292,   293,   295,   298,   219,
   203,   204,     4,   226,   220,   154,    24,   210,   268,   288,
   199,   209,   291,   279,   278,   150,   162
};

static const short yycheck[] = {     3,
     0,     3,    10,     7,     8,   114,    40,    11,   178,   179,
   213,   181,   182,   183,   184,     5,    50,    76,     5,     5,
     5,    90,    22,    27,     5,   107,    41,    16,     0,    33,
    14,    11,    36,    37,    32,    39,     8,    11,    36,     5,
    44,    45,    31,     9,    10,    49,    61,    31,     9,   158,
    11,    12,    60,    16,    15,   137,    22,    31,    47,    46,
   263,    50,     5,     5,    68,    37,    47,     9,    10,    59,
   139,    47,    76,    59,    59,     3,    42,     5,     6,    79,
    22,    42,   285,    49,    47,    87,    52,   257,     0,    55,
    56,    57,    58,     8,    94,   129,     8,   101,    59,     8,
   270,    13,    30,   107,    13,   164,    11,    49,   142,     5,
    52,    16,    11,    55,    56,    57,    58,    16,    34,    35,
    48,     9,    37,   127,    31,    37,    87,    15,    37,    33,
    46,   133,   134,   137,     5,    14,     5,    12,   247,   147,
   144,     5,    11,    50,    60,     9,    10,    46,    14,    14,
    21,     5,    31,   114,    42,     9,    10,    21,    22,    34,
   164,   165,   166,     3,    13,    31,    31,   171,    22,   130,
   174,    11,   133,   134,   178,   179,     5,   181,   182,   183,
   184,    45,   143,   217,   188,    49,    34,    16,    52,    11,
     5,    55,    56,    57,    58,    49,    11,   158,    52,    26,
    27,    55,    56,    57,    58,    49,     5,   211,    52,   213,
    52,   245,    49,    57,    58,    57,    58,    11,   222,    11,
    11,    15,    16,     5,    15,    16,    32,     9,    10,     5,
    36,    13,    11,     9,   238,    17,    18,    16,    20,    21,
    22,   202,    24,    32,    26,    27,    28,   133,   134,    49,
    12,    33,    16,   257,    11,   255,    16,    39,    53,   263,
     5,    48,   266,    45,     9,    10,   270,    49,    16,    46,
    52,    13,    54,    55,    56,    57,    58,    22,    31,    31,
   284,   285,     5,    12,    35,     5,   247,    12,   292,     9,
    10,    13,    13,    17,    13,     5,    13,   258,     5,    13,
    45,    21,    22,    23,    49,    46,     6,    52,    28,    47,
    55,    56,    57,    58,    29,    31,    51,    14,    16,    39,
    40,    15,   283,     5,    15,    45,    16,     9,    10,    49,
    16,     5,    52,    13,    12,    55,    56,    57,    58,     3,
     4,     5,     6,     7,    16,    16,     5,    16,    12,    22,
     3,     4,     5,     6,     7,    11,    16,    34,    15,    35,
     0,    16,    16,    12,    16,    31,    30,    49,    14,    16,
    52,    16,    13,    55,    56,    57,    58,    30,    14,    43,
    44,    31,    16,    31,    48,     5,    16,    16,     0,   164,
    43,    44,     1,   176,   165,    48,     3,   147,   251,   283,
   143,   145,   285,   266,   264,   106,   119
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "//usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
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

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

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
     unsigned int count;
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
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "//usr/lib/bison.simple"

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

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

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
  int yyfree_stacks = 0;

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
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
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
#line 146 "yacc.yy"
{
	  ;
    break;}
case 3:
#line 152 "yacc.yy"
{
		printf("<INCLUDE>%s</INCLUDE>\n", yyvsp[-1]._str->latin1() );
	  ;
    break;}
case 4:
#line 156 "yacc.yy"
{
	  ;
    break;}
case 5:
#line 159 "yacc.yy"
{
          ;
    break;}
case 6:
#line 164 "yacc.yy"
{ yyval._int = 1; ;
    break;}
case 7:
#line 165 "yacc.yy"
{ yyval._int = 0; ;
    break;}
case 8:
#line 169 "yacc.yy"
{
	 	if (yyvsp[-2]._int)
			  printf("<CLASS>\n    <NAME>%s</NAME>\n%s%s</CLASS>\n", yyvsp[-4]._str->latin1(), yyvsp[-3]._str->latin1(), yyvsp[-1]._str->latin1() );
	  ;
    break;}
case 9:
#line 174 "yacc.yy"
{
	  ;
    break;}
case 10:
#line 177 "yacc.yy"
{
	  ;
    break;}
case 11:
#line 180 "yacc.yy"
{
	  ;
    break;}
case 12:
#line 183 "yacc.yy"
{
	  ;
    break;}
case 13:
#line 186 "yacc.yy"
{
	  ;
    break;}
case 14:
#line 189 "yacc.yy"
{
	  ;
    break;}
case 15:
#line 192 "yacc.yy"
{
	  ;
    break;}
case 16:
#line 195 "yacc.yy"
{
	  ;
    break;}
case 17:
#line 198 "yacc.yy"
{
	  ;
    break;}
case 18:
#line 201 "yacc.yy"
{
	  ;
    break;}
case 19:
#line 204 "yacc.yy"
{
	  ;
    break;}
case 30:
#line 219 "yacc.yy"
{
	  dcop_area = 0;
	;
    break;}
case 31:
#line 223 "yacc.yy"
{
	  dcop_area = 0;
	;
    break;}
case 32:
#line 229 "yacc.yy"
{
	  dcop_area = 1;
	;
    break;}
case 33:
#line 234 "yacc.yy"
{
	  yyval._str = yyvsp[0]._str;
	;
    break;}
case 34:
#line 237 "yacc.yy"
{
	   QString* tmp = new QString( "%1::%2" );
           *tmp = tmp->arg(*(yyvsp[-2]._str)).arg(*(yyvsp[0]._str));
           yyval._str = tmp;
	;
    break;}
case 35:
#line 245 "yacc.yy"
{
		QString* tmp = new QString( "    <SUPER>%1</SUPER>\n" );
		*tmp = tmp->arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	  ;
    break;}
case 36:
#line 253 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 37:
#line 257 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 38:
#line 264 "yacc.yy"
{
		yyval._str = yyvsp[-1]._str;
	  ;
    break;}
case 39:
#line 268 "yacc.yy"
{
		/* $$ = $1; */
		yyval._str = new QString( *(yyvsp[-2]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 40:
#line 276 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 41:
#line 280 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 42:
#line 287 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 43:
#line 291 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-1]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 44:
#line 295 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-1]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 45:
#line 299 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 46:
#line 303 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 47:
#line 307 "yacc.yy"
{	
	        yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 48:
#line 311 "yacc.yy"
{
 	        yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 49:
#line 315 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 50:
#line 319 "yacc.yy"
{
		yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 56:
#line 333 "yacc.yy"
{;
    break;}
case 57:
#line 334 "yacc.yy"
{;
    break;}
case 58:
#line 337 "yacc.yy"
{;
    break;}
case 59:
#line 338 "yacc.yy"
{;
    break;}
case 60:
#line 339 "yacc.yy"
{;
    break;}
case 61:
#line 340 "yacc.yy"
{;
    break;}
case 62:
#line 341 "yacc.yy"
{;
    break;}
case 63:
#line 344 "yacc.yy"
{;
    break;}
case 64:
#line 345 "yacc.yy"
{;
    break;}
case 65:
#line 349 "yacc.yy"
{
		if (dcop_area) {
 		  QString* tmp = new QString("<TYPEDEF name=\"%1\" template=\"%2\"><PARAM %3</TYPEDEF>\n");
		  *tmp = tmp->arg( *(yyvsp[-1]._str) ).arg( *(yyvsp[-5]._str) ).arg( *(yyvsp[-3]._str) );
		  yyval._str = tmp;
		} else {
		  yyval._str = new QString("");
		}
	  ;
    break;}
case 66:
#line 359 "yacc.yy"
{
		if (dcop_area)
		  yyerror("scoped template typedefs are not supported in dcop areas!");
	  ;
    break;}
case 67:
#line 364 "yacc.yy"
{
	  ;
    break;}
case 68:
#line 369 "yacc.yy"
{
		yyval._int = 0;
	  ;
    break;}
case 69:
#line 373 "yacc.yy"
{
		yyval._int = 1;
	  ;
    break;}
case 70:
#line 379 "yacc.yy"
{ yyval._str = new QString("signed int"); ;
    break;}
case 71:
#line 380 "yacc.yy"
{ yyval._str = new QString("unsigned int"); ;
    break;}
case 72:
#line 381 "yacc.yy"
{ yyval._str = new QString("signed short int"); ;
    break;}
case 73:
#line 382 "yacc.yy"
{ yyval._str = new QString("signed long int"); ;
    break;}
case 74:
#line 383 "yacc.yy"
{ yyval._str = new QString("signed short int"); ;
    break;}
case 75:
#line 384 "yacc.yy"
{ yyval._str = new QString("unsigned short int"); ;
    break;}
case 76:
#line 385 "yacc.yy"
{ yyval._str = new QString("unsigned long int"); ;
    break;}
case 77:
#line 386 "yacc.yy"
{ yyval._str = new QString("unsigned short int"); ;
    break;}
case 78:
#line 387 "yacc.yy"
{ yyval._str = new QString("unsigned int"); ;
    break;}
case 79:
#line 388 "yacc.yy"
{ yyval._str = new QString("int"); ;
    break;}
case 80:
#line 389 "yacc.yy"
{ yyval._str = new QString("long int"); ;
    break;}
case 81:
#line 390 "yacc.yy"
{ yyval._str = new QString("short int"); ;
    break;}
case 82:
#line 391 "yacc.yy"
{ yyval._str = new QString("char"); ;
    break;}
case 83:
#line 392 "yacc.yy"
{ yyval._str = new QString("signed char"); ;
    break;}
case 84:
#line 393 "yacc.yy"
{ yyval._str = new QString("unsigned char"); ;
    break;}
case 87:
#line 401 "yacc.yy"
{
		yyval._str = new QString( "" );
	  ;
    break;}
case 89:
#line 406 "yacc.yy"
{
		yyval._str = new QString( *(yyvsp[-2]._str) + *(yyvsp[0]._str) );
	  ;
    break;}
case 91:
#line 413 "yacc.yy"
{ yyval._str = yyvsp[0]._str; ;
    break;}
case 92:
#line 414 "yacc.yy"
{ yyval._str = yyvsp[0]._str; ;
    break;}
case 93:
#line 415 "yacc.yy"
{ yyval._str = yyvsp[0]._str; ;
    break;}
case 94:
#line 416 "yacc.yy"
{
		QString *tmp = new QString("%1&lt;%2&gt;");
		*tmp = tmp->arg(*(yyvsp[-3]._str));
		*tmp = tmp->arg(*(yyvsp[-1]._str));
		yyval._str = tmp;
	 ;
    break;}
case 95:
#line 422 "yacc.yy"
{
		QString *tmp = new QString("%1&lt;%2&gt;::%3");
		*tmp = tmp->arg(*(yyvsp[-5]._str));
		*tmp = tmp->arg(*(yyvsp[-3]._str));
		*tmp = tmp->arg(*(yyvsp[0]._str));
		yyval._str = tmp;
	 ;
    break;}
case 96:
#line 432 "yacc.yy"
{
	    yyval._str = new QString(*(yyvsp[-2]._str) + "," + *(yyvsp[0]._str));
	  ;
    break;}
case 97:
#line 436 "yacc.yy"
{
 	    yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 98:
#line 441 "yacc.yy"
{
	    if (dcop_area)
	      yyerror("in dcop areas are no pointers allowed");
	  ;
    break;}
case 99:
#line 445 "yacc.yy"
{
	     if (dcop_area) {
	  	QString* tmp = new QString("<TYPE  qleft=\"const\" qright=\"" AMP_ENTITY "\">%1</TYPE>");
		*tmp = tmp->arg( *(yyvsp[-1]._str) );
		yyval._str = tmp;
	     }
	  ;
    break;}
case 100:
#line 452 "yacc.yy"
{
		QString* tmp = new QString("<TYPE>%1</TYPE>");
		*tmp = tmp->arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	;
    break;}
case 101:
#line 457 "yacc.yy"
{
	     if (dcop_area)
		yyerror("in dcop areas are only const references allowed!");
	  ;
    break;}
case 102:
#line 462 "yacc.yy"
{
		QString* tmp = new QString("<TYPE>%1</TYPE>");
		*tmp = tmp->arg( *(yyvsp[0]._str) );
		yyval._str = tmp;
	;
    break;}
case 103:
#line 468 "yacc.yy"
{
	    if (dcop_area)
	      yyerror("in dcop areas are no pointers allowed");
	  ;
    break;}
case 104:
#line 476 "yacc.yy"
{
	    yyval._str = new QString(*(yyvsp[-2]._str) + "," + *(yyvsp[0]._str));
	  ;
    break;}
case 105:
#line 480 "yacc.yy"
{
 	    yyval._str = yyvsp[0]._str;
	  ;
    break;}
case 106:
#line 486 "yacc.yy"
{
		if (dcop_area) {
		   QString* tmp = new QString("\n        <ARG>%1<NAME>%2</NAME></ARG>");
  		   *tmp = tmp->arg( *(yyvsp[-2]._str) );
  		   *tmp = tmp->arg( *(yyvsp[-1]._str) );
		   yyval._str = tmp;		
		} else yyval._str = new QString();
	  ;
    break;}
case 107:
#line 495 "yacc.yy"
{
		if (dcop_area) {
		   QString* tmp = new QString("\n        <ARG>%1</ARG>");
  		   *tmp = tmp->arg( *(yyvsp[-1]._str) );
		   yyval._str = tmp;		
		} else yyval._str = new QString();
	  ;
    break;}
case 108:
#line 503 "yacc.yy"
{
		if (dcop_area)
			yyerror("variable arguments not supported in dcop area.");
		yyval._str = new QString("");
	  ;
    break;}
case 109:
#line 511 "yacc.yy"
{
	  ;
    break;}
case 110:
#line 514 "yacc.yy"
{
	  ;
    break;}
case 111:
#line 517 "yacc.yy"
{
	  ;
    break;}
case 112:
#line 523 "yacc.yy"
{
          ;
    break;}
case 113:
#line 526 "yacc.yy"
{
          ;
    break;}
case 114:
#line 529 "yacc.yy"
{
          ;
    break;}
case 115:
#line 532 "yacc.yy"
{
          ;
    break;}
case 116:
#line 535 "yacc.yy"
{
          ;
    break;}
case 117:
#line 540 "yacc.yy"
{ yyval._int = 0; ;
    break;}
case 118:
#line 541 "yacc.yy"
{ yyval._int = 1; ;
    break;}
case 123:
#line 548 "yacc.yy"
{
	     if (dcop_area) {
		QString* tmp = 0;
		if ( yyvsp[0]._int )
			tmp = new QString(
				"    <FUNC qual=\"const\">\n"
				"        %2\n"
				"        <NAME>%1</NAME>"
				"%3\n"
				"     </FUNC>\n");
		else
			tmp = new QString(
				"    <FUNC>\n"
				"        %2\n"
				"        <NAME>%1</NAME>"
				"%3\n"
				"     </FUNC>\n");
		*tmp = tmp->arg( *(yyvsp[-4]._str) );
		*tmp = tmp->arg( *(yyvsp[-5]._str) );
		*tmp = tmp->arg( *(yyvsp[-2]._str) );
		yyval._str = tmp;
   	     } else
	        yyval._str = new QString("");
	  ;
    break;}
case 124:
#line 573 "yacc.yy"
{
	     if (dcop_area)
		yyerror("operators aren't allowed in dcop areas!");
	     yyval._str = new QString("");
	  ;
    break;}
case 125:
#line 579 "yacc.yy"
{;
    break;}
case 126:
#line 582 "yacc.yy"
{;
    break;}
case 127:
#line 583 "yacc.yy"
{;
    break;}
case 128:
#line 586 "yacc.yy"
{;
    break;}
case 129:
#line 589 "yacc.yy"
{;
    break;}
case 130:
#line 590 "yacc.yy"
{;
    break;}
case 131:
#line 594 "yacc.yy"
{
	        yyval._str = yyvsp[-1]._str;
	  ;
    break;}
case 132:
#line 598 "yacc.yy"
{
		yyval._str = yyvsp[-3]._str;
	  ;
    break;}
case 133:
#line 602 "yacc.yy"
{
		yyval._str = yyvsp[-1]._str;
	  ;
    break;}
case 134:
#line 606 "yacc.yy"
{
	      /* The constructor */
	      assert(!dcop_area);
              yyval._str = new QString("");
	  ;
    break;}
case 135:
#line 612 "yacc.yy"
{
	      /* The constructor */
	      assert(!dcop_area);
              yyval._str = new QString("");
	  ;
    break;}
case 136:
#line 618 "yacc.yy"
{
	      /* The destructor */
  	      assert(!dcop_area);
              yyval._str = new QString("");
	  ;
    break;}
case 137:
#line 624 "yacc.yy"
{
		if (dcop_area)
		  yyerror("static is not allowed in dcop area!");
		yyval._str = new QString();
	  ;
    break;}
case 138:
#line 631 "yacc.yy"
{
		function_mode = 1;
	;
    break;}
case 142:
#line 641 "yacc.yy"
{;
    break;}
case 143:
#line 642 "yacc.yy"
{;
    break;}
case 147:
#line 651 "yacc.yy"
{;
    break;}
case 148:
#line 654 "yacc.yy"
{;
    break;}
case 149:
#line 655 "yacc.yy"
{;
    break;}
case 150:
#line 656 "yacc.yy"
{;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "//usr/lib/bison.simple"

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

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 658 "yacc.yy"


void dcopidlParse( const char *_code )
{
    dcopidlInitFlex( _code );
    yyparse();
}
