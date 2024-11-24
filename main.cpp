#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

int main() {
  double x[3] = {1.0, 1.5, 2.0};
  double y[3] = {1.0, 2.0, 1.5};
  double r[3] = {1.0, std::sqrt(5.0) / 2.0, std::sqrt(5.0) / 2.0};
  double exact_area = 0.25 * 3.14159265358979323846 + 1.25 * std::asin(0.8) - 1.0;

  std::ofstream out_file("РЕЗУЛЬТАТЫ.csv"); // файл для записи результатов
  out_file << "Номер,Предполагаемый масштаб,Относительный размах ошибки,Приблизительный промежуток времени,Относительная ошибка в узком месте\n";

  // Генератор случайных чисел с фикс зерном для воспроизводимости
  std::mt19937 rng(250);

  // Предварительный расчет квадратов радиусов
  double r_squared[3];
  for (int i = 0; i < 3; ++i) r_squared[i] = r[i] * r[i]; // Заполнение квадратами радиусов

  // ГРАНИЦЫ
  // Широкой области
  double x_min_wide = std::min({x[0] - r[0], x[1] - r[1], x[2] - r[2]});
  double x_max_wide = std::max({x[0] + r[0], x[1] + r[1], x[2] + r[2]});
  double y_min_wide = std::min({y[0] - r[0], y[1] - r[1], y[2] - r[2]});
  double y_max_wide = std::max({y[0] + r[0], y[1] + r[1], y[2] + r[2]});
  double area_rectangle_wide = (x_max_wide - x_min_wide) * (y_max_wide - y_min_wide);

  // Узкой области
  double x_min_narrow = std::max({x[0] - r[0], x[1] - r[1], x[2] - r[2]});
  double x_max_narrow = std::min({x[0] + r[0], x[1] + r[1], x[2] + r[2]});
  double y_min_narrow = std::max({y[0] - r[0], y[1] - r[1], y[2] - r[2]});
  double y_max_narrow = std::min({y[0] + r[0], y[1] + r[1], y[2] + r[2]});
  double area_rectangle_narrow = (x_max_narrow - x_min_narrow) * (y_max_narrow - y_min_narrow);

  // Проверка на валидность узкой области
  if (area_rectangle_narrow <= 0) {
    std::cerr << "Неверные данные узкой области, проверьте входные данные.\n";
    return 1;
  }

  // Цикл по кол-ву точек
  for (int n = 100; n <= 100000; n += 500) {
    // Широкая область
    std::uniform_real_distribution<double> dist_x_wide(x_min_wide, x_max_wide);
    std::uniform_real_distribution<double> dist_y_wide(y_min_wide, y_max_wide);
    int count_inside_wide = 0;

    for (int i = 0; i < n; ++i) {
      double x0 = dist_x_wide(rng);
      double y0 = dist_y_wide(rng);

      bool inside_all = true;
      for (int j = 0; j < 3; ++j) {
        double dx = x0 - x[j];
        double dy = y0 - y[j];
        if (dx * dx + dy * dy > r_squared[j]) {
          inside_all = false;
          break;
        }
      }
      if (inside_all) {
        ++count_inside_wide;
      }
    }

    double estimated_area_wide = (static_cast<double>(count_inside_wide) / n) * area_rectangle_wide;
    double relative_error_wide = std::abs(estimated_area_wide - exact_area) / exact_area * 100.0;

    // Узкая область
    std::uniform_real_distribution<double> dist_x_narrow(x_min_narrow, x_max_narrow);
    std::uniform_real_distribution<double> dist_y_narrow(y_min_narrow, y_max_narrow);
    int count_inside_narrow = 0;

    for (int i = 0; i < n; ++i) {
      double x0 = dist_x_narrow(rng);
      double y0 = dist_y_narrow(rng);

      bool inside_all = true;
      for (int j = 0; j < 3; ++j) {
        double dx = x0 - x[j];
        double dy = y0 - y[j];
        if (dx * dx + dy * dy > r_squared[j]) {
          inside_all = false;
          break;
        }
      }
      if (inside_all) {
        ++count_inside_narrow;
      }
    }

    double estimated_area_narrow = (static_cast<double>(count_inside_narrow) / n) * area_rectangle_narrow;
    double relative_error_narrow = std::abs(estimated_area_narrow - exact_area) / exact_area * 100.0;

    // Запись результатов в файл
    out_file << n << ",";
    out_file << std::fixed << std::setprecision(8) << estimated_area_wide << "," << relative_error_wide << ",";
    out_file << estimated_area_narrow << "," << relative_error_narrow << "\n";
  }

  out_file.close();
  std::cout << "Сохранено в РЕЗУЛЬТАТЫ.csv (.../cmake-build-debug/РЕЗУЛЬТАТЫ.csv)\n";
  return 0;
}
