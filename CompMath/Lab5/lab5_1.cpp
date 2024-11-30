#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>

// Определение правой части уравнения
double f(double x, double y) {
    return 1 - sin(2 * x + y);
}

// Метод Эйлера-Коши 2-го порядка точности
std::map<double, double> eulerCauchy(double a, double b, int n, double y0) {
    double h = (b - a) / n; // Шаг интегрирования
    double x = a;
    double y = y0;
    std::map<double, double> EC;

    while (x < b) {
        double y_temp = y + h * f(x, y);
        y += h * (f(x, y) + f(x + h, y_temp)) / 2;
        x += h;
        EC[x] = y;
    }

    return EC;
}

// Проверка точности
bool checkAccuracy(const std::map<double, double>& y1, const std::map<double, double>& y2, double eps) {
    for (const auto& pair : y1) {
        double x = pair.first;
        if (y2.count(x)) {
            if (fabs(y1.at(x) - y2.at(x)) >= eps) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    double a = 0.0;
    double b = 0.25;
    int n = 2;
    double y0 = 0.0; // Начальное условие y(0) = 0
    double eps = 0.001; // Точность

    std::map<double, double> not_checked;
    std::map<double, double> checked;

    std::cout << "Метод Эйлера-Коши 2-го порядка:\n";

    not_checked = eulerCauchy(a, b, n, y0);

    while (true) {
        checked = not_checked;
        n *= 2;
        not_checked = eulerCauchy(a, b, n, y0);
        if (checkAccuracy(checked, not_checked, eps)) {
            break;
        }
    }

    int count = not_checked.size();

    // Получаем последние 16 точек
    int last_points_count = std::min(16, count);

    // Вывод данных в консоль
    std::cout << std::setw(10) << "x"
              << std::setw(10) << "y1"
              << std::setw(10) << "y2"
              << std::setw(12) << "Difference" << '\n';
    std::cout << std::string(40, '-') << '\n';

    int i = 0;
    for (auto it = not_checked.rbegin(); it != not_checked.rend() && i < last_points_count; ++it, ++i) {
        double x = it->first;
        double y1_value = round(it->second * 1000) / 1000.0;
        double y2_value = checked.count(x) ? round(checked.at(x) * 1000) / 1000.0 : 0.0;

        std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x
                  << std::setw(10) << y1_value;

        if (checked.count(x)) {
            double difference = round(fabs(y1_value - y2_value) * 1000) / 1000.0;
            std::cout << std::setw(10) << y2_value
                      << std::setw(12) << difference << '\n';
        } else {
            std::cout << std::setw(10) << ""
                      << std::setw(12) << "" << '\n';
        }
    }

    std::cout << "Количество точек на последней итерации равно: " << count << '\n';

    return 0;
}
