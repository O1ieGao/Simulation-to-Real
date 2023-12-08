import csv
import sys
import os
# Set up file names
press_file = "./pressure_info.csv"
cop_file = "./cop_info.csv"

# Create lists to store values from file
time = []
sensor_x = []
sensor_z = []
sensor_vals = []

# Read in values from raw pressure file
with open(press_file) as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')
    for line in reader:
        time.append(float(line[0]))
        sensor_x.append((float(line[1]),float(line[4]),float(line[7]),float(line[10]),float(line[13]),float(line[16]),float(line[19]),float(line[22])))
        sensor_z.append((float(line[2]),float(line[5]),float(line[8]),float(line[11]),float(line[14]),float(line[17]),float(line[20]),float(line[23])))
        sensor_vals.append((float(line[3]),float(line[6]),float(line[9]),float(line[12]),float(line[15]),float(line[18]),float(line[21]),float(line[24])))

# Compute CoP coordinates
cop_coords = []
for i in range(len(sensor_x)):
    cop_x = 0.0
    cop_z = 0.0
    sensor_total = 0.0
    for j in range(len(sensor_x[i])):
        cop_x += sensor_x[i][j] * sensor_vals[i][j]
        cop_z += sensor_z[i][j] * sensor_vals[i][j]
        sensor_total += sensor_vals[i][j]
    if sensor_total <= 0.0001:
        cop_coords.append((999.999, 999.999))
    else:
        cop_x = cop_x / sensor_total
        cop_z = cop_z / sensor_total
        cop_coords.append((cop_x, cop_z))

# Write out CoP data file
output_file = open(cop_file, "w")
for i in range(len(cop_coords)):
    output_file.write("{:.4f},{:.6f},{:.6f}\n".format(time[i], cop_coords[i][0], cop_coords[i][1]))
output_file.close()