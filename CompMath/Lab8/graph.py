import numpy as np
import matplotlib.pyplot as plt

# Определение начальных и граничных условий
def initial_condition(x):
    return x**2

def boundary_condition_left():
    return 0

def boundary_condition_right():
    return 1

def initial_velocity():
    return 1

# Параметры сетки
h = 0.1
tau = 0.01
x_points = int(1 / h) + 1
t_points = int(10 / tau) + 1
l_coef = (tau / h)**2

# Функция для явного метода
def explicit_method():
    u = np.zeros((x_points, t_points))

    # Задание начальных условий
    for i in range(x_points):
        x = i * h
        u[i, 0] = initial_condition(x)
        u[i, 1] = u[i, 0] + tau * initial_velocity()

    # Граничные условия
    u[0, :] = boundary_condition_left()
    u[-1, :] = boundary_condition_right()

    # Итерации по времени
    for j in range(1, t_points - 1):
        for i in range(1, x_points - 1):
            u[i, j + 1] = (
                2 * (1 - l_coef) * u[i, j] +
                l_coef * (u[i + 1, j] + u[i - 1, j]) -
                u[i, j - 1]
            )
    return u

# Функция для неявного метода
def implicit_method():
    u = np.zeros((x_points, t_points))

    # Задание начальных условий
    for i in range(x_points):
        x = i * h
        u[i, 0] = initial_condition(x)
        u[i, 1] = u[i, 0] + tau * initial_velocity()

    # Граничные условия
    u[0, :] = boundary_condition_left()
    u[-1, :] = boundary_condition_right()

    # Итерации по времени
    for j in range(1, t_points - 1):
        a = np.full(x_points, -l_coef)
        b = np.full(x_points, 1 + 2 * l_coef)
        c = np.full(x_points, -l_coef)
        d = np.zeros(x_points)

        for i in range(1, x_points - 1):
            d[i] = 2 * u[i, j] - u[i, j - 1]
        d[1] += l_coef * u[0, j]
        d[-2] += l_coef * u[-1, j]

        # Прогонка (метод трехдиагональной матрицы)
        for i in range(2, x_points - 1):
            factor = a[i] / b[i - 1]
            b[i] -= factor * c[i - 1]
            d[i] -= factor * d[i - 1]

        u[-2, j + 1] = d[-2] / b[-2]
        for i in range(x_points - 3, 0, -1):
            u[i, j + 1] = (d[i] - c[i] * u[i + 1, j + 1]) / b[i]

    return u

# Построение графиков
for method, title in [(explicit_method, "Явный метод"), (implicit_method, "Неявный метод")]:
    u = method()

    x = np.linspace(0, 1, x_points)
    t = np.linspace(0, 10, t_points)
    x_grid, t_grid = np.meshgrid(x, t, indexing='ij')

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.plot_surface(t_grid, x_grid, u, cmap='viridis')
    ax.set_title(title)
    ax.set_xlabel('Время (t)')
    ax.set_ylabel('Координата (x)')
    ax.set_zlabel('U')
    plt.show()
