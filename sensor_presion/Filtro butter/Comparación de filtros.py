import pandas as pd
import numpy as np
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

# Aplicar el filtro de Butter a los datos
filtered_data = filtfilt(b, a, data)

# Filtro exponencialmente ponderado
class ExponentialAverage:
    def __init__(self, alpha):
        self.alpha = alpha
        self.ema = None

    def add(self, value):
        if self.ema is None:
            self.ema = value
        else:
            self.ema = self.alpha * (value) + (1 - self.alpha) * self.ema
        return self.ema

# Aplicar filtro
def apply_filter(data):
    ema_filter = ExponentialAverage(alpha=0.03) #Crea una instancia de la clase ExponentialAverage con un valor alpha de 0.05

    ema_result = [] #Lista para almacenar los resultados del filtro

    for value in data:
        ema_result.append(ema_filter.add(value)) #Append añade este valor actualizado a la lista ema_result y Add actualiza el promedio exponencial con el nuevo valor y devuelve el promedio actualizado.
    return ema_result
ema_result = apply_filter(data)

# Calcular la velocidad
def calculate_velocity(filtered_data):
    velocity = []
    for value in filtered_data:
        expression_value = 1.9888 * (value + 50) #SE SUMA 50Pa PORQUE ES EL DESFASE DEL SENSOR 1 CUANDO LA DIFERENCIA DE PRESION ES CERO
        if expression_value < 0:
            velocity.append(0)
        else:
            velocity.append(np.sqrt(expression_value))
    return velocity
ema_velocity_function = calculate_velocity(ema_result)

# Graficar los datos originales y filtrados
plt.figure(figsize=(14, 10))

#Filtro butter orden 10
plt.subplot(2, 1, 1)
plt.plot(data, label='Original', alpha=0.5)
plt.plot(filtered_data, label='Filtrado', linestyle='--')
plt.xlabel('Tiempo (s)')
plt.ylabel('Diferencia de presión')
plt.title('Datos con filtro de Butter orden 10')
plt.legend()
plt.grid()
plt.tight_layout()

# Plot filtro exponencialmente ponderado
plt.subplot(2, 1, 2)
plt.plot(data, label='Datos Originales', alpha=0.5)
plt.plot(ema_result, label='Filtro Exponencialmente Ponderado', color='green')
plt.xlabel('Tiempo (s)')
plt.ylabel('Diferencia de presión')
plt.title('Datos con Filtro Exponencialmente Ponderado')
plt.legend()
plt.grid()
plt.tight_layout()


plt.show()

# print(data)

