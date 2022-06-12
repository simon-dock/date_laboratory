#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double Sigmoid(double a){
    double b;

    b = 1 / (1 + exp(-a));
    return b;
}

double Uniform(void){

    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
}

double rand_normal(double mu, double sigma){
    double z = sqrt(-2.0 * log(Uniform())) * sin(2.0 * M_PI * Uniform());
    return mu + sigma * z;
}

int main(){

    srand((unsigned)time(NULL));

    int i,j,k;
    int count = 0;
    int index = 0;
    double E_ave = 0.0;
    double E_memo[4] = {1,1,1,1};

    double mu = 0.7;
    double sum = 0.0;

    int xt[4][3] = {{1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    int yt[4] = {0, 1, 1, 0};

    double rs[2] = {0, 0};
    double r = 0.0;
    double u[3] = {1, 0, 0};
    double z = 0.0;
    double E = 0.0;

    double S[2][3] = {{0, 0, 0},{0, 0, 0}};
    double dS[2][3] = {{0, 0, 0},{0, 0, 0}};
    double W[3] = {0, 0, 0};
    double dW[3] = {0, 0, 0};


    for (j=0;j<2;j++){
        for (k=0;k<3;k++){
            S[j][k] = rand_normal(0, 1);
        }
    }

    for (i=0;i<2;i++){
        W[i] = rand_normal(0, 1);
    }
    

    while(1){

        index = count % 4;

        sum = 0.0;
        for (j=0;j<2;j++){
            for (k=0;k<3;k++){
                sum += xt[index][k]*S[j][k];
            }
            u[j+1] = Sigmoid(sum);
            sum = 0.0;
        }

        for (j=0;j<3;j++){
            sum += u[j] * W[j];
        }

        z = Sigmoid(sum);
        E = pow((z-yt[index]), 2) / 2;

        r = (yt[index] - z) * z * (1 - z);

        for (j=0; j<3;j++){
            dW[j] = mu * r * u[j];
        }

        for (j=0; j < 2; j++){
            rs[j] = r * W[j+1] * u[j+1] * (1 - u[j+1]);
        }

        for (j=0; j<2; j++){
            for (k=0; k<3; k++){
                dS[j][k] = mu * rs[j] * xt[index][k];
            }
        }


        for (j=0;j<2; j++){
            for (k=0; k<3; k++){
                S[j][k] = S[j][k] + dS[j][k];
            }
        }
        for (j=0; j<3; j++){
            W[j] = W[j] + dW[j];
        }

        E_memo[index] = E;
        for (i=0;i<4;i++){
            E_ave += E_memo[i];
        }
        E_ave = E_ave/4;

        if(count == 10000000){
            printf("count over\n");
            break;
        }
        if(E_ave < 0.0001){
            printf("E is small enough.\n");
            break;
        }
        E_ave = 0.0;
        count++;
    }

    

    printf("\n");
    printf("result\n");
    for (i = 0; i < 4; i++){

        sum = 0.0;
        for (j=0;j<2;j++){
            for (k=0;k<3;k++){
                sum += xt[i][k]*S[j][k];
            }
            u[j+1] = Sigmoid(sum);
            sum = 0.0;
        }

        for (j=0;j<3;j++){
            sum += u[j] * W[j];
        }

        z = Sigmoid(sum);

        printf("z = %f\n", z);

        E = pow((z - yt[i]), 2) / 2;

        printf("Error %f\n", E);
    }

    return 0;
}