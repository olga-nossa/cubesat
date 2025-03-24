import pandas as pd
from scipy.signal import butter, filtfilt
import matplotlib.pyplot as plt

# Importar archivo - propiedad de Olga
file_path = 'C:/Users/pipix/Documents/VOLTA/Sensores de presión diferencial/Filtros/presiones.ods'  # Ruta archivo
data = pd.read_excel(file_path, usecols=[0], header=None)  # Lee la primera columna
data = data.squeeze().values  # Convierte a un array de numpy
valor_min = data[0]
data = data - valor_min

# Parámetros
fs = 400000  # Frecuencia de muestreo
fc = 10000    # Frecuencia de corte 

# Diseñar el filtro Butterworth
order = 10
nyquist = 0.5 * fs
normal_cutoff = fc / nyquist
b, a = butter(order, normal_cutoff, btype='low', analog=False)

# Aplicar el filtro a los datos
filtered_data = filtfilt(b, a, data)

# Graficar los datos originales y filtrados
plt.figure(figsize=(14, 10))
plt.plot(data, label='Original', alpha=0.5)
plt.plot(filtered_data, label='Filtrado', linestyle='--')
plt.xlabel('Tiempo (s)')
plt.ylabel('Diferencia de presión')
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()

# print(data)