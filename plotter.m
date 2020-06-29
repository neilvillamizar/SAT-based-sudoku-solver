X = csvread('time_log_zchaff.csv')
Y = csvread('time_log_sat.csv')

X = X(1:30,:)
Y = Y(1:30,:)

figure
M = [ X, Y ]
x = 1:1:30
bar(x,M,'grouped')
xlabel("Ejecuciones")
ylabel("Tiempo")
legend("zchaff", "Nuestro sat")

print("time_bars","-djpeg")