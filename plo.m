load("-ascii", "oiv.txt");
subplot(3,1,1)
plot(oiv(:,1), oiv(:,2))
title("blue")

subplot(3,1,2)
plot(oiv(:,1), oiv(:,3))
title("green")

subplot(3,1,3)
plot(oiv(:,1), oiv(:,4))
title("red")

