cd "D:\EnglishStandardPath\Practice_File\P_C_Cpp\P_Project2\python_help"
$aaa = Get-Content  "../data/mat-C-32_by_python.txt"
$bbb = Get-Content  "../data/mat-C-32.txt"
Compare-Object  $aaa  $bbb



#bash
diff -E -b -B -I --minimal
 #>