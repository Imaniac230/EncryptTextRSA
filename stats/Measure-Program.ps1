$File = "example_message"


if ($args[2])
	{
	if ($args[2].Split('\') -eq $args[2])
		{ $File = $args[2].Split('.')[0] }
	else
		{
		$File = $args[2].Split('\')[1]
		$File = $File.Split('.')[0]
		}
	}

if ($args[1])
	{
	$Sufix = $args[1].Substring($args[1].Length - 4)
	if ($Sufix -eq ".exe")
		{ $Program2 = $args[1] }
	else
		{
		if ($args[1].Split('\') -eq $args[1])
			{ $File = $args[1].Split('.')[0] }
		else
			{
			$File = $args[1].Split('\')[1]
			$File = $File.Split('.')[0]
			}
		}
	}
	
if ($args[0])
	{
	$Sufix = $args[0].Substring($args[0].Length - 4)
	if ($Sufix -eq ".exe")
		{ $Program1 = $args[0] }
	}

$File_out = $File + "_decrypted" + ".txt"
$File_out2 = $File + "_encrypted"
$File = $File + ".txt"

$FileStats = Get-Content $File | Measure-Object -Character -Line -Word


if ($Program1 -and $Program2)
	{
	$Elapsed1 = Measure-Command {'\n' | & $Program1 $File $File_out2}
	
	$Output = '\n' | & $Program1 $File $File_out2
	$Output = $Output[5]
	foreach ($el in $Output)
		{ $Secret = $Secret + $el }
	$Secret = $Secret.Split('{')[1]
	$Secret = $Secret.Split('}')[0]

	$Elapsed2 = Measure-Command {'\n' | & $Program2 $File_out2 $File_out $Secret}
	
	Write-Output "`nEncryption time:"
	Write-Output $Elapsed1
	
	Write-Output "`nDecryption time:`n`n"
	Write-Output $Elapsed2
	
	Write-Output "`nUsed File Statistics:"
	Write-Output $File
	Write-Output $FileStats
	}
elseif ($Program1)
	{
	$Elapsed1 = Measure-Command {'\n' | & $Program1 $File $File_out2}
	
	Write-Output "`nEncryption time:"
	Write-Output $Elapsed1
	
	Write-Output "`nUsed File Statistics:"
	Write-Output $File
	Write-Output $FileStats
	}
else
	{
	Write-Output "`nNo program given.`n"
	}