---- usage --------

1) mpicc -std=c99 CannonAlgo.c -lm -o out
2) sbatch "script-name"

----- results (bench.txt)-----

10 times average:

p - 1, matrix    60x   60, time - 0.00118. 
p - 4, matrix    60x   60, time - 0.00062. 
p - 9, matrix    60x   60, time - 0.00041. 
p - 16, matrix    60x   60, time - 0.00068. 
p - 25, matrix    60x   60, time - 0.05928. 
p - 36, matrix    60x   60, time - 0.08503. 

p - 1, matrix   120x  120, time - 0.00827. 
p - 4, matrix   120x  120, time - 0.00388. 
p - 9, matrix   120x  120, time - 0.00239. 
p - 16, matrix   120x  120, time - 0.00158. 
p - 25, matrix   120x  120, time - 0.04132.
p - 36, matrix   120x  120, time - 0.04375. 

p - 1, matrix   300x  300, time - 0.13288.
p - 4, matrix   300x  300, time - 0.06185. 
p - 9, matrix   300x  300, time - 0.02603.
p - 16, matrix   300x  300, time - 0.01350. 
p - 25, matrix   300x  300, time - 0.06008. 
p - 36, matrix   300x  300, time - 0.09080. 

p - 1, matrix   540x  540, time - 0.85836. 
p - 4, matrix   540x  540, time - 0.39130. 
p - 9, matrix   540x  540, time - 0.16339. 
p - 16, matrix   540x  540, time - 0.09889. 
p - 25, matrix   540x  540, time - 0.17405. 
p - 36, matrix   540x  540, time - 0.12207. 

p - 1, matrix   840x  840, time - 3.30125. 
p - 4, matrix   840x  840, time - 1.56562. 
p - 9, matrix   840x  840, time - 0.63811. 
p - 16, matrix   840x  840, time - 0.40219. 
p - 25, matrix   840x  840, time - 0.45967. 
p - 36, matrix   840x  840, time - 0.38120.

p - 1, matrix  2520x 2520, time - 106.61254. 
p - 4, matrix  2520x 2520, time - 68.89383. 
p - 9, matrix  2520x 2520, time - 24.23688. 
p - 16, matrix  2520x 2520, time - 19.47284.
p - 25, matrix  2520x 2520, time - 11.05175.
p - 36, matrix  2520x 2520, time - 9.13515. 

----- conclusion -----

Za pokretanje koda sa 25 i 36 procesa moze se primetiti da samo u slucaju najvece matrice 2520 x 2520 daju bolje vreme (kolko toliko) . 
U ostalim slucajevima komunikacija medju procesima cini mnogo veci overhead u odnosu na kolicinu posla koji svaki proces dobija, te su
vremena cak i losija u odnosu na ona pokretanja sa manjim brojem procesa. U slucaju matrice 840 x 840 uocava se granica da vise od 16 
procesa ne doprinosi nikakvom povecanju performansi.

----------------------
