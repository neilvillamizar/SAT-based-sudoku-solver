X = csvread('log/time_log_zchaff.csv')
Y = csvread('log/time_log_sat.csv')

figure
M = [ X, Y ]
x = 1:1:length(X)
bar(x,M,'grouped')
xlabel("Ejecuciones")
ylabel("Tiempo")
legend("zchaff", "Implementación propia")

print("media/time_bars","-djpeg")