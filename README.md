---- usage --------

1) mpicc -std=c99 CannonAlgo.c -lm -o out
2) sbatch "script-name"

----- results (bench.txt)-----

10 times average:

|matrix \ num of p  |    1      |    4     |    9     |   16     |   25     |   36    |
| ----------------- | --------- | -------  | -------  | -------  | -------  | ------- |
| 60x60             | 0.00118   | 0.00062  | 0.00041  | 0.00068  | 0.05928  | 0.08503 |
| 120x120           | 0.00827   | 0.00388  | 0.00239  | 0.00158  | 0.04132  | 0.04375 |
| 300x300           | 0.13288   | 0.06185  | 0.02603  | 0.01350  | 0.06008  | 0.09080 |
| 540x540           | 0.85836   | 0.39130  | 0.16339  | 0.09889  | 0.17405  | 0.12207 |
| 840x840           | 3.30125   | 1.56562  | 0.63811  | 0.40219  | 0.45967  | 0.38120 |
| 2520x252          | 106.61254 | 68.89383 | 24.23688 | 19.47284 | 11.05175 | 9.13515 |



----- conclusion -----

Za pokretanje koda sa 25 i 36 procesa moze se primetiti da samo u slucaju najvece matrice 2520 x 2520 daju bolje vreme (kolko toliko) . 
U ostalim slucajevima komunikacija medju procesima cini mnogo veci overhead u odnosu na kolicinu posla koji svaki proces dobija, te su
vremena cak i losija u odnosu na ona pokretanja sa manjim brojem procesa. U slucaju matrice 840 x 840 uocava se granica da vise od 16 
procesa ne doprinosi nikakvom povecanju performansi.

----------------------
