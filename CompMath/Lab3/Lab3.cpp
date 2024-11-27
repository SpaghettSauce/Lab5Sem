#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>

// Function definitions
inline double foo(double x) {
    return x * std::log(x);
}

double leftRect(int n, std::function<double(double)> f, double a, double b) {
    double h = (b - a) / n, sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double rightRect(int n, std::function<double(double)> f, double a, double b) {
    double h = (b - a) / n, sum = 0;
    for (int i = 1; i <= n; ++i) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double centreRect(int n, std::function<double(double)> f, double a, double b) {
    double h = (b - a) / n, sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += f(a + (i + 0.5) * h);
    }
    return sum * h;
}

double trapezia(int n, std::function<double(double)> f, double a, double b) {
    double h = (b - a) / n, sum = 0;
    for (int i = 1; i < n; ++i) {
        sum += f(a + i * h);
    }
    return h * ((f(a) + f(b)) / 2 + sum);
}

double simpson(int n, std::function<double(double)> f, double a, double b) {
    int N = 2 * n;
    double h = (b - a) / N, sum_odd = 0, sum_even = 0;
    for (int i = 1; i <= n; ++i) {
        sum_odd += f(a + (2 * i - 1) * h);
    }
    for (int i = 1; i < n; ++i) {
        sum_even += f(a + 2 * i * h);
    }
    return h / 3 * (f(a) + 2 * sum_even + 4 * sum_odd + f(b));
}

double exactIntegral() {
    return 50.574334211426;
}

void calculate(std::string methodName, std::function<double(int, std::function<double(double)>, double, double)> method, double exactValue, double a, double b, double targetRelError) {
    int points = 2;
    double res_prev = method(1, foo, a, b);
    double res_next = method(2, foo, a, b);

    while (std::abs((res_next - exactValue) / exactValue) > targetRelError) {
        points *= 2;
        res_prev = res_next;
        res_next = method(points, foo, a, b);
    }

    std::cout << "\n" << methodName << "\n";
    std::cout << "Value: " << res_next << "\n";
    std::cout << "Points: " << points << "\n";
    std::cout << "Step size: " << (b - a) / points << "\n";
    std::cout << "Relative error: " << std::abs((res_next - exactValue) / exactValue) * 100 << " %\n";
}

int main() {
    double a = 2.0, b = 6.0;
    double exactValue = exactIntegral();

    std::cout << std::fixed << std::setprecision(6);

    calculate("Метод левых прямоугольников:", leftRect, exactValue, a, b, 0.00031);
    calculate("Метод правых прямоугольников:", rightRect, exactValue, a, b, 0.00031);
    calculate("Метод центральных прямоугольников:", centreRect, exactValue, a, b, 0.00015);
    calculate("Метод трапеций:", trapezia, exactValue, a, b, 0.00029);
    calculate("Метод Симпсона: ", simpson, exactValue, a, b, 0.00030);

    return 0;
}

   