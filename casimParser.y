%{
/* Yacc-Source for the CA-Description Language */

#include <stdio.h>
#include <QDebug>
#include "Queue.h"
#include "AutomataFactory.h"
#include "Expression.h"

AutomataFactory *factory;
Queue *qFunc;

extern int casimScannerlex();
extern void casimParsererror(QString errStr);
int casimParserlex();

%}

%error-verbose
%union {
	char string[1024];    /* string buffer */
	int num;              /* number buffer */
	unsigned char symbol; /* symbol buffer */
}

%token <string> STRING FILENAME
%token <num> NUMBER 
%token <symbol> SYMBOL
%token AUTOMATA AUTOMATA1D ARRAY STATES NEIGHBOURS PARAMETERS RULES OBSERVERS
%token AGENT
%token STEP END COLOR PICTURE CHAR TMACHINE CREATE NUM
%token NEUMANN MOORE POINT RADIUS MARGOLUS EIGHT FOUR PLANARMAP
%token CYCLIC PLANE REFLEX SOLID CHECKER RANDOM CLOSED OPEN GRIDCOLOR
%token SQUARE LOAD ASCII INIT_RANDOM ABSOLUT
%token FIELDS CHANGES
%token MOD LT GT LE RND STATE AND OR XOR SUM ABS
%token MOVETO IF OTHERWISE STOP
%token MAX TIME NSTATES NCELLS MRAND ARAND CELL CCW CW OPP
%token TRIGHT TLEFT TSTRAIGHT TTURN

%left AND OR XOR
%left LT LE GT GE EQ NE
%left '-' '+' MOD
%left '*' '/'
%left RND STATE SUM ABS
%left '^'

%type <num> likelyhood border
%type <num> tnum

%start automata 

%%
automata: 
		TMACHINE tm
	|	ca
	|	AGENT agent
	;

ca:
		ca_name gridmap states neighbourhood parameters rules observers
		{ factory->lastSettings (); }
	|	ca_name planarmap states parameters rules observers
		{ factory->lastSettings (); }
	;

tm:
		tm_name gridmap states parameters t_rules observers
		{ factory->lastSettings (); }
	;

ca_name: 
        AUTOMATA STRING { factory->setAutomataName($2); factory->setAutomataType(CAMODEL);}
    |	AUTOMATA1D STRING {factory->setAutomataName($2); factory->setAutomataType(CA1DMODEL); }
	;

tm_name:
		STRING 
        { factory->setAutomataName($1); factory->setAutomataType(TURINGMODEL); }
	;

agent:
		agent_name gridmap states parameters
		{ factory->lastSettings (); }
	;

agent_name:
		STRING
        { factory->setAutomataName ($1); factory->setAutomataType(AGENTMODEL); }
	;
	
gridmap: 
		ARRAY NUMBER ',' NUMBER steps { factory->setArray ($2,$4); }
	;

planarmap:
		PLANARMAP FILENAME             { factory->addPlanarmap ($2);}
	;

steps:
	|	STEP NUMBER ',' NUMBER { factory->setSteps ($2,$4); }
	;

states:
		STATES state_description END 
	;

state_description:
	|	state_description normal_states
	;

normal_states: 
        STRING ':' STRING likelyhood                        { factory->addState (4,$1,'-',$3,$4,""); }
    |	STRING ':' PICTURE  FILENAME likelyhood             { factory->addState (1,$1,'-',$4,$5,""); }
    |	STRING ':' CHAR SYMBOL STRING STRING likelyhood		{ factory->addState (2,$1,$4,$5,$7,$6); }
    |	STRING ':' CHAR SYMBOL likelyhood ';'               { factory->addState (5,$1,$4,"",$5,""); }
    |	STRING ':' NUM likelyhood                           { factory->addState (3,$1,'-',"",$4,""); }
    |	CREATE NUMBER STRING ':' COLOR NUMBER likelyhood 	{ factory->create_states (0,$2,$3,$6,$7); }
    |	CREATE NUMBER STRING ':' NUM likelyhood     		{ factory->create_states (1,$2,$3,0,$6); }
	;

likelyhood:      { $$=0; }
	|	',' NUMBER { $$=$2; }
	;

neighbourhood: 
		NEIGHBOURS neighbour_description END 
	;

neighbour_description:
	|	neighbour_description neighbour 
	;

neighbour:
		NEUMANN NUMBER                     { factory->addNeumannNeighborhood ($2);    }
	|	MOORE NUMBER                       { factory->addMooreNeighborhood ($2);      }
	|	POINT NUMBER ',' NUMBER            { factory->addPoint ($2,$4);   }
	|	POINT '-' NUMBER ',' NUMBER        { factory->addPoint (-$3,$5);  }
	|	POINT NUMBER ',' '-' NUMBER        { factory->addPoint ($2,-$5);  }
	|	POINT '-' NUMBER ',' '-' NUMBER    { factory->addPoint (-$3,-$6); }
	|	RADIUS NUMBER                      { factory->addRadiusNeighborhood ($2);     }
	|	MARGOLUS                           { factory->addMargolusNeighborhood ();     }
	;

parameters: 
	|	PARAMETERS parameter_description END 
	;

parameter_description:
	|	parameter_description parameter_settings
	|	parameter_description field_initialize
	|	parameter_description variable_setting
	;

parameter_settings:
		CYCLIC         { factory->setBorderType(0, 0); }
	|	PLANE border   { factory->setBorderType(1,$2); }
	|	REFLEX         { factory->setBorderType(2, 0); }
	|	SOLID          { factory->setParameter("NEWCELLMETHOD",0); }
	|	CHECKER        { factory->setParameter("NEWCELLMETHOD",1); }
	|	RANDOM         { factory->setParameter("NEWCELLMETHOD",2); }
	|	OPEN           { factory->setParameter("ADDCELL",0); }
	|	CLOSED         { factory->setParameter("ADDCELL",1); }
	|	GRIDCOLOR STRING { factory->add_gridcolor ($2); }
	|	ABSOLUT        { factory->setParameter ("DIRTYPE", 0); }
	|	FOUR           { factory->addTuringFour(); }
	|	EIGHT          { factory->addTuringEight(); }
	;	

border:      { $$=0; }
	|	NUMBER { $$=$1; }
	;

field_initialize:
		SQUARE NUMBER ',' NUMBER ',' NUMBER ',' NUMBER
			{ factory->setFieldSquare ($2,$4,$6,$8); }
	|	LOAD FILENAME    { factory->loadField ($2, false); }
	|  ASCII FILENAME   { factory->loadField ($2, true); }
	|	INIT_RANDOM      { factory->randomField (); }
	;

variable_setting:
		STRING EQ NUMBER
			{ factory->setVariable($1,$3); }
	;

rules: 
		RULES rule_description END 
	;

t_rules:
		RULES turing_rules END
	;

rule_description:
	|	rule_description normal_rule ';'
	|	rule_description moveto_rule ';'
	;

normal_rule:
                function condition
	;

moveto_rule:
		MOVETO funct_1 ',' function ',' funct_2 condition 
                        { factory->setParameter("MOVEABLECELLS", 1); }
	;


function:
                expression { factory->setFunction(RP_ACTION, qFunc); }
	;

funct_1:
                expression { factory->setFunction(RP_MOVE1, qFunc); }
	;

funct_2:
                expression { factory->setFunction(RP_MOVE2, qFunc); }
	;

condition:
                IF expression {	factory->setFunction(RP_CONDITION, qFunc); }
		  |       OTHERWISE { qFunc->enqueue(2,1); factory->setFunction(RP_CONDITION, qFunc); }
	;


expression: 
		expression AND expression    { qFunc->enqueue (3, 0); }
	|	expression OR expression     { qFunc->enqueue (3, 1); }
	|	expression XOR expression    { qFunc->enqueue (3, 2); }
	|	expression LE expression     { qFunc->enqueue (3, 3); }
	|	expression LT expression     { qFunc->enqueue (3, 4); }
	|	expression EQ expression     { qFunc->enqueue (3, 5); }
	|	expression NE expression     { qFunc->enqueue (3, 6); }
	|	expression GT expression     { qFunc->enqueue (3, 7); }
	|	expression GE expression     { qFunc->enqueue (3, 8); }
	|	expression '+' expression    { qFunc->enqueue (3, 9); }
	|	expression '-' expression    { qFunc->enqueue (3,10); }
	|	expression MOD expression    { qFunc->enqueue (3,11); }
	|	expression '*' expression    { qFunc->enqueue (3,12); }
	|	expression '/' expression    { qFunc->enqueue (3,13); }
	|	RND  '(' expression ')'      { qFunc->enqueue (3,14); }
	|	STATE '(' expression ')'     { qFunc->enqueue (3,15); }
	|	CELL                         { qFunc->enqueue (2,0); qFunc->enqueue (3,15); }
	|	CCW                          { qFunc->enqueue (2,3); qFunc->enqueue (3,15); }
	|	CW                           { qFunc->enqueue (2,1); qFunc->enqueue (3,15); }
	|	OPP                          { qFunc->enqueue (2,2); qFunc->enqueue (3,15); }
	|	'[' expression ']'           { qFunc->enqueue (3,15); }
	|	SUM '(' expression ')'       { qFunc->enqueue (3,16); }
	|	'|' expression '|'           { qFunc->enqueue (3,16); }
	|	ABS '(' expression ')'       { qFunc->enqueue (3,17); }
	|	expression '^' expression    { qFunc->enqueue (3,18); }
	|	'(' expression ')'           
	|	NUMBER                       { qFunc->enqueue (2,$1);}
	|	STRING                       { factory->addStateOrVariable ($1, qFunc); }
	|	MAX                          { qFunc->enqueue (1,factory->setVariable("MAX",0));}
	|	TIME                         { qFunc->enqueue (1,factory->setVariable("TIME",0));}
	|	NSTATES                      { qFunc->enqueue (1,factory->setVariable("NSTATES",0));}
	|	NCELLS                       { qFunc->enqueue (1,factory->setVariable("NCELLS",0));}
	|	MRAND                        { qFunc->enqueue (1,factory->setVariable("MRAND",0));}
	|	ARAND                        { qFunc->enqueue (1,factory->setVariable("ARAND",0));}
	;

turing_rules:
	|	turing_rules tnum ',' tnum ',' tnum ',' tnum ',' tnum 
		{ factory->add_turing_rule ( $2,$4,$6,$8,$10); }
	|	turing_rules tnum ',' tnum ',' '-' tnum ',' tnum ',' tnum 
		{ factory->add_turing_rule ( $2,$4,(-1)*$7,$9,$11); }
	|	turing_rules tnum ',' tnum ',' tnum ',' tnum ',' '-' tnum 
		{ factory->add_turing_rule ( $2,$4,$6,$8,(-1)*$11); }
        |       turing_rules tnum ',' tnum ',' STOP
                { factory->add_turing_rule ( $2,$4,-1,0,0); }
	;

tnum:
		NUMBER {$$=$1;}
        |       TRIGHT {$$=1; }
        |	TLEFT  {$$=-1; }
        |       TSTRAIGHT {$$=0; }
        |       TTURN  { $$=-2; }
        |       STRING {$$=factory->get_statenumber ($1);}
        ;

observers:
	| OBSERVERS observer_description END
	;

observer_description:
	|	observer_description STATES   { factory->set_observer (1); }
	|	observer_description FIELDS   { factory->set_observer (2); }
	|	observer_description CHANGES  { factory->set_observer (3); }
	;
%%

int casimParserlex()
{
	return casimScannerlex();
}

int parseAutomataFile (QString name, AutomataFactory *f)
{
	int vRet = 0;
	extern FILE *casimScannerin;

	factory = f;
	qFunc = new Queue();
	casimScannerin = fopen(name.toLatin1(),"r");
	if (casimScannerin != NULL)
	{
            casimParserparse ();
				vRet = 1;
	}


	delete qFunc;
	return vRet;
}
