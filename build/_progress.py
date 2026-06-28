import sys

if len(sys.argv) != 3:
    sys.stderr.write("Usage: _progress.py <current> <max>\n")
    sys.exit(1)

try:
    current = float(sys.argv[1])
    max_val = float(sys.argv[2])
    if max_val == 0:
        raise ValueError("max must not be zero")
    percent = int(100 * current / max_val)
    print(f"[{percent:>3}%]")
except ValueError as e:
    sys.stderr.write(f"Invalid arguments: {e}\n")
    sys.exit(1)
