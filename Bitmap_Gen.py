"""
By TSBread
手打Bitmap三维数组太痛苦了，这种活还是交给py吧
"""
from PIL import Image

im = Image.open("bitmap.png")  # a==255->’#‘, a==0->' '
bitmap_w = 8  # bitmap的宽与高
bitmap_h = 8
o = f"char bitmap[{str(int(im.size[0] / bitmap_w))}][{str(bitmap_h)}][{str(bitmap_w)}] = "  # 输出
o += "{\n"

print(f'{im.size[0]}')
for i in range(int(im.size[0] / bitmap_w)):
    str_temp = "{\n"
    for pic_y in range(bitmap_h):
        for pic_x in range(bitmap_h):
            if im.getpixel((pic_x + i * bitmap_h, pic_y))[3] == 255:
                str_temp += "'#',"
            if im.getpixel((pic_x + i * bitmap_h, pic_y))[3] == 0:
                if pic_y != bitmap_h:
                    str_temp += "' ',"
                else:
                    str_temp += "' '"
        str_temp += "\n"
    if i != int(im.size[0] / bitmap_w):
        str_temp += "},"
    else:
        str_temp += "\n}"
    o += str_temp
o += "\n};"

print(f'{o}')  # 输出，格式化交给Clion了
