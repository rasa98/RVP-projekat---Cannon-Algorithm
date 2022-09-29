## usage

1) mpicc -std=c99 CannonAlgo.c -lm -o out
2) sbatch "script-name"

## results (bench.txt)

10 times average:

|matrix \ num of p  |    1      |    4     |    9     |   16     |   25     |   36    |
| ----------------- | --------- | -------  | -------  | -------  | -------  | ------- |
| 60x60             | 0.00118   | 0.00062  | 0.00041  | 0.00068  | 0.05928  | 0.08503 |
| 120x120           | 0.00827   | 0.00388  | 0.00239  | 0.00158  | 0.04132  | 0.04375 |
| 300x300           | 0.13288   | 0.06185  | 0.02603  | 0.01350  | 0.06008  | 0.09080 |
| 540x540           | 0.85836   | 0.39130  | 0.16339  | 0.09889  | 0.17405  | 0.12207 |
| 840x840           | 3.30125   | 1.56562  | 0.63811  | 0.40219  | 0.45967  | 0.38120 |
| 2520x2520         | 106.61254 | 68.89383 | 24.23688 | 19.47284 | 11.05175 | 9.13515 |

## UPDATE

10 times average:

|matrix \ num of p  |    1      |    4     |    9     |   16     |   25     |   36    |
| ----------------- | --------- | -------  | -------  | -------  | -------  | ------- |
| 60x60             | 0.00101   | 0.00036  | 0.00019  | 0.00025  | 0.00035  | 0.00034 |
| 120x120           | 0.00790   | 0.00288  | 0.00133  | 0.00094  | 0.00081  | 0.00087 |
| 300x300           | 0.13032   | 0.04931  | 0.02037  | 0.01201  | 0.00783  | 0.00564 |
| 540x540           | 0.82733   | 0.33294  | 0.13473  | 0.07143  | 0.04532  | 0.03068 |
| 840x840           | 2.75092   | 1.60804  | 0.55369  | 0.30825  | 0.19856  | 0.12308 |
| 2520x2520         | 104.19213 | 53.00128 | 21.60256 | 14.38713 | 7.16330  | 5.03271 |

# conclusion 

Za pokretanje koda sa 25 i 36 procesa moze se primetiti da samo u slucaju najvece matrice 2520 x 2520 daju bolje vreme (kolko toliko) . 
U ostalim slucajevima komunikacija medju procesima cini mnogo veci overhead u odnosu na kolicinu posla koji svaki proces dobija, te su
vremena cak i losija u odnosu na ona pokretanja sa manjim brojem procesa. U slucaju matrice 840 x 840 uocava se granica da vise od 16 
procesa ne doprinosi nikakvom povecanju performansi.


