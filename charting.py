import pandas as pd

import matplotlib
matplotlib.use('TkAgg')  # Устанавливаем TkAgg в качестве бэкенда для matplotlib
import matplotlib.pyplot as plt

import numpy as np

# Чтение данных из CSV файла
res = pd.read_csv("/Users/macboock/Desktop/РЕЗУЛЬТАТЫ.csv")

# Вычисление точной площади
exact_area = 0.25 * np.pi + 1.25 * np.arcsin(0.8) - 1

# Создание первого графика
plt.figure(figsize=(12, 8))
plt.plot(res['Номер'], res['Предполагаемый масштаб'], label='Предполагаемый масштаб', linestyle='-', color='red')
plt.plot(res['Номер'], res['Приблизительный промежуток времени'], label='Приблизительный промежуток времени', linestyle='-', color='blue')

plt.axhline(y=exact_area, color='green', linestyle='--', label='Точная площадь')

plt.xlabel('Количество точек')
plt.ylabel('Предполагаемая площадь')

plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Создание второго графика
plt.figure(figsize=(12, 8))
plt.plot(res['Номер'], res['Относительный размах ошибки'], label='Относительный размах ошибки', linestyle='-', color='red')
plt.plot(res['Номер'], res['Относительная ошибка в узком месте'], label='Относительная ошибка в узком месте', linestyle='-', color='blue')

plt.xlabel('Количество точек')
plt.ylabel('Относительная погрешность в %')

plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
