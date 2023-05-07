#include "list.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 31

// Subfunciones

typedef struct
{
  char nombre[MAX];
  int prioridad;
  List *tareasPrecedentes; // Creamos una lista de tareas precedentes;
  bool completada;

  //Stack *registro; // pila de última función accedida
  //Stack *registroSum; // pila de últimos puntos de habilidad registrados
  //Stack *registroItem; // pila de último ítem registrado

} Tarea; // Definimos nuestra estructura a trabajar.

void validarOpcion(int *opcion) // Valida las opciones del menú.
{
  char opcionAux[MAX];
  
  while (true) 
  {
    scanf("%30s", opcionAux);
    printf("\n");
      
    if (isdigit(*opcionAux)) 
    {
      *opcion = atoi(opcionAux);
        
      if (*opcion >= 1 && *opcion <= 10) break;
      else printf("Debe ingresar un número válido entre 1 y 10.\n");
    }
    else printf("Debe ingresar un número válido entre 1 y 10.\n");
    
    getchar();
  }
}

void menu() // Opciones del menú.
{
  printf("\nEliga una opción a continuación.\n\n");

  printf("1. Agregar tarea.\n");
  printf("2. Establecer precedencia entre dos tareas.\n");
  printf("3. Mostrar tareas por hacer.\n");
  printf("4. Marcar tarea como completada.\n");
  printf("5. Deshacer última acción.\n");
  printf("6. Cargar datos de tareas.\n");
  printf("7. Salir del programa.\n\n");
}

void validar(int *user_continue) // Validamos que el usuario desee seguir con la ejecución del programa.
{
  char salida[2];
  
  printf("Desea realizar otra operación? (s/n)\n");
  scanf("%1s", salida);
  
  if(strcmp(salida, "n") == 0)
  {
    *user_continue = 0;
    printf("\nGracias por usar el programa, adiós!");
  }
  else while (strcmp(salida, "s") != 0)
  {
    printf("Ingrese una opción válida\n\n");
    printf("Desea realizar otra operación? (s/n)\n");
    scanf("%1s", salida);
  }
}

char *gets_csv_field(char *tmp, int k) //
{
  int open_mark = 0;
  char *ret = (char*) malloc(100 * sizeof(char));
  int ini_i = 0, i = 0;
  int j = 0;
  int last_field = 0;
  
  while (tmp[i + 1] != '\0')
  {  
    if(tmp[i] == '\"')
    {
      open_mark = 1 - open_mark;
      if(open_mark) ini_i = i + 1;
      i++;
      continue;
    }
    
    if(open_mark || tmp[i] != ',')
    {
      if(k == j) ret[i - ini_i] = tmp[i];
      i++;
      continue;
    }

    if(tmp[i] == ',')
    {
      if(k == j) 
      {
        ret[i - ini_i] = 0;
        return ret;
      }
      j++; 
      ini_i = i + 1;
    }
    i++;
  }

  if(k == j) 
  {
    last_field = 1;
    ret[i - ini_i] = 0;
    return ret;
  }
  return NULL;
}

void preguntarTarea(char *nombreTarea) // Solicita que el usuario ingrese una tarea.
{
  printf("Ingrese tarea:\n");
  getchar();
  scanf("%[^\n]s", nombreTarea);
}

// Funciones

// 1.

void agregarTarea(List *TPH, int *registrado)
{
  Tarea *tareaAux = malloc(sizeof(Tarea));
  char tarea[MAX];
  int prioridadT;
  tareaAux -> tareasPrecedentes = createList();
  tareaAux -> completada = false;
  
  preguntarTarea(tarea);

  printf("\nIngrese prioridad:\n");
  scanf("%d", &prioridadT);
  
  strcpy(tareaAux -> nombre, tarea);
  tareaAux -> prioridad = prioridadT;

  *registrado = 1;

  pushBack(TPH, tareaAux);

  printf("\nTarea agregada!\n\n");
  
}

// 2.

void establecerPrecedencia(List *TPH)
{
  return;
}

// 3.

void mostrarTareas(List *TPH)
{
  Tarea *tareaAux = firstList(TPH);
  char *auxT = firstList(tareaAux -> tareasPrecedentes);
  
  int cont = 1;
  
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n\n");
  
  while(tareaAux != NULL)
  {
    if(tareaAux -> completada == true) tareaAux = nextList(TPH);
    else
    {
      printf("%i. %s (Prioridad: %i)", cont, tareaAux -> nombre, tareaAux -> prioridad);
      if(auxT != NULL) printf("- Precedente: ######");
      printf("\n");
      tareaAux = nextList(TPH);
    }
    cont++;
  }
  
  printf("\n");
}

// 4.

void marcarCompletada(List *TPH)
{
  printf("ola");
  return;
}

// 5.

void deshacerAccion(List *TPH)
{
  return;
}

// 6.

void cargarTareas(List *TPH, int *registrado)
{
  return;
}

// Programa principal

int main()
{
  /*
                      ____ _             _           _             
                     / ___(_) __ _  __ _| |__  _   _| |_ ___     
                    | |  _| |/ _` |/ _` | '_ \| | | | __/ _ \     
                    | |_| | | (_| | (_| | |_) | |_| | ||  __/     
                     \____|_|\__, |\__,_|_.__/ \__, |\__\___| 
                             |___/             |___/                                                        
                ,--------.                                 ,----.  
                '--.  .--',--,--.,--.--. ,---.  ,--,--.    '.-.  | 
                   |  |  ' ,-.  ||  .--'| .-. :' ,-.  |      .' <  
                   |  |  \ '-'  ||  |   \   --.\ '-'  |    /'-'  | 
                   `--'   `--`--'`--'    `----' `--`--'    `----'                         
  
  Claudio Toledo && José Lara Arce ver. */
  
  int user_continue = 1;

  int registrado = 0;

  List *tareasPorHacer = createList();
  
  printf("Bienvenido! :D\n");

  while(user_continue)
  {
    menu();
    
    int opcion = 0;
    
    validarOpcion(&opcion); // Validamos que opción sea un número.
    
    if(registrado == 0 && opcion != 1 && opcion != 6 && opcion != 7) printf("No hay tareas registradas, registre una primero.\n");
    else
    {
      switch(opcion)
      {
        case 1 :
          agregarTarea(tareasPorHacer, &registrado);
          validar(&user_continue);
          break;

        case 2 :
          establecerPrecedencia(tareasPorHacer);
          validar(&user_continue);
          break;

        case 3 :
          mostrarTareas(tareasPorHacer);
          validar(&user_continue);
          break;

        case 4 :
          marcarCompletada(tareasPorHacer);
          validar(&user_continue);
          break;

        case 5 :
          deshacerAccion(tareasPorHacer);
          validar(&user_continue);
          break;

        case 6 :
          cargarTareas(tareasPorHacer, &registrado);
          validar(&user_continue);
          break;
        
        case 7 :
          printf("Gracias por usar el programa, adiós!");
          
          return 0;
      }
    }
  }
}