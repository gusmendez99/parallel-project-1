# Heat Dissipation Equation (Alternative version)

Project 1 - Parallel Programming
CC3069 class

## Timing & Speedup

|    	| i5-10400F (12 threads) 	| M1 Pro (8 threads) 	|
|----	|------------------------	|--------------------	|
| ts 	| 24.518s                	| 25.114s             	|
| tp 	| 1.997s                 	| 4.216s             	|
| S  	| 11.931                   	| 5.96               	|


## Test and run 💻

First build executables

```console
make
```

Run parallel program

```console
make run
```

Run sequential program

Compile it

```console
gcc -o seq sequential.c
```

Run it

```console
./seq
```

If no arguments the programs will use default ones. If you want to test, pass the follow arguments:

```console
{./seq | ./parallel} N T0 Tl Tr
```

- N : Division on the rod
- T0 : Intial temp in the rod
- Tl : Temperature in left side of the rod
- Tr : Temperature in right side of the rod

## Build with 🔧

- C
- OpenMP


## Authors

* **Gus Mendez** ([gusmendez99](https://github.com/gusmendez99))
* **Roberto Figueroa** ([RobertoFigueroa](https://github.com/RobertoFigueroa))
* **Michele Benvenuto** ([michelebenveuto](https://github.com/michelebenvenuto))






