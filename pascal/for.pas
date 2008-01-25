program forprog;

procedure forme();
var i, j;
begin
    j := 0;

    for i := 0 to 10 do
    begin
        j := j * 5 + 1;
        printint(j);
        println()
    end

end;

begin
    forme()
end.
