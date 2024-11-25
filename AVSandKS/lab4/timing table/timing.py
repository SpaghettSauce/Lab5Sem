import matplotlib.pyplot as plt
import numpy as np

# Time steps (t0 to t9)
time_new = np.arange(10)


# Signal data from the new timing table
clock_340_390 = [1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
clock_340_550 = [0, 1, 0, 1, 0, 1, 0, 1, 0, 1]
q_new = [1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
q_complement_new = [0, 1, 0, 1, 0, 1, 0, 1, 0, 1]

# Plotting the waveform diagram for the new timing table
plt.figure(figsize=(12, 6))
plt.title("Временная диаграмма")

# Plot each signal
plt.step(time_new, clock_340_390, where="post", label="Clock (340,390)", color="yellow")
plt.step(time_new, clock_340_550, where="post", label="Clock (340,550)", color="green")
plt.step(time_new, q_new, where="post", label="Q", color="blue")
plt.step(time_new, q_complement_new, where="post", label="Q'", color="red")

# Add grid, labels, and legend
plt.yticks([0, 1], ["0", "1"])
plt.xticks(time_new)
plt.xlabel("Time Steps")
plt.ylabel("Logic Level")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(loc="upper right")
plt.tight_layout()

# Show the diagram
plt.show()
