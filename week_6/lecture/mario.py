def main():
    for i in range(get_height()):
        print("#")

def get_height():
    while True:
        try:
            n = int(input("Number: "))
            if n > 0:
                break
        except ValueError:
            print("That's not an integer!")
    return n

main()