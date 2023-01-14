import matplotlib.pyplot as plt

def print_all(im):
    for i in im:
        print()
        for j in range(len(i)):
            print(hex(i[j]), end=" ")

def main():
    f_name = "lena.pgm"
    with open(f_name, 'rb') as pgmf:
        im = plt.imread(pgmf)
    print_all(im)

    center = (0, 7)
    for i in range(-1, 2):
        for j in range(-1, 2):
            if (center[0] + i >= 0) and (center[1] + j >= 0):
                print(f"\n\n\n{hex(im[center[0] + i][center[1] + j])}")


if __name__ == "__main__":
    main()
