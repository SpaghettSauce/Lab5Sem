#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>

// Определяем функцию f(x) = exp(x/2)
double f(double x) {
    return std::exp(x / 2);
}

// Определяем первую производную функции f(x)
double df(double x) {
    return std::exp(x / 2) / 2;
}

// Определяем вторую производную функции f(x)
double d2f(double x) {
    return std::exp(x / 2) / 4;
}

// Структура для хранения результатов
struct Result {
    double x;
    double f_prime_left;
    double f_prime_right;
    double f_prime_center;
    double exact_first_derivative;
    double error_left;
    double error_right;
    double error_center;
    double f_second;
    double exact_second_derivative;
    double error_second;
};

int main() {
    // Задаем границы интервала и количество точек
    double a = 3.4;
    double b = 4.3;
    int n = 5;

    // Вычисляем шаг между точками
    double h = (b - a) / (n - 1);

    // Генерируем n точек на интервале [a, b]
    std::vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = a + i * h;
    }

    // Вектор для хранения результатов
    std::vector<Result> results;

    // Вычисляем производные и ошибки для каждой точки
    for (int i = 0; i < n; ++i) {
        double xi = x[i];

        // Вычисляем точные значения первой и второй производных
        double exact_first_derivative = df(xi);
        double exact_second_derivative = d2f(xi);

        // Вычисляем приближенные значения первой производной
        double f_prime_left = (i > 0) ? (f(xi) - f(xi - h)) / h : NAN;
        double f_prime_right = (i < n - 1) ? (f(xi + h) - f(xi)) / h : NAN;
        double f_prime_center = (i > 0 && i < n - 1) ? (f(xi + h) - f(xi - h)) / (2 * h) : NAN;

        // Вычисляем приближенное значение второй производной только для промежуточных точек
        double f_second = (i > 0 && i < n - 1) ? (f(xi + h) - 2 * f(xi) + f(xi - h)) / (h * h) : NAN;

        // Вычисляем ошибки между точными и приближенными значениями производных
        double error_left = (i > 0) ? std::abs(exact_first_derivative - f_prime_left) : NAN;
        double error_right = (i < n - 1) ? std::abs(exact_first_derivative - f_prime_right) : NAN;
        double error_center = (i > 0 && i < n - 1) ? std::abs(exact_first_derivative - f_prime_center) : NAN;
        double error_second = (i > 0 && i < n - 1) ? std::abs(exact_second_derivative - f_second) : NAN;

        // Сохраняем результаты
        results.push_back({
            xi,
            f_prime_left,
            f_prime_right,
            f_prime_center,
            exact_first_derivative,
            error_left,
            error_right,
            error_center,
            f_second,
            exact_second_derivative,
            error_second
        });
    }

    // Выводим результаты в таблице
    std::cout << std::fixed << std::setprecision(4);
    std::cout << " x      f'(x) (Левая) f'(x) (Правая) f'(x) (Центр) Точная f'(x) Погрешность (Левая) Погрешность (Правая) Погрешность (Центр) f''(x) Точная f''(x) Погрешность\n";
    for (const auto& result : results) {
        
       std::cout << std::setw(6) << result.x << " "
                  << std::setw(12) << (std::isnan(result.f_prime_left) ? "N/A" : std::to_string(result.f_prime_left).substr(0, 6)) << " "
                  << std::setw(11) << (std::isnan(result.f_prime_right) ? "N/A" : std::to_string(result.f_prime_right).substr(0, 6)) << " "
                  << std::setw(15) << (std::isnan(result.f_prime_center) ? "N/A" : std::to_string(result.f_prime_center).substr(0, 6)) << " "
                  << std::setw(12) << result.exact_first_derivative << " "
                  << std::setw(14) << (std::isnan(result.error_left) ? "N/A" : std::to_string(result.error_left).substr(0, 6)) << " "
                  << std::setw(16) << (std::isnan(result.error_right) ? "N/A" : std::to_string(result.error_right).substr(0, 6)) << " "
                  << std::setw(18) << (std::isnan(result.error_center) ? "N/A" : std::to_string(result.error_center).substr(0, 6)) << " "
                  << std::setw(18) << (std::isnan(result.f_second) ? "N/A" : std::to_string(result.f_second).substr(0, 6)) << " "
                  << std::setw(12) << result.exact_second_derivative << " "
                  << std::setw(13) << (std::isnan(result.error_second) ? "N/A" : std::to_string(result.error_second).substr(0, 6)) << "\n";
    }

    return 0;
}
