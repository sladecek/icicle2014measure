f=fopen("icicle.dat", "r");
da = fread(f, Inf, "uchar");
load("-ascii","icicle_size.txt");
sy = icicle_size(1);
sx = icicle_size(2);
st = size(da)(1)/sx/sy;
d = reshape(da, sy, sx, st);

d=d(1:20:end, 1:20:end, 1:120:end);
iis = size(d)
sy = iis(1)
sx = iis(2)
st = iis(3)

w=reshape(sum(d,2),sy,st);
p = 30;
lambda = 0.01;
size(w)

theta0 = [0 1 0 0]';

tix = linspace(0,1,st);
ttix = tix.*tix;
tttix = ttix.*tix;
tax = [ones(st,1), tix', ttix', tttix'];

v=cumsum(flipud(w));
wt = w .* tix;
wtt = w .* ttix;
wttt = w .* tttix;

pp = p * [1, 1/2, 1/3, 1/4];

function [J, grad] = costFunction(theta, lambda, pp, tax, sy, st, v, w, wt, wtt, wttt)
J = 0;
grad = zeros(size(theta));
l=tax * theta;
li=min(sy,max(1,round(sy*l)));
lix=[0:(st-1)]*sy+li';

J1 = pp * theta;
g1 = pp';

J2 = sum(v(lix));
g2 = zeros(size(theta));
g2(1) = sum(w(lix));
g2(2) = sum(wt(lix));
g2(3) = sum(wtt(lix));
g2(4) = sum(wttt(lix));

temp = theta;
temp(1) = 0;
J3 = lambda / (2 * 4) * temp'*temp;
g3 = lambda * temp / 4;

J = J1 + J2 + J3
grad = g1 + g2 + g3;

endfunction


options = optimset('GradObj', 'on', 'MaxIter', 50);
[theta] = fmincg (@(t)(costFunction(t, lambda, pp, tax, sy, st, v, w, wt, wtt, wttt)), theta0, options);
theta




