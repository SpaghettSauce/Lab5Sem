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

// Метод Адамса-Бэшфорта 4-го порядка
std::map<double, double> forth_order_adams(double a, double b, int n, double y0, double z0) {
    double h = (b - a) / n;
    std::map<double, double> Y, Z;

    Y[a] = y0;
    Z[a] = z0;

    // Начальные условия с помощью метода Рунге-Кутта
    Z[a + h] = runge_kutta(a, Y[a], Z[a], h);
    Y[a + h] = Y[a] + h * Z[a];

    Z[a + 2 * h] = runge_kutta(a + h, Y[a + h], Z[a + h], h);
    Y[a + 2 * h] = Y[a + h] + h * Z[a + h];

    Z[a + 3 * h] = runge_kutta(a + 2 * h, Y[a + 2 * h], Z[a + 2 * h], h);
    Y[a + 3 * h] = Y[a + 2 * h] + h * Z[a + 2 * h];

    // Метод Адамса-Башфорта для последующих точек
    for (int i = 4; i <= n; ++i) {
        double x = a + i * h;
        Z[x] = Z[x - h] + h * (55 * f(x - h, Y[x - h]) -
                               59 * f(x - 2 * h, Y[x - 2 * h]) +
                               37 * f(x - 3 * h, Y[x - 3 * h]) -
                               9 * f(x - 4 * h, Y[x - 4 * h])) / 24.0;

        Y[x] = Y[x - h] + h * (55 * Z[x - h] -
                               59 * Z[x - 2 * h] +
                               37 * Z[x - 3 * h] -
                               9 * Z[x - 4 * h]) / 24.0;
    }

    return Y;
}

// Проверка точности
bool check_accuracy(const std::map<double, double>& y1, const std::map<double, double>& y2, double eps) {
    for (const auto& [x, val1] : y1) {
        auto it = y2.find(x);
        if (it != y2.end() && std::abs(val1 - it->second) >= eps) {
            return false;
        }
    }
    return true;
}

int main() {
    double a = 0;
    double b = 0.25;
    double y0 = 0;  // Начальное условие y(0) = 0
    double z0 = 1;  // Начальное условие y'(0) = z = 1
    double eps = 0.001;  // Точность

    std::map<double, double> not_checked, checked;
    int n = 16;

    std::cout << "Интегрирование методом Адамса четвертого порядка:\n";

    not_checked = forth_order_adams(a, b, n, y0, z0);

    while (true) {
        checked = not_checked;
        n *= 2;
        not_checked = forth_order_adams(a, b, n, y0, z0);
        if (check_accuracy(checked, not_checked, eps)) {
            break;
        }
    }

    int count = not_checked.size();

    // Подготовка данных для вывода в консоль
    std::cout << std::setw(10) << "x" << std::setw(10) << "y1" << std::setw(10) << "y2" << std::setw(10) << "разность" << "\n";
    std::cout << std::string(40, '-') << "\n";

    for (auto it = not_checked.rbegin(); it != not_checked.rend() && count-- > 16; ++it) {
        double x = it->first;
        double y1_value = round(it->second * 10000) / 10000.0;
        double y2_value = checked.count(x) > 0 ? round(checked.at(x) * 10000) / 10000.0 : 0.0;
        double difference = std::abs(y1_value - y2_value);

        std::cout << std::setw(10) << std::fixed << std::setprecision(4) << x
                  << std::setw(10) << y1_value
                  << std::setw(10) << y2_value
                  << std::setw(10) << difference << "\n";
    }

    std::cout << "\nКоличество точек на последней итерации равно: " << not_checked.size() << "\n";

    return 0;
}
