import matplotlib.pyplot as plt
x = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
y_5_с = [34, 58, 73, 96, 125, 159, 212, 245, 262, 281]
y_5_nginx = [102, 257, 433, 483, 534, 711, 760, 1200, 1241, 1453]
plt.plot(x, y_5_с, '--bo', label='Разработанный сервер, 5 клиентов')
plt.plot(x, y_5_nginx, '-rx', label='NGINX, 5 клиентов')
plt.legend(loc="upper left")
plt.ylabel('Время, мс')
plt.xlabel('Количество запросов')
plt.yticks([200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500])
plt.xticks(x)
plt.savefig('5.png')
plt.show()

y_50_с = [30, 46, 72, 72, 100, 159, 181, 188, 190, 261]
y_50_nginx = [53, 135, 168, 273, 411, 495, 567, 688, 730, 793]
plt.plot(x, y_50_с, '--bo', label='Разработанный сервер, 50 клиентов')
plt.plot(x, y_50_nginx, '-rx', label='NGINX, 50 клиентов')
plt.legend(loc="upper left")
plt.ylabel('Время, мс')
plt.xlabel('Количество запросов')
plt.xticks(x)
plt.yticks([200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500])
plt.savefig('50.png')
plt.show()

y_100_с = [30, 49, 72, 123, 143, 168, 185, 188, 190, 268]
y_100_nginx = [43, 110, 154, 242, 361, 381, 435, 605, 679, 754]
plt.plot(x, y_100_с, '--bo', label='Разработанный сервер, 100 клиентов')
plt.plot(x, y_100_nginx, '-rx', label='NGINX, 100 клиентов')
plt.legend(loc="upper left")
plt.ylabel('Время, мс')
plt.xlabel('Количество запросов')
plt.xticks(x)
plt.yticks([200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500])
plt.savefig('100.png')
plt.show()