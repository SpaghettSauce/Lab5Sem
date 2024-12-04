import matplotlib.pyplot as plt
import numpy as np

# Временные интервалы для входов и выходов
time = np.arange(0, 10, 0.1)

# Сигналы входов C, J, K
C_signal = np.zeros_like(time)
J_signal = np.zeros_like(time)
K_signal = np.zeros_like(time)

# Задаем входные сигналы C, J, K
C_signal[10:90] = 1  # C активен с 1 до 9 секунд
J_signal[30:50] = 1  # J активен с 3 до 5 секунд
K_signal[60:80] = 1  # K активен с 6 до 8 секунд

# Логика работы JK-триггера
Q = np.zeros_like(time)  # Выход Q
Q_not = np.ones_like(time)  # Инверсный выход Q'

for i in range(1, len(time)):
    if C_signal[i] == 1:
        if J_signal[i] == 1 and K_signal[i] == 0:
            Q[i] = 1  # Установка
        elif J_signal[i] == 0 and K_signal[i] == 1:
            Q[i] = 0  # Сброс
        elif J_signal[i] == 1 and K_signal[i] == 1:
            Q[i] = 1 - Q[i-1]  # Инвертируем Q
        else:
            Q[i] = Q[i-1]  # Сохраняем состояние
    else:
        Q[i] = Q[i-1]  # Сохраняем состояние

    Q_not[i] = 1 - Q[i]  # Инверсный выход

# Создание графика
plt.figure(figsize=(12, 10))

# График C
plt.subplot(4, 1, 1)
plt.plot(time, C_signal, drawstyle='steps-post', label='C (Clock)', color='purple')
plt.ylim(-0.1, 1.2)
plt.title('Входы и выходы JK-триггера')
plt.ylabel('Сигнал')
plt.legend()

# График J
plt.subplot(4, 1, 2)
plt.plot(time, J_signal, drawstyle='steps-post', label='J (Set)', color='blue')
plt.ylim(-0.1, 1.2)
plt.ylabel('Сигнал')
plt.legend()

# График K
plt.subplot(4, 1, 3)
plt.plot(time, K_signal, drawstyle='steps-post', label='K (Reset)', color='green')
plt.ylim(-0.1, 1.2)
plt.ylabel('Сигнал')
plt.legend()

# График Q и Q'
plt.subplot(4, 1, 4)
plt.plot(time, Q, drawstyle='steps-post', label='Q (Выход)', color='red')
plt.plot(time, Q_not, drawstyle='steps-post', label="Q' (Инверсный выход)", color='orange')
plt.ylim(-0.1, 1.2)
plt.ylabel('Сигнал')
plt.legend()

plt.xlabel('Время (с)')
plt.tight_layout()
plt.show()
