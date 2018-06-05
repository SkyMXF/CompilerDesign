%{  
int wordCounter;  
%}  
%x COMMENT
nondigit    ([_A-Za-z])
digit       ([0-9])
integer     ({digit}+)
identifier  ({nondigit}({nondigit}|{digit})*)
blank_chars ([ \f\r\t\v]+)
%%
{identifier}    { /*do something*/ }
{integer}       { /*do something*/ }
{blank_chars}   { /*do nothing*/ }
\n            { /*do nothing*/ }
"//"          { BEGIN COMMENT; }
.             { /*do something*/ }
<COMMENT>\n   { BEGIN INITIAL; }
<COMMENT>.    { /*do nothing*/ }
%%
 void main(int argc, char *argv[])
 {
    if(argc != 3)
    {
        printf("Need an input file name and an output file name.\n");
        exit(0);
    }

    yyin = fopen(argv[1],"rt+");
    if(yyin == NULL)
    {
        printf("Can't open file \"argv[1]\"\n");
        exit(0);
    }
    yyout = fopen(argv[2],"wt+");
    if(yyout == NULL)
    {
        printf("Can't open or build file \"argv[2]\"\n");
        fclose(yyin);
        exit(0);
    }

    yylex(); /* start the analysis*/
    printf(" No of words:%d\n", wordCount);

    fclose(yyin);
    fclose(yyout);
    return 0;
 }