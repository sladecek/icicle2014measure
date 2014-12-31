% icicle length from movie
% experiment 541
% --------------------------------------------------------------------------------------
f=fopen("icicle.dat", "r");
da = fread(f, Inf, "uchar");
load("-ascii","icicle_size.txt");
sy = icicle_size(1);
sx = icicle_size(2);
st = size(da)(1)/sx/sy;
d = reshape(da, sy, sx, st);

% --- debug - make movie smaller ------------------------------------------------------
if false
d=d(1:20:end, 1:20:end, 1:120:end);
iis = size(d)
sy = iis(1)
sx = iis(2)
st = iis(3)
endif
% --------------------------------------------------------------------------------------

w=reshape(sum(d,2),sy,st);
p = 500;
lambda = 5000;
size(w)

theta0 = [0 1 0 0]';

tix = linspace(0,1,st);
ttix = tix.*tix;
tttix = ttix.*tix;
tax = [ones(st,1), tix', ttix', tttix'];


v=flipud(cumsum(flipud(w)));
wt = w .* tix;
wtt = w .* ttix;
wttt = w .* tttix;

pp = p * [1, 1/2, 1/3, 1/4];

% --------------------------------------------------------------------------------------
function [J, grad] = costFunction(theta, lambda, pp, tax, sy, st, v, w, wt, wtt, wttt, dbg1, dbg2, dbg3)
J = 0;
grad = zeros(size(theta));
l=min(1,max(0,tax * theta));
li=min(sy,max(1,round(sy*l)));
lix=[0:(st-1)]*sy+li';

J1 = pp(1) * sum(l);
g1 = sum(+(l >= 0))*pp';


J2 = sum(v(lix));
g2 = zeros(size(theta));
g2(1) = -sum(w(lix));
g2(2) = -sum(wt(lix));
g2(3) = -sum(wtt(lix));
g2(4) = -sum(wttt(lix));


temp = theta;
temp(1) = 0;
J3 = lambda / (2 * 4) * temp'*temp;
g3 = lambda * temp / 4;

J = dbg1*J1 + dbg2*J2 +dbg3*J3;
grad = dbg1*g1 + dbg2*g2 +dbg3*g3;

endfunction
% --------------------------------------------------------------------------------------

if true
options = optimset('GradObj', 'off', 'MaxIter', 500);
[theta] = fmincg (@(t)(costFunction(t, lambda, pp, tax, sy, st, v, w, wt, wtt, wttt, 1, 1, 1)), theta0, options);
theta
endif

% -debug plot cost function ------------------------------------------------------------
nn0 = 30;
nn1 = 30;
th0 = linspace(-2, 1, nn0);
th1 = linspace(-1, 2, nn1);

pl=zeros(nn0,nn1);
for t0=1:nn0
 for t1=1:nn1
   theta = zeros(4,1);
   theta(1) = th0(t0);
   theta(2) = th1(t1);
   pl(t0,t1) = costFunction(theta, lambda, pp, tax, sy, st, v, w, wt, wtt, wttt, 1, 1, 1);
endfor
endfor
meshc(th0, th1, pl);
xlabel("theta_0");
ylabel("theta_1");
title("cost function");
% --------------------------------------------------------------------------------------

