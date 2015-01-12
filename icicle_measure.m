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

load("-ascii","icicle_time.txt");
iti=icicle_time - icicle_time(1);
tix = iti / max(iti);
% --- debug - make movie smaller ------------------------------------------------------
if false
%d=d(1:20:end, 1:20:end, 1:120:end);
d=d(1:5:end, 1:5:end, 1:30:end);
iis = size(d)
sy = iis(1)
sx = iis(2)
st = iis(3)
endif
% --------------------------------------------------------------------------------------

www=reshape(sum(d,2),sy,st);
penalty0 = mean(mean(www(:,1:3)));
ww = min(2*penalty0,www);
penalty = mean(mean(ww));
w = ww - penalty;
lambda = 0 * sx*sy*st;
kappa = 0*sx*sy*st;

theta0 = [0 1 0 0]';


ttix = tix.*tix;
tttix = ttix.*tix;
tax = [ones(st,1), tix', ttix', tttix'];


v=flipud(cumsum(flipud(w))) - cumsum(w);
wt = w .* tix;
wtt = w .* ttix;
wttt = w .* tttix;



% --------------------------------------------------------------------------------------
function [J, grad] = costFunction(theta, lambda,  kappa, tax, sy, st, v, w, wt, wtt, wttt)
J = 0;
grad = zeros(size(theta));
l=min(1,max(0,tax * theta));
li=min(sy,max(1,round(sy*l)));
lix=[0:(st-1)]*sy+li';


J1 = sum(v(lix));
g1 = zeros(size(theta));
g1(1) = -2*sum(w(lix))*sy;
g1(2) = -2*sum(wt(lix))*sy;
g1(3) = -2*sum(wtt(lix))*sy;
g1(4) = -2*sum(wttt(lix))*sy;

temp = theta;
temp(1) = 0;
J2 = lambda / (2 * 4) * temp'*temp;
g2 = lambda * temp / 4;

J3 = kappa * ( theta(1)^2 + (sum(theta) - 1)^2);
g3 = 2 * kappa * (theta + [theta(1),0,0,0]');

J = J1 + J2 + J3;
grad = g1 + g2 + g3;

endfunction
% --------------------------------------------------------------------------------------

if true
options = optimset('GradObj', 'on', 'MaxIter', 50);
[theta] = fminunc (@(t)(costFunction(t, lambda, kappa,  tax, sy, st, v, w, wt, wtt, wttt)), theta0, options);

icicle_length=min(1,max(0,tax * theta));
save("-ascii","icicle_length.txt", "icicle_length");

l1 = [0; icicle_length];
l2 = [icicle_length; icicle_length(end)];
t1 = [0; tix'];
t2 = [tix'; 1];
icicle_speed = ((l2-l1)./(t2-t1))(1:st);
plot(icicle_speed);
save("-ascii","icicle_speed.txt", "icicle_speed");



% plot(l)
endif

% -debug plot cost function ------------------------------------------------------------
if false
nn0 = 30;
th0 = linspace(-2, 2, nn0);

pl=zeros(nn0);
for t0=1:nn0
   theta = zeros(4,1);
   theta(1) = th0(t0);
   pl(t0) = costFunction(theta, lambda, kappa,  tax, sy, st, v, w, wt, wtt, wttt);
endfor
plot(th0, pl);
ylabel("theta_0");
title("cost function");
endif
% --------------------------------------------------------------------------------------

% -debug plot cost function ------------------------------------------------------------
if false
nn0 = 30;
nn1 = 30;
th0 = linspace(-2, 2, nn0);
th1 = linspace(-1, 2, nn1);

pl=zeros(nn0,nn1);
for t0=1:nn0
 for t1=1:nn1
   theta = zeros(4,1);
   theta(1) = th0(t0);
   theta(2) = th1(t1);
   pl(t0,t1) = costFunction(theta, lambda, kappa,  tax, sy, st, v, w, wt, wtt, wttt);
endfor
endfor
meshc(th0, th1, pl);
xlabel("theta_1");
ylabel("theta_0");
title("cost function");
endif
% --------------------------------------------------------------------------------------


% -validate gradient computation -----------------------------------------------
if false
th0 = [0 1 0 0]';
delta = 0.01

[J,g] = costFunction(th0, lambda, kappa,  tax, sy, st, v, w, wt, wtt, wttt);
for i=1:4
 th = th0;
 th(i) = th(i) + delta;
 Jplus = costFunction(th, lambda,  kappa, tax, sy, st, v, w, wt, wtt, wttt)
 th = th0;
 th(i) = th(i) - delta;
 Jminus = costFunction(th, lambda,  kappa, tax, sy, st, v, w, wt, wtt, wttt)
 g2 = (Jplus - Jminus) / (2*delta)
 g(i)
endfor
endif