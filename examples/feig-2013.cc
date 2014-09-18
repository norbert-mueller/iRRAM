#include <iostream>
#include <iRRAM/core.h>


using namespace std;
using namespace iRRAM;

int  initialize(int argc, char* argv[]);
void iterate(REAL nu[], const int N, const int iterations);


int main(int argc, char* argv[])
{
  iRRAM_initialize(argc, argv);
    
  return iRRAM_exec(initialize, argc, argv);
}



int initialize(int argc, char* argv[])
{
  const int iterations = atoi(argv[1]);
  const int N          = 10;
  const int M          = N + 2 * iterations;
  
  REAL g0[N + 1], nu[M + 1];
  REAL bin[N + 1][N + 1];
  
  g0[1]  = REAL("-1.527632997036301454035890310240");
  g0[2]  = REAL("0.104815194787303733216742613801");
  g0[3]  = REAL("0.026705670525193354032652094944");
  g0[4]  = REAL("-0.003527409660908709170234190769");
  g0[5]  = REAL("0.000081600966547531745172190486");
  g0[6]  = REAL("0.000025285084233963536176262552");
  g0[7]  = REAL("-2.5563171662784938463532541E-6");
  g0[8]  = REAL("-9.6512715508912032163725768E-8");
  g0[9]  = REAL("2.8193463974504091370756629E-8");
  g0[10] = REAL("-2.77305116079901172437E-10");
  

  bin[0][0] = 1;
  nu[0]     = 1;
  
  for (int k = 1; k <= N; k++) {
    bin[k][0] = 1;
    nu[0]    += g0[k];
  }
  
  for (int j = 1; j <= N; j++) {
    bin[j][j] = 1;
    nu[j]     = g0[j];
    
    for (int k = j + 1; k <= N; k++) {
      bin[k][j] = bin[k - 1][j - 1] + bin[k - 1][j];
      nu[j]    += bin[k][j] * g0[k];
    }
  }
  

  iterate(nu, N, iterations);
  
  return 0;
}


void iterate(REAL nu[], const int N, const int iterations)
{
  const int M = N + 2 * iterations;

  REAL prec  = REAL("1E-1");;
  int  oprec = 1;
  
  REAL rho[M], mu[M];
  iRRAM::cerr << M << " Groesse\n";
  REAL bin[M][M];
  iRRAM::cerr << M << " Groesse\n";
  
  REAL eta[M][M];

  
  iRRAM::cerr << M << " Groesse\n";
  for (int k = 0; k < M; k++) {
    bin[k][0] = 1;
  }
  
  for (int j = 1; j < M; j++) {
    bin[j][j] = 1;
    
    for (int k = j + 1; k < M; k++) {
      bin[k][j] = bin[k - 1][j - 1] + bin[k - 1][j];
    }
  }

  REAL con = REAL("0.91");
  
  // iteration
  
  for (int m = 0; m < iterations; m++) {
    // rho
    
    REAL scal = 1;

    for (int k = 0; k < N + 2 * (m + 1); k++) {
      REAL pow = 1;
      
      rho[k] = 0;

      for (int i = 0; i <= N + 2 * m - k; i++) {
        rho[k] += bin[k + i][k] * pow * nu[k + i];
        pow    *= nu[0] * nu[0] - 1;
      }

      rho[k] *= scal;
      scal   *= nu[0] * nu[0];
    }

    // eta

    for (int k = 0; k < N + 2 * (m + 1); k++) {
      if (k == 0) {
        eta[1][k] = -1;
      }
      else {
        eta[1][k] = 0;
      }

      for (int j = 0; j <= k; j++) {
        eta[1][k] += rho[j] * rho[k - j];
      }
    }

    for (int i = 2; i <= N + 2 * m; i++) {
      for (int k = 0; k < N + 2 * (m + 1); k++) {
        eta[i][k] = 0;

        for (int j = 0; j <= k; j++) {
          eta[i][k] += eta[1][j] * eta[i - 1][k - j];
        }
      }
    }

    // mu

    for (int k = 0; k < N + 2 * (m + 1); k++) {
      if (k == 0) {
        mu[k] = nu[0];
      }
      else {
        mu[k] = 0;
      }
      
      for (int i = 1; i <= N + 2 * m; i++) {
        mu[k] += nu[i] * eta[i][k];
      }

      mu[k] /= nu[0];
    }

    // new values

    nu[0] = mu[0] / REAL("-3.669") + (1 + 1 / REAL("3.669")) * nu[0];
    nu[1] = mu[1] - mu[0] + nu[0];

    for (int i = 2; i < N + 2 * (m + 1); i++) {
      nu[i] = mu[i];
    }

    nu[N + 2 * (m + 1)] = 1;

    int sgn = 1;

    for (int i = 0; i < N + 2 * (m + 1); i++) {
      nu[N + 2 * (m + 1)] -= sgn * nu[i];

      sgn *= -1;
    }

    // error + output

    REAL err = con / 1000;

    if (abs(nu[0]) >= err) {
      err = 1 / (abs(nu[0]) * (abs(nu[0]) / err - 1));

      iRRAM::cout << "m = " << m + 1 << endl;

      if (abs(nu[0]) * err < prec) {
        iRRAM::cout << swrite(1 / nu[0] - err, 8 + oprec) << endl;
        iRRAM::cout << swrite(1 / nu[0] + err, 8 + oprec) << endl;

        prec /= 10;
        oprec++;
      }
      
      iRRAM::cout << swrite(1 / nu[0], 100) << endl;
    }
    else {
      iRRAM::cout << "error too big." << endl;
    }


    con *= REAL("0.91");
  }
}
