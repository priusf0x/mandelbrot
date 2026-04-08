import pandas as pd
import statistics
import matplotlib.pyplot as plt
from math import sqrt

df = pd.read_json('measure.json')
data = df['time']

plt.hist(data, bins=500, edgecolor='black')
plt.title("Basic Distribution (Histogram)")
plt.xlabel("Time, tact")
plt.ylabel("Times")
plt.savefig("meow.png")


mean = statistics.mean(data)
variance = statistics.variance(data)

print(f"({round(float(mean)/10**6, 2)} ± {round((sqrt(variance)/mean * 100), 2)}%) * 10^6 \n")
