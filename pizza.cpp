#include <iostream>
#include <pthread.h>
#include <vector>
#include <ctime>

using namespace std;

// Constantes
const int pizzas = 2;
const int estudiantes = 4;
const int rebanadas = 8;

// Variables globales
int contador_pizzas = 0;
int rebanadas_restantes = 0;
pthread_mutex_t bloqueo;

// Función que representa el comportamiento de un estudiante
void* estudiante(void* arg) {
    string nombre = *((string*) arg);
    
    // Mientras no se alcance el máximo de pizzas
    while (contador_pizzas < pizzas) {
        // Bloquear el acceso a las variables compartidas
        pthread_mutex_lock(&bloqueo);
        
        // Si no hay rebanadas restantes
        if (rebanadas_restantes == 0) {
            cout << nombre << " Llama a la pizzeria " << endl;
            contador_pizzas++;
            rebanadas_restantes = rebanadas;
            cout << "Nueva ORDEN DE Pizza :D. Contiene: " << rebanadas_restantes <<" Rebanadas "<< endl;
        }
        else {
            // Tomar una rebanada y actualizar el contador
            rebanadas_restantes--;
            cout << nombre << " toma una, quedan:  " << rebanadas_restantes << endl;
        }
        
        // Desbloquear el acceso a las variables compartidas
        pthread_mutex_unlock(&bloqueo);
    }
    return NULL;
}

int main() {
    // Crear un arreglo de hilos para los estudiantes
    pthread_t hilos[estudiantes];
    
    // Lista de nombres de estudiantes
    vector<string> nombres_estudiantes = {"Uri", "Mafer", "Jose", "Ramns"};

    // Inicializar la semilla para la generación de números aleatorios
    srand(time(NULL));

    // Inicializar el bloqueo (mutex) para controlar el acceso a las variables compartidas
    pthread_mutex_init(&bloqueo, NULL);

    // Crear los hilos para los estudiantes
    for (int i=0; i<estudiantes; i++) {
        // Seleccionar un nombre aleatorio de la lista
        string nombre_aleatorio = nombres_estudiantes[rand() % nombres_estudiantes.size()];
        
        // Crear el hilo del estudiante con el nombre aleatorio
        pthread_create(&hilos[i], NULL, estudiante, (void*) &nombre_aleatorio);
    }

    // Esperar a que todos los hilos de los estudiantes finalicen
    for (int i=0; i <estudiantes; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destruir el bloqueo (mutex) una vez que todos los hilos hayan finalizado
    pthread_mutex_destroy(&bloqueo);

    return 0;
}