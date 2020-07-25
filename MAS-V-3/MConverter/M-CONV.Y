%token EOS

%union { double d; long l; char c; }
%type <d> YY_Float
%type <l> YY_Int
%type <c> ZCHAR DCHAR

%start DXF_FILE

%%

YY_0EOS
  : '0' EOS
  { YY_CLEAR_FLAGS(); }
  ;
yy_endsec : YY_0EOS 'E''N''D''S''E''C' EOS
  ;
yy_endtab : YY_0EOS 'E''N''D''T''A''B' EOS
  ;
yy_endblk : YY_0EOS 'E''N''D''B''L''K' EOS
  ;
yy_seqend : YY_0EOS 'S''E''Q''E''N''D' EOS
  ;
YY_ENDSEC : yy_endsec | yy_endsec YY_08_ITEM
  ;
YY_ENDTAB : yy_endtab | yy_endtab YY_08_ITEM
  ;
YY_ENDBLK : yy_endblk | yy_endblk YY_08_ITEM
  ;
YY_SEQEND : yy_seqend | yy_seqend YY_08_ITEM | yy_seqend YY_05_ITEM YY_08_ITEM
  ;
DXF_FILE
  : DXF_SECTIONS YY_0EOS 'E''O''F'
  { return(0) }
  ;
DXF_SECTIONS
  : SECTION_INSTANCE
  | DXF_SECTIONS SECTION_INSTANCE
  ;
SECTION_INSTANCE
  : YY_0EOS 'S''E''C''T''I''O''N' EOS '2' EOS DXF_SECTION
  ;
DXF_SECTION
  : 'T''A''B''L''E''S' EOS SECTION_TABLES YY_ENDSEC
  | 'H''E''A''D''E''R' EOS SECTION_HEADER YY_ENDSEC
  | 'B''L''O''C''K''S' EOS SECTION_BLOCKS YY_ENDSEC
  | 'E''N''T''I''T''I''E''S' EOS SECTION_ENTITIES YY_ENDSEC
  ;
SECTION_HEADER
  : YY_COMMON_GROUP
  ;
SECTION_TABLES
  : TABLE_INSTANCE
  | SECTION_TABLES TABLE_INSTANCE
  ;
TABLE_INSTANCE
  : YY_0EOS 'T''A''B''L''E' EOS
  { YY_2String = YY_3String; }
  TABLE_BODY
  ;
TABLE_BODY
  : YY_COMMON_GROUP YY_ENDTAB
  | YY_COMMON_GROUP TABLES_HEAP YY_ENDTAB
  ;
TABLES_HEAP
  : YY_0EOS TABLE_ENTITIE
  | TABLES_HEAP YY_0EOS TABLE_ENTITIE
  ;
TABLE_ENTITIE
  : 'V''P''O''R''T' EOS YY_COMMON_GROUP
  | 'L''T''Y''P''E' EOS YY_COMMON_GROUP
  | 'L''A''Y''E''R' EOS YY_COMMON_GROUP
  | 'S''T''Y''L''E' EOS YY_COMMON_GROUP
  | 'V''I''E''W' EOS YY_COMMON_GROUP
  | 'U''C''S' EOS YY_COMMON_GROUP
  ;
SECTION_BLOCKS
  : BLOCK_INSTANCE
  | SECTION_BLOCKS BLOCK_INSTANCE
  ;
BLOCK_INSTANCE
  : YY_0EOS 'B''L''O''C''K' EOS
  { YY_2String = YY_4String; }
  BLOCK_BODY
  ;
BLOCK_BODY
  : YY_COMMON_GROUP YY_ENDBLK
  | YY_COMMON_GROUP BLOCK_HEAP YY_ENDBLK
  ;
BLOCK_HEAP
  : YY_0EOS BLOCK_ENTITIE
  | BLOCK_HEAP YY_0EOS BLOCK_ENTITIE
  ;
BLOCK_ENTITIE
  : DRAW_ENTITIE
  | 'A''T''T''D''E''F' EOS ATTDEF_ENTITIE
  ;
ATTDEF_ENTITIE
  : { YY_2String = YY_7String; }
  YY_COMMON_GROUP
  ;
SECTION_ENTITIES
  : YY_0EOS DRAW_ENTITIE
  | SECTION_ENTITIES YY_0EOS DRAW_ENTITIE
  ;
DRAW_ENTITIE
  : 'P''O''L''Y''L''I''N''E' EOS PLINE_ENTITIE
  | 'L''I''N''E' EOS LINE_ENTITIE
  | 'P''O''I''N''T' EOS POINT_ENTITIE
  | 'C''I''R''C''L''E' EOS CIRCLE_ENTITIE
  | 'T''E''X''T' EOS TEXT_ENTITIE
  | 'T''R''A''C''E' EOS TRACE_ENTITIE
  | 'S''O''L''I''D' EOS SOLID_ENTITIE
  | 'A''R''C' EOS ARC_ENTITIE
  | 'I''N''S''E''R''T' EOS INSERT_ENTITIE
  | 'S''H''A''P''E' EOS SHAPE_ENTITIE
  ;
YY_01_ITEM : '1' EOS CHARS
  {
  YY_01_FLAG = 1;
  strncpy(YY_6String, YY_1String, 30);
  }
  ;
YY_02_ITEM : '2' EOS CHARS
  {
  YY_02_FLAG = 1;
  strncpy(YY_2String, YY_1String, 30);
  }
  ;
YY_03_ITEM : '3' EOS CHARS
  ;
YY_04_ITEM : '4' EOS CHARS
  ;
YY_05_ITEM : '5' EOS CHARS
  ;
YY_06_ITEM : '6' EOS CHARS
  ;
YY_07_ITEM : '7' EOS CHARS
  ;
YY_08_ITEM : '8' EOS CHARS
  {
  YY_08_FLAG = 1;
  strncpy(YY_5String, YY_1String, 30);
  }
  ;
YY_09_ITEM : '9' EOS CHARS
  ;
YY_10_ITEM : '1''0' EOS YY_Float
  {
  YY_10_FLAG = 1;
  Item_10_Val = $4;
  }
  ;
YY_11_ITEM : '1''1' EOS YY_Float
  {
  YY_11_FLAG = 1;
  Item_11_Val = $4;
  }
  ;
YY_12_ITEM : '1''2' EOS YY_Float
  {
  YY_12_FLAG = 1;
  Item_12_Val = $4;
  }
  ;
YY_13_ITEM : '1''3' EOS YY_Float
  {
  YY_13_FLAG = 1;
  Item_13_Val = $4;
  }
  ;
YY_14_ITEM : '1''4' EOS YY_Float
  ;
YY_15_ITEM : '1''5' EOS YY_Float
  ;
YY_16_ITEM : '1''6' EOS YY_Float
  ;
YY_17_ITEM : '1''7' EOS YY_Float
  ;
YY_20_ITEM : '2''0' EOS YY_Float
  {
  YY_20_FLAG = 1;
  Item_20_Val = $4;
  }
  ;
YY_21_ITEM : '2''1' EOS YY_Float
  {
  YY_21_FLAG = 1;
  Item_21_Val = $4;
  }
  ;
YY_22_ITEM : '2''2' EOS YY_Float
  {
  YY_22_FLAG = 1;
  Item_22_Val = $4;
  }
  ;
YY_23_ITEM : '2''3' EOS YY_Float
  {
  YY_23_FLAG = 1;
  Item_23_Val = $4;
  }
  ;
YY_24_ITEM : '2''4' EOS YY_Float
  ;
YY_25_ITEM : '2''5' EOS YY_Float
  ;
YY_26_ITEM : '2''6' EOS YY_Float
  ;
YY_27_ITEM : '2''7' EOS YY_Float
  ;
YY_30_ITEM : '3''0' EOS YY_Float
  {
  YY_30_FLAG = 1;
  Item_30_Val = $4;
  }
  ;
YY_31_ITEM : '3''1' EOS YY_Float
  {
  YY_31_FLAG = 1;
  Item_31_Val = $4;
  }
  ;
YY_32_ITEM : '3''2' EOS YY_Float
  ;
YY_33_ITEM : '3''3' EOS YY_Float
  ;
YY_34_ITEM : '3''4' EOS YY_Float
  ;
YY_35_ITEM : '3''5' EOS YY_Float
  ;
YY_36_ITEM : '3''6' EOS YY_Float
  ;
YY_37_ITEM : '3''7' EOS YY_Float
  ;
YY_38_ITEM : '3''8' EOS YY_Float
  {
  YY_38_FLAG = 1;
  Item_38_Val = $4;
  }
  ;
YY_39_ITEM : '3''9' EOS YY_Float
  {
  YY_39_FLAG = 1;
  Item_39_Val = $4;
  }
  ;
YY_40_ITEM : '4''0' EOS YY_Float
  {
  YY_40_FLAG = 1;
  Item_40_Val = $4;
  }
  ;
YY_41_ITEM : '4''1' EOS YY_Float
  {
  YY_41_FLAG = 1;
  Item_41_Val = $4;
  }
  ;
YY_42_ITEM : '4''2' EOS YY_Float
  {
  YY_42_FLAG = 1;
  Item_42_Val = $4;
  }
  ;
YY_43_ITEM : '4''3' EOS YY_Float
  {
  YY_43_FLAG = 1;
  Item_43_Val = $4;
  }
  ;
YY_44_ITEM : '4''4' EOS YY_Float
  ;
YY_48_ITEM : '4''8' EOS YY_Float
  ;
YY_49_ITEM : '4''9' EOS YY_Float
  ;
YY_50_ITEM : '5''0' EOS YY_Float
  {
  YY_50_FLAG = 1;
  Item_50_Val = $4;
  }
  ;
YY_51_ITEM : '5''1' EOS YY_Float
  {
  YY_51_FLAG = 1;
  Item_51_Val = $4;
  }
  ;
YY_62_ITEM : '6''2' EOS YY_Int
  {
  YY_62_FLAG = 1;
  Item_62_Val = $4;
  }
  ;
YY_66_ITEM : '6''6' EOS YY_Int
  {
  YY_66_FLAG = 1;
  Item_66_Val = $4;
  }
  ;
YY_70_ITEM : '7''0' EOS YY_Int
  {
  YY_70_FLAG = 1;
  Bit_70_01 = $4 & 01;
  Bit_70_02 = $4 & 02;
  Bit_70_04 = $4 & 04;
  Bit_70_08 = $4 & 8L;
  Bit_70_16 = $4 & 16;
  }
  ;
YY_71_ITEM : '7''1' EOS YY_Int
  {
  YY_71_FLAG = 1;
  Bit_71_01 = $4 & 01;
  Bit_71_02 = $4 & 02;
  Bit_71_04 = $4 & 04;
  Bit_71_08 = $4 & 8L;
  Bit_71_16 = $4 & 16;
  }
  ;
YY_72_ITEM : '7''2' EOS YY_Int
  {
  YY_72_FLAG = 1;
  Item_72_Val = $4;
  }
  ;
YY_73_ITEM : '7''3' EOS YY_Int
  ;
YY_74_ITEM : '7''4' EOS YY_Int
  ;
YY_75_ITEM : '7''5' EOS YY_Int
  ;
YY_76_ITEM : '7''6' EOS YY_Int
  ;
YY_77_ITEM : '7''7' EOS YY_Int
  ;
YY_78_ITEM : '7''8' EOS YY_Int
  ;
YY210_ITEM : '2''1''0' EOS YY_Float
  {
  YY210_FLAG = 1;
  Item210_Val = $5;
  }
  ;
YY220_ITEM : '2''2''0' EOS YY_Float
  {
  YY220_FLAG = 1;
  Item220_Val = $5;
  }
  ;
YY230_ITEM : '2''3''0' EOS YY_Float
  {
  YY230_FLAG = 1;
  Item230_Val = $5;
  }
  ;
YY_COMMON_ITEM
  : YY_01_ITEM
  | YY_02_ITEM
  | YY_03_ITEM
  | YY_04_ITEM
  | YY_05_ITEM
  | YY_06_ITEM
  | YY_07_ITEM
  | YY_08_ITEM
  | YY_09_ITEM
  | YY_10_ITEM
  | YY_11_ITEM
  | YY_12_ITEM
  | YY_13_ITEM
  | YY_14_ITEM
  | YY_15_ITEM
  | YY_16_ITEM
  | YY_17_ITEM
  | YY_20_ITEM
  | YY_21_ITEM
  | YY_22_ITEM
  | YY_23_ITEM
  | YY_24_ITEM
  | YY_25_ITEM
  | YY_26_ITEM
  | YY_27_ITEM
  | YY_30_ITEM
  | YY_31_ITEM
  | YY_32_ITEM
  | YY_33_ITEM
  | YY_34_ITEM
  | YY_35_ITEM
  | YY_36_ITEM
  | YY_37_ITEM
  | YY_38_ITEM
  | YY_39_ITEM
  | YY_40_ITEM
  | YY_41_ITEM
  | YY_42_ITEM
  | YY_43_ITEM
  | YY_44_ITEM
  | YY_48_ITEM
  | YY_49_ITEM
  | YY_50_ITEM
  | YY_51_ITEM
  | YY_62_ITEM
  | YY_66_ITEM
  | YY_70_ITEM
  | YY_71_ITEM
  | YY_72_ITEM
  | YY_73_ITEM
  | YY_74_ITEM
  | YY_75_ITEM
  | YY_76_ITEM
  | YY_77_ITEM
  | YY_78_ITEM
  | YY210_ITEM
  | YY220_ITEM
  | YY230_ITEM
  ;
YY_COMMON_GROUP
  : YY_COMMON_ITEM
  | YY_COMMON_GROUP YY_COMMON_ITEM
  ;
LINE_ENTITIE   : YY_COMMON_GROUP
  ;
POINT_ENTITIE  : YY_COMMON_GROUP
  ;
ARC_ENTITIE    : YY_COMMON_GROUP
  ;
SOLID_ENTITIE  : YY_COMMON_GROUP
  ;
SHAPE_ENTITIE  : YY_COMMON_GROUP
  ;
TEXT_ENTITIE   : YY_COMMON_GROUP
  ;
TRACE_ENTITIE  : YY_COMMON_GROUP
  ;
INSERT_ENTITIE
  : { YY_2String = YY_4String; }
  INSERT_BODY
  { Terminate_Insert(); }
  ;
INSERT_BODY
  : YY_COMMON_GROUP YY_SEQEND
  | YY_COMMON_GROUP ATTRIB_HEAP YY_SEQEND
  ;
ATTRIB_HEAP
  : ATTRIB_ENTITIE
  | ATTRIB_HEAP ATTRIB_ENTITIE
  ;
ATTRIB_ENTITIE
  : YY_0EOS 'A''T''T''R''I''B' EOS
  { YY_2String = YY_7String; }
  YY_COMMON_GROUP
  ;
YY_VERTEX
  : YY_0EOS 'V''E''R''T''E''X' EOS YY_COMMON_GROUP
  { Terminate_Vertex(); }
  ;
YY_VERTEXES
  : YY_VERTEX
  | YY_VERTEXES YY_VERTEX
  ;
PLINE_ENTITIE
  : YY_COMMON_GROUP
  {
  VERLIM = 1;
  YY_Interpolate = 0;

  Closed_Polygon = YY_70_FLAG ? Bit_70_01 :0;

  ZZ_START_WIDTH = YY_40_FLAG ? Item_40_Val :0;
  ZZ_ENDIN_WIDTH = YY_41_FLAG ? Item_41_Val :0;

  MAX_WIDTH = ZZ_START_WIDTH;
  if(MAX_WIDTH<ZZ_ENDIN_WIDTH)
  MAX_WIDTH = ZZ_ENDIN_WIDTH;
  }
  YY_VERTEXES YY_SEQEND
  { Terminate_Poly_1(); }
  ;
CIRCLE_ENTITIE
  : YY_COMMON_GROUP
  { Terminate_Circle(); }
  ;
YY_Float
  : { yylex = YY_FLOAT_LEX; } EOS { yylex = DXFyylex; $$ = YY_F_VAL; }
  ;
YY_Int
  : { yylex = YY_INT_LEX; } EOS { yylex = DXFyylex; $$ = YY_I_VAL; }
  ;
DCHAR
  :  '0' { $$='0' }
  |  '1' { $$='1' }
  |  '2' { $$='2' }
  |  '3' { $$='3' }
  |  '4' { $$='4' }
  |  '5' { $$='5' }
  |  '6' { $$='6' }
  |  '7' { $$='7' }
  |  '8' { $$='8' }
  |  '9' { $$='9' }
  ;
ZCHAR
  : 'A' { $$='A' }
  | 'B' { $$='B' }
  | 'C' { $$='C' }
  | 'D' { $$='D' }
  | 'E' { $$='E' }
  | 'F' { $$='F' }
  | 'G' { $$='G' }
  | 'H' { $$='H' }
  | 'I' { $$='I' }
  | 'J' { $$='J' }
  | 'K' { $$='K' }
  | 'L' { $$='L' }
  | 'M' { $$='M' }
  | 'N' { $$='N' }
  | 'O' { $$='O' }
  | 'P' { $$='P' }
  | 'Q' { $$='Q' }
  | 'R' { $$='R' }
  | 'S' { $$='S' }
  | 'T' { $$='T' }
  | 'U' { $$='U' }
  | 'V' { $$='V' }
  | 'W' { $$='W' }
  | 'X' { $$='X' }
  | 'Y' { $$='Y' }
  | 'Z' { $$='Z' }
  | '$' { $$='$' }
  | '*' { $$='*' }
  | '+' { $$='+' }
  | '-' { $$='-' }
  | '_' { $$='_' }
  ;
CHARS
  : { I_Str = 0; } EOS
  { YY_1String[I_Str++] = 0; }
  | { I_Str = 0; } XCHAR EOS
  { YY_1String[I_Str++] = 0; }
  ;
XCHAR : SCHAR | XCHAR SCHAR
  ;
SCHAR
  : ZCHAR { YY_1String[I_Str++] = $1; }
  | DCHAR { YY_1String[I_Str++] = $1; }
  ;
%%
