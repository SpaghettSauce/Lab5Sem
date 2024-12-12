import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

def visualize_results(results):
    rows, cols = np.size(results, 0), np.size(results, 1)
    x_vals, t_vals, u_vals = [0] * rows * cols, [0] * rows * cols, [0] * rows * cols

    for r in range(rows):
        for c in range(cols):
            x_vals[c + r * cols] = results[r, c][0]
            t_vals[c + r * cols] = results[r, c][1]
            u_vals[c + r * cols] = results[r, c][2]

    x, t, u = np.reshape(x_vals, (rows, cols)), np.reshape(t_vals, (rows, cols)), np.reshape(u_vals, (rows, cols))

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(x, t, u, cmap=cm.plasma, alpha=0.7)
    ax.set_xlabel('x')
    ax.set_ylabel('t')
    ax.set_zlabel('U')
    plt.colorbar(surf, ax=ax, shrink=0.5, aspect=10)
    plt.show()

def explicit_scheme(init_x, init_t0, init_t1, coeff, step):
    def u0x(x):
        return init_x(x)

    def ut0(t):
        return init_t0(t)

    def ut1(t):
        return init_t1(t)

    tau = step**2 / (2 * coeff + 3)
    t_steps = math.floor(10 / tau) + 1
    x_steps = math.floor(1 / step) + 1
    results = np.zeros((t_steps, x_steps), dtype='f,f,f')
    lamb = coeff * tau / step**2

    for xi in range(x_steps):
        x_val = xi * step
        u_val = u0x(x_val)
        results[0, xi] = (x_val, 0.0, u_val)

    last_x_val = (x_steps - 1) * step

    for ti in range(t_steps):
        t_val = ti * tau
        results[ti, 0] = (0.0, t_val, ut0(t_val))
        results[ti, x_steps - 1] = (last_x_val, t_val, ut1(t_val))

    for ti in range(1, t_steps):
        print(f"step {ti} of {t_steps}")
        t_val = ti * tau
        for xi in range(1, x_steps - 1):
            results[ti, xi] = (
                results[ti - 1, xi][0],
                t_val,
                (1 - 2 * lamb) * results[ti - 1, xi][2] +
                lamb * results[ti - 1, xi - 1][2] +
                lamb * results[ti - 1, xi + 1][2]
            )
    return results

def implicit_method(init_x, init_t0, init_t1, coeff, step):
    def u0x(x):
        return init_x(x)

    def ut0(t):
        return init_t0(t)

    def ut1(t):
        return init_t1(t)

    tau = step**2 / (2 * coeff + 3)
    t_steps = math.floor(10 / tau) + 1
    x_steps = math.floor(1 / step) + 1
    results = np.zeros((t_steps, x_steps), dtype='f,f,f')
    lamb = coeff * tau / step**2

    for xi in range(x_steps):
        x_val = xi * step
        u_val = u0x(x_val)
        results[0, xi] = (x_val, 0.0, u_val)

    last_x_val = (x_steps - 1) * step

    for ti in range(t_steps):
        t_val = ti * tau
        results[ti, 0] = (0.0, t_val, ut0(t_val))
        results[ti, x_steps - 1] = (last_x_val, t_val, ut1(t_val))

    coeffs = np.zeros(x_steps - 1, dtype='f,f,f')

    for ti in range(1, t_steps):
        print(f"step {ti} of {t_steps}")
        t_val = ti * tau
        d = -results[ti - 1, 1][2] - lamb * results[ti, 0][2]
        a, b = lamb / (1 + 2 * lamb), -d / (1 + 2 * lamb)

        coeffs[0] = (a, b)
        for xi in range(2, x_steps - 2):
            d = -results[ti - 1, xi][2]
            e = lamb * coeffs[xi - 2][0] - (1 + 2 * lamb)
            a = -lamb / e
            b = (d - lamb * coeffs[xi - 2][1]) / e
            coeffs[xi - 1] = (a, b)

        d = -results[ti - 1, x_steps - 2][2] - lamb * results[ti, x_steps - 1][2]
        u_val = (d - lamb * coeffs[x_steps - 4][1]) / (-(1 + 2 * lamb) + lamb * coeffs[x_steps - 4][0])
        results[ti, x_steps - 2] = (step * (x_steps - 2), t_val, u_val)
        for xi in range(x_steps - 3, 0, -1):
            u_val = coeffs[xi - 1][0] * results[ti, xi + 1][2] + coeffs[xi - 1][1]
            results[ti, xi] = (step * xi, t_val, u_val)

    return results

def u0x(x):
    return x**2

def ut0(t):
    return 0

def ut1(t):
    return 1

res_ = explicit_scheme(u0x, ut0, ut1, 1, 0.1)
visualize_results(res_)
