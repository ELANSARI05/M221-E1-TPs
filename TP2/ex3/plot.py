import matplotlib.pyplot as plt
import numpy as np

p = np.array([1,2,4,8,16,32,64])
fs_values = [fs_5e6, fs_1e7, fs_1e8]

for fs, N in zip(fs_values, [5e6, 1e7, 1e8]):
    S = 1 / (fs + (1 - fs)/p)
    plt.plot(p, S, label=f"N={int(N)}")
    plt.axhline(1/fs, color='gray', linestyle='--')

plt.xlabel("Number of processors (p)")
plt.ylabel("Speedup S(p)")
plt.title("Amdahl Speedup Curves for Different N")
plt.legend()
plt.grid(True)
plt.show()
