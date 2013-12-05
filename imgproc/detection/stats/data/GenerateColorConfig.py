import sys
import math

filename = sys.argv[1];

if('top' in filename):
    side = 'TOP'
elif('left' in filename):
    side = 'LEFT'
elif('right' in filename):
    side = 'RIGHT'
elif('bottom' in filename):
    side = 'BOTTOM'

if('1080' in filename):
    resolution = 1080
elif('720' in filename):
    resolution = 720

getPooledData(filename, 'a');

def getPooledStats(filename, data_type):
    f = open(filename)
    lines = f.readlines()

    mean = 0
    n = 0
    sd = 0

    for line in lines:
        if('std l' in line):
            n2 = 1
            mean2 = float(line.split(',')[0].split(':')[1])
            sd2 = float(line.split(',')[1][1:-1])
           
            mean = ((n*mean) + (n2*mean2))/(n+n2)
            n = n + n2;
            sd = math.sqrt( ( ((n - 1)*math.pow(sd,2)) + ((n2 - 1)*math.pow(sd2,2)) + ( ((n*n2)/(n + n2))*(math.pow(mean,2) + math.pow(mean2,2) - 2*mean*mean2) )  ) / (n + n2 -1) )


    print("mean: " + str(mean))
    print("sd: " + str(sd))

#print("#define " + side + "_" + str(resolution) + "_l_MEAN")
