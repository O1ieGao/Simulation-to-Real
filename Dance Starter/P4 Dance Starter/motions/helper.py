import math

def radianceToDegree(l):
    for x in l:
        print(math.ceil(x / 3.14 * 180))

def degreeToRadiance(l):
    for x in l:
        print(x * 3.14 / 180)

# r = [1.57, 2.44, 2.88, 1.57, 0.7, 0.26, 1.57, 1.05, 1.33, 1.92, 1.57, 2.09, 1.81, 1.22, 1.57, 1.57]
# radianceToDegree(r)

# d = [3.14/2]
# degreeToRadiance([100, 80])

# radianceToDegree([1.33, 1.81])

degreeList = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180]
radiantList = []
degreeToRadiance([145, 35, 55, 125])

for x in range(len(degreeList)):
    radiantList.append(degreeList[x] * 3.14 / 180)

for x in range(len(degreeList)):
    print("degree: ", degreeList[x], ", radiant: ", radiantList[x])