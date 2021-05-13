import os
import open3d

def ply_binary2ascii(file_full_name):
    pointcloud_in = open3d.io.read_point_cloud(file_full_name)
    # write_point_cloud(filename, pointcloud, write_ascii=False, compressed=False,
    #                   print_progress=False):  # real signature unknown; restored from __doc__
    open3d.io.write_point_cloud(filename=file_full_name, pointcloud=pointcloud_in, write_ascii=True)
    print("transformed " + file_full_name + " from binary to ascii")


def ply_double2float(file_full_name):
    # 打开当前文件
    with open(file_full_name, 'r', encoding="utf-8") as f:
        lines = []  # 创建了一个空列表，里面没有元素
        # 按行读取到内存
        for line in f.readlines():
            if "float" in line:
                print("the datatype of " + file_full_name + " is already float")
                return
            if line != '\n':
                lines.append(line)
        f.close()
    # 更改计数
    count = 0
    n = len(lines)  # 文件行数
    for i in range(n):
        # 更改了3个则break，x,y,z的property由double改为float
        if count == 3:
            break
        ''' 按空格进行字符串分割，比如“property double x”
        分割为str_split[0]="property"
        str_split[1]="double"
        str_split[2]="x"
        '''
        # 判断是否是xyz的property行，是就更改double为float
        str_split = lines[i].split(" ")
        if len(str_split) == 3:
            if str_split[1] == "double":
                if str_split[2] == "x\n" or str_split[2] == "y\n" or str_split[2] == "z\n":
                    lines[i] = "property float " + str_split[2]
                    count = count + 1
    # 文件写入
    f = open(file_full_name, "w")
    for i in range(n):
        f.write(lines[i])
    f.close()
    print("transformed datatype of" + file_full_name + " from double to float")


def findAllFile(base):
    for root, ds, fs in os.walk(base):
        for f in fs:
            ext_str = f.split(".")[1]
            if ext_str != "ply":
                print(f + "不是ply文件")
                yield fullname
                continue
            fullname = os.path.join(root, f)
            yield fullname


if __name__ == '__main__':

    for filename in findAllFile("./"):
        print("processing: " + filename)
        # binary--->ascii
        ply_binary2ascii(file_full_name=filename)
        # double--->float
        ply_double2float(file_full_name=filename)
