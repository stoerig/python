# Programmeringsteknik webbkurs KTH inlämningsuppgift 1.
# <Edvin Wahlberg>
# <22/6/2015>
# <Takes user-input, a four digit integer and performs Kaprekar's algorith on it. It counts the amount of iterations needed and prints out that value to the user>
# Input integer must be of four digits. 

four_digit_int = input("Input a four digit integer: ")

def Kaprekar_main(four_digit_int):

    four_digit_int = str(four_digit_int)

    i = 0

    while(int(four_digit_int) != 6174):

        four_digit_int = str(four_digit_int)

        while(len(four_digit_int) < 4):
            four_digit_int = '0' + four_digit_int

        greater_Int = int("".join(sorted(four_digit_int, reverse=True)))
        lesser_Int = int("".join(sorted(four_digit_int)))
        four_digit_int = (int(greater_Int) - int(lesser_Int))
        i += 1

    print("It took " + str(i) + " iteratious to reach 6174")


Kaprekar_main(four_digit_int)