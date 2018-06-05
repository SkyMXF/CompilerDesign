#include "SEULex.hpp"

int main(int argc, char* argv[])
{
    SEULex lex;

    /****build inputStream****/
    lex.inFile.open("exampleinput.lex");
    if(!lex.inFile.is_open())
    {
        cout<<"Cannot open input file.\n";
        return 0;
    }
    /**************************/
    
    /****build outputStream****/
    lex.outhppFile.open("out/mxf_Scanner.hpp");
    if(!lex.outhppFile.is_open())
    {
        cout<<"Cannot build or open hpp file.\n";
        lex.inFile.close();
        return 0;
    }
    lex.outcppFile.open("out/mxf_Scanner.cpp");
    if(!lex.outcppFile.is_open())
    {
        cout<<"Cannot build or open cpp file.\n";
        lex.inFile.close();
        lex.outhppFile.close();
        return 0;
    }
    lex.outmainFile.open("out/main.cpp");
    if(!lex.outmainFile.is_open())
    {
        cout<<"Cannot build or open main file.\n";
        lex.inFile.close();
        lex.outhppFile.close();
        lex.outcppFile.close();
        return 0;
    }
    /**************************/

    lex.run();

    lex.inFile.close();
    lex.outhppFile.close();
    lex.outcppFile.close();
    lex.outmainFile.close();

    return 0;
}
