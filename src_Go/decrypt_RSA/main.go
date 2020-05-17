package main

import (
	"fmt"
	"io"
	"math/rand"
	"os"
	"strconv"
	"time"

	"github.com/pkg/errors"
)

/* displays all printed chars as DEC values on console */
const (
	DebugMode            = false
	DebugNumberOfColumns = 4
)

/* simplifying constants */
const (
	LIMIT       = calcint(255)
	LowLimit    = calcint(227)
	StartPoint  = calcint(2)
	PERMISSIONS = os.FileMode(0666)
)

/* white characters definitions */
const (
	TAB     = 9
	NewLine = '\n'
	SPACE   = ' '
)

type calcint uint64
type calcchar byte

type tError int

const (
	eNullptr tError = 0
	eBadName tError = 1
	eBadOpen tError = 2
	eBadScan tError = 3
)

func handleErrors(err interface{}) error {
	if err == eNullptr {
		return errors.New("NullPointerException")
	}
	if err == eBadName {
		return errors.New("BadFileNameException")
	}
	if err == eBadOpen {
		return errors.New("BadFileOpenException")
	}
	if err == eBadScan {
		return errors.New("BadFileScanException")
	}

	return errors.Errorf(fmt.Sprint(err))
}

func isInLetterRange(character calcchar) string {
	var res string
	if (character >= calcchar(32)) && (character <= calcchar(126)) {
		res = string(character)
	} else {
		res = "Ee"
	}
	return res
}

func closeFile(aInFile, aOutFile *os.File) error {
	var Einf, Eoutf error = nil, nil
	if aInFile != nil {
		Einf = aInFile.Close()
	}
	if aOutFile != nil {
		Eoutf = aOutFile.Close()
	}

	if Eoutf != nil {
		fmt.Fprintf(os.Stderr, "\nWARNING! Attempting to close a non-existing output file. (%s)", Eoutf)
	}
	if Einf != nil {
		fmt.Fprintf(os.Stderr, "\nWARNING! Attempting to close a non-existing input file. (%s)\n\n", Einf)
	}

	if Eoutf != nil {
		return Eoutf
	}
	if Einf != nil {
		return Einf
	}
	return nil
}

func closeProgram(aFin, aFout *os.File) {
	var (
		end []byte = make([]byte, 1)
		err error  = nil
	)

	if err = closeFile(aFin, aFout); err != nil {
		fmt.Fprintf(os.Stderr, "\n%s: WARNING! Error closing files. (%s)\n\n", os.Args[0], err)
	}

	if e := recover(); e != nil {
		fmt.Fprintf(os.Stderr, "%s: Recovered from panic! \n{\n%+v\n}\n\n", os.Args[0], handleErrors(e))
	}

	fmt.Fprintf(os.Stdout, "\n%s: Press \"enter\" to exit.", os.Args[0])

	if _, err = os.Stdin.Read(end); err != nil {
		fmt.Fprintf(os.Stderr, "\n%s: WARNING! Error exiting program. (%s)\n\n", os.Args[0], err)
	}
}

func openInputFile(aFilename *string, aFile **os.File) {
	var err error = nil
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stdout, "%s: Enter input file: ", os.Args[0])
		if _, err = fmt.Fscanf(os.Stdin, "%s", aFilename); err != nil {
			fmt.Fprintf(os.Stderr, "%s: ERROR! Could not scan input file name. (%s)\n\n", os.Args[0], err)
			panic(eBadName)
		}
		fmt.Scanln()
	} else {
		*aFilename = os.Args[1]
	}
	fmt.Fprintf(os.Stdout, "%s: Input file: \"%s\"\n\n", os.Args[0], *aFilename)

	*aFile, err = os.OpenFile(*aFilename, os.O_RDONLY, PERMISSIONS)
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s: ERROR! Could not open input file \"%s\". (%s)\n\n", os.Args[0], *aFilename, err)
		panic(eBadOpen)
	}
}

func openOutputFile(aFilename *string, aFile **os.File) {
	var err error = nil
	if len(os.Args) < 3 {
		fmt.Fprintf(os.Stdout, "%s: Enter output file: ", os.Args[0])
		if _, err = fmt.Fscanf(os.Stdin, "%s", aFilename); err != nil {
			fmt.Fprintf(os.Stderr, "%s: ERROR! Could not scan output file name. (%s)\n\n", os.Args[0], err)
			panic(eBadName)
		}
		fmt.Scanln()
	} else {
		*aFilename = os.Args[2]
	}
	fmt.Fprintf(os.Stdout, "%s: Output file: \"%s\"\n\n", os.Args[0], *aFilename)

	*aFile, err = os.OpenFile(*aFilename, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, PERMISSIONS)
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s: ERROR! Could not open output file \"%s\". (%s)\n\n", os.Args[0], *aFilename, err)
		panic(eBadOpen)
	}
}

func getSecret() calcint {
	var (
		Sec calcint = 0
		err error   = nil
	)

	if len(os.Args) <= 3 {
		fmt.Fprintf(os.Stdout, "%s: Secret? ", os.Args[0])
		if _, err = fmt.Fscanf(os.Stdin, "%d", &Sec); err != nil {
			fmt.Fprintf(os.Stderr, "%s: ERROR! Could not read secret. (%s)\n\n", os.Args[0], err)
			panic(eBadName)
		}
		fmt.Scanln()
	} else {
		Sec = parseCalcchar(os.Args[3])
	}

	Sec /= StartPoint * calcint(2)
	return Sec
}

func parseCalcchar(aVal string) calcint {
	res, err := strconv.ParseUint(aVal, 10, 64)
	if err != nil {
		panic(err)
	}
	return calcint(res)
}

func isPrime(aNum calcint) bool {
	if aNum == 0 || aNum == 1 {
		return false
	}
	if aNum <= 3 {
		return true
	}

	if aNum%2 == 0 || aNum%3 == 0 {
		return false
	}
	for i := calcint(5); i*i <= aNum; i = i + 6 {
		if aNum%i == 0 || aNum%(i+2) == 0 {
			return false
		}
	}

	return true
}

func generateRandomPrimes(aP, aQ *calcint) {
	if aP == nil || aQ == nil {
		panic(eNullptr)
	}

	rand.Seed(time.Now().UTC().UnixNano())

	for (*aP**aQ > LIMIT) || (*aP**aQ < LowLimit) {
		*aP = calcint(rand.Uint64()) % (LIMIT + 1) / 2
		for !isPrime(*aP) {
			*aP = calcint(rand.Uint64()) % (LIMIT + 1) / 2
		}

		*aQ = calcint(rand.Uint64()) % (LIMIT + 1) / 2
		for !isPrime(*aQ) {
			*aQ = calcint(rand.Uint64()) % (LIMIT + 1) / 2
		}

	}
}

func generatePrimes(aP, aQ *calcint) {
	if aP == nil || aQ == nil {
		panic(eNullptr)
	}

	Sec := getSecret()

	end := false
	for *aP = 2; *aP < (LIMIT+1)/2; *aP++ {
		for !isPrime(*aP) {
			*aP++
		}
		for *aQ = 2; *aQ < (LIMIT+1)/2; *aQ++ {
			for !isPrime(*aQ) {
				*aQ++
			}
			if *aP**aQ == Sec {
				end = true
				break
			}
		}
		if end {
			break
		}
	}

	if !end {
		generateRandomPrimes(aP, aQ)
	}
}

func calculateKeyPair(aPubE, aPrivE *calcint) calcint {
	if aPubE == nil {
		panic(eNullptr)
	}

	var Pprime, Qprime calcint = 0, 0
	generatePrimes(&Pprime, &Qprime)

	var totientphi calcint = (Pprime - 1) * (Qprime - 1)

	var modres, factorA, factorB, factorBout calcint = 0, 0, 0, 0
	*aPubE = StartPoint
	for (*aPubE < totientphi) && (factorBout != 1) {
		modres = 1
		*aPubE++
		factorA = *aPubE
		factorB = totientphi
		factorBout = 0
		for modres != 0 {
			modres = factorA % factorB
			factorBout = factorB

			factorA = factorB
			factorB = modres
		}
	}

	if aPrivE != nil {
		var (
			kinteger      calcint = StartPoint * StartPoint
			zerocondition calcint = (1 + kinteger*totientphi) % *aPubE
		)
		for zerocondition != 0 {
			kinteger++
			zerocondition = (1 + kinteger*totientphi) % *aPubE
		}
		*aPrivE = (kinteger*totientphi + 1) / *aPubE
	}

	return Pprime * Qprime
}

func encryptCharacter(aChar calcchar, aExp, aBound calcint) calcchar {
	if aBound == 1 {
		return calcchar(0)
	}

	var result, ch calcint = 1, calcint(aChar)
	ch %= aBound
	for aExp > 0 {
		if aExp%2 == 1 {
			result = (result * ch) % aBound
		}
		aExp >>= 1
		ch = (ch * ch) % aBound
	}
	return calcchar(result)
}

func decryptFile(aInfile, aOutfile *os.File) {
	var (
		currentchar               calcchar = 0
		pubexponent, privexponent calcint  = 0, 0
		maxbound                  calcint  = calculateKeyPair(&pubexponent, &privexponent)
	)

	/* Debug variables */
	var (
		count        uint     = 0
		ecurrentchar calcchar = 0
	)
	/* Debug variables */

	var (
		err       error  = nil
		readchar  []byte = make([]byte, 1)
		writechar []byte = make([]byte, 1)
	)
	for err != io.EOF {
		if _, err = aInfile.Read(readchar); err != nil && err != io.EOF {
			closeFile(aInfile, aOutfile)
			panic(eBadScan)
		}
		currentchar = calcchar(readchar[0])

		if err == nil {
			writechar[0] = byte(encryptCharacter(currentchar, privexponent, maxbound))
			aOutfile.Write(writechar)

			/* Debug code */
			if DebugMode {
				ecurrentchar = encryptCharacter(currentchar, privexponent, maxbound)
				fmt.Fprintf(os.Stdout, "[%d(%s) > %d(%s)]%-12s", currentchar, isInLetterRange(currentchar), ecurrentchar, isInLetterRange(ecurrentchar), "")
				count++
				if count%DebugNumberOfColumns == 0 {
					fmt.Fprintf(os.Stdout, "\n")
				}
			}
			/* Debug code */

		}
	}

	/* Debug code */
	if DebugMode {
		fmt.Fprintf(os.Stdout, "\nNumber of characters: %d", count)
		fmt.Fprintf(os.Stdout, "\n")
	}
	/* Debug code */
}

func main() {
	if len(os.Args) > 4 {
		fmt.Fprintf(os.Stderr, "%s: Too many input arguments.\n\n", os.Args[0])
		closeProgram(nil, nil)
		return
	}

	var (
		input, output string
		fin, fout     *os.File = nil, nil
	)
	defer closeProgram(fin, fout)

	openInputFile(&input, &fin)
	openOutputFile(&output, &fout)

	decryptFile(fin, fout)

	fmt.Fprintf(os.Stdout, "\n%s: File \"%s\" decrypted (\"%s\").\n\n", os.Args[0], input, output)
}
