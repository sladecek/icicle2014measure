

load("-ascii","av.txt");
load("-ascii","at.txt");
load("-ascii","aa.txt");

b = zeros(length(aa),1);
for i = 1:length(aa)
 it = lookup(at, aa(i,3));
 y = aa(i,1);
 v = av(it, y);
 b(i,1) = v;
endfor


