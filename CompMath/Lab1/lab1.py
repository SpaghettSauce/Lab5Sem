class DiffFunction:
    def __init__(self, *derivatives):
        self.derivatives = derivatives

    def __call__(self, x, order=0):
        return self.derivatives[order](x)

def try_spread(arr_like):
    return list(arr_like) if hasattr(arr_like, '__iter__') else [arr_like]

def iterative_solver(start, step_func, epsilon=1e-7, halt_predicate=lambda pr, nx, e: abs(nx - pr) < e):
    steps = try_spread(start)
    steps.append(step_func(steps))
    while not halt_predicate(steps[-2], steps[-1], epsilon):
        steps.append(step_func(steps))
    return steps[-1], steps

def newton(f: DiffFunction, a, b, epsilon=1e-7): #Ньютона (метод касательных)
    step_function = lambda x: x[-1] - f(x[-1]) / f(x[-1], 1)
    start = a if f(a) * f(a, 2) > 0 else b
    return iterative_solver(start, step_function, epsilon)

def chords(f: DiffFunction, a, b, epsilon=1e-7): #метод хорд
    if f(b) * f(b, 2) <= 0:
        a, b = b, a
    step_function = lambda x: x[-1] - f(x[-1]) * (b - x[-1]) / (f(b) - f(x[-1]))
    return iterative_solver(a, step_function, epsilon)

def secants(f: DiffFunction, a, b, epsilon=1e-7): #метод секущих
    step_function = lambda x: x[-1] - f(x[-1]) * (x[-1] - x[-2]) / (f(x[-1]) - f(x[-2]))
    return iterative_solver([a, b], step_function, epsilon)

def finite_sum_newton(f: DiffFunction, a, b, epsilon=1e-7, h=0.01): #конечноразностный Нютона
    step_function = lambda x: x[-1] - h * f(x[-1]) / (f(x[-1] + h) - f(x[-1]))
    return iterative_solver(a, step_function, epsilon)


def simple_iter(f: DiffFunction, a, b, epsilon=1e-7, tau=None): #просых итераций
    c = (a + b) / 2
    tau = tau or 1 / f(c, 1)  # Default tau based on first derivative at midpoint
    step_function = lambda x: x[-1] - tau * f(x[-1])
    return iterative_solver(a, step_function, epsilon)
