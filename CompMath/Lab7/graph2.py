import matplotlib.pyplot as plt
import numpy as np

# Parameters
dx = 0.1
dt = 0.01
epsilon = 0.01
x_min, x_max, t_min, t_max = 0, 1, 0, 1

# Grid dimensions
num_x_points = int((x_max - x_min) / dx) + 1
num_t_points = int((t_max - t_min) / dt) + 1

# Initialize the solution array
solution = np.zeros((num_t_points, num_x_points))

# Setting initial conditions
for idx_x in range(num_x_points):
    x_pos = x_min + idx_x * dx
    solution[0, idx_x] = 2 if x_pos >= 0.5 else 4

# Stability condition update
for idx_t in range(num_t_points - 1):
    for idx_x in range(1, num_x_points - 1):
        solution[idx_t + 1, idx_x] = (
            solution[idx_t, idx_x]
            + dt / (2 * dx) * (solution[idx_t, idx_x - 1]**2 - solution[idx_t, idx_x]**2)
            - epsilon * (solution[idx_t, idx_x + 1] - 2 * solution[idx_t, idx_x] + solution[idx_t, idx_x - 1])
        )

# Create the mesh grid for visualization
x_vals = np.linspace(x_min, x_max, num_x_points)
t_vals = np.linspace(t_min, t_max, num_t_points)
X, T = np.meshgrid(x_vals, t_vals)

# Plotting the solution
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_xlabel("Position (x)")
ax.set_ylabel("Time (t)")
ax.set_zlabel("Solution (U)")
ax.plot_surface(X, T, solution, cmap="plasma")
plt.show()
