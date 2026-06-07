import os
import argparse
import sys

def collect_and_combine(input_dir, output_file, encoding='utf-8'):
    """
    Рекурсивно обходит input_dir, читает все файлы (кроме выходного) и записывает их
    содержимое в output_file с заголовками-разделителями.
    """
    # Нормализуем пути
    input_dir = os.path.abspath(input_dir)
    output_file = os.path.abspath(output_file)

    if not os.path.isdir(input_dir):
        print(f"Ошибка: папка '{input_dir}' не существует.", file=sys.stderr)
        sys.exit(1)

    # Проверяем, не пытаемся ли мы записать в файл внутри самой обрабатываемой папки
    # (это приведёт к чтению растущего файла). Если да – предупреждаем.
    if output_file.startswith(input_dir):
        print(
            f"Предупреждение: выходной файл '{output_file}' находится внутри обрабатываемой папки.",
            file=sys.stderr
        )
        print("Он будет пропущен при чтении, но это может вызвать путаницу.", file=sys.stderr)

    with open(output_file, 'w', encoding=encoding) as out_f:
        for root, dirs, files in os.walk(input_dir):
            for file in files:
                full_path = os.path.join(root, file)

                # Пропускаем сам выходной файл, если он попался в обходе
                if full_path == output_file:
                    continue

                # Получаем относительный путь от input_dir для красоты
                rel_path = os.path.relpath(full_path, input_dir)

                try:
                    with open(full_path, 'r', encoding=encoding) as in_f:
                        content = in_f.read()
                except Exception as e:
                    print(f"Не удалось прочитать файл {rel_path}: {e}", file=sys.stderr)
                    # Можно записать в выходной файл сообщение об ошибке
                    content = f"[Ошибка чтения: {e}]"

                # Записываем заголовок и содержимое
                out_f.write(f"{rel_path}:\n")
                out_f.write("```\n")
                out_f.write(content)
                # Если файл не заканчивается переводом строки, добавим его для аккуратности
                if not content.endswith('\n'):
                    out_f.write('\n')
                out_f.write("```\n\n")

    print(f"Готово! Результат сохранён в {output_file}")

def main():
    parser = argparse.ArgumentParser(
        description="Объединяет текстовые файлы из папки в один большой файл с метками источников."
    )
    parser.add_argument(
        '-i', '--input-dir',
        default='.',
        help="Корневая папка для поиска текстовых файлов (по умолчанию текущая)"
    )
    parser.add_argument(
        '-o', '--output-file',
        default='combined.txt',
        help="Имя выходного файла (по умолчанию combined.txt)"
    )
    parser.add_argument(
        '--encoding',
        default='utf-8',
        help="Кодировка текстовых файлов (по умолчанию utf-8)"
    )
    args = parser.parse_args()

    collect_and_combine(args.input_dir, args.output_file, args.encoding)

if __name__ == '__main__':
    main()