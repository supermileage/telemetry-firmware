import numpy as np

# Constants
ACCEL_GRAVITY = 9.8067
ALPHA = 0.25
EULERS = np.e
ACCEL_READ_INTERVAL = 10  # ms

COEFF = np.array([-36, 9, 44, 69, 84, 89, 84, 69, 44, 9, -36])
NORM = 429
WINDOW_SIZE = 11

class LowPassFilter:
    def __init__(self, alpha):
        self.alpha = alpha
        self.prev = None

    def filter(self, value):
        if self.prev is None:
            self.prev = value
        else:
            self.prev = self.alpha * value + (1 - self.alpha) * self.prev
        return self.prev

class SavGolFilter:
    def __init__(self, coeff, norm, window_size):
        self.coeff = coeff
        self.norm = norm
        self.window_size = window_size
        self.buffer = []

    def filter(self, value):
        self.buffer.append(value)
        if len(self.buffer) < self.window_size:
            return np.mean(self.buffer)
        elif len(self.buffer) > self.window_size:
            self.buffer.pop(0)
        result = 0
        for i in range(self.window_size):
            result += self.coeff[i] * self.buffer[i]
        result /= self.norm
        return result

def normalY(x):
    median = 6
    variance = 0.5
    return ACCEL_GRAVITY * np.exp(-0.5 * ((x - median) / variance) ** 2)

def normalZ(x):
    median = 9
    variance = 2
    return ACCEL_GRAVITY * np.exp(-0.5 * ((x - median) / variance) ** 2)

def calc_pitch(y, z):
    denom = np.sqrt(y*y + z*z)
    if denom == 0:
        return 0
    pitch = np.arccos(y / denom)
    if z <= 0:
        pitch *= -1
    return pitch

# Simulation
times = np.arange(0.01, 20.0, ACCEL_READ_INTERVAL / 1000.0)

# Margins for LowPass
max_margin_incline_lpf = 0
max_margin_haccel_lpf = 0
max_margin_vaccel_lpf = 0
max_margin_actualY_lpf = 0
max_margin_actualZ_lpf = 0

# Margins for SavGol
max_margin_incline_sg = 0
max_margin_haccel_sg = 0
max_margin_vaccel_sg = 0
max_margin_actualY_sg = 0
max_margin_actualZ_sg = 0

# Filters
lpf_y = LowPassFilter(ALPHA)
lpf_z = LowPassFilter(ALPHA)
sgf_y = SavGolFilter(COEFF, NORM, WINDOW_SIZE)
sgf_z = SavGolFilter(COEFF, NORM, WINDOW_SIZE)

for t in times:
    y = normalY(t)
    z = normalZ(t)
    y_plus_g = y + ACCEL_GRAVITY

    # LowPass
    fy_lpf = lpf_y.filter(y_plus_g)
    fz_lpf = lpf_z.filter(z)
    pitch_lpf = calc_pitch(fy_lpf, fz_lpf)
    gravityY_lpf = ACCEL_GRAVITY * np.cos(pitch_lpf)
    gravityZ_lpf = ACCEL_GRAVITY * np.sin(pitch_lpf)
    haccel_lpf = round((fz_lpf - gravityZ_lpf) * 100) / 100
    vaccel_lpf = round((fy_lpf - gravityY_lpf) * 100) / 100
    actualY_lpf = fy_lpf - ACCEL_GRAVITY
    actualZ_lpf = fz_lpf

    max_margin_incline_lpf = max(max_margin_incline_lpf, abs(pitch_lpf - 0))
    max_margin_haccel_lpf = max(max_margin_haccel_lpf, abs(haccel_lpf - z))
    max_margin_vaccel_lpf = max(max_margin_vaccel_lpf, abs(vaccel_lpf - (y + ACCEL_GRAVITY)))
    max_margin_actualY_lpf = max(max_margin_actualY_lpf, abs(actualY_lpf - y))
    max_margin_actualZ_lpf = max(max_margin_actualZ_lpf, abs(actualZ_lpf - z))

    # SavGol
    fy_sg = sgf_y.filter(y_plus_g)
    fz_sg = sgf_z.filter(z)
    pitch_sg = calc_pitch(fy_sg, fz_sg)
    gravityY_sg = ACCEL_GRAVITY * np.cos(pitch_sg)
    gravityZ_sg = ACCEL_GRAVITY * np.sin(pitch_sg)
    haccel_sg = round((fz_sg - gravityZ_sg) * 100) / 100
    vaccel_sg = round((fy_sg - gravityY_sg) * 100) / 100
    actualY_sg = fy_sg - ACCEL_GRAVITY
    actualZ_sg = fz_sg

    max_margin_incline_sg = max(max_margin_incline_sg, abs(pitch_sg - 0))
    max_margin_haccel_sg = max(max_margin_haccel_sg, abs(haccel_sg - z))
    max_margin_vaccel_sg = max(max_margin_vaccel_sg, abs(vaccel_sg - (y + ACCEL_GRAVITY)))
    max_margin_actualY_sg = max(max_margin_actualY_sg, abs(actualY_sg - y))
    max_margin_actualZ_sg = max(max_margin_actualZ_sg, abs(actualZ_sg - z))

print("LowPass Filter:")
print(f"  Max margin for incline: {max_margin_incline_lpf:.3f}")
print(f"  Max margin for haccel:  {max_margin_haccel_lpf:.3f}")
print(f"  Max margin for vaccel:  {max_margin_vaccel_lpf:.3f}")
print(f"  Max margin for actualY: {max_margin_actualY_lpf:.3f}")
print(f"  Max margin for actualZ: {max_margin_actualZ_lpf:.3f}")

print("SavGol Filter:")
print(f"  Max margin for incline: {max_margin_incline_sg:.3f}")
print(f"  Max margin for haccel:  {max_margin_haccel_sg:.3f}")
print(f"  Max margin for vaccel:  {max_margin_vaccel_sg:.3f}")
print(f"  Max margin for actualY: {max_margin_actualY_sg:.3f}")
print(f"  Max margin for actualZ: {max_margin_actualZ_sg:.3f}")
