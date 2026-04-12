import csv
import random

def generate_data():
    classes = ['SLOW', 'NORMAL', 'AGGRESSIVE']
    with open('data/obd_data.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        # Заголовки строго по ТЗ
        writer.writerow(['speed_kmh', 'engine_rpm', 'throttle_pos', 'coolant_temp', 'fuel_level', 'intake_air_temp', 'label'])
        
        for _ in range(5000):
            label = random.choice(classes)
            # Генерируем "реалистичные" значения в зависимости от стиля
            if label == 'SLOW':
                speed = random.uniform(20, 40)
                rpm = random.uniform(800, 2000)
                throttle = random.uniform(5, 20)
            elif label == 'NORMAL':
                speed = random.uniform(40, 80)
                rpm = random.uniform(2000, 3500)
                throttle = random.uniform(20, 50)
            else: # AGGRESSIVE
                speed = random.uniform(80, 160)
                rpm = random.uniform(3500, 6500)
                throttle = random.uniform(60, 100)
            
            writer.writerow([
                round(speed, 2), 
                round(rpm, 0), 
                round(throttle, 2),
                round(random.uniform(80, 105), 1), # coolant
                round(random.uniform(10, 100), 1), # fuel
                round(random.uniform(20, 50), 1),  # intake
                label
            ])

import os
if not os.path.exists('data'): os.makedirs('data')
generate_data()
print("Датасет data/obd_data.csv создан: 5000 строк, 3 класса.")