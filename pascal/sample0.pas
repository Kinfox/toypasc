program sample0;

Var x : Integer;
{var a, b: Boolean;
var c: char;}

Procedure myproc (i : integer);
var ck : integer;
Begin
    {ck := i * 2 + 3;}
    ck := 2 + 3;
    x := 5 + 5;
    printint(ck);
    println();
    printint(i)
End;

{FunCtion myfunc (i : integer) : integer;
Begin
    myfunc := 539;
    println()
End;

{FunCtion otherfunc (fint : Integer, fchar: Char) : Integer;
var myvar : Boolean;
var c : Char;
Begin
    myvar := 34;
    c := '{';
End;
}

Begin
    {x := 2 + 3;}
    {x := 2 + 3 * 5 + 9;}
    {printint(x);}
    println();
    myproc(7);
    {x := myfunc(65);
    printint(x);
    {printbool(false);
    printchar('\n');
    printchar('W');
    println();
    printint(137);
    println();
    printint(x);}
    println()
End.

