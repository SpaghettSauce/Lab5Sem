import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# Определение граничных условий
def boundary_conditions():
    return {
        'Ux0': lambda x: x,
        'Ux1': lambda x: x + 10,
        'U0y': lambda y: y,
        'U1y': lambda y: y + 10
    }

# Правая часть уравнения
def equation_rhs(x, y):
    return y * (10 - x)

# Функция для визуализации результата
def visualize_solution(grid, step, method):
    size = len(grid) 
    x_vals = np.linspace(0, 10, size)
    y_vals = np.linspace(0, 10, size)
    x, y = np.meshgrid(x_vals, y_vals)

    grid_x, grid_y = np.mgrid[0:10:100j, 0:10:100j]
    grid_z = griddata((x.flatten(), y.flatten()), np.array(grid).flatten(), (grid_x, grid_y), method='cubic')

    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title(f"Метод: {method}, Сетка: {size - 1}x{size - 1}")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("U")

    surface = ax.plot_surface(grid_x, grid_y, grid_z, cmap="plasma", edgecolor='none', alpha=0.9)
    ticks = np.linspace(0, 10, int(10 / step) + 1)
    ax.set_xticks(ticks)
    ax.set_yticks(ticks)

    fig.colorbar(surface, ax=ax, shrink=0.5, aspect=10)
    plt.show()

# Универсальная функция для решения методом итераций
def solve_with_iterations(step=0.1, epsilon=0.01, method='simple'):
    size = int(10 / step) + 1
    grid = np.zeros((size, size))
    boundaries = boundary_conditions()

    # Установка граничных условий
    for i in range(size):
        x = step * i
        grid[i][0] = boundaries['Ux0'](x)
        grid[i][-1] = boundaries['Ux1'](x)

    for j in range(1, size):
        y = step * j
        grid[0][j] = boundaries['U0y'](y)
        grid[-1][j] = boundaries['U1y'](y)

    # Итерационный процесс
    while True:
        max_diff = 0
        new_grid = grid.copy() if method == 'simple' else grid

        for i in range(1, size - 1):
            for j in range(1, size - 1):
                x, y = step * i, step * j
                updated_value = (grid[i + 1][j] + grid[i - 1][j] + grid[i][j + 1] + grid[i][j - 1] - step**2 * equation_rhs(x, y)) / 4

                if method == 'simple':
                    new_grid[i][j] = updated_value
                else:
                    max_diff = max(max_diff, abs(updated_value - grid[i][j]))
                    grid[i][j] = updated_value

                max_diff = max(max_diff, abs(updated_value - grid[i][j]))

        grid = new_grid if method == 'simple' else grid

        if max_diff < epsilon:
            break

    method_name = "Простая итерация" if method == 'simple' else "Зейделя"
    visualize_solution(grid, step, method_name)

# Вызов методов решения
solve_with_iterations(step=2.0, method='simple')
solve_with_iterations(step=1.0, method='simple')

solve_with_iterations(step=2.0, method='zeidel')
solve_with_iterations(step=1.0, method='zeidel')
