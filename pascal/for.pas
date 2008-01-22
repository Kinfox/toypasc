program forprog;

var i, j: Integer;

begin
    j := 0;

    for i := 0 to 10 do
        println();

    for i := 0 to 10 do
    begin
        j := j * 5 + 1;
        printint(j)
    end;

    println()
end.

