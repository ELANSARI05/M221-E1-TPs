import re
import matplotlib.pyplot as plt

def parse_file(filename):
    n_vals = []
    times = []
    bandwidths = []

    with open(filename, 'r') as f:
        for line in f:
            match = re.search(
                r'n\s*=\s*(\d+)\s*\|\s*Time\s*=\s*([\d.]+)\s*s\s*\|\s*Bandwidth\s*=\s*([\d.]+)',
                line
            )
            if match:
                n_vals.append(int(match.group(1)))
                times.append(float(match.group(2)))
                bandwidths.append(float(match.group(3)))

    return n_vals, times, bandwidths


# Parse both files
n1, t1, b1 = parse_file("output.txt")     # mxm_jk
n2, t2, b2 = parse_file("output2.txt")    # mxm_kj

# ---------------- TIME PLOT ----------------
plt.figure()
plt.plot(n1, t1, marker='o', label='mxm_jk')
plt.plot(n2, t2, marker='s', label='mxm_kj')

plt.xlabel("n (matrix size)")
plt.ylabel("Time (seconds)")
plt.title("Execution Time Comparison")
plt.legend()
plt.grid(True)

plt.savefig("time.png", dpi=300, bbox_inches="tight")
plt.close()

# ---------------- MEMORY BANDWIDTH PLOT ----------------
plt.figure()
plt.plot(n1, b1, marker='o', label='mxm_jk')
plt.plot(n2, b2, marker='s', label='mxm_kj')

plt.xlabel("n (matrix size)")
plt.ylabel("Memory Bandwidth (GB/s)")
plt.title("Memory Bandwidth Comparison")
plt.legend()
plt.grid(True)

plt.savefig("memory.png", dpi=300, bbox_inches="tight")
plt.close()
