var x1, >= 0;
var x2, >= 0;
var x3, >= 0;
var x4, >= 0;

minimize

value: 2 * x1 + x2 + 2 * x3 + 8 * x4; 

subject to

y1: 2 * x1 - x2 + 3 * x3 - 2 * x4 = 3;

y2: -x1 + 3 * x2 - 4 * x3 = 3;

end;

/* eof */
