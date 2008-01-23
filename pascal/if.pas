program ifprog;

Var i: integer;
var foo, bar : Boolean;

begin
    foo := true;
    bar := false;
    i := 1;

    if i * 204 > 5012 then
    begin
        printchar('O');
        printchar('K');
        println()
    end
    else
    begin
        printbool(true);
        printbool(false)
    end;

    if ((1 = 1) or (2 = 3)) and (foo = not bar) then
    begin
        printchar('t');
        if bar then
            printchar ('B')
    end;

    if true then
        if false then
            printchar('1')
    else
        printchar('2');

    printbool(false)
end.
