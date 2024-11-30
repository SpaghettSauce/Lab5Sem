# Задание 1.1
import math

# Определение правой части уравнения
def f(x, y):
    return 1 - math.sin(2 * x + y)

# Метод Эйлера-Коши 2-го порядка точности
def euler_cauchy(a, b, n, y0):
    h = (b - a) / n  # Шаг интегрирования
    x = a
    y = y0
    EC = {}

    while x < b:
        x += h
        y_temp = y + h * f(x, y)
        y += h * (f(x, y) + f(x + h, y_temp)) / 2
        EC[x] = y

    return EC

def check_accuracy(y1, y2, eps):
    for x in y1:
        if x in y2:
            if abs(y1[x] - y2[x]) >= eps:
                return False
    return True

def main():
    a = 0
    b = 0.25
    n = 2
    y0 = 0  # Начальное условие y(0) = 0
    eps = 0.001  # Точность

    not_checked = {}
    checked = {}

    print("Метод Эйлера-Коши 2-го порядка:")

    not_checked = euler_cauchy(a, b, n, y0)

    while True:
        checked = not_checked.copy()
        n *= 2
        not_checked = euler_cauchy(a, b, n, y0)
        if check_accuracy(checked, not_checked, eps):
            break

    count = len(not_checked)

    # Получаем последние 16 точек
    last_points_count = min(16, count)

    # Сортируем по x для корректного вывода последних значений
    sorted_xs = sorted(not_checked.keys())

    # Вывод данных в консоль
    print(f"{'x':<10} {'y1':<10} {'y2':<10} {'Difference':<10}")
    print("-" * 40)

    for x in sorted_xs[-last_points_count:]:
        y1_value = round(not_checked[x], 3)
        y2_value = round(checked.get(x, None), 3) if checked.get(x) is not None else ""

        if y2_value != "":
            difference = round(abs(y1_value - y2_value), 3)
            print(f"{round(x, 3):<10} {y1_value:<10} {y2_value:<10} {difference:<10}")
        else:
            print(f"{round(x, 3):<10} {y1_value:<10} {'':<10} {'':<10}")

    print(f"\nКоличество точек на последней итерации равно: {count}")
 
if __name__ == "__main__":
    main()
