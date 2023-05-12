#include "list.h"
#include "stack.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 31

typedef struct
{
  char nombre[MAX];
  bool completada;
  Stack *tareasPrecedentes; // Creamos una pila de tareas precedentes.
  Stack *registro;
} Tarea; // Definimos nuestra estructura a trabajar.

// Subfunciones

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

char *gets_csv_field(char *tmp, int k) // Obtenemos la linea k del archivo.
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

void agregarTarea(Heap *TPH, int *registrado)
{
  Tarea *tareaAux = malloc(sizeof(Tarea));
  char tarea[MAX];
  int prioridadT;
  
  tareaAux -> tareasPrecedentes = createStack();
  tareaAux -> completada = false;
  
  preguntarTarea(tarea);

  printf("\nIngrese prioridad:\n");
  scanf("%d", &prioridadT);
  
  strcpy(tareaAux -> nombre, tarea);
  
  (*registrado)++;

  heap_push(TPH, tareaAux, prioridadT);

  printf("\nTarea agregada!\n\n");
  
}

// 2.

void establecerPrecedencia(Heap *TPH)
{
  Heap *aux = heap_clone(TPH);
  Heap *auxDos = heap_clone(TPH);
  
  char *tarea1 = (char*) malloc(sizeof(char));
  bool banderitaTarea1 = false;
  char *tarea2 = (char*) malloc(sizeof(char));
  bool banderitaTarea2 = false;
  
  printf("Ingrese tarea 1:\n");
  getchar();
  scanf("%30s", tarea1);
  
  printf("Ingrese tarea 2:\n");
  getchar();
  scanf("%30s", tarea2);

  while (heap_top(aux) != NULL) 
  {
    Tarea *tareaActual = (Tarea *) heap_top(aux);
    Tarea *tareaActualAux = (Tarea *) heap_top(aux);

    if(strcmp(tareaActual->nombre,tarea1) == 0) 
    {
      banderitaTarea1 = true;

      while (heap_top(auxDos) != NULL) 
      {
        if(strcmp(tareaActualAux->nombre,tarea2) == 0) banderitaTarea2 = true;
        heap_pop(auxDos);
      }

      if(banderitaTarea2 == true) 
      {
        printf("Ahora %s es precedente de %s", tarea2, tarea1);
        stack_push(tareaActual -> tareasPrecedentes, tarea2);
      }
    }
    heap_pop(aux);
  }
}

// 3.

void mostrarTareas(Heap *TPH)
{
  Heap *aux = heap_clone(TPH);
  
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n\n");

  while (heap_top(aux) != NULL) 
  {
    Tarea *tareaActual = (Tarea *) heap_top(aux);
    Stack *pila = tareaActual->tareasPrecedentes;
    int cont = 0;
    
    printf("%i. %s (Prioridad: %i)\n", cont + 1, tareaActual -> nombre, heap_top_priority(aux));
    
    if(tareaActual -> tareasPrecedentes != NULL)
    {
      while(pila != NULL)
      {
        char *precedente = stack_pop(tareaActual -> tareasPrecedentes);
        if (cont == 0) printf("- Precedente(s): %s", precedente);
        else printf(", %s", precedente);
      }
      printf("\n");
    }
    
    cont++;
    heap_pop(aux);
  }
  
  free(aux);
  
  printf("\n");
}

// 4.

void marcarCompletada(Heap *TPH)
{
  printf("ola");
  return;
}

// 5.

void deshacerAccion(Heap *TPH)
{
  return;
}

// 6.

void cargarTareas(Heap *TPH, int *registrado)
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

  Heap *tareasPorHacer = createHeap();
  
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
          if(registrado < 2)
          {
            printf("Deben registrarse al menos 2 tareas.\n");
            break;
          }
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