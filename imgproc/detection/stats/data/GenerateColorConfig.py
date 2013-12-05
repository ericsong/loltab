import sys
import math

def getPooledStats(filename, data_type):
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

    f = open(filename)
    lines = f.readlines()
    data_type = 'std ' + data_type

    mean = 0
    n = 0
    sd = 0

    for line in lines:
        if(data_type in line):
            n2 = 800
            mean2 = float(line.split(',')[0].split(':')[1])
            sd2 = float(line.split(',')[1][1:-1])
           
            sd = math.sqrt( ( ((n - 1)*math.pow(sd,2)) + ((n2 - 1)*math.pow(sd2,2)) + ( ((n*n2)/(n + n2))*(math.pow(mean,2) + math.pow(mean2,2) - 2*mean*mean2) )  ) / (n + n2 - 1) )
            mean = ((n*mean) + (n2*mean2))/(n+n2)
            n = n + n2;

#    if(data_type[-1] == 'l'):
#        mean = mean/100*255

    print("#define " + side + "_" + str(resolution) + "_" + data_type[-1] + "_MEAN " + str(mean))
    print("#define " + side + "_" + str(resolution) + "_" + data_type[-1] + "_DIFF " + str(sd*4))

filename = sys.argv[1];

getPooledStats(filename, 'l');
getPooledStats(filename, 'a');
getPooledStats(filename, 'b');

#print("#define " + side + "_" + str(resolution) + "_l_MEAN")
