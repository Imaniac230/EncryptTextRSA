### `Measure-Program.ps1`

Simple powershell script for measuring time performance of the programs.

## Usage

* All related files must be in the same root directory.
* Encryption program must always be the first parameter.
* The used text file must always be the last parameter (or it is hard-coded in script).

## Results

	C++                             Go

	Encryption time:                Encryption time:
	TotalMilliseconds : 335,015     TotalMilliseconds : 8542,9206

	Decryption time:                Decryption time:
	TotalMilliseconds : 547,648     TotalMilliseconds : 8675,1545

	Used File Statistics:           Used File Statistics:
	very_large_file.txt             very_large_file.txt
	Lines      : 19008              Lines      : 19008
	Words      : 167266             Words      : 167266
	Characters : 1486706            Characters : 1486706
	
<sup>*</sup> This is not really comparing the performance of languages themselves but rather the implementations. <br />