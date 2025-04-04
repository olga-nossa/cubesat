import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Parámetros del filtro 
fs = 10       # Frecuencia de muestreo (Hz)
fc = 1        # Frecuencia de corte (Hz)
orden = 5     # Orden más alto para mayor suavizado
rho = 0.97    # Densidad del aire en Rionegro (kg/m³)

# Leer el archivo generado 
archivo = "datos_sensor_velocidad_2025-03-24_21-09-57.xlsx"  
datos = pd.read_excel(archivo)

# Asegurar que las columnas están como float
datos["Presion"] = datos["Presion"].astype(float)
datos["Presion_filtrada"] = datos["Presion_filtrada"].astype(float)

# Aplicar filtro Butterworth potente (filtfilt)
b, a = butter(orden, fc / (fs / 2), btype='low')
presion_filtrada = filtfilt(b, a, datos["Presion"].values)
datos["Presion_filtrada_fuerte"] = presion_filtrada

# Calcular velocidad del viento nuevamente con presión filtrada fuerte
velocidad = np.sqrt(2 * np.maximum(presion_filtrada, 0) / rho)
datos["Velocidad_viento_post"] = velocidad

# Gráfico 1: Presión original vs filtrada
plt.figure(figsize=(12, 5))
plt.plot(datos["Presion"], label="Presión original", alpha=0.5)
plt.plot(datos["Presion_filtrada_fuerte"], label="Presión filtrada (Butterworth orden 5)", linewidth=2)
plt.title("Comparación de presión original vs filtrada")
plt.xlabel("Muestras")
plt.ylabel("Presión (Pa)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Gráfico 2: Comparación de filtros
plt.figure(figsize=(12, 5))
plt.plot(datos["Presion_filtrada"], label="Presión filtrada en tiempo real", alpha=0.5)
plt.plot(datos["Presion_filtrada_fuerte"], label="Presión filtrada en post-procesamiento(Butterworth orden 5)", linewidth=2)
plt.title("Comparación de presión filtrada en tiempo real y en post-procesamiento")
plt.xlabel("Muestras")
plt.ylabel("Presión (Pa)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Gráfico 3: Velocidad del viento
plt.figure(figsize=(12, 4))
plt.plot(datos["Velocidad_viento_post"], label="Velocidad del viento (m/s)", color='green')
plt.title("Velocidad del viento estimada desde presión diferencial")
plt.xlabel("Muestras")
plt.ylabel("Velocidad (m/s)")
plt.grid(True)
plt.tight_layout()
plt.show()

