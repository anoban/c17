import numpy as np

x = np.random.randint(0, 255, 100, dtype = np.int64)

bins = []

for num in x:
        for bit in "{:08b}".format(num):
                bins.append(bit)

print(x)

for i, bit in enumerate(bins):
        print(f"assert(getbit(array, {i}) == {bit});")