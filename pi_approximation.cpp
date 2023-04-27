// =================================================================
// File: pi_approximation.cpp
// Author: María Fernanda Moreno Gómez A01708653
//         Uri Jared Gopar Morales  A01709413
// Description: Este archivo contiene el código para obtener
//              Una aproximación de PI usando hilos
//              To compile: g++ -std=c++11 pi_approximation.cpp -o app   y después  ./app
// =================================================================
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <random>

using namespace std;

// Número de hilos a utilizar
const int NUM_THREADS= 4;
// Número de puntos random a generar
const double NO_POINTS= 1000000000; // 1e9
//Contador para contar los puntos que estén en el círculo
double pInCircle= 0;
//Mutex para proteger el acceso a pInCircle
pthread_mutex_t points_mutex;

//Función que genera puntos random en el rango del círculo
void* randomPoints(void* param) {
    double NO_POINTS= *(double*)param;
    double counter= 0;

    unsigned int seed= time(NULL);
    for (long long i= 0; i< NO_POINTS; i++) {
        //Define las coordenadas "x" y "y" para cada punto
        double x= (double)rand()/RAND_MAX*2-1;
        double y= (double)rand()/RAND_MAX*2-1;
        //Fórmula de la distancia de un punto al origen: Si es menor a 1, quiere decir que está dentro del círculo
        if ((x*x + y*y) <= 1.0) {
            counter++;
        }
    }
    pthread_mutex_lock(&points_mutex);
    pInCircle += counter;
    pthread_mutex_unlock(&points_mutex);
    pthread_exit(0); //Fin del hilo
}

int main(int arg, char *argv[]) {
    cout <<"Using "<<NUM_THREADS<<" threads"<< endl;
    //Divide el trabajo: El número de puntos entre los hilos, para ver cuántos puntos tiene que hacer un hilo
    double pointsThread= NO_POINTS/NUM_THREADS;
    //Arreglo para almacenar los objetos del hilo
    pthread_t threads[NUM_THREADS];
    //Inicializa mutex
    pthread_mutex_init(&points_mutex, NULL);
    //Ciclo que crea hilos, ejecutando randomPoints para ponerle una coordenada de punto a cada uno
    for (int i=0; i<NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, randomPoints, &pointsThread);
    }
    //Une los hilos para que no comience uno sin haber terminado otro
    for (int i=0; i<NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    //Destruye mutex
    pthread_mutex_destroy(&points_mutex);
    //Calcula pi de acuerdo a la fórmula de MonteCarlo
    double pi_estimate = 4.0 * static_cast<double>(pInCircle)/static_cast<double>(NO_POINTS);
    //Imprime pi
    cout << "La estimacion de Pi usando MonteCarlo: " << pi_estimate << endl;
    return 0;
}