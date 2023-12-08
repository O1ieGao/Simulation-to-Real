import csv
import math
import sys

# Filenames
com_file = "./com_info.csv"
cop_file = "./cop_info.csv"
stability_file = "./stability_info.csv"

# Helper function for distance between two points
def get_dist(ptA, ptB):
    xdiff = ptA[0] - ptB[0]
    ydiff = ptA[1] - ptB[1]
    zdiff = ptA[2] - ptB[2]
    return math.sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff)

# Create lists to store values from files
time = []
com = []
cop = []

# Read time and CoM
with open(com_file) as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')
    for line in reader:
        time.append(float(line[0]))
        com.append((float(line[1]), 0.0, float(line[3])))

# Read CoP
with open(cop_file) as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')
    for line in reader:
        cop.append((float(line[1]), 0.0, float(line[2])))

# Compute stability at each time step
stability = []
for i in range(len(time)):
    if cop[i][0] == 999.999 and cop[i][2] == 999.999:
        stability.append(-0.01)
    else:
        stability.append(get_dist(com[i], cop[i]))

# Write out stability
output_file = open(stability_file, "w")
for i in range(len(time)):
    output_file.write("{:.4f},{:.6f}\n".format(time[i], stability[i]))
output_file.close()