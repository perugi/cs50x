# Print out a pyramid of a specified height.

while True:
    try:
        height = int(input("Height: "))
    # Check for the fact that an integer was input.
    except ValueError:
        print("Please input a numeric value!")
        continue

    # Check for the height limits
    if height > 0 and height < 9:
        break
    print("Please input number between 1 and 8!")

# Print the actual pyramid
for row in range(height - 1, -1, -1):
    print(" " * row, end="")
    print("#" * (height - row), end="")
    print("  ", end="")
    print("#" * (height - row))
