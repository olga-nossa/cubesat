import serial
import pandas as pd
from IPython.display import clear_output
from datetime import datetime
from scipy.signal import butter, lfilter, lfilter_zi
import numpy as np

# --- Configuración del puerto serial ---
puerto = 'COM3'
baudios = 9600
timeout = 1
ser = serial.Serial(puerto, baudios, timeout=timeout)

# --- Filtro Butterworth ---
fs = 10  # Hz
fc = 1   # Hz
orden = 3
b, a = butter(orden, fc / (fs / 2), btype='low')
zi = lfilter_zi(b, a)
estado = zi * 0

# --- Densidad del aire en Rionegro (aproximada) ---
rho = 0.97  # kg/m³

# --- DataFrame inicial ---
column_names = ["Presion", "Temperatura", "Presion_filtrada", "Velocidad_viento", "Timestamp"]
datos = pd.DataFrame(columns=column_names)

try:
    while True:
        datos_linea = ser.readline().decode('utf-8').strip()
        datos_lista = datos_linea.split(',')

        if len(datos_lista) != 2:
            print(f"Warning: Se recibieron {len(datos_lista)} valores, se esperaban 2 (presión y temperatura)")
            continue

        try:
            presion = float(datos_lista[0])
            temperatura = float(datos_lista[1])
        except ValueError:
            print("Error de conversión a float")
            continue

        # Filtro en tiempo real
        presion_filtrada, estado = lfilter(b, a, [presion], zi=estado)

        # Cálculo de velocidad del viento (solo si presión > 0 para evitar raíz negativa)
        if presion_filtrada[0] > 0:
            velocidad = np.sqrt(2 * presion_filtrada[0] / rho)
        else:
            velocidad = 0.0

        # Timestamp
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        # Nueva fila
        nueva_fila = pd.DataFrame([[presion, temperatura, presion_filtrada[0], velocidad, timestamp]], columns=column_names)
        datos = pd.concat([datos, nueva_fila], ignore_index=True)

        # Mostrar última fila
        print(datos.iloc[-1])
        clear_output(wait=True)

except KeyboardInterrupt:
    ser.close()
    print('Lectura de datos finalizada.')

# --- Guardar archivo ---
fecha_actual = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
filename = f'datos_sensor_velocidad_{fecha_actual}.xlsx'
datos.to_excel(filename, sheet_name='Sensor1', index=False)
print(f'Datos guardados en: {filename}')
