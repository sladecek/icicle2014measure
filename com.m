f=fopen("icicle.dat", "r");
da = fread(f, Inf, "uchar");
load("-ascii","icicle_size.txt");
sy = icicle_size(1);
sx = icicle_size(2);
st = size(da)(1)/sx/sy;
d = reshape(da, sy, sx, st);