#define TIME_FINAL           200000
#define SNAPSHOT             2000
#define L_TENTATIVA          36

#define QUADRADO             0
#define CIRCULO              1
#define CANTO                2
#define EVERYWHERE           3

#define NO                   0
#define YES                  1

#define KILL                 YES
#define SORTEIO              CANTO

#define R_MAX                1.00    //1.0  //0.55
#define R_CORE               0.18    //0.36 //0.2
#define R_EQ                 0.80    //0.73 //0.4

#define V1                   0.05

// 0.316 / R_core => 1.755
// // 0.558 / R_core => 3.1
// // 0.8   / R_core => 4.444
// // #define RESEVOIR_DENSITY 4.23 / (0.785398163*R_EQ*R_EQ)  
// // // densidade de eq = 1.0 / (3.14159*R_EQ/2*R_EQ/2) 
//#define RESEVOIR_DENSITY .8 / R_CORE

#define RESEVOIR_DENSITY     1.00 / (0.785398163*R_EQ*R_EQ)  

#define ALPHA                0.0  / V1
#define BETA                 2.0
#define ETA                  1.0

#define F_0                  1000.0

#define PI                   3.14159265
#define LOAD                 1

#define Lx                   105     //275
#define Ly                   25   //67

#define OBSTACULO            1
#define PAREDE               1

#define L_CANTO              1.0
#define R_ESFERA             3.75  

#define R_FRONT              13.0

#define L_CENTRO_X           50.  //133.333
#define L_CENTRO_Y           12.5    //34.5

#define N                    200000
#define N0                   50000
#define CREATION_PROBABILITY 0.007

#define MAX_VIZ              20

