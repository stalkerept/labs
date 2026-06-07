#!/usr/bin/env python3
import os
import argparse
import sys

def collect_and_combine(input_dir, output_file, encoding='utf-8'):
    """Рекурсивно обходит input_dir, читает все файлы и записывает в output_file."""
    input_dir = os.path.abspath(input_dir)
    output_file = os.path.abspath(output_file)

    if not os.path.isdir(input_dir):
        print(f"Ошибка: папка '{input_dir}' не существует.", file=sys.stderr)
        sys.exit(1)

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

                if full_path == output_file:
                    continue

                rel_path = os.path.relpath(full_path, input_dir)

                try:
                    with open(full_path, 'r', encoding=encoding) as in_f:
                        content = in_f.read()
                except Exception as e:
                    print(f"Не удалось прочитать файл {rel_path}: {e}", file=sys.stderr)
                    content = f"[Ошибка чтения: {e}]"

                out_f.write(f"{rel_path}:\n")
                out_f.write("```\n")
                out_f.write(content)
                if not content.endswith('\n'):
                    out_f.write('\n')
                out_f.write("```\n\n")

    print(f"Готово! Результат сохранён в {output_file}")

def main():
    parser = argparse.ArgumentParser(
        description="Объединяет все текстовые файлы из папки в один файл с метками путей."
    )
    parser.add_argument(
        '-i', '--input-dir',
        default='.',
        help="Корневая папка для поиска файлов (по умолчанию текущая)"
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