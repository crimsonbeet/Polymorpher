
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */


/* Line 189 of yacc.c  */
#line 73 "m-conv.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EOS = 258
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 3 "m-conv.y"
 double d; long l; char c; 


/* Line 214 of yacc.c  */
#line 116 "m-conv.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 128 "m-conv.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   569

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  120
/* YYNRULES -- Number of rules.  */
#define YYNRULES  254
/* YYNRULES -- Number of states.  */
#define YYNSTATES  592

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   258

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    40,     2,     2,     2,
       2,     2,    41,    42,     2,    43,     2,     2,     4,    28,
      19,    29,    30,    31,    32,    33,    34,    35,     2,     2,
       2,     2,     2,     2,     2,    11,    12,     9,     7,     5,
      17,    36,    20,    18,    37,    14,    13,    38,     6,    16,
      23,    15,    21,     8,    10,    26,    22,    25,    27,    24,
      39,     2,     2,     2,     2,    44,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,    15,    24,    33,    42,    44,    47,
      49,    52,    54,    57,    59,    62,    66,    72,    74,    77,
      90,   100,   110,   120,   132,   134,   136,   139,   140,   150,
     153,   157,   160,   164,   172,   180,   188,   196,   203,   209,
     211,   214,   215,   225,   228,   232,   235,   239,   241,   250,
     251,   254,   257,   261,   272,   279,   287,   296,   303,   311,
     319,   325,   334,   342,   346,   350,   354,   358,   362,   366,
     370,   374,   378,   383,   388,   393,   398,   403,   408,   413,
     418,   423,   428,   433,   438,   443,   448,   453,   458,   463,
     468,   473,   478,   483,   488,   493,   498,   503,   508,   513,
     518,   523,   528,   533,   538,   543,   548,   553,   558,   563,
     568,   573,   578,   583,   588,   593,   598,   603,   608,   614,
     620,   626,   628,   630,   632,   634,   636,   638,   640,   642,
     644,   646,   648,   650,   652,   654,   656,   658,   660,   662,
     664,   666,   668,   670,   672,   674,   676,   678,   680,   682,
     684,   686,   688,   690,   692,   694,   696,   698,   700,   702,
     704,   706,   708,   710,   712,   714,   716,   718,   720,   722,
     724,   726,   728,   730,   732,   734,   736,   738,   740,   742,
     744,   747,   749,   751,   753,   755,   757,   759,   761,   762,
     765,   768,   772,   774,   777,   778,   789,   799,   801,   804,
     805,   810,   812,   813,   816,   817,   820,   822,   824,   826,
     828,   830,   832,   834,   836,   838,   840,   842,   844,   846,
     848,   850,   852,   854,   856,   858,   860,   862,   864,   866,
     868,   870,   872,   874,   876,   878,   880,   882,   884,   886,
     888,   890,   892,   894,   896,   898,   900,   902,   903,   906,
     907,   911,   913,   916,   918
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      55,     0,    -1,     4,     3,    -1,    46,     5,     6,     7,
       8,     5,     9,     3,    -1,    46,     5,     6,     7,    10,
      11,    12,     3,    -1,    46,     5,     6,     7,    12,    13,
      14,     3,    -1,    46,     8,     5,    15,     5,     6,     7,
       3,    -1,    47,    -1,    47,    83,    -1,    48,    -1,    48,
      83,    -1,    49,    -1,    49,    83,    -1,    50,    -1,    50,
      83,    -1,    50,    80,    83,    -1,    56,    46,     5,    16,
      17,    -1,    57,    -1,    56,    57,    -1,    46,     8,     5,
       9,    10,    18,    16,     6,     3,    19,     3,    58,    -1,
      10,    11,    12,    13,     5,     8,     3,    60,    51,    -1,
      20,     5,    11,     7,     5,    21,     3,    59,    51,    -1,
      12,    13,    16,     9,    14,     8,     3,    66,    51,    -1,
       5,     6,    10,    18,    10,    18,     5,     8,     3,    74,
      51,    -1,   135,    -1,    61,    -1,    60,    61,    -1,    -1,
      46,    10,    11,    12,    13,     5,     3,    62,    63,    -1,
     135,    52,    -1,   135,    64,    52,    -1,    46,    65,    -1,
      64,    46,    65,    -1,    22,    23,    16,    21,    10,     3,
     135,    -1,    13,    10,    24,    23,     5,     3,   135,    -1,
      13,    11,    24,     5,    21,     3,   135,    -1,     8,    10,
      24,    13,     5,     3,   135,    -1,    22,    18,     5,    25,
       3,   135,    -1,    26,     9,     8,     3,   135,    -1,    67,
      -1,    66,    67,    -1,    -1,    46,    12,    13,    16,     9,
      14,     3,    68,    69,    -1,   135,    53,    -1,   135,    70,
      53,    -1,    46,    71,    -1,    70,    46,    71,    -1,    75,
      -1,    11,    10,    10,     7,     5,    17,     3,    72,    -1,
      -1,    73,   135,    -1,    46,    75,    -1,    74,    46,    75,
      -1,    23,    16,    13,    24,    13,    18,     6,     5,     3,
     151,    -1,    13,    18,     6,     5,     3,   136,    -1,    23,
      16,    18,     6,    10,     3,   137,    -1,     9,    18,    21,
       9,    13,     5,     3,   153,    -1,    10,     5,    27,    10,
       3,   141,    -1,    10,    21,    11,     9,     5,     3,   142,
      -1,     8,    16,    13,    18,     7,     3,   139,    -1,    11,
      21,     9,     3,   138,    -1,    18,     6,     8,     5,    21,
      10,     3,   143,    -1,     8,    20,    11,    23,     5,     3,
     140,    -1,    28,     3,   160,    -1,    19,     3,   160,    -1,
      29,     3,   160,    -1,    30,     3,   160,    -1,    31,     3,
     160,    -1,    32,     3,   160,    -1,    33,     3,   160,    -1,
      34,     3,   160,    -1,    35,     3,   160,    -1,    28,     4,
       3,   154,    -1,    28,    28,     3,   154,    -1,    28,    19,
       3,   154,    -1,    28,    29,     3,   154,    -1,    28,    30,
       3,   154,    -1,    28,    31,     3,   154,    -1,    28,    32,
       3,   154,    -1,    28,    33,     3,   154,    -1,    19,     4,
       3,   154,    -1,    19,    28,     3,   154,    -1,    19,    19,
       3,   154,    -1,    19,    29,     3,   154,    -1,    19,    30,
       3,   154,    -1,    19,    31,     3,   154,    -1,    19,    32,
       3,   154,    -1,    19,    33,     3,   154,    -1,    29,     4,
       3,   154,    -1,    29,    28,     3,   154,    -1,    29,    19,
       3,   154,    -1,    29,    29,     3,   154,    -1,    29,    30,
       3,   154,    -1,    29,    31,     3,   154,    -1,    29,    32,
       3,   154,    -1,    29,    33,     3,   154,    -1,    29,    34,
       3,   154,    -1,    29,    35,     3,   154,    -1,    30,     4,
       3,   154,    -1,    30,    28,     3,   154,    -1,    30,    19,
       3,   154,    -1,    30,    29,     3,   154,    -1,    30,    30,
       3,   154,    -1,    30,    34,     3,   154,    -1,    30,    35,
       3,   154,    -1,    31,     4,     3,   154,    -1,    31,    28,
       3,   154,    -1,    32,    19,     3,   156,    -1,    32,    32,
       3,   156,    -1,    33,     4,     3,   156,    -1,    33,    28,
       3,   156,    -1,    33,    19,     3,   156,    -1,    33,    29,
       3,   156,    -1,    33,    30,     3,   156,    -1,    33,    31,
       3,   156,    -1,    33,    32,     3,   156,    -1,    33,    33,
       3,   156,    -1,    33,    34,     3,   156,    -1,    19,    28,
       4,     3,   154,    -1,    19,    19,     4,     3,   154,    -1,
      19,    29,     4,     3,   154,    -1,    76,    -1,    77,    -1,
      78,    -1,    79,    -1,    80,    -1,    81,    -1,    82,    -1,
      83,    -1,    84,    -1,    85,    -1,    86,    -1,    87,    -1,
      88,    -1,    89,    -1,    90,    -1,    91,    -1,    92,    -1,
      93,    -1,    94,    -1,    95,    -1,    96,    -1,    97,    -1,
      98,    -1,    99,    -1,   100,    -1,   101,    -1,   102,    -1,
     103,    -1,   104,    -1,   105,    -1,   106,    -1,   107,    -1,
     108,    -1,   109,    -1,   110,    -1,   111,    -1,   112,    -1,
     113,    -1,   114,    -1,   115,    -1,   116,    -1,   117,    -1,
     118,    -1,   119,    -1,   120,    -1,   121,    -1,   122,    -1,
     123,    -1,   124,    -1,   125,    -1,   126,    -1,   127,    -1,
     128,    -1,   129,    -1,   130,    -1,   131,    -1,   132,    -1,
     133,    -1,   134,    -1,   135,   134,    -1,   135,    -1,   135,
      -1,   135,    -1,   135,    -1,   135,    -1,   135,    -1,   135,
      -1,    -1,   144,   145,    -1,   135,    54,    -1,   135,   146,
      54,    -1,   147,    -1,   146,   147,    -1,    -1,    46,    11,
      10,    10,    21,    18,    12,     3,   148,   135,    -1,    46,
      22,     5,    21,    10,     5,    27,     3,   135,    -1,   149,
      -1,   150,   149,    -1,    -1,   135,   152,   150,    54,    -1,
     135,    -1,    -1,   155,     3,    -1,    -1,   157,     3,    -1,
       4,    -1,    28,    -1,    19,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    33,    -1,    34,    -1,    35,    -1,
      11,    -1,    12,    -1,     9,    -1,     7,    -1,     5,    -1,
      17,    -1,    36,    -1,    20,    -1,    18,    -1,    37,    -1,
      14,    -1,    13,    -1,    38,    -1,     6,    -1,    16,    -1,
      23,    -1,    15,    -1,    21,    -1,     8,    -1,    10,    -1,
      26,    -1,    22,    -1,    25,    -1,    27,    -1,    24,    -1,
      39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1,
      44,    -1,    -1,   161,     3,    -1,    -1,   162,   163,     3,
      -1,   164,    -1,   163,   164,    -1,   159,    -1,   158,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    13,    13,    16,    18,    20,    22,    24,    24,    26,
      26,    28,    28,    30,    30,    30,    33,    37,    38,    41,
      44,    45,    46,    47,    50,    53,    54,    58,    57,    62,
      63,    66,    67,    70,    71,    72,    73,    74,    75,    78,
      79,    83,    82,    87,    88,    91,    92,    95,    96,    99,
      99,   103,   104,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   118,   124,   130,   132,   134,   136,   138,
     140,   146,   148,   154,   160,   166,   172,   174,   176,   178,
     180,   186,   192,   198,   204,   206,   208,   210,   212,   218,
     224,   226,   228,   230,   232,   234,   236,   242,   248,   254,
     260,   266,   272,   274,   276,   278,   284,   290,   296,   302,
     312,   322,   328,   330,   332,   334,   336,   338,   340,   346,
     352,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   419,
     420,   422,   424,   426,   428,   430,   432,   434,   437,   437,
     442,   443,   446,   447,   451,   450,   455,   459,   460,   464,
     463,   481,   485,   485,   488,   488,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   536,   536,   538,
     538,   541,   541,   544,   545
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EOS", "'0'", "'E'", "'N'", "'D'", "'S'",
  "'C'", "'T'", "'A'", "'B'", "'L'", "'K'", "'Q'", "'O'", "'F'", "'I'",
  "'2'", "'H'", "'R'", "'V'", "'P'", "'Y'", "'W'", "'U'", "'X'", "'1'",
  "'3'", "'4'", "'5'", "'6'", "'7'", "'8'", "'9'", "'G'", "'J'", "'M'",
  "'Z'", "'$'", "'*'", "'+'", "'-'", "'_'", "$accept", "YY_0EOS",
  "yy_endsec", "yy_endtab", "yy_endblk", "yy_seqend", "YY_ENDSEC",
  "YY_ENDTAB", "YY_ENDBLK", "YY_SEQEND", "DXF_FILE", "DXF_SECTIONS",
  "SECTION_INSTANCE", "DXF_SECTION", "SECTION_HEADER", "SECTION_TABLES",
  "TABLE_INSTANCE", "$@1", "TABLE_BODY", "TABLES_HEAP", "TABLE_ENTITIE",
  "SECTION_BLOCKS", "BLOCK_INSTANCE", "$@2", "BLOCK_BODY", "BLOCK_HEAP",
  "BLOCK_ENTITIE", "ATTDEF_ENTITIE", "$@3", "SECTION_ENTITIES",
  "DRAW_ENTITIE", "YY_01_ITEM", "YY_02_ITEM", "YY_03_ITEM", "YY_04_ITEM",
  "YY_05_ITEM", "YY_06_ITEM", "YY_07_ITEM", "YY_08_ITEM", "YY_09_ITEM",
  "YY_10_ITEM", "YY_11_ITEM", "YY_12_ITEM", "YY_13_ITEM", "YY_14_ITEM",
  "YY_15_ITEM", "YY_16_ITEM", "YY_17_ITEM", "YY_20_ITEM", "YY_21_ITEM",
  "YY_22_ITEM", "YY_23_ITEM", "YY_24_ITEM", "YY_25_ITEM", "YY_26_ITEM",
  "YY_27_ITEM", "YY_30_ITEM", "YY_31_ITEM", "YY_32_ITEM", "YY_33_ITEM",
  "YY_34_ITEM", "YY_35_ITEM", "YY_36_ITEM", "YY_37_ITEM", "YY_38_ITEM",
  "YY_39_ITEM", "YY_40_ITEM", "YY_41_ITEM", "YY_42_ITEM", "YY_43_ITEM",
  "YY_44_ITEM", "YY_48_ITEM", "YY_49_ITEM", "YY_50_ITEM", "YY_51_ITEM",
  "YY_62_ITEM", "YY_66_ITEM", "YY_70_ITEM", "YY_71_ITEM", "YY_72_ITEM",
  "YY_73_ITEM", "YY_74_ITEM", "YY_75_ITEM", "YY_76_ITEM", "YY_77_ITEM",
  "YY_78_ITEM", "YY210_ITEM", "YY220_ITEM", "YY230_ITEM", "YY_COMMON_ITEM",
  "YY_COMMON_GROUP", "LINE_ENTITIE", "POINT_ENTITIE", "ARC_ENTITIE",
  "SOLID_ENTITIE", "SHAPE_ENTITIE", "TEXT_ENTITIE", "TRACE_ENTITIE",
  "INSERT_ENTITIE", "$@4", "INSERT_BODY", "ATTRIB_HEAP", "ATTRIB_ENTITIE",
  "$@5", "YY_VERTEX", "YY_VERTEXES", "PLINE_ENTITIE", "$@6",
  "CIRCLE_ENTITIE", "YY_Float", "$@7", "YY_Int", "$@8", "DCHAR", "ZCHAR",
  "CHARS", "$@9", "$@10", "XCHAR", "SCHAR", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,    48,    69,    78,    68,    83,    67,
      84,    65,    66,    76,    75,    81,    79,    70,    73,    50,
      72,    82,    86,    80,    89,    87,    85,    88,    49,    51,
      52,    53,    54,    55,    56,    57,    71,    74,    77,    90,
      36,    42,    43,    45,    95
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    47,    48,    49,    50,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    55,    56,    56,    57,
      58,    58,    58,    58,    59,    60,    60,    62,    61,    63,
      63,    64,    64,    65,    65,    65,    65,    65,    65,    66,
      66,    68,    67,    69,    69,    70,    70,    71,    71,    73,
      72,    74,    74,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     135,   136,   137,   138,   139,   140,   141,   142,   144,   143,
     145,   145,   146,   146,   148,   147,   149,   150,   150,   152,
     151,   153,   155,   154,   157,   156,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   161,   160,   162,
     160,   163,   163,   164,   164
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     8,     8,     8,     8,     1,     2,     1,
       2,     1,     2,     1,     2,     3,     5,     1,     2,    12,
       9,     9,     9,    11,     1,     1,     2,     0,     9,     2,
       3,     2,     3,     7,     7,     7,     7,     6,     5,     1,
       2,     0,     9,     2,     3,     2,     3,     1,     8,     0,
       2,     2,     3,    10,     6,     7,     8,     6,     7,     7,
       5,     8,     7,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     5,     5,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     3,     1,     2,     0,    10,     9,     1,     2,     0,
       4,     1,     0,     2,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     0,
       3,     1,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,    17,     2,     0,     1,     0,
      18,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    24,     0,
       0,     0,     7,    20,    26,     0,     0,    22,    40,   249,
       0,     0,     0,     0,     0,     0,     0,     0,   249,     0,
       0,     0,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,     0,
       0,     0,     0,     0,     0,     0,   249,     0,     0,   249,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   249,     0,    21,   180,     0,     0,     0,
       0,     8,     0,    64,     0,     0,   202,   202,     0,   202,
       0,   202,     0,   202,   202,   202,   202,    63,   202,   202,
     202,   202,   202,   202,   202,   202,    65,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,    66,   202,   202,
     202,   202,   202,   202,   202,    67,   202,   202,    68,   204,
     204,    69,   204,   204,   204,   204,   204,   204,   204,   204,
     204,    70,    71,     0,     0,     0,     0,     0,     0,     0,
      51,     0,    23,     0,     0,     0,   248,   206,   220,   229,
     219,   234,   218,   235,   216,   217,   227,   226,   232,   230,
     221,   224,   208,   223,   233,   237,   231,   240,   238,   236,
     239,   207,   209,   210,   211,   212,   213,   214,   215,   222,
     225,   228,   241,   242,   243,   244,   245,   246,   254,   253,
       0,   251,    80,     0,    82,   202,    81,   202,    83,   202,
      84,    85,    86,    87,    72,    74,    73,    75,    76,    77,
      78,    79,    88,    90,    89,    91,    92,    93,    94,    95,
      96,    97,    98,   100,    99,   101,   102,   103,   104,   105,
     106,   107,     0,   108,   109,   111,   110,   112,   113,   114,
     115,   116,   117,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,   250,   252,   203,   119,
     118,   120,   205,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,     0,    41,     0,
       0,     0,     0,     0,   183,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,    57,     0,   181,
      54,     0,     0,     0,    28,     0,     3,    42,     0,   184,
      59,   185,    62,     0,   187,    58,   188,     0,   182,    55,
       0,     9,    29,     0,     0,    11,    43,     0,   201,    56,
      61,     0,     0,     0,     0,     0,     0,     0,    31,    10,
       0,    30,     0,     0,    45,    47,    12,     0,    44,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,    32,
       0,     0,    46,     0,    13,   190,     0,   192,   199,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,   191,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
     197,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,   200,   198,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     4,    36,
      34,    35,    33,     5,    49,     0,     0,     0,    48,     0,
       0,     0,     0,    50,     6,   194,     0,     0,     0,   195,
       0,   196
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   503,   132,   461,   465,   504,   133,   462,   466,   505,
       3,     4,     5,    27,    68,    54,    55,   430,   444,   463,
     478,    57,    58,   432,   447,   467,   484,   578,   579,   198,
     485,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   440,   459,   425,   450,   452,   437,   455,   470,   471,
     490,   506,   507,   587,   540,   541,   509,   526,   469,   322,
     323,   361,   362,   318,   319,   203,   204,   205,   320,   321
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -504
static const yytype_int16 yypact[] =
{
      15,    26,    31,    51,    15,  -504,  -504,    69,  -504,    13,
    -504,    81,   103,    56,   136,   129,  -504,   111,   145,   151,
     146,   160,   104,   158,   155,   154,   165,  -504,   168,   176,
     159,   179,   177,   181,   187,   281,   280,   286,   278,   288,
     277,   289,   292,   275,   296,   299,   300,   301,   298,    15,
      15,   350,   311,   305,    15,  -504,   306,    15,  -504,   109,
     333,   127,     7,     4,    12,   279,   316,   318,    15,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,   350,    15,
     313,    57,   291,  -504,  -504,   309,    35,  -504,  -504,   320,
     324,    61,   122,   130,   325,   330,   345,   347,   320,   348,
     351,   352,   353,   354,   355,   356,   357,   320,   364,   365,
     368,   369,   370,   371,   372,   373,   383,   384,   320,   385,
     387,   388,   389,   390,   402,   403,   320,   404,   405,   320,
     406,   407,   320,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   320,   320,   417,  -504,  -504,   276,    15,   341,
     418,  -504,   419,  -504,   420,   237,  -504,  -504,   422,  -504,
     423,  -504,   424,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,    32,   421,     9,   399,   425,   426,   428,
    -504,   163,  -504,   427,   429,   432,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
     196,  -504,  -504,   430,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,   431,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,   433,   436,   400,   401,   438,   445,   449,
     448,   102,  -504,   452,   450,   446,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,   441,   439,   456,   451,   457,   434,   463,
     464,   447,   467,   435,   469,   472,   470,   471,   465,   476,
     475,   350,   478,   461,   473,   474,  -504,   479,  -504,   480,
     482,   484,   350,   487,   350,  -504,   350,   477,   481,   488,
     350,   489,   350,   350,   350,   490,   350,  -504,   350,   350,
    -504,   491,   492,   350,  -504,   312,  -504,  -504,   312,   350,
    -504,   350,  -504,   350,   350,  -504,  -504,   495,   350,  -504,
      50,   291,  -504,    15,   174,   291,  -504,    15,   350,  -504,
    -504,   350,   493,   496,   485,   134,    99,   494,  -504,  -504,
      50,  -504,   498,    -4,  -504,  -504,  -504,   174,  -504,   312,
    -504,   350,   499,   483,   486,   497,   500,   501,   503,  -504,
     502,   504,  -504,   110,   101,  -504,    15,  -504,   350,  -504,
     505,   506,   507,   508,   509,   510,   513,   511,   515,   519,
     517,   522,   291,  -504,  -504,  -504,    15,   518,   523,   527,
     512,   532,   526,   350,   524,   533,   525,   529,  -504,   520,
    -504,    15,   531,   541,   542,   543,   350,   544,   350,   534,
     535,   545,   528,   546,    38,  -504,  -504,   550,   350,   350,
     350,   350,   350,   551,   552,   514,   538,   536,  -504,   350,
     350,   350,   350,  -504,  -504,   553,   547,   548,  -504,   350,
     558,   559,   560,   350,  -504,  -504,   537,   350,   563,   350,
     350,   350
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -504,     0,  -504,  -504,  -504,  -504,   -55,   -33,   -36,  -503,
    -504,  -504,   564,  -504,  -504,  -504,   443,  -504,  -504,  -504,
      21,  -504,   455,  -504,  -504,  -504,    76,  -504,  -504,  -504,
    -170,  -504,  -504,  -504,  -504,    14,  -504,  -504,  -127,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -119,
    -410,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,  -504,
    -504,  -504,     2,  -504,    28,  -504,  -504,  -504,  -504,  -136,
    -504,   144,  -504,  -504,  -504,  -123,  -504,  -504,  -504,   247
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -248
static const yytype_int16 yytable[] =
{
       2,   424,   137,   524,     9,   201,   501,   176,   177,   196,
     168,   169,   436,   195,   376,   179,   439,   378,    12,     1,
     445,     7,   448,   449,   451,   217,   170,   270,   454,     6,
     377,   180,   178,   458,   226,   171,   172,   173,   555,     7,
     200,   174,   175,   468,   181,   237,   519,   135,   373,    53,
      56,     8,   374,   245,   131,   473,   248,   136,   474,   251,
     553,   489,   200,   475,   207,   208,    15,   130,   194,   261,
     262,   324,   476,   326,    11,   328,   477,   330,   331,   332,
     333,   508,   334,   335,   336,   337,   338,   339,   340,   341,
      13,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   382,   352,   353,   354,   355,   356,   357,   358,    23,
     359,   360,   139,   140,    24,   401,    25,   496,   519,    14,
     402,   520,   497,   548,    26,   209,   210,    18,   141,   197,
     157,   158,   521,   211,   212,    66,   561,   142,   143,   144,
     145,   146,   147,   272,   494,   495,   159,    17,   569,   570,
     571,    19,   572,    16,    20,   160,   161,   162,   163,   164,
     165,   166,   167,    22,    28,    21,    29,    30,   200,   583,
      31,   263,   264,   265,   266,    34,   267,   589,    32,   482,
     591,   268,   263,   264,   265,   483,   269,   267,    33,   389,
      35,   390,   268,   391,    37,    36,    38,   269,   271,   386,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   182,   183,   263,   264,   265,   266,    39,   267,
      40,    41,    42,    43,   268,    44,    47,    45,   184,   269,
      46,    48,    49,    50,    51,   196,    52,   185,   186,   187,
     188,   189,   190,   191,   129,   130,     1,   196,   135,   192,
     196,   193,   202,  -247,   199,    66,   196,   206,   213,   196,
     196,    59,   196,   214,   479,   196,   148,   149,   486,   196,
      60,    61,    62,    63,    64,    65,    66,    67,   215,   196,
     216,   218,   150,   273,   219,   220,   221,   222,   223,   224,
     225,   151,   152,   153,   154,   155,   156,   227,   228,    59,
     196,   229,   230,   231,   232,   233,   234,   523,    60,    61,
      62,    63,    64,    65,    66,    67,   235,   236,   238,   196,
     239,   240,   241,   242,   363,   538,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   243,   244,   246,   247,   249,
     250,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     378,   395,   200,   276,   274,   325,   327,   329,   396,   196,
     481,   488,   380,   388,   392,   275,   384,   411,   416,   375,
     383,   385,   196,   379,   381,   460,   393,   394,   464,   397,
     196,   196,   196,   196,   398,   399,   400,   403,   404,   406,
     405,   409,   407,   480,   196,   408,   410,   487,   412,   413,
     196,   414,   196,   415,   417,   418,   420,   419,   421,   422,
     423,   426,   427,   433,   429,   434,   428,   441,   431,   435,
     438,   443,   446,   453,   456,   493,   491,   134,   457,   442,
     472,   499,   492,   498,   500,   514,   510,   511,   525,   517,
     512,   516,   138,   530,   518,   527,   533,   515,   522,   528,
     575,   513,   535,   534,   536,   176,   539,   537,   543,   542,
     529,   532,   544,   545,   531,   546,   547,   549,   550,   552,
     551,   554,   553,   557,   558,   559,   560,   562,   563,   566,
     565,   567,   564,   568,   573,   574,   576,   577,   582,   581,
     580,   584,   585,   502,   588,   586,   590,   387,    10,   556
};

static const yytype_uint16 yycheck[] =
{
       0,   411,    57,   506,     4,   132,    10,     3,     4,   128,
       3,     4,   422,    68,     5,     3,   426,    21,     5,     4,
     430,     8,   432,   433,   434,   148,    19,   197,   438,     3,
      21,    19,    28,   443,   157,    28,    29,    30,   541,     8,
       5,    34,    35,   453,    32,   168,     8,    12,    16,    49,
      50,     0,    20,   176,    54,     5,   179,    57,     8,   182,
      22,   471,     5,    13,     3,     4,    10,    10,    68,   192,
     193,   207,    22,   209,     5,   211,    26,   213,   214,   215,
     216,   491,   218,   219,   220,   221,   222,   223,   224,   225,
       9,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   271,   238,   239,   240,   241,   242,   243,   244,     5,
     246,   247,     3,     4,    10,    13,    12,    18,     8,    16,
      18,    11,    23,   533,    20,     3,     4,    16,    19,   129,
       3,     4,    31,     3,     4,    34,   546,    28,    29,    30,
      31,    32,    33,   198,    10,    11,    19,    18,   558,   559,
     560,     6,   562,    17,     3,    28,    29,    30,    31,    32,
      33,    34,    35,     3,     6,    19,    11,    13,     5,   579,
       5,     8,     9,    10,    11,    16,    13,   587,    10,     5,
     590,    18,     8,     9,    10,    11,    23,    13,    12,   325,
      11,   327,    18,   329,    13,    18,     9,    23,   198,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,     3,     4,     8,     9,    10,    11,     7,    13,
      10,     5,    14,     5,    18,    18,    21,     8,    19,    23,
       8,     5,     3,     3,     3,   424,     8,    28,    29,    30,
      31,    32,    33,    34,     3,    10,     4,   436,    12,     3,
     439,     3,    13,     3,    11,    34,   445,     3,     3,   448,
     449,    19,   451,     3,   461,   454,     3,     4,   465,   458,
      28,    29,    30,    31,    32,    33,    34,    35,     3,   468,
       3,     3,    19,    12,     3,     3,     3,     3,     3,     3,
       3,    28,    29,    30,    31,    32,    33,     3,     3,    19,
     489,     3,     3,     3,     3,     3,     3,   504,    28,    29,
      30,    31,    32,    33,    34,    35,     3,     3,     3,   508,
       3,     3,     3,     3,   250,   522,   252,   253,   254,   255,
     256,   257,   258,   259,   260,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
      21,    21,     5,     3,     6,     3,     3,     3,    27,   548,
     463,   467,     6,     3,     3,    16,     7,     3,     3,    18,
      13,     9,   561,    18,    16,   445,    13,    11,   448,    11,
     569,   570,   571,   572,     9,     6,     8,     5,     8,    18,
      14,    10,    23,   463,   583,     9,     9,   467,     5,     5,
     589,    24,   591,     6,     5,     3,     5,     7,    13,     3,
       5,     3,    21,     3,    10,     3,    13,    10,     9,     5,
       3,     3,     3,     3,     3,    10,     3,    54,     6,    18,
       5,   480,     6,     9,     6,     5,     7,    24,   506,     7,
      24,     8,    57,     5,    10,    10,     3,    16,   504,    13,
       6,    24,     7,    12,     5,     3,   526,    10,     5,    11,
      23,    21,     5,    21,    25,     3,    10,    13,     5,    10,
      15,   541,    22,    12,     3,     3,     3,     3,    14,    21,
       5,     5,    17,     3,     3,     3,    18,    21,    10,    12,
       7,     3,     3,   487,    27,     5,     3,   320,     4,   541
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    46,    55,    56,    57,     3,     8,     0,    46,
      57,     5,     5,     9,    16,    10,    17,    18,    16,     6,
       3,    19,     3,     5,    10,    12,    20,    58,     6,    11,
      13,     5,    10,    12,    16,    11,    18,    13,     9,     7,
      10,     5,    14,     5,    18,     8,     8,    21,     5,     3,
       3,     3,     8,    46,    60,    61,    46,    66,    67,    19,
      28,    29,    30,    31,    32,    33,    34,    35,    59,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,     3,
      10,    46,    47,    51,    61,    12,    46,    51,    67,     3,
       4,    19,    28,    29,    30,    31,    32,    33,     3,     4,
      19,    28,    29,    30,    31,    32,    33,     3,     4,    19,
      28,    29,    30,    31,    32,    33,    34,    35,     3,     4,
      19,    28,    29,    30,    34,    35,     3,     4,    28,     3,
      19,    32,     3,     4,    19,    28,    29,    30,    31,    32,
      33,    34,     3,     3,    46,    51,   134,    46,    74,    11,
       5,    83,    13,   160,   161,   162,     3,     3,     4,     3,
       4,     3,     4,     3,     3,     3,     3,   160,     3,     3,
       3,     3,     3,     3,     3,     3,   160,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,   160,     3,     3,
       3,     3,     3,     3,     3,   160,     3,     3,   160,     3,
       3,   160,     3,     3,     3,     3,     3,     3,     3,     3,
       3,   160,   160,     8,     9,    10,    11,    13,    18,    23,
      75,    46,    51,    12,     6,    16,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,   158,   159,
     163,   164,   154,   155,   154,     3,   154,     3,   154,     3,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   156,   157,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,    16,    20,    18,     5,    21,    21,    18,
       6,    16,    75,    13,     7,     9,     3,   164,     3,   154,
     154,   154,     3,    13,    11,    21,    27,    11,     9,     6,
       8,    13,    18,     5,     8,    14,    18,    23,     9,    10,
       9,     3,     5,     5,    24,     6,     3,     5,     3,     7,
       5,    13,     3,     5,   135,   138,     3,    21,    13,    10,
      62,     9,    68,     3,     3,     5,   135,   141,     3,   135,
     136,    10,    18,     3,    63,   135,     3,    69,   135,   135,
     139,   135,   140,     3,   135,   142,     3,     6,   135,   137,
      46,    48,    52,    64,    46,    49,    53,    70,   135,   153,
     143,   144,     5,     5,     8,    13,    22,    26,    65,    83,
      46,    52,     5,    11,    71,    75,    83,    46,    53,   135,
     145,     3,     6,    10,    10,    11,    18,    23,     9,    65,
       6,    10,    71,    46,    50,    54,   146,   147,   135,   151,
       7,    24,    24,    24,     5,    16,     8,     7,    10,     8,
      11,    31,    80,    83,    54,   147,   152,    10,    13,    23,
       5,    25,    21,     3,    12,     7,     5,    10,    83,    46,
     149,   150,    11,     5,     5,    21,     3,    10,   135,    13,
       5,    15,    10,    22,    46,    54,   149,    12,     3,     3,
       3,   135,     3,    14,    17,     5,    21,     5,     3,   135,
     135,   135,   135,     3,     3,     6,    18,    21,    72,    73,
       7,    12,    10,   135,     3,     3,     5,   148,    27,   135,
       3,   135
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 14 "m-conv.y"
    { YY_CLEAR_FLAGS(); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 34 "m-conv.y"
    { return(0) ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 58 "m-conv.y"
    { YY_2String = YY_3String; ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 83 "m-conv.y"
    { YY_2String = YY_4String; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 99 "m-conv.y"
    { YY_2String = YY_7String; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 119 "m-conv.y"
    {
  YY_01_FLAG = 1;
  strncpy(YY_6String, YY_1String, 30);
  ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 125 "m-conv.y"
    {
  YY_02_FLAG = 1;
  strncpy(YY_2String, YY_1String, 30);
  ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 141 "m-conv.y"
    {
  YY_08_FLAG = 1;
  strncpy(YY_5String, YY_1String, 30);
  ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 149 "m-conv.y"
    {
  YY_10_FLAG = 1;
  Item_10_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 155 "m-conv.y"
    {
  YY_11_FLAG = 1;
  Item_11_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 161 "m-conv.y"
    {
  YY_12_FLAG = 1;
  Item_12_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 167 "m-conv.y"
    {
  YY_13_FLAG = 1;
  Item_13_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 181 "m-conv.y"
    {
  YY_20_FLAG = 1;
  Item_20_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 187 "m-conv.y"
    {
  YY_21_FLAG = 1;
  Item_21_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 193 "m-conv.y"
    {
  YY_22_FLAG = 1;
  Item_22_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 199 "m-conv.y"
    {
  YY_23_FLAG = 1;
  Item_23_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 213 "m-conv.y"
    {
  YY_30_FLAG = 1;
  Item_30_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 219 "m-conv.y"
    {
  YY_31_FLAG = 1;
  Item_31_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 237 "m-conv.y"
    {
  YY_38_FLAG = 1;
  Item_38_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 243 "m-conv.y"
    {
  YY_39_FLAG = 1;
  Item_39_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 249 "m-conv.y"
    {
  YY_40_FLAG = 1;
  Item_40_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 255 "m-conv.y"
    {
  YY_41_FLAG = 1;
  Item_41_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 261 "m-conv.y"
    {
  YY_42_FLAG = 1;
  Item_42_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 267 "m-conv.y"
    {
  YY_43_FLAG = 1;
  Item_43_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 279 "m-conv.y"
    {
  YY_50_FLAG = 1;
  Item_50_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 285 "m-conv.y"
    {
  YY_51_FLAG = 1;
  Item_51_Val = (yyvsp[(4) - (4)].d);
  ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 291 "m-conv.y"
    {
  YY_62_FLAG = 1;
  Item_62_Val = (yyvsp[(4) - (4)].l);
  ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 297 "m-conv.y"
    {
  YY_66_FLAG = 1;
  Item_66_Val = (yyvsp[(4) - (4)].l);
  ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 303 "m-conv.y"
    {
  YY_70_FLAG = 1;
  Bit_70_01 = (yyvsp[(4) - (4)].l) & 01;
  Bit_70_02 = (yyvsp[(4) - (4)].l) & 02;
  Bit_70_04 = (yyvsp[(4) - (4)].l) & 04;
  Bit_70_08 = (yyvsp[(4) - (4)].l) & 8L;
  Bit_70_16 = (yyvsp[(4) - (4)].l) & 16;
  ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 313 "m-conv.y"
    {
  YY_71_FLAG = 1;
  Bit_71_01 = (yyvsp[(4) - (4)].l) & 01;
  Bit_71_02 = (yyvsp[(4) - (4)].l) & 02;
  Bit_71_04 = (yyvsp[(4) - (4)].l) & 04;
  Bit_71_08 = (yyvsp[(4) - (4)].l) & 8L;
  Bit_71_16 = (yyvsp[(4) - (4)].l) & 16;
  ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 323 "m-conv.y"
    {
  YY_72_FLAG = 1;
  Item_72_Val = (yyvsp[(4) - (4)].l);
  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 341 "m-conv.y"
    {
  YY210_FLAG = 1;
  Item210_Val = (yyvsp[(5) - (5)].d);
  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 347 "m-conv.y"
    {
  YY220_FLAG = 1;
  Item220_Val = (yyvsp[(5) - (5)].d);
  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 353 "m-conv.y"
    {
  YY230_FLAG = 1;
  Item230_Val = (yyvsp[(5) - (5)].d);
  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 437 "m-conv.y"
    { YY_2String = YY_4String; ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 439 "m-conv.y"
    { Terminate_Insert(); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 451 "m-conv.y"
    { YY_2String = YY_7String; ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 456 "m-conv.y"
    { Terminate_Vertex(); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 464 "m-conv.y"
    {
  VERLIM = 1;
  YY_Interpolate = 0;

  Closed_Polygon = YY_70_FLAG ? Bit_70_01 :0;

  ZZ_START_WIDTH = YY_40_FLAG ? Item_40_Val :0;
  ZZ_ENDIN_WIDTH = YY_41_FLAG ? Item_41_Val :0;

  MAX_WIDTH = ZZ_START_WIDTH;
  if(MAX_WIDTH<ZZ_ENDIN_WIDTH)
  MAX_WIDTH = ZZ_ENDIN_WIDTH;
  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 478 "m-conv.y"
    { Terminate_Poly_1(); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 482 "m-conv.y"
    { Terminate_Circle(); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 485 "m-conv.y"
    { yylex = YY_FLOAT_LEX; ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 485 "m-conv.y"
    { yylex = DXFyylex; (yyval.d) = YY_F_VAL; ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 488 "m-conv.y"
    { yylex = YY_INT_LEX; ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 488 "m-conv.y"
    { yylex = DXFyylex; (yyval.l) = YY_I_VAL; ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 491 "m-conv.y"
    { (yyval.c)='0' ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 492 "m-conv.y"
    { (yyval.c)='1' ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 493 "m-conv.y"
    { (yyval.c)='2' ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 494 "m-conv.y"
    { (yyval.c)='3' ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 495 "m-conv.y"
    { (yyval.c)='4' ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 496 "m-conv.y"
    { (yyval.c)='5' ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 497 "m-conv.y"
    { (yyval.c)='6' ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 498 "m-conv.y"
    { (yyval.c)='7' ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 499 "m-conv.y"
    { (yyval.c)='8' ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 500 "m-conv.y"
    { (yyval.c)='9' ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 503 "m-conv.y"
    { (yyval.c)='A' ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 504 "m-conv.y"
    { (yyval.c)='B' ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 505 "m-conv.y"
    { (yyval.c)='C' ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 506 "m-conv.y"
    { (yyval.c)='D' ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 507 "m-conv.y"
    { (yyval.c)='E' ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 508 "m-conv.y"
    { (yyval.c)='F' ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 509 "m-conv.y"
    { (yyval.c)='G' ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 510 "m-conv.y"
    { (yyval.c)='H' ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 511 "m-conv.y"
    { (yyval.c)='I' ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 512 "m-conv.y"
    { (yyval.c)='J' ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 513 "m-conv.y"
    { (yyval.c)='K' ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 514 "m-conv.y"
    { (yyval.c)='L' ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 515 "m-conv.y"
    { (yyval.c)='M' ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 516 "m-conv.y"
    { (yyval.c)='N' ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 517 "m-conv.y"
    { (yyval.c)='O' ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 518 "m-conv.y"
    { (yyval.c)='P' ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 519 "m-conv.y"
    { (yyval.c)='Q' ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 520 "m-conv.y"
    { (yyval.c)='R' ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 521 "m-conv.y"
    { (yyval.c)='S' ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 522 "m-conv.y"
    { (yyval.c)='T' ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 523 "m-conv.y"
    { (yyval.c)='U' ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 524 "m-conv.y"
    { (yyval.c)='V' ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 525 "m-conv.y"
    { (yyval.c)='W' ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 526 "m-conv.y"
    { (yyval.c)='X' ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 527 "m-conv.y"
    { (yyval.c)='Y' ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 528 "m-conv.y"
    { (yyval.c)='Z' ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 529 "m-conv.y"
    { (yyval.c)='$' ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 530 "m-conv.y"
    { (yyval.c)='*' ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 531 "m-conv.y"
    { (yyval.c)='+' ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 532 "m-conv.y"
    { (yyval.c)='-' ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 533 "m-conv.y"
    { (yyval.c)='_' ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 536 "m-conv.y"
    { I_Str = 0; ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 537 "m-conv.y"
    { YY_1String[I_Str++] = 0; ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 538 "m-conv.y"
    { I_Str = 0; ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 539 "m-conv.y"
    { YY_1String[I_Str++] = 0; ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 544 "m-conv.y"
    { YY_1String[I_Str++] = (yyvsp[(1) - (1)].c); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 545 "m-conv.y"
    { YY_1String[I_Str++] = (yyvsp[(1) - (1)].c); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2589 "m-conv.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 547 "m-conv.y"


