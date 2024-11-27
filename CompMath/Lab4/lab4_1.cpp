#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

// Функция для вычисления логарифма
double f(double x) {
    return std::log10(x);
}

// Производная функции f
double df(double x) {
    return 1 / (x * std::log(10));
}

// Функция для вычисления полинома Лагранжа
double lagrangePolynomial(double x, const std::vector<double>& xk, const std::vector<double>& yk) {
    int n = xk.size();
    double p = 0.0;

    for (int i = 0; i < n; ++i) {
        double l = 1.0;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                l *= (x - xk[j]) / (xk[i] - xk[j]);
            }
        }
        p += yk[i] * l;
    }
    return p;
}

// Функция для оценки погрешности интерполяции
double errorEstimate(double x, const std::vector<double>& xk, double (*df)(double)) {
    int n = xk.size();

    // Находим максимальное значение производной на интервале
    double M = 0.0;
    for (double xi = xk.front(); xi <= xk.back(); xi += (xk.back() - xk.front()) / 1000) {
        M = std::max(M, std::abs(df(xi)));
    }

    // Вычисляем произведение (x - xk[i])
    double w = 1.0;
    for (int i = 0; i < n; ++i) {
        w *= (x - xk[i]);
    }

    return (M * std::abs(w)) / std::tgamma(n + 1); // std::tgamma(n+1) = n!
}

int main() {
    // Узлы интерполяции
    std::vector<double> xk = {7, 7.5, 8, 8.5};
    std::vector<double> yk;

    // Заполняем yk значениями функции f
    for (double x : xk) {
        yk.push_back(std::round(f(x) * 10000) / 10000); // округляем до 4 знаков
    }

    // Точка, в которой будем вычислять значение функции
    double x = 7.2;

    // Вычисляем аналитическое значение функции в точке x
    double yAnalytical = f(x);

    // Вычисляем значение полинома Лагранжа в точке x
    double yLagrange = lagrangePolynomial(x, xk, yk);

    // Оценка погрешности и абсолютная погрешность
    double error = errorEstimate(x, xk, df);
    double absoluteError = std::abs(yAnalytical - yLagrange);

    // Вывод результатов
    std::cout << std::fixed << std::setprecision(4);
    for (size_t i = 0; i < xk.size(); ++i) {
        std::cout << "Значение функции в точке x = " << xk[i] << ": " << yk[i] << std::endl;
    }
    std::cout << "\n";
    std::cout << "Значение функции в точке x = " << x << " Аналитически: " << yAnalytical << std::endl;
    std::cout << "Значение функции в точке x = " << x << " По ЛАгранжу: " << yLagrange << std::endl;
    std::cout << "Абсолютная погрешность вычислений: " << absoluteError << std::endl;
    std::cout << "Оценка погрешности формулы Лагранжа: " << error << std::endl;

    return 0;
}
