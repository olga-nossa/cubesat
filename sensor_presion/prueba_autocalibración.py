import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Función para cargar datos
def load_ods_data(filepath, column=0):
    df = pd.read_excel(filepath, engine="odf")
    data = df.iloc[:, column].dropna().values # Selecciona la primera columna
    return pd.DataFrame(data, columns=['Pressure'])

# Función para estimar y corregir el desfase inicial
def initial_offset_correction(data, init_samples=50):
    initial_offset = np.mean(data[:init_samples])
    return data - initial_offset

# Filtro adaptativo para ajustar el desfase
def adaptive_offset_correction(data, window_size=30):
    offsets = np.zeros_like(data)
    corrected_data = np.zeros_like(data)
    for i in range(len(data)):
        start = max(0, i - window_size // 2)
        end = min(len(data), i + window_size // 2)
        offsets[i] = np.mean(data[start:end])
        corrected_data[i] = data[i] - offsets[i]
    return corrected_data

# Filtro de Butterworth
def butter_filter(data, cutoff, fs, filter_type='low', order=4):
    nyquist = 0.5 * fs
    normal_cutoff = cutoff / nyquist
    b, a = butter(order, normal_cutoff, btype=filter_type, analog=False)
    return filtfilt(b, a, data)

# Proceso completo
def process_data(filepath, init_samples, window_size, cutoff_freq, fs):
    # Cargar datos desde archivo ODS
    df = load_ods_data(filepath)
    
    # Paso 1: Corrección inicial de desfase
    df['Corrected_Initial'] = initial_offset_correction(df['Pressure'].values, init_samples=init_samples)
    
    # Paso 2: Corrección adaptativa del desfase
    df['Calibrated_Pressure'] = adaptive_offset_correction(df['Corrected_Initial'].values, window_size=window_size)
    
    # Paso 3: Filtrado de frecuencias no deseadas
    df['Filtered_Pressure'] = butter_filter(df['Calibrated_Pressure'].values, cutoff=cutoff_freq, fs=fs, filter_type='low')
    
    return df

# Parámetros
filepath = 'presiones.ods'         # Ruta del archivo ODS
init_samples = 50             # Número de muestras iniciales para la calibración
window_size = 30              # Tamaño de la ventana para la corrección adaptativa
cutoff_freq = 1.0             # Frecuencia de corte para el filtro (Hz)
fs = 10                       # Frecuencia de muestreo (Hz)

# Procesar los datos y obtener el DataFrame
df = process_data(filepath, init_samples, window_size, cutoff_freq, fs)

# Graficar los resultados
plt.figure(figsize=(10, 6))
plt.plot(df.index, df['Pressure'], label='Presión Sin Calibrar', color='blue', alpha=0.7)
plt.plot(df.index, df['Calibrated_Pressure'], label='Presión Calibrada', color='orange', alpha=0.7)
plt.plot(df.index, df['Filtered_Pressure'], label='Presión Filtrada', color='green', alpha=0.7)
plt.xlabel("Tiempo (s)")
plt.ylabel("Presión (Pa)")
plt.title("Corrección y Filtrado de Datos de Presión")
plt.legend()
plt.grid(True)
plt.show()
