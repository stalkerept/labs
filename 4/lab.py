import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных
df = pd.read_csv('results.csv')

# Размер дерева
x = df['size']

# Построение графиков для каждой операции
plt.figure(figsize=(12, 8))

plt.subplot(2, 3, 1)
plt.plot(x, df['insert_us'], 'o-', label='Insert')
plt.xlabel('Количество элементов')
plt.ylabel('Время, мкс')
plt.title('Добавление')
plt.grid(True)

plt.subplot(2, 3, 2)
plt.plot(x, df['delete_us'], 'o-', color='orange', label='Delete')
plt.xlabel('Количество элементов')
plt.ylabel('Время, мкс')
plt.title('Удаление')
plt.grid(True)

plt.subplot(2, 3, 3)
plt.plot(x, df['find_us'], 'o-', color='green', label='Find')
plt.xlabel('Количество элементов')
plt.ylabel('Время, мкс')
plt.title('Поиск по ключу')
plt.grid(True)

plt.subplot(2, 3, 4)
plt.plot(x, df['special_us'], 'o-', color='red', label='Special')
plt.xlabel('Количество элементов')
plt.ylabel('Время, мкс')
plt.title('Специальный поиск')
plt.grid(True)

plt.subplot(2, 3, 5)
plt.plot(x, df['traverse_us'], 'o-', color='purple', label='Traverse')
plt.xlabel('Количество элементов')
plt.ylabel('Время, мкс')
plt.title('Обход (RNL)')
plt.grid(True)

plt.tight_layout()
plt.savefig('benchmark_plots.png', dpi=150)
plt.show()