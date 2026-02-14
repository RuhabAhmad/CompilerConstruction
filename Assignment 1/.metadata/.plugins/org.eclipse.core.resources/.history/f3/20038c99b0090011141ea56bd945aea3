%%

%class Yylex
%public
%unicode
%line
%column

%{

import tokens.Token;
import tokens.TokenType;

%}

DIGIT = [0-9]
LETTER = [A-Z]

%%

"start"        { System.out.println("KEYWORD: start"); }
"finish"       { System.out.println("KEYWORD: finish"); }

{DIGIT}+       { System.out.println("INTEGER: " + yytext()); }

[ \t\r\n]+     { /* skip whitespace */ }

.              { System.out.println("INVALID: " + yytext()); }
