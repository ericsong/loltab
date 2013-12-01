from PIL import Image
img = Image.open('img1098.jpg')
pix = img.load()

#143 150 143

def isBorder(pixel):
    r = pixel[0]
    g = pixel[1]
    b = pixel[2]
    if(140 < r and r < 160):
        if(140 < g and g < 160):
            if(140 < b and b < 160):
                return True
    return False

for y in range(250):
    count = 0
    for x in range(400,1500):
        if(isBorder(pix[x,y])):
            count = count + 1 
    if(count > 300):
        print("scoreboard found")
