load("-ascii", "oit.txt");
subplot(2,1,1)
plot(oit(:,1), oit(:,2))


subplot(2,1,2)
plot(diff(oit(:,2)))

