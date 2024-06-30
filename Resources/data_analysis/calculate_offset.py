def grams_to_hex(g):
    # Convert grams to milligrams
    mg = g * 1000

    # Scale factor
    scale_factor = 195

    # Convert milligrams to LSB
    lsb = round(mg / scale_factor)

    # If the value is negative, convert to two's complement
    if lsb < 0:
        lsb = lsb & 0xff

    # Convert to hexadecimal
    hex_value = hex(lsb)[2:].upper()

    # Print the hexadecimal value
    print(hex_value)

# Take a 16 bit floating point value from standard input
value = float(input("Enter a 8 bit floating point value: "))
grams_to_hex(value)