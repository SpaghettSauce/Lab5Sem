import matplotlib.pyplot as plt
import numpy as np

# Parameters
space_step = 0.1
time_step = 0.01
diffusion_coef = 0.01
x_start, x_end = 0, 1
t_start, t_end = 0, 1

# Grid dimensions
num_x = int((x_end - x_start) / space_step) + 1
num_t = int((t_end - t_start) / time_step) + 1

# Initialize the grid
grid = np.zeros((num_t, num_x))

# Initial conditions
for idx_x in range(num_x):
    x_value = x_start + idx_x * space_step
    grid[0, idx_x] = 2 if x_value >= 0.5 else 4

# Update values using the scheme
for idx_t in range(num_t - 1):
    for idx_x in range(1, num_x - 1):
        grid[idx_t + 1, idx_x] = (
            grid[idx_t, idx_x]
            - time_step / space_step * grid[idx_t, idx_x] * (grid[idx_t, idx_x] - grid[idx_t, idx_x - 1])
            - diffusion_coef**2 * time_step / (2 * space_step**3)
            * (grid[idx_t, idx_x + 1] - 2 * grid[idx_t, idx_x] + grid[idx_t, idx_x - 1])
        )

# Stability condition check
for idx_t in range(num_t):
    for idx_x in range(num_x):
        if grid[idx_t, idx_x] > (space_step / time_step):
            print("Stability condition violated!")
            break

# Create mesh grid
x_vals = np.linspace(x_start, x_end, num_x)
t_vals = np.linspace(t_start, t_end, num_t)
X, T = np.meshgrid(x_vals, t_vals)

# Plotting the 3D surface
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_xlabel("Time (t)")
ax.set_ylabel("Space (x)")
ax.set_zlabel("Value (U)")
ax.plot_surface(T, X, grid, cmap='viridis')
plt.show()
