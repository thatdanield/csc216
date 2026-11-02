#!/usr/bin/env python3
"""Create linear and log-log SVG plots from the Catch2 benchmark CSV."""

import csv
import math
import shutil
import subprocess
import sys


WIDTH = 900
HEIGHT = 560
LEFT = 90
RIGHT = 35
TOP = 55
BOTTOM = 85


def read_data(path):
    with open(path, newline="", encoding="utf-8") as csv_file:
        rows = list(csv.DictReader(csv_file))
    return [
        (int(row["words"]), float(row["median_ms"]))
        for row in rows
        if int(row["words"]) > 0 and float(row["median_ms"]) > 0
    ]


def svg_escape(text):
    return (text.replace("&", "&amp;").replace("<", "&lt;")
            .replace(">", "&gt;").replace('"', "&quot;"))


def make_plot(data, output_path, logarithmic):
    plot_width = WIDTH - LEFT - RIGHT
    plot_height = HEIGHT - TOP - BOTTOM
    if logarithmic:
        x_values = [math.log10(words) for words, _ in data]
        y_values = [math.log10(ms) for _, ms in data]
        x_label = "log10(document words)"
        y_label = "log10(median runtime in ms)"
        title = "Inverted File Index Runtime (Log-Log Scale)"
    else:
        x_values = [float(words) for words, _ in data]
        y_values = [ms for _, ms in data]
        x_label = "document words"
        y_label = "median runtime (ms)"
        title = "Inverted File Index Runtime (Linear Scale)"

    min_x, max_x = min(x_values), max(x_values)
    min_y, max_y = min(y_values), max(y_values)
    if min_x == max_x:
        max_x += 1
    if min_y == max_y:
        max_y += 1
    x_padding = (max_x - min_x) * 0.05
    y_padding = (max_y - min_y) * 0.08
    min_x -= x_padding
    max_x += x_padding
    min_y -= y_padding
    max_y += y_padding

    def point(words, ms):
        x_value = math.log10(words) if logarithmic else float(words)
        y_value = math.log10(ms) if logarithmic else ms
        x = LEFT + (x_value - min_x) / (max_x - min_x) * plot_width
        y = TOP + plot_height - (y_value - min_y) / (max_y - min_y) * plot_height
        return x, y

    points = [point(words, ms) for words, ms in data]
    with open(output_path, "w", encoding="utf-8") as svg:
        svg.write(f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" '
                  f'height="{HEIGHT}" viewBox="0 0 {WIDTH} {HEIGHT}">\n')
        svg.write('<rect width="100%" height="100%" fill="white"/>\n')
        svg.write(f'<text x="{WIDTH / 2}" y="28" text-anchor="middle" '
                  f'font-family="sans-serif" font-size="20">{title}</text>\n')

        for fraction in range(6):
            x = LEFT + fraction / 5 * plot_width
            y = TOP + plot_height - fraction / 5 * plot_height
            svg.write(f'<line x1="{x:.1f}" y1="{TOP}" x2="{x:.1f}" '
                      f'y2="{TOP + plot_height}" stroke="#dddddd"/>\n')
            svg.write(f'<line x1="{LEFT}" y1="{y:.1f}" x2="{LEFT + plot_width}" '
                      f'y2="{y:.1f}" stroke="#dddddd"/>\n')

        svg.write(f'<line x1="{LEFT}" y1="{TOP + plot_height}" '
                  f'x2="{LEFT + plot_width}" y2="{TOP + plot_height}" '
                  'stroke="black"/>\n')
        svg.write(f'<line x1="{LEFT}" y1="{TOP}" x2="{LEFT}" '
                  f'y2="{TOP + plot_height}" stroke="black"/>\n')
        svg.write(f'<polyline points="{" ".join(f"{x:.1f},{y:.1f}" for x, y in points)}" '
                  'fill="none" stroke="#1769aa" stroke-width="3"/>\n')
        for x, y in points:
            svg.write(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="5" fill="#d1495b"/>\n')

        for fraction in range(6):
            x_value = min_x + fraction / 5 * (max_x - min_x)
            y_value = min_y + fraction / 5 * (max_y - min_y)
            x_label_value = 10 ** x_value if logarithmic else x_value
            y_label_value = 10 ** y_value if logarithmic else y_value
            x = LEFT + fraction / 5 * plot_width
            y = TOP + plot_height - fraction / 5 * plot_height
            svg.write(f'<text x="{x:.1f}" y="{TOP + plot_height + 22}" '
                      f'text-anchor="middle" font-family="sans-serif" font-size="11">'
                      f'{x_label_value:.0f}</text>\n')
            svg.write(f'<text x="{LEFT - 10}" y="{y + 4:.1f}" text-anchor="end" '
                      f'font-family="sans-serif" font-size="11">{y_label_value:.2f}</text>\n')

        svg.write(f'<text x="{LEFT + plot_width / 2}" y="{HEIGHT - 24}" '
                  f'text-anchor="middle" font-family="sans-serif" font-size="14">'
                  f'{svg_escape(x_label)}</text>\n')
        svg.write(f'<text x="20" y="{TOP + plot_height / 2}" '
                  f'transform="rotate(-90 20 {TOP + plot_height / 2})" '
                  f'text-anchor="middle" font-family="sans-serif" font-size="14">'
                  f'{svg_escape(y_label)}</text>\n')
        svg.write('</svg>\n')


def main():
    if len(sys.argv) not in (2, 3):
        print("Usage: python3 plot_benchmarks.py INPUT.csv [OUTPUT_PREFIX]", file=sys.stderr)
        return 2
    input_path = sys.argv[1]
    prefix = sys.argv[2] if len(sys.argv) == 3 else "inverted_file_index_runtime"
    data = read_data(input_path)
    if not data:
        print("No positive timing data found in the CSV.", file=sys.stderr)
        return 1
    outputs = []
    for scale, logarithmic in (("linear", False), ("log", True)):
        svg_path = prefix + "_" + scale + ".svg"
        png_path = prefix + "_" + scale + ".png"
        make_plot(data, svg_path, logarithmic)
        outputs.append(svg_path)

        sips = shutil.which("sips")
        if sips is None:
            print("SVG graphs were created, but PNG conversion needs macOS `sips`.",
                  file=sys.stderr)
            return 1
        subprocess.run(
            [sips, "-s", "format", "png", svg_path, "--out", png_path],
            check=True,
            stdout=subprocess.DEVNULL,
        )
        outputs.append(png_path)

    print("Wrote " + " and ".join(outputs))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
