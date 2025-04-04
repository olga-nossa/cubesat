import serial, time
import pandas as pd
from IPython.display import clear_output
from datetime import datetime

# Configuración del puerto serial
puerto = 'COM3'  # Cambia según tu configuración
baudios = 9600
timeout = 1
ser = serial.Serial(puerto, baudios, timeout=timeout)

# Definir las columnas para un solo sensor
column_names = ["Presion", "Temperatura", "Timestamp"]

# Crear un DataFrame vacío
datos = pd.DataFrame(columns=column_names)

try:
    while True:
        # Leer línea del puerto serial
        datos_linea = ser.readline().decode('utf-8').strip()

        # Separar los datos por coma
        datos_lista = datos_linea.split(',')

        # Verificar si recibimos exactamente dos valores: presión y temperatura
        if len(datos_lista) != 2:
            print(f"Warning: Se recibieron {len(datos_lista)} valores, se esperaban 2 (presión y temperatura)")
            continue

        # Agregar timestamp
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        datos_lista.append(timestamp)

        # Crear DataFrame con una fila
        nueva_fila = pd.DataFrame([datos_lista], columns=column_names)

        # Agregar al DataFrame principal
        datos = pd.concat([datos, nueva_fila], ignore_index=True)

        # Convertir columnas numéricas
        datos[["Presion", "Temperatura"]] = datos[["Presion", "Temperatura"]].astype(float)

        # Mostrar la última fila
        print(datos.iloc[-1])
        clear_output(wait=True)

except KeyboardInterrupt:
    ser.close()
    print('Lectura de datos finalizada.')

# Guardar a Excel
fecha_actual = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
filename = f'datos_sensor_{fecha_actual}.xlsx'
datos.to_excel(filename, sheet_name='Sensor1', index=False)

print(f'Datos guardados en: {filename}')
