load("-ascii", "oiv.txt");
subplot(3,2,1)
plot(oiv(:,1), oiv(:,2))
title("blue")

subplot(3,2,2)
plot(diff(oiv(:,2)))
title("blue")

subplot(3,2,3)
plot(oiv(:,1), oiv(:,3))
title("green")

subplot(3,2,4)
plot(diff(oiv(:,3)))
title("green")

subplot(3,2,5)
plot(oiv(:,1), oiv(:,4))
title("red")

subplot(3,2,6)
plot(diff(oiv(:,4)))
title("red")
