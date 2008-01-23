program sample0;

Var x : Integer;
var a, b: Boolean;
var c: char;

Procedure myproc (i : integer);
Begin
    printint(i)
End;

FunCtion myfunc (i : integer) : integer;
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
    {x := 2;}
    {x := 2 + 3 * 5 + 9;}
    printint(137);
    println();
    myproc(1);
    x := myfunc(65);
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

