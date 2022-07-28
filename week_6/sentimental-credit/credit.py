from cs50 import get_string


def luhn_check(number):
    """Runs the Luhn's algorigthm on a cc number. If the number is
    ok, returns True."""
    sum = 0

    for index, digit in enumerate(reversed(number)):

        if index % 2:
            multiple = str(int(digit) * 2)
            if len(multiple) == 2:
                sum += int(multiple[1]) + int(multiple[0])
            else:
                sum += int(multiple[0])
        else:
            sum += int(digit)

    if str(sum)[-1] == "0":
        return True
    else:
        return False


number = get_string("Number: ").strip()
if luhn_check(number):
    # Check for AMEX.
    if (len(number) == 15) and (number[0:2] in ["34", "37"]):
        print("AMEX")
    # Check for MASTERCARD.
    elif (len(number) == 16) and (number[0:2] in ["51", "52", "53", "54", "55"]):
        print("MASTERCARD")
    # Check for VISA.
    elif (len(number) in [13, 16]) and (number[0] == "4"):
        print("VISA")
    # If none of the above pass, the card number is invalid
    else:
        print("INVALID")
else:
    print("INVALID")
