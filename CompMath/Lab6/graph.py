import numpy as np
import matplotlib.pyplot as plt

#f(x,t)
def f(x, t):
    return 3 * x

#U(x,0)
def Ux(x):
    return x ** 2 - x - 1

#U(0,t) для a>0 и прямоугольной области
def Ut_0(t):
    return t ** 2 - t - 1

#U(1,t) для a<0 и прямоугольной области
def Ut_1(t):
    return t ** 2 - t - 1

# 1 схема
def first_scheme(I, J, tau, U, f, x, t):
    for i in range(1, I + 1):
        for j in range(0, J):
            U[i][j + 1] = U[i - 1][j] + tau * f(x[i], t[j])
    return U

# 2 схема
def second_scheme(I, J, tau, U, f, x, t):
    for i in range(I - 1, -1, -1):
        for j in range(0, J):
            U[i][j + 1] = U[i + 1][j] - tau * f(x[i], t[j])
    return U

# 3 схема
def third_scheme(I, J, tau, U, f, x, t, a):
    if a > 0:
        for i in range(1, I + 1):
            for j in range(0, J):
                U[i][j + 1] = (U[i][j] + U[i - 1][j + 1] + tau * f(x[i], t[j])) / 2
    else:
        for i in range(I - 1, -1, -1):
            for j in range(0, J):
                U[i][j + 1] = (U[i][j] + U[i + 1][j + 1] - tau * f(x[i], t[j])) / 2
    return U

# 4 схема
def fourth_scheme(I, J, tau, U, f, x, t, a, h):
    if a > 0:
        for i in range(1, I + 1):
            for j in range(0, J):
                U[i][j + 1] = U[i - 1][j] + tau * f(x[i] + h / 2, t[j] + tau / 2)
    else:
        for i in range(I - 1, -1, -1):
            for j in range(0, J):
                U[i][j + 1] = U[i + 1][j] - tau * f(x[i] + h / 2, t[j] + tau / 2)
    return U



# Функция для инициализации условий задачи
def init(I, J, h, tau, a, fx, ft, rectangle):
    if rectangle:
        I1 = I
    else:
        I1 = I + J
    U = np.zeros((I1 + 1, J + 1))
    x = np.zeros(I1 + 1)
    t = np.zeros(J + 1)
    if rectangle:
        for i in range(I + 1):
            x[i] = h * i
            U[i][0] = fx(x[i])
        if (a > 0):
            for j in range(J + 1):
                t[j] = tau * j
                if (j != 0): U[0][j] = ft(t[j])
        else:
            for j in range(J + 1):
                t[j] = tau * j
                if (j != 0): U[I][j] = ft(t[j])
    else:
        if (a > 0):
            for i in range(J + I + 1):
                x[i] = h * (i - J)
                U[i][0] = fx(x[i])
        else:
            for i in range(I + J + 1):
                x[i] = i * h
                U[i][0] = fx(x[i])
                for i in range(J + 1):
                    t[i] = i * tau
    return x, t, U

# Функция для построения 3D графика
def draw_plot(graph):
    for gr in graph:
        gr[0], gr[1] = np.meshgrid(gr[0], gr[1])
        gr[0], gr[1] = gr[0].T, gr[1].T
        fig = plt.figure()
        ax = fig.add_subplot(projection='3d')
        ax.set_xlabel("x")
        ax.set_ylabel("t")
        ax.set_zlabel("U")
        ax.set_rasterization_zorder(1)
        ax.plot_surface(gr[0], gr[1], gr[2], cmap='plasma')
    plt.show()

# Задаем параметры задачи
rectangle = False
x_start = 0
x_end = 1
time_start = 0
time_end = 10
h = 0.1
tau = 0.05
a1 = -2
a2 = 2
I = int((x_end - x_start) / h)
J = int((time_end - time_start) / tau)
graph = []

# Код для построения графиков
x, t, U = init(I, J, h, tau, a2, Ux, Ut_0, False)
U = first_scheme(I + J, J, tau, U, f, x, t)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
U = U[J:J + I + 1, :]

graph.append([x, t, U])
print('Полуплоскость a>0 схема 1')

x, t, U = init(I, J, h, tau, a1, Ux, Ut_1, False)
U = second_scheme(I + J, J, tau, U, f, x, t)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
U = U[0:I + 1, :]
graph.append([x, t, U])
print('Полуплоскость a<0 схема 2')

x, t, U = init(I, J, h, tau, a2, Ux, Ut_0, True)
U = first_scheme(I, J, tau, U, f, x, t)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
graph.append([x, t, U])
print('Прямоугольник a>0 схема 1')

x, t, U = init(I, J, h, tau, a2, Ux, Ut_0, True)
U = third_scheme(I, J, tau, U, f, x, t, a2)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
graph.append([x, t, U])
print('Прямоугольник a>0 схема 3')

x, t, U = init(I, J, h, tau, a2, Ux, Ut_0, True)
U = fourth_scheme(I, J, tau, U, f, x, t, a2, h)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
graph.append([x, t, U])
print('Прямоугольник a>0 схема 4')

x, t, U = init(I, J, h, tau, a1, Ux, Ut_1, True)
U = second_scheme(I, J, tau, U, f, x, t)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
graph.append([x, t, U])
print('Прямоугольник a<0 схема 2')

x, t, U = init(I, J, h, tau, a1, Ux, Ut_1, True)
U = fourth_scheme(I, J, tau, U, f, x, t, a1, h)
x, t = np.linspace(x_start, x_end, I + 1), np.linspace(time_start, time_end, J + 1)
graph.append([x, t, U])
print('Прямоугольник a<0 схема 4')

draw_plot(graph)