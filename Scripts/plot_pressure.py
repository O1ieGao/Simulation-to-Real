import os
import csv
import cv2
import glob
from scipy.spatial import ConvexHull
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

################################################################################
### Configuration
################################################################################
# Input files
com_file = "./com_info.csv"
press_file = "./pressure_info.csv"
cop_file = "./cop_info.csv"

# Output files
frame_dir = "./temp_frames/"
video_file = "./pressure_plot.mp4"

# Output parameters
show_figure = False
write_files = True
print_freq = 10
fps = 50

# Figure parameters
sliding = True
slide_window = 180

################################################################################
### Helper Functions
################################################################################
# Helper function for getting foot center
def get_foot_center(xdata, zdata):
    xmean = 0.0
    zmean = 0.0
    for i in range(len(xdata)):
        xmean += xdata[i]
        zmean += zdata[i]
    xmean /= len(xdata)
    zmean /= len(zdata)
    return xmean, zmean


# Helper function for getting foot boundaries
def get_foot_loops(xdata, zdata):
    xleft = [xdata[0], xdata[1], xdata[3], xdata[2], xdata[0]]
    zleft = [zdata[0], zdata[1], zdata[3], zdata[2], zdata[0]]
    xright = [xdata[4], xdata[5], xdata[7], xdata[6], xdata[4]]
    zright = [zdata[4], zdata[5], zdata[7], zdata[6], zdata[4]]
    return xleft, zleft, xright, zright


# Helper function for getting splits between sensors
def get_sensor_splits(xdata, zdata):
    # Preprocess left x
    x01 = (xdata[0] + xdata[1]) / 2
    x13 = (xdata[1] + xdata[3]) / 2
    x32 = (xdata[3] + xdata[2]) / 2
    x20 = (xdata[2] + xdata[0]) / 2
    # Preprocess left z
    z01 = (zdata[0] + zdata[1]) / 2
    z13 = (zdata[1] + zdata[3]) / 2
    z32 = (zdata[3] + zdata[2]) / 2
    z20 = (zdata[2] + zdata[0]) / 2
    # Preprocess right x
    x45 = (xdata[4] + xdata[5]) / 2
    x57 = (xdata[5] + xdata[7]) / 2
    x76 = (xdata[7] + xdata[6]) / 2
    x64 = (xdata[6] + xdata[4]) / 2
    # Preprocess right z
    z45 = (zdata[4] + zdata[5]) / 2
    z57 = (zdata[5] + zdata[7]) / 2
    z76 = (zdata[7] + zdata[6]) / 2
    z64 = (zdata[6] + zdata[4]) / 2
    # Return values
    xout = [[x01, x32], [x13, x20], [x45, x76], [x57, x64]]
    zout = [[z01, z32], [z13, z20], [z45, z76], [z57, z64]]
    return xout, zout


# Helper function for getting coordinates associated with each sensor
def get_sensor_points(xdata, zdata):
    # Preprocess left x
    x01 = (xdata[0] + xdata[1]) / 2
    x13 = (xdata[1] + xdata[3]) / 2
    x32 = (xdata[3] + xdata[2]) / 2
    x20 = (xdata[2] + xdata[0]) / 2
    xmidleft = (x13 + x20) / 2
    # Preprocess left z
    z01 = (zdata[0] + zdata[1]) / 2
    z13 = (zdata[1] + zdata[3]) / 2
    z32 = (zdata[3] + zdata[2]) / 2
    z20 = (zdata[2] + zdata[0]) / 2
    zmidleft = (z13 + z20) / 2
    # Preprocess right x
    x45 = (xdata[4] + xdata[5]) / 2
    x57 = (xdata[5] + xdata[7]) / 2
    x76 = (xdata[7] + xdata[6]) / 2
    x64 = (xdata[6] + xdata[4]) / 2
    xmidright = (x57 + x64) / 2
    # Preprocess right z
    z45 = (zdata[4] + zdata[5]) / 2
    z57 = (zdata[5] + zdata[7]) / 2
    z76 = (zdata[7] + zdata[6]) / 2
    z64 = (zdata[6] + zdata[4]) / 2
    zmidright = (z57 + z64) / 2
    # X loops
    x0loop = [xdata[0], x01, xmidleft, x20]
    x1loop = [xdata[1], x13, xmidleft, x01]
    x2loop = [xdata[2], x20, xmidleft, x32]
    x3loop = [xdata[3], x32, xmidleft, x13]
    x4loop = [xdata[4], x45, xmidright, x64]
    x5loop = [xdata[5], x57, xmidright, x45]
    x6loop = [xdata[6], x64, xmidright, x76]
    x7loop = [xdata[7], x76, xmidright, x57]
    # Z loops
    z0loop = [zdata[0], z01, zmidleft, z20]
    z1loop = [zdata[1], z13, zmidleft, z01]
    z2loop = [zdata[2], z20, zmidleft, z32]
    z3loop = [zdata[3], z32, zmidleft, z13]
    z4loop = [zdata[4], z45, zmidright, z64]
    z5loop = [zdata[5], z57, zmidright, z45]
    z6loop = [zdata[6], z64, zmidright, z76]
    z7loop = [zdata[7], z76, zmidright, z57]
    # Return values
    xout = [x0loop, x1loop, x2loop, x3loop, x4loop, x5loop, x6loop, x7loop]
    zout = [z0loop, z1loop, z2loop, z3loop, z4loop, z5loop, z6loop, z7loop]
    return xout, zout


# Helper function for getting BoS as convex hull
def get_bos(xdata, zdata, pdata):
    # Figure out which points have pressure on them
    xpoints, zpoints = get_sensor_points(xdata, zdata)
    pts = []
    for i in range(len(pdata)):
        if pdata[i] >= 0.001:
            for j in range(4):
                pts.append([xpoints[i][j], zpoints[i][j]])
    # Get convex hull of BoS
    if pts:
        hull = ConvexHull(pts)
        # Create closed loop from hull vertices
        xout = []
        zout = []
        for vert in hull.vertices:
            xout.append(pts[vert][0])
            zout.append(pts[vert][1])
        xout.append(pts[hull.vertices[0]][0])
        zout.append(pts[hull.vertices[0]][1])
        return xout, zout
    else:
        return [], []


################################################################################
### Main functionality
################################################################################
if __name__ == '__main__':
    # Create lists to store values from file
    times = []
    sensor_x_raw = []
    sensor_z_raw = []
    sensor_vals = []
    com_x = []
    com_z = []
    cop_x = []
    cop_z = []

    # Read time and CoM
    with open(com_file) as csv_file:
        reader = csv.reader(csv_file, delimiter = ',')
        for line in reader:
            times.append(float(line[0]))
            com_x.append(float(line[1]) * 1000.0)
            com_z.append(float(line[3]) * 1000.0)

    # Read in values from raw pressure file
    with open(press_file) as csv_file:
        reader = csv.reader(csv_file, delimiter = ',')
        for line in reader:
            sensor_x_raw.append([float(line[1]),float(line[4]),float(line[7]),float(line[10]),float(line[13]),float(line[16]),float(line[19]),float(line[22])])
            sensor_z_raw.append([float(line[2]),float(line[5]),float(line[8]),float(line[11]),float(line[14]),float(line[17]),float(line[20]),float(line[23])])
            sensor_vals.append([float(line[3]),float(line[6]),float(line[9]),float(line[12]),float(line[15]),float(line[18]),float(line[21]),float(line[24])])

    # Read CoP
    with open(cop_file) as csv_file:
        reader = csv.reader(csv_file, delimiter = ',')
        for line in reader:
            cop_x.append(float(line[1]) * 1000.0)
            cop_z.append(float(line[2]) * 1000.0)

    # Process the pressure sensor locations
    sensor_x = []
    sensor_z = []
    for fnum in range(len(sensor_x_raw)):
        # Get individual lines
        x_line = [xval * 1000.0 for xval in sensor_x_raw[fnum]]
        z_line = [zval * 1000.0 for zval in sensor_z_raw[fnum]]
        # (0, 3) pair
        x0 = x_line[0] + (x_line[0] - x_line[3]) / 2.0
        z0 = z_line[0] + (z_line[0] - z_line[3]) / 2.0
        x3 = x_line[3] + (x_line[3] - x_line[0]) / 2.0
        z3 = z_line[3] + (z_line[3] - z_line[0]) / 2.0

        # (1, 2) pair
        x1 = x_line[1] + (x_line[1] - x_line[2]) / 2.0
        z1 = z_line[1] + (z_line[1] - z_line[2]) / 2.0
        x2 = x_line[2] + (x_line[2] - x_line[1]) / 2.0
        z2 = z_line[2] + (z_line[2] - z_line[1]) / 2.0

        # (4, 7) pair
        x4 = x_line[4] + (x_line[4] - x_line[7]) / 2.0
        z4 = z_line[4] + (z_line[4] - z_line[7]) / 2.0
        x7 = x_line[7] + (x_line[7] - x_line[4]) / 2.0
        z7 = z_line[7] + (z_line[7] - z_line[4]) / 2.0

        # (5, 6) pair
        x5 = x_line[5] + (x_line[5] - x_line[6]) / 2.0
        z5 = z_line[5] + (z_line[5] - z_line[6]) / 2.0
        x6 = x_line[6] + (x_line[6] - x_line[5]) / 2.0
        z6 = z_line[6] + (z_line[6] - z_line[5]) / 2.0

        # Append the line
        sensor_x.append([x0, x1, x2, x3, x4, x5, x6, x7])
        sensor_z.append([z0, z1, z2, z3, z4, z5, z6, z7])


    # Set up the plots
    fig, ax = plt.subplots()
    if show_figure:
        fig.show()
    fig.set_figwidth(8)
    fig.set_figheight(8)
    ax.set_aspect(1.0)
    ax.set_axisbelow(True)
    ax.grid(c='#cccccc', lw=0.3)
    ax.set_xlabel("Global x-axis (mm)", fontsize=18)
    ax.set_ylabel("Global z-axis (mm)", fontsize=18)
    ax.tick_params(axis='both', labelsize=14)
    ax.xaxis.set_major_locator(plt.MultipleLocator(50.0))
    ax.yaxis.set_major_locator(plt.MultipleLocator(50.0))
    
    # Set the window limits
    if sliding:
        xmid, zmid = get_foot_center(sensor_x[0], sensor_z[0])
        ax.set_xlim((xmid - slide_window, xmid + slide_window))
        ax.set_ylim((zmid - slide_window, zmid + slide_window))
    else:
        ax.set_xlim((-250.0, 250.0))
        ax.set_ylim((-250.0, 250.0))

    # Plot the foot boundaries for the first frame
    xleft, zleft, xright, zright = get_foot_loops(sensor_x[0], sensor_z[0])
    box_left, = ax.plot(xleft, zleft, '-', c='#ff8888', lw=2.0, aa=True)
    box_right, = ax.plot(xright, zright, '-', c='#ff8888', lw=2.0, aa=True)

    # Plot the separators for individual sensors
    splitlines = []
    xsplits, zsplits = get_sensor_splits(sensor_x[0], sensor_z[0])
    for i in range(len(xsplits)):
        temp_split, = ax.plot(xsplits[i], zsplits[i], '-', c='#ff8888', lw=1.0, aa=True)
        splitlines.append(temp_split)

    # Plot the BoS
    xbos, zbos = get_bos(sensor_x[0], sensor_z[0], sensor_vals[0])
    bos, = ax.plot(xbos, zbos, '-', c='#000000', lw=6.0, aa=True)

    # Plot the CoP and CoM
    cop, = ax.plot(cop_x[0], cop_z[0], 'P', mfc='#ff00ff', mec='#333333', ms=25.0, mew=1.5, aa=True)
    com, = ax.plot(com_x[0], com_z[0], 'X', mfc='#00ffff', mec='#333333', ms=25.0, mew=1.5, aa=True)

    # Set up the legend
    legend_lines = [plt.Line2D([0], [0], c='#ffffff', marker='X', mfc='#00ffff', mec='#333333', ms=25.0, mew=1.5, aa=True),
                    plt.Line2D([0], [0], c='#ffffff', marker='P', mfc='#ff00ff', mec='#333333', ms=25.0, mew=1.5, aa=True),
                    plt.Line2D([0], [0], c='#222222', lw=6.0, aa=True),
                    plt.Line2D([0], [0], c='#ff8888', lw=2.0, aa=True)]
    legend_vals = ['CoM', 'CoP', 'BoS', 'Sensors']
    ax.legend(legend_lines, legend_vals, fontsize=18, ncol=len(legend_lines), loc='upper center', bbox_to_anchor=(0.5, 1.1), frameon=False)

    # If the output directory doesn't exist, create it
    if write_files and not os.path.exists(frame_dir):
        os.makedirs(frame_dir)

    # Iterate through the data and render the frames
    for i in range(len(sensor_x)):
        # Print frame number to console every so often for sanity
        if (i + 1) % print_freq == 0:
            print("Currently processing frame {}".format(i + 1))
        # Get the data
        xdata = sensor_x[i].copy()
        zdata = sensor_z[i].copy()
        pdata = sensor_vals[i].copy()
        # If sliding, update window bounds
        if sliding:
            xmid, zmid = get_foot_center(xdata, zdata)
            ax.set_xlim((xmid - slide_window, xmid + slide_window))
            ax.set_ylim((zmid - slide_window, zmid + slide_window))
        # Update foot outline data
        xleft, zleft, xright, zright = get_foot_loops(xdata, zdata)
        box_left.set_data(xleft, zleft)
        box_right.set_data(xright, zright)
        # Update sensor split data
        xsplits, zsplits = get_sensor_splits(xdata, zdata)
        for j in range(len(splitlines)):
            splitlines[j].set_data(xsplits[j], zsplits[j])
        # Update BoS data
        xbos, zbos = get_bos(xdata, zdata, pdata)
        bos.set_data(xbos, zbos)
        # Update the CoP and CoM data
        cop.set_data(cop_x[i], cop_z[i])
        com.set_data(com_x[i], com_z[i])
        # Render the frame
        ax.invert_yaxis()
        fig.canvas.draw()
        if write_files:
            frame_filename = frame_dir + "frame_{:05d}.png".format(i)
            fig.savefig(frame_filename, dpi=150.0, pad_inches=0.1)
        fig.canvas.flush_events()

    # Create the video from the frames
    if write_files:
        print("Beginning video write")
        # Get metadata from first frame
        fname_list = glob.glob(frame_dir + "frame_*.png")
        h, w, l = cv2.imread(fname_list[0]).shape
        # Write the video
        writer = cv2.VideoWriter(video_file, cv2.VideoWriter_fourcc(*'mp4v'), fps, (w, h))
        for i in range(len(fname_list)):
            if (i + 1) % print_freq == 0:
                print("Writing frame {}".format(i + 1))
            writer.write(cv2.imread(fname_list[i]))
        writer.release()
        print("Video write complete")