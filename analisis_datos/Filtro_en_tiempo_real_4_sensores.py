import serial
import pandas as pd
from IPython.display import clear_output
from datetime import datetime
from scipy.signal import butter, lfilter, lfilter_zi
import numpy as np

# Configuración del puerto serial
puerto = 'COM7'
baudios = 9600
timeout = 1
ser = serial.Serial(puerto, baudios, timeout=timeout)

# Parámetros del filtro Butterworth
fs = 10   # Frecuencia de muestreo (Hz)
fc = 1    # Frecuencia de corte (Hz)
orden = 3
b, a = butter(orden, fc / (fs / 2), btype='low')

# Inicializar estado del filtro para cada sensor
estado_filtros = {
    "Presion_1": lfilter_zi(b, a) * 0,
    "Presion_2": lfilter_zi(b, a) * 0,
    "Presion_3": lfilter_zi(b, a) * 0,
    "Presion_4": lfilter_zi(b, a) * 0,
}

# Densidad del aire en Rionegro (aproximada)
rho = 0.97  # kg/m³

# --- Columnas del DataFrame ---
column_names = [
    "Presion_1", "Presion_2", "Presion_3", "Presion_4",
    "Temperatura_1", "Temperatura_2", "Temperatura_3", "Temperatura_4",
    "Presion_1_filtrada", "Presion_2_filtrada", "Presion_3_filtrada", "Presion_4_filtrada",
    "Velocidad_1", "Velocidad_2", "Velocidad_3", "Velocidad_4",
    "Timestamp"
]

# --- Crear DataFrame vacío ---
datos = pd.DataFrame(columns=column_names)

try:
    while True:
        datos_linea = ser.readline().decode('utf-8').strip()
        datos_lista = datos_linea.split(',')

        if len(datos_lista) != 8:
            print(f"Warning: Se recibieron {len(datos_lista)} valores, se esperaban 8")
            continue

        try:
            datos_float = list(map(float, datos_lista))
        except ValueError:
            print("Error de conversión a float")
            continue

        presiones = datos_float[0:4]
        temperaturas = datos_float[4:8]

        # Aplicar filtro y calcular velocidad por sensor
        presiones_filtradas = []
        velocidades = []

        for i in range(4):
            key = f"Presion_{i+1}"
            pres_filtrada, estado_filtros[key] = lfilter(b, a, [presiones[i]], zi=estado_filtros[key])
            presiones_filtradas.append(pres_filtrada[0])

            if pres_filtrada[0] > 0:
                velocidad = np.sqrt(2 * pres_filtrada[0] / rho)
            else:
                velocidad = 0.0
            velocidades.append(velocidad)

        # Timestamp
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        # Crear y añadir nueva fila
        fila = presiones + temperaturas + presiones_filtradas + velocidades + [timestamp]
        nueva_fila = pd.DataFrame([fila], columns=column_names)
        datos = pd.concat([datos, nueva_fila], ignore_index=True)

        # Mostrar última fila
        print(datos.iloc[-1])
        clear_output(wait=True)

except KeyboardInterrupt:
    ser.close()
    print('Lectura de datos finalizada.')

# --- Guardar archivo Excel ---
fecha_actual = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
filename = f'datos_4sensores_con_velocidad_{fecha_actual}.xlsx'
datos.to_excel(filename, sheet_name='Sensores', index=False)
print(f'Datos guardados en: {filename}')
