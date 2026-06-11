import sys
import matplotlib.pyplot as plt

speeds = []
actuals = []

def parse_lines(lines):
    for line in lines:
        line = line.strip()
        if line == "DONE":
            break
        if "," in line:
            try:
                s, a = line.split(",")
                speeds.append(int(s))
                actuals.append(float(a))
            except ValueError:
                pass

if len(sys.argv) > 1:
    # tryb plikowy: python plot_speed.py dane.txt
    with open(sys.argv[1]) as f:
        parse_lines(f.readlines())
else:
    # tryb serial: python plot_speed.py  (wymaga pyserial)
    import serial

    PORT = "COM3"   # <-- zmień na swój port (np. COM4, COM5)
    BAUD = 9600

    print(f"Czekam na dane z {PORT}...")
    with serial.Serial(PORT, BAUD, timeout=60) as ser:
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue
            print(line)
            if line == "DONE":
                break
            if "," in line:
                try:
                    s, a = line.split(",")
                    speeds.append(int(s))
                    actuals.append(float(a))
                except ValueError:
                    pass

if not speeds:
    print("Brak danych.")
    sys.exit(1)

plt.figure(figsize=(8, 5))
plt.plot(speeds, actuals, "o-", color="steelblue", linewidth=2, markersize=6)
plt.xlabel("setSpeed() [0–255 PWM]")
plt.ylabel("Prędkość rzeczywista [cm/s]")
plt.title("Charakterystyka PWM → prędkość")
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig("speed_chart.png", dpi=150)
print("Zapisano speed_chart.png")
plt.show()
