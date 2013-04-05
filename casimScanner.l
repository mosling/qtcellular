%{
#include "casimParser_yacc.h"
#include <QDebug>
#include <QString>

int lineno=1;
char linebuf[1024];

%}

bom     \xEF\xBB\xBF

u2a     [\xC2-\xDF][\x80-\xBF]
u2b     \xE0[\xA0-\xBF][\x80-\xBF]
u3a     [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}
u3b     \xED[\x80-\x9F][\x80-\xBF]
u4a     \xF0[\x90-\xBF][\x80-\xBF]{2}
u4b     [\xF1-\xF3][\x80-\xBF]{3}
u4c     \xF4[\x80-\x8F][\x80-\xBF]{2}

utf_8   {u2a}|{u2b}|{u3a}|{u3b}|{u4a}|{u4b}|{u4c}

file       \".+\"
id         [a-zA-Z][a-zA-Z0-9_]*
number     [0-9]+
symbol     \'.\'
comment    --.*
ws         [ \t]+

%o 4000
%a 3500

%%
\n.*       { strcpy (linebuf, yytext+1); lineno++; yyless(1); }
{bom}      { qDebug() << "UTF-8 BOM detected"; putchar('B');  }
{utf_8}    { qDebug() << "UTF-8 Sign detected"; putchar('u');  }
END        { return END; }
AUTOMATA   { return AUTOMATA; }
"1DIM-CA"  { return AUTOMATA1D; }
TURING     { return TMACHINE; }
AGENT      { return AGENT; }
ARRAY      { return ARRAY; }
PLANARMAP  { return PLANARMAP; }
STEP       { return STEP; }
STATES     { return STATES; }
PARAMETERS { return PARAMETERS; }
NEIGHBOURS |
NEIGHBORS  { return NEIGHBOURS; }
RULES      { return RULES; }
OBSERVERS  { return OBSERVERS; }
Moore      { return MOORE; }
Neumann    { return NEUMANN; }
Margolus   { return MARGOLUS; }
Point      { return POINT; }
Radius     { return RADIUS; }
Eight      { return EIGHT; }
Four       { return FOUR; }
Add |
Closed     { return CLOSED; }
Sub |
Open       { return OPEN; }
GridColor  { return GRIDCOLOR; }
ShowNum    { return NUM; }
Color |
Colour     { return COLOR; }
Picture    { return PICTURE; }
Char       { return CHAR; }
Create     { return CREATE; }
Square     { return SQUARE; }
Load       { return LOAD; }
Ascii      { return ASCII; }
Init_Rnd   { return INIT_RANDOM; }
Cyclic     { return CYCLIC; }
Plane      { return PLANE;}
Reflex     { return REFLEX; }
Solid      { return SOLID; }
All        { return SOLID; }
Absolut    { return ABSOLUT; }
Fields     { return FIELDS; }
Changes    { return CHANGES; }
OTHERWISE  { return OTHERWISE; }
CELL       { return CELL; }
CCW        { return CCW; }
CW         { return CW; }
OPP        { return OPP; }
STOP       { return STOP; }
TRIGHT     { return TRIGHT; }
TLEFT      { return TLEFT; }
TSTRAIGHT  { return TSTRAIGHT; }
TTURN      { return TTURN; }
MAX        { return MAX; }
TIME       { return TIME; }
NSTATES    { return NSTATES; }
NCELLS     { return NCELLS; }
MRAND      { return MRAND; }
ARAND      { return ARAND; }
STATE      { return STATE; }
SUM        { return SUM; }
RND        { return RND; }
ABS        { return ABS; }
(MOD|"%")  { return MOD; }
AND        { return AND; }
OR         { return OR; }
XOR        { return XOR; }
(GT|">")   { return GT; }
(GE|">=")  { return GE; }
(LT|"<")   { return LT; }
(LE|"<=")  { return LE; }
(EQ|"=")   { return EQ; }
(NE|"<>"|"/="|"!=") { return NE; }
IF         { return IF; }
MOVETO     { return MOVETO; }
{ws}       ; /* ignore whitspaces */
{comment}  ; /* comment */
{number}   { casimParserlval.num = atoi (yytext); return NUMBER;}
{id}       { strcpy(casimParserlval.string, yytext); return STRING; }
{file}     { strcpy(casimParserlval.string, yytext); return FILENAME; }
{symbol}   { casimParserlval.symbol = yytext[1]; return SYMBOL; }
.          { return yytext[0]; }
%%

/*!
Fehler ausgeben und beenden.
*/
void casimParsererror (QString s)
{
        qDebug() << "Error at line " << lineno << ": " << s << " at "
                 << QString(yytext) << " in this line: '" << linebuf << "'";
	exit (2) ;
}

int casimScannerwrap(void)
{
	return 1;
}


