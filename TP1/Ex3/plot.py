import re
import matplotlib.pyplot as plt

def parse_file(filename):
    blocks = []
    times = []
    bandwidths = []

    with open(filename, 'r') as f:
        for line in f:
            match = re.search(
                r'Block\s*=\s*(\d+)\s*\|\s*Time\s*=\s*([\d.]+)\s*s\s*\|\s*Bandwidth\s*=\s*([\d.]+)',
                line
            )
            if match:
                blocks.append(int(match.group(1)))
                times.append(float(match.group(2)))
                bandwidths.append(float(match.group(3)))

    return blocks, times, bandwidths

# Parse the output file
blocks, times, bandwidths = parse_file("output.txt")  # change filename if needed

# ---------------- TIME vs BLOCK PLOT ----------------
plt.figure()
plt.plot(blocks, times, marker='o', linestyle='-', color='blue')
plt.xlabel("Block size")
plt.ylabel("Time (seconds)")
plt.title("Execution Time vs Block Size")
plt.grid(True)
plt.savefig("time_vs_block.png", dpi=300, bbox_inches="tight")
plt.close()

# ---------------- MEMORY BANDWIDTH vs BLOCK PLOT ----------------
plt.figure()
plt.plot(blocks, bandwidths, marker='s', linestyle='-', color='green')
plt.xlabel("Block size")
plt.ylabel("Memory Bandwidth (GB/s)")
plt.title("Memory Bandwidth vs Block Size")
plt.grid(True)
plt.savefig("bandwidth_vs_block.png", dpi=300, bbox_inches="tight")
plt.close()
