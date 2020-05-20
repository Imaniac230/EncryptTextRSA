#!/usr/bin/env python3

import sys, os, random
from enum import IntEnum
from datetime import datetime

# displays all printed chars as DEC values on console
DEBUG_MODE = False
DEBUG_NUMBER_OF_COLUMNS = 4

# simplifying constants
LIMIT = 255
LOW_LIMIT = 227
START_POINT = 2

# white characters definitions
TAB = 9
NEW_LINE = '\n'
SPACE   = ' '

class TError(IntEnum):
    ENullptr = 0
    EBadName = 1
    EBadOpen = 2
    EBadScan = 3


def is_in_letter_range(character):
    if (character >= 32) and (character <= 126):
        res = chr(character)
    else:
        res = "Ee"

    return res


def close_file(aInFile, aOutFile = None):
    if aInFile is not None:
        try:
            aInFile.close()
        except Exception as err:
            raise Exception(err)

    if aOutFile is not None:
        try:
            aOutFile.close()
        except Exception as err:
            raise Exception(err)


def close_program(aFin = None, aFout = None):
    try:
        close_file(aFin, aFout)
    except Exception as  err:
        print(f"\n{sys.argv[0]}: WARNING! Error closing files. ({err})\n", file=sys.stderr)

    try:
        input(f"\n{sys.argv[0]}: Press \"enter\" to exit.")
    except Exception as err:
        print(f"\n{sys.argv[0]}: WARNING! Error exiting program. ({err})\n", file=sys.stderr)


def open_input_file():
    if len(sys.argv) < 2:
        try:
            aFilename = input(f"{sys.argv[0]}: Enter input file: ")
        except Exception as err:
            print(f"{sys.argv[0]}: ERROR! Could not scan input file name.\n\t({err})\n", file=sys.stderr)
            raise Exception(TError.EBadName)
    else:
        aFilename = sys.argv[1]

    print(f"{sys.argv[0]}: Input file: \"{aFilename}\"\n", file=sys.stdout)

    try:
        aFile = open(aFilename, "rb")
    except Exception as err:
        print(f"{sys.argv[0]}: ERROR! Could not open input file \"{aFilename}\".\n\t({err})\n", file=sys.stderr)
        raise Exception(TError.EBadOpen)

    return aFilename, aFile

    
def open_output_file():
    if len(sys.argv) < 3:
        try:
            aFilename = input(f"{sys.argv[0]}: Enter output file: ")
        except Exception as err:
            print(f"{sys.argv[0]}: ERROR! Could not scan output file name.\n\t({err})\n", file=sys.stderr)
            raise Exception(TError.EBadName)
    else:
        aFilename = sys.argv[2]
    
    print(f"{sys.argv[0]}: Output file: \"{aFilename}\"\n", file=sys.stdout)

    try:
        aFile = open(aFilename, "wb")
    except Exception as err:
        print("{sys.argv[0]}: ERROR! Could not open output file \"{aFilename}\".\n\t({err})\n", file=sys.stderr)
        raise Exception(TError.EBadOpen)

    return aFilename, aFile
  
        
def share_secret(aVal):
    res = aVal * START_POINT * 2
    return res


def is_prime(aNum):
    if aNum == 0 or aNum == 1:
        return False
    if aNum <= 3:
        return True
    if (aNum % 2) == 0 or (aNum % 3) == 0:
        return False
    
    i = 5
    while i*i <= aNum:
        if (aNum % i) == 0 or (aNum % (i+2)) == 0:
            return False
        i += 6

    return True


def generate_random_primes():
    random.seed(datetime.now())

    aP = aQ = 0
    while (aP*aQ > LIMIT) or (aP*aQ < LOW_LIMIT):
        aP = random.randint(1, (LIMIT + 1) / 2)
        while not is_prime(aP):
            aP = random.randint(1, (LIMIT + 1) / 2)
        
        aQ = random.randint(1, (LIMIT + 1) / 2)
        while not is_prime(aQ):
            aQ = random.randint(1, (LIMIT + 1) / 2)

    return aP, aQ


def calculate_key_pair():    
    Pprime, Qprime = generate_random_primes()

    totientphi = (Pprime - 1) * (Qprime - 1)

    factorBout = 0
    aPubE = START_POINT
    while (aPubE < totientphi) and (factorBout != 1):
        modres = 1
        aPubE += 1
        factorA = aPubE
        factorB = totientphi
        factorBout = 0
        while modres != 0:
            modres = factorA % factorB
            factorBout = factorB

            factorA = factorB
            factorB = modres
        
    kinteger = START_POINT * START_POINT
    zerocondition = (1 + kinteger*totientphi) % aPubE

    while zerocondition != 0:
        kinteger += 1
        zerocondition = (1 + kinteger*totientphi) % aPubE
    
    aPrivE = (kinteger*totientphi + 1) // aPubE

    return (Pprime * Qprime), aPubE, aPrivE


def encrypt_character(aChar, aExp, aBound):
    if aBound == 1:
        return 0
   
    result, ch = 1, aChar
    ch %= aBound
    while aExp > 0:
        if aExp%2 == 1:
            result = (result * ch) % aBound
        
        aExp >>= 1
        ch = (ch * ch) % aBound
    
    return result


def encrypt_file(aInfile, aOutfile):
    maxbound, pubexponent, _ = calculate_key_pair()

# Debug code
    if DEBUG_MODE:
        count = 0
# Debug code

    for line in aInfile:  
        for currentchar in line:
            aOutfile.write(bytes(chr(encrypt_character(currentchar, pubexponent, maxbound)),'latin-1'))

# Debug code
            if DEBUG_MODE:
                ecurrentchar = encrypt_character(currentchar, pubexponent, maxbound)
                print("[{0}({1}) > {2}({3})]{4:>12}".format(currentchar, is_in_letter_range(currentchar), ecurrentchar, is_in_letter_range(ecurrentchar), ""), end='', file=sys.stdout)
                count += 1
                if count % DEBUG_NUMBER_OF_COLUMNS == 0:
                    print("",file=sys.stdout)

    if DEBUG_MODE:
        print(f"\nNumber of characters: {count}", file=sys.stdout)
# Debug code

    return share_secret(maxbound)


def main():
    if len(sys.argv) > 3:
        print(f"{sys.argv[0]}: Too many input arguments.\n", file=sys.stderr)
        close_program()
        return -1

    fin = fout = None
    try:
        input, fin = open_input_file()
        output, fout = open_output_file()
    except Exception as err: 
        print(f"{sys.argv[0]}: Error while attempting to open files ({err}).\n", file=sys.stderr)
        close_program(fin, fout)
        return -1

    try:
        print(f"\n{sys.argv[0]}: Secret! {{{format(encrypt_file(fin, fout), 'd')}}}\n", file=sys.stdout)
    except Exception as err:
        print(f"{sys.argv[0]}: Error while attempting to scan file \"{input}\" ({err}).\n", file=sys.stderr)
        close_program(fin, fout)
        return -2

#    print("\n" + sys.argv[0] + ": File \"" + input + "\" encrypted (\"" + output + "\").\n", file=sys.stdout)
#    print("\n{0}: File \"{1}\" encrypted (\"{2}\").\n".format(sys.argv[0], input, output), file=sys.stdout)
    print(f"\n{sys.argv[0]}: File \"{input}\" encrypted (\"{output}\").\n", file=sys.stdout)

    close_program(fin, fout)


if __name__ == "__main__":
    main()