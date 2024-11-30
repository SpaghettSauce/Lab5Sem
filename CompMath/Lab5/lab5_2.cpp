#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>

// Определение правой части уравнения
double f(double x, double y) {
    return 1 - std::sin(2 * x + y);
}

// Метод Рунге-Кутта 4-го порядка точности
std::map<double, double> runge_kutta(double a, double b, int n, double y0) {
    double h = (b - a) / n; // Шаг интегрирования
    double x = a;
    double y = y0;
    std::map<double, double> RK;

    while (x < b) {
        double k1 = h * f(x, y);
        double k2 = h * f(x + h / 2, y + k1 / 2);
        double k3 = h * f(x + h / 2, y + k2 / 2);
        double k4 = h * f(x + h, y + k3);

        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x += h;
        RK[x] = y;
    }

    return RK;
}

// Проверка точности результатов
bool check_accuracy(const std::map<double, double>& y1, const std::map<double, double>& y2, double eps) {
    for (const auto& [x, y1_value] : y1) {
        if (y2.find(x) != y2.end()) {
            if (std::abs(y1_value - y2.at(x)) >= eps) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    double a = 0;
    double b = 0.25;
    int n = 2;
    double y0 = 0; // Начальное условие y(0) = 0
    double eps = 0.001; // Точность

    std::map<double, double> not_checked;
    std::map<double, double> checked;

    std::cout << "Метод Рунге-Кутта 4-го порядка:" << std::endl;

    not_checked = runge_kutta(a, b, n, y0);

    while (true) {
        checked = not_checked;
        n *= 2;
        not_checked = runge_kutta(a, b, n, y0);
        if (check_accuracy(checked, not_checked, eps)) {
            break;
        }
    }

    int count = not_checked.size();

    // Получаем последние 16 точек
    int last_points_count = std::min(16, count);

    // Вывод данных в консоль
    std::cout << std::setw(10) << "x" << std::setw(10) << "y1" << std::setw(10) << "y2" << std::setw(12) << "Difference" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    int printed_points = 0;
    for (auto it = not_checked.rbegin(); it != not_checked.rend() && printed_points < last_points_count; ++it) {
        double x = it->first;
        double y1_value = std::round(it->second * 1000.0) / 1000.0;
        double y2_value = checked.count(x) > 0 ? std::round(checked.at(x) * 1000.0) / 1000.0 : 0;

        if (checked.count(x) > 0) {
            double difference = std::round(std::abs(y1_value - y2_value) * 1000.0) / 1000.0;
            std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x
                      << std::setw(10) << y1_value
                      << std::setw(10) << y2_value
                      << std::setw(12) << difference << std::endl;
        } else {
            std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x
                      << std::setw(10) << y1_value
                      << std::setw(10) << ""
                      << std::setw(12) << "" << std::endl;
        }

        ++printed_points;
    }

    std::cout << "\nКоличество точек на последней итерации равно: " << count << std::endl;

    return 0;
}
