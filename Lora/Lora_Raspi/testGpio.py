import gpiod

chip = gpiod.Chip('gpiochip0')
line = chip.get_line(5)  # Ganti dengan nomor pin yang sesuai
line.request(consumer='test', request_type=gpiod.LINE_REQ_DIR_OUT)

line.set_value(1)  # Aktifkan pin
print("Pin diaktifkan")
line.set_value(0)  # Nonaktifkan pin
print("Pin dinonaktifkan")
