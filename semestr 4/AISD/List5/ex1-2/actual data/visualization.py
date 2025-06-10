import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

# Ustaw styl wykresów
plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

# Wczytaj dane z pliku CSV
try:
    data = pd.read_csv('benchmark_results.csv')
    print("Dane wczytane pomyślnie!")
    print(f"Liczba testowanych rozmiarów grafów: {len(data)}")
    print(f"Zakres n: {data['n'].min()} - {data['n'].max()}")
except FileNotFoundError:
    print("Błąd: Nie znaleziono pliku benchmark_results.csv")
    print("Najpierw uruchom testy wydajności z programu C++")
    exit(1)

# Tworzenie wykresów
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))

# Wykres 1: Porównanie czasów wykonania
ax1.errorbar(data['n'], data['prim_avg_time'], yerr=data['prim_std_dev'], 
             label='Algorytm Prima', marker='o', linewidth=2, capsize=3)
ax1.errorbar(data['n'], data['kruskal_avg_time'], yerr=data['kruskal_std_dev'], 
             label='Algorytm Kruskala', marker='s', linewidth=2, capsize=3)
ax1.set_xlabel('Liczba wierzchołków (n)')
ax1.set_ylabel('Średni czas wykonania (μs)')
ax1.set_title('Porównanie czasów wykonania algorytmów MST')
ax1.legend()
ax1.grid(True, alpha=0.3)

# Wykres 2: Skala logarytmiczna
ax2.errorbar(data['n'], data['prim_avg_time'], yerr=data['prim_std_dev'], 
             label='Algorytm Prima', marker='o', linewidth=2, capsize=3)
ax2.errorbar(data['n'], data['kruskal_avg_time'], yerr=data['kruskal_std_dev'], 
             label='Algorytm Kruskala', marker='s', linewidth=2, capsize=3)
ax2.set_xlabel('Liczba wierzchołków (n)')
ax2.set_ylabel('Średni czas wykonania (μs) - skala log')
ax2.set_title('Porównanie czasów - skala logarytmiczna')
ax2.set_yscale('log')
ax2.legend()
ax2.grid(True, alpha=0.3)

# Wykres 3: Stosunek czasów wykonania
ratio = data['kruskal_avg_time'] / data['prim_avg_time']
ax3.plot(data['n'], ratio, 'ro-', linewidth=2, markersize=6)
ax3.axhline(y=1, color='black', linestyle='--', alpha=0.5, label='Kruskal = Prima')
ax3.set_xlabel('Liczba wierzchołków (n)')
ax3.set_ylabel('Stosunek Kruskal/Prima')
ax3.set_title('Względna wydajność algorytmów')
ax3.legend()
ax3.grid(True, alpha=0.3)

# Wykres 4: Złożoność teoretyczna vs praktyczna
n_values = data['n'].values
# Teoretyczna złożoność O(V² log V)
theoretical_complexity = n_values**2 * np.log2(n_values)
# Normalizacja do pierwszego punktu
theoretical_complexity = theoretical_complexity / theoretical_complexity[0]

prim_normalized = data['prim_avg_time'] / data['prim_avg_time'].iloc[0]
kruskal_normalized = data['kruskal_avg_time'] / data['kruskal_avg_time'].iloc[0]

ax4.plot(data['n'], prim_normalized, 'o-', label='Prima (rzeczywisty)', linewidth=2)
ax4.plot(data['n'], kruskal_normalized, 's-', label='Kruskal (rzeczywisty)', linewidth=2)
ax4.plot(data['n'], theoretical_complexity, '--', label='O(V² log V) teoretyczny', 
         linewidth=2, alpha=0.7)
ax4.set_xlabel('Liczba wierzchołków (n)')
ax4.set_ylabel('Czas znormalizowany')
ax4.set_title('Złożoność teoretyczna vs praktyczna')
ax4.legend()
ax4.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('mst_algorithms_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

# Analiza statystyczna
print("\n=== ANALIZA STATYSTYCZNA ===")
print(f"Średni stosunek Kruskal/Prima: {ratio.mean():.3f}")
print(f"Odchylenie standardowe stosunku: {ratio.std():.3f}")
print(f"Mediana stosunku: {ratio.median():.3f}")

# Korelacja między n a czasem wykonania
prim_corr = np.corrcoef(data['n'], data['prim_avg_time'])[0, 1]
kruskal_corr = np.corrcoef(data['n'], data['kruskal_avg_time'])[0, 1]
print(f"\nKorelacja n vs czas Prima: {prim_corr:.4f}")
print(f"Korelacja n vs czas Kruskal: {kruskal_corr:.4f}")

# Dopasowanie funkcji potęgowej
from scipy.optimize import curve_fit

def power_function(x, a, b):
    return a * np.power(x, b)

# Dopasowanie dla Prima
prim_params, _ = curve_fit(power_function, data['n'], data['prim_avg_time'])
print(f"\nDopasowanie Prima: {prim_params[0]:.3f} * n^{prim_params[1]:.3f}")

# Dopasowanie dla Kruskala
kruskal_params, _ = curve_fit(power_function, data['n'], data['kruskal_avg_time'])
print(f"Dopasowanie Kruskal: {kruskal_params[0]:.3f} * n^{kruskal_params[1]:.3f}")

print(f"\nWykresy zapisano do pliku: mst_algorithms_comparison.png")

# Dodatkowy wykres szczegółowy dla małych n
if len(data) > 10:
    plt.figure(figsize=(10, 6))
    small_data = data[data['n'] <= 30]  # Tylko małe grafy
    
    plt.errorbar(small_data['n'], small_data['prim_avg_time'], 
                yerr=small_data['prim_std_dev'], 
                label='Algorytm Prima', marker='o', linewidth=2, capsize=3)
    plt.errorbar(small_data['n'], small_data['kruskal_avg_time'], 
                yerr=small_data['kruskal_std_dev'], 
                label='Algorytm Kruskala', marker='s', linewidth=2, capsize=3)
    
    plt.xlabel('Liczba wierzchołków (n)')
    plt.ylabel('Średni czas wykonania (μs)')
    plt.title('Szczegółowe porównanie dla małych grafów (n ≤ 30)')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('mst_small_graphs_detail.png', dpi=300, bbox_inches='tight')
    plt.show()