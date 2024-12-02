import matplotlib.pyplot as plt
import numpy as np

# Временные интервалы для входов и выходов
time = np.arange(0, 10, 0.1)

# Сигнал входа T
T_signal = np.zeros_like(time)

# Задаем входной сигнал T
T_signal[20:40] = 1  # Устанавливаем T в период с 2 до 4 секунд
T_signal[60:80] = 1  # Устанавливаем T в период с 6 до 8 секунд

# Логика работы T-триггера
Q = np.zeros_like(time)  # Выход Q
Q_not = np.ones_like(time)  # Инверсный выход Q'

for i in range(1, len(time)):
    if T_signal[i] == 1:
        Q[i] = 1 - Q[i-1]  # Инвертируем Q
    else:
        Q[i] = Q[i-1]  # Состояние сохраняется

    Q_not[i] = 1 - Q[i]  # Инверсный выход

# Создание графика
plt.figure(figsize=(12, 8))

# График T
plt.subplot(3, 1, 1)
plt.plot(time, T_signal, drawstyle='steps-post', label='T (Toggle)', color='purple')
plt.ylim(-0.1, 1.2)
plt.title('Вход и выход T-триггера')
plt.ylabel('Сигнал')
plt.legend()

# График Q
plt.subplot(3, 1, 2)
plt.plot(time, Q, drawstyle='steps-post', label='Q (Выход)', color='blue')
plt.ylim(-0.1, 1.2)
plt.ylabel('Сигнал')
plt.legend()

# График Q'
plt.subplot(3, 1, 3)
plt.plot(time, Q_not, drawstyle='steps-post', label="Q' (Инверсный выход)", color='orange')
plt.ylim(-0.1, 1.2)
plt.ylabel('Сигнал')
plt.legend()

plt.xlabel('Время (с)')
plt.tight_layout()
plt.show()
