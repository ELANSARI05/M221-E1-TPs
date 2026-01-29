import matplotlib.pyplot as plt

# Example data (replace or load from files if needed)
stride = list(range(1, 21))

time_stride = [2.521, 2.840, 3.208, 3.236, 3.723, 4.142, 4.748, 5.182, 5.953, 7.013,
               7.641, 8.724, 9.733, 10.934, 11.712, 12.345, 13.761, 14.263, 14.759, 15.021]

time_stride2 = [1.220, 1.397, 1.602, 1.902, 2.331, 2.849, 3.332, 3.965, 4.378, 4.718,
                5.076, 5.503, 5.732, 6.128, 6.332, 7.597, 7.169, 7.356, 7.592, 7.837]

# Create plot
plt.figure()
plt.plot(stride, time_stride, label="O0")
plt.plot(stride, time_stride2, label="O2")
plt.xlabel("Stride")
plt.ylabel("Execution Time (ms)")
plt.legend()

# Save plot in current directory
plt.savefig("image.png")
plt.close()


