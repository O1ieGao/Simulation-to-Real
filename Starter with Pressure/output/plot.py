import pandas as pd
import matplotlib.pyplot as plt

# Load data from CSV file
data = pd.read_csv('stability_info.csv', header=None)

# Extract time and distance data into separate arrays
time = data[0].values
distance = data[1].values
print(distance)
# Create the plot
plt.plot(time, distance)
plt.xlabel('Time (s)')
plt.ylabel('Distance (m)')
plt.title('CoM to CoP Distance vs. Time')
plt.grid(True)

# Display the plot
plt.show()