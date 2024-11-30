#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>

// Функция f(x, y)
double f(double x, double y) {
    return 1 + (1 - x) * sin(y);
}

// Метод Рунге-Кутта 4-го порядка
double runge_kutta(double x, double y, double y0, double h) {
    double k1 = h * f(x, y);
    double k2 = h * f(x + h / 2, y + k1 / 2);
    double k3 = h * f(x + h / 2, y + k2 / 2);
    double k4 = h * f(x + h, y + k3);

    return y0 + (h / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
}

// Метод Адамса 3-го порядка
std::map<double, double> third_order_adams(double a, double b, int n, double y0, double z0) {
    double h = (b - a) / n; // Шаг интегрирования
    std::map<double, double> Y, Z;

    Y[a] = y0;
    Z[a] = z0;

    // Начальные условия с помощью метода Рунге-Кутта
    Z[a + h] = runge_kutta(a, Y[a], Z[a], h);
    Y[a + h] = Y[a] + h * Z[a];

    Z[a + 2 * h] = runge_kutta(a + h, Y[a + h], Z[a + h], h);
    Y[a + 2 * h] = Y[a + h] + h * Z[a + h];

    // Метод Адамса-Башфорта для последующих точек
    for (int i = 3; i <= n; ++i) {
        double x_i = a + i * h;
        double x_i1 = a + (i - 1) * h;
        double x_i2 = a + (i - 2) * h;
        double x_i3 = a + (i - 3) * h;

        Z[x_i] = Z[x_i1] + h * (23 * f(x_i1, Y[x_i1]) - 16 * f(x_i2, Y[x_i2]) + 5 * f(x_i3, Y[x_i3])) / 12;
        Y[x_i] = Y[x_i1] + h * (23 * Z[x_i1] - 16 * Z[x_i2] + 5 * Z[x_i3]) / 12;
    }

    return Y;
}

// Проверка точности
bool check_accuracy(const std::map<double, double>& y1, const std::map<double, double>& y2, double eps) {
    for (const auto& [x, y] : y1) {
        if (y2.find(x) != y2.end() && std::abs(y - y2.at(x)) >= eps) {
            return false;
        }
    }
    return true;
}

int main() {
    double a = 0;
    double b = 0.25;
    double y0 = 0; // Начальное условие y(0) = 0
    double z0 = 1; // Начальное условие y'(0) = z = 1
    double eps = 0.001; // Точность

    std::map<double, double> not_checked;
    std::map<double, double> checked;

    int n = 16;

    std::cout << "Интегрирование методом Адамса третьего порядка:\n";

    not_checked = third_order_adams(a, b, n, y0, z0);

    while (true) {
        checked = not_checked;
        n *= 2;
        not_checked = third_order_adams(a, b, n, y0, z0);

        if (check_accuracy(checked, not_checked, eps)) {
            break;
        }
    }

    int count = not_checked.size();

    // Подготовка данных для вывода в консоль
    std::cout << std::setw(10) << "x" << std::setw(10) << "y1" << std::setw(10) << "y2" << std::setw(10) << "разность" << "\n";
    std::cout << std::string(40, '-') << "\n";

    auto it_checked = checked.rbegin();
    for (auto it_not_checked = not_checked.rbegin(); it_not_checked != not_checked.rend(); ++it_not_checked) {
        double x = it_not_checked->first;
        double y1_value = std::round(it_not_checked->second * 1000.0) / 1000.0;
        double y2_value = (it_checked != checked.rend() && it_checked->first == x) ? std::round(it_checked->second * 1000.0) / 1000.0 : 0;

        if (it_checked != checked.rend() && it_checked->first == x) {
            double difference = std::round(std::abs(y1_value - y2_value) * 1000.0) / 1000.0;
            std::cout << std::setw(10) << x << std::setw(10) << y1_value << std::setw(10) << y2_value << std::setw(10) << difference << "\n";
            ++it_checked;
        } else {
            std::cout << std::setw(10) << x << std::setw(10) << y1_value << std::setw(10) << "" << std::setw(10) << "" << "\n";
        }
    }

    std::cout << "\nКоличество точек на последней итерации равно: " << count << "\n";

    return 0;
}
