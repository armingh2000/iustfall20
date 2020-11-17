#!/usr/bin/python3
from random import randint


count_files = 100
count_num_in_file = 1000000
res = 0
M = 1000000000+7

for i in range(count_files):
    file_path = './{}.dat'.format(i)
    with open(file_path, 'w') as data_file:
        lines = []
        for j in range(count_num_in_file):
            current_num = randint(-10000, 10000)
            res += current_num
            res %= M
            lines.append(str(current_num))
        lines.append('\0')
        data_file.write('\r\n'.join(lines))

with open('res.txt', 'w') as resfile:
    resfile.write('{}\r\n'.format(res))

print("result is", res)

