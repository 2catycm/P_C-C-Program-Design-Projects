#./matrix_multiplication.exe ../data/mat-A-32.txt ../data/mat-B-32.txt ../data/mat-C-32.txt -type float
#
#
#  Measure-Command {start-process -FilePath "./matmul.exe" -ArgumentList "./data/mat-A-256.txt","./data/mat-B-256.txt","./data/mat-C-256.txt","-type","float" -Wait}
#  Measure-Command {start-process -FilePath "./matmul.exe" -ArgumentList "./data/mat-A-256.txt","./data/mat-B-256.txt","./data/mat-C-256.txt","-type","float" -Wait}
#
#  Measure-Command {./matmul.exe ./data/mat-A-256.txt ./data/mat-B-256.txt ./data/mat-C-256.txt -type double}#89.269186s
#
#  Measure-Command {./matmul.exe ./data/mat-A-2048.txt ./data/mat-B-2048.txt ./data/mat-C-2048.txt -type float}#90.9192037s

  #改变顺序 42.0819839s
  #改变顺序+omp初步 17.387943s

    #不改变顺序+omp

#O2优化 14.74s
#O3优化 12.86s
cd D:/EnglishStandardPath/Practice_File/P_C_Cpp/P_Project2/
./matmul.exe ./data/mat-A-32.txt ./data/mat-B-32.txt ./data/mat-C-32.txt -type double
./matmul.exe ./data/mat-A-64.txt ./data/mat-B-64.txt ./data/mat-C-64.txt -type double
./matmul.exe ./data/mat-A-128.txt ./data/mat-B-128.txt ./data/mat-C-128.txt -type double
./matmul.exe ./data/mat-A-256.txt ./data/mat-B-256.txt ./data/mat-C-256.txt -type double
./matmul.exe ./data/mat-A-512.txt ./data/mat-B-512.txt ./data/mat-C-512.txt -type double
./matmul.exe ./data/mat-A-1024.txt ./data/mat-B-1024.txt ./data/mat-C-1024.txt -type double
./matmul.exe ./data/mat-A-2048.txt ./data/mat-B-2048.txt ./data/mat-C-2048.txt -type double

./matmul.exe ./data/mat-A-32.txt ./data/mat-B-32.txt ./data/mat-C-32_float.txt -type float
./matmul.exe ./data/mat-A-64.txt ./data/mat-B-64.txt ./data/mat-C-64_float.txt -type float
./matmul.exe ./data/mat-A-128.txt ./data/mat-B-128.txt ./data/mat-C-128_float.txt -type float
./matmul.exe ./data/mat-A-256.txt ./data/mat-B-256.txt ./data/mat-C-256_float.txt -type float
./matmul.exe ./data/mat-A-512.txt ./data/mat-B-512.txt ./data/mat-C-512_float.txt -type float
./matmul.exe ./data/mat-A-1024.txt ./data/mat-B-1024.txt ./data/mat-C-1024_float.txt -type float
./matmul.exe ./data/mat-A-2048.txt ./data/mat-B-2048.txt ./data/mat-C-2048_float.txt -type float