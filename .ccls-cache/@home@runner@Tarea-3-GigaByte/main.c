#include "list.h"
#include "stack.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 31

typedef struct
{
  char nombre[MAX];
  bool completada;
  bool procedentes;
  List *tareasPrecedentes; // Creamos una pila de tareas precedentes.

} Tarea; // Definimos nuestra estructura a trabajar.

typedef struct
{
  Stack *registro;
  Stack *registroNombres;
} Pila;
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
  scanf("%30[^\n]s", nombreTarea);
}

// Funciones

// 1.

void agregarTarea(Heap *TPH, int *tareasRegistradas, Pila *pilaGlobal)
{
  Tarea *tareaAux = malloc(sizeof(Tarea));
  char *tarea = (char*) malloc(sizeof(char));
  int prioridadT;
  
  tareaAux -> tareasPrecedentes = createList();
  tareaAux -> completada = false;
  tareaAux -> procedentes = false;
  stack_push(pilaGlobal -> registro, "1");
  
  preguntarTarea(tarea);

  printf("\nIngrese prioridad:\n");
  scanf("%d", &prioridadT);
  strcpy(tareaAux -> nombre, tarea);
  prioridadT = prioridadT * (-1);
  
  (*tareasRegistradas)++;

  heap_push(TPH, tareaAux, prioridadT);
  stack_push(pilaGlobal -> registroNombres, tarea);
  printf("\nTarea agregada!\n\n");
}

// 2.

void establecerPrecedencia(Heap *TPH, Pila *pilaGlobal)
{
  Heap *aux = heap_clone(TPH);
  Heap *auxDos = heap_clone(TPH);
  
  char *tarea1 = (char*) malloc(sizeof(char)), *tarea2 = (char*) malloc(sizeof(char));
  bool banderitaTarea1 = false, banderitaTarea2 = false;

  preguntarTarea(tarea1); 
  printf("\n");
  preguntarTarea(tarea2);
  printf("\n");
  
  Tarea *tareaActual1;
  
  while (heap_top(aux) != NULL) // Recorremos el heap auxiliar
  {
    tareaActual1 = (Tarea *) heap_top(aux);
    
    if(strcmp(tareaActual1 -> nombre, tarea1) == 0) 
    {
      banderitaTarea1 = true;
      
      while (heap_top(auxDos) != NULL)   
      {
        Tarea *tareaActual2 = (Tarea *) heap_top(auxDos);
      
        if(strcmp(tareaActual2->nombre, tarea2) == 0) 
        {
          banderitaTarea2 = true; // Usamos booleanos para marcar que encontramos la tarea
          
          if(banderitaTarea2 == true) 
          {
            if(heap_top_priority(aux) < heap_top_priority(auxDos))
            {
              stack_push(pilaGlobal -> registro, "2");
              pushBack(tareaActual1 -> tareasPrecedentes, tarea2);
              tareaActual1 -> procedentes = true;
              
              printf("Ahora la tarea %s es precedente de %s!\n\n", tarea2, tarea1);
              
              TPH = heap_clone(auxDos); // Actualizamos el heap original
              
              free(aux);
              free(auxDos);
              return;
            }
            else
            {
              printf("No se pueden asignar las tareas como procedentes\n\n");
              free(aux);
              free(auxDos);
              return;
            }
          }
        }
        heap_pop(auxDos);
      }
      
      if(banderitaTarea2 == false)
      {
        printf("No se encontró la tarea ingresada.\n");
        free(aux);
        free(auxDos);
        return;
      }
    }
    heap_pop(aux);
  }
  printf("No se encontró la tarea ingresada.\n");
  free(aux);
  free(auxDos);
  
}
// 3.

void mostrarTareas(Heap *TPH)
{
  Heap *auxSinProcedentes = heap_clone(TPH);
  Heap *auxConProcedentes = heap_clone(TPH);
  int cont, cont2 = 0;
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n\n");
  
  while (heap_top(auxSinProcedentes) != NULL) // Mostramos primero las tareas sin precedentes.
  {
    Tarea *tareaActual = (Tarea *) heap_top(auxSinProcedentes);
    
    if(tareaActual -> procedentes == false)
    {
      printf("%i. %s (Prioridad: %i)", cont2 + 1, tareaActual -> nombre, (int) heap_top_priority(auxSinProcedentes) * (-1));
      printf("\n");
      cont2++;
    }
    heap_pop(auxSinProcedentes);
  }

  while (heap_top(auxConProcedentes) != NULL) // Mostramos solo las tareas con precedentes.
  {
    Tarea *tareaActual = (Tarea *) heap_top(auxConProcedentes);
    List *lista = List_clone(tareaActual -> tareasPrecedentes);

    if(tareaActual -> procedentes == true) 
    {
      printf("%i. %s (Prioridad: %i)", cont2 + 1, tareaActual -> nombre, (int) heap_top_priority(auxConProcedentes) * (-1));
    
      cont = 0;
      
      if(!firstList(tareaActual -> tareasPrecedentes)) printf("\n");
        
      else while (firstList(lista) != NULL) // Recorremos la lista de precedentes.
      {
        char *precedente = popFront(lista);
        if (cont == 0) printf(" - Precedente(s): %s", precedente);
        else printf(", %s", precedente);
        cont++;
      }
      printf("\n");
      cont2++;
    }
    heap_pop(auxConProcedentes);
  }
  printf("\n");
  free(auxSinProcedentes);
  free(auxConProcedentes);
}
//4 

void borrarDePrecedentes(Heap* TPH, char *tarea)
{
  Heap *heapAux = heap_clone(TPH);

  while(heap_top(heapAux) != NULL) // Recorremos TPH.
  {
    Tarea *aux = heap_top(heapAux);

    if(aux -> procedentes == true)
    {
      char *nodo = firstList(aux -> tareasPrecedentes);
      while(nodo != NULL) // Recorremos la lista
      {
        if(strcmp(nodo, tarea) == 0) // Si encuentra la tarea buscada, la elimina de la lista.
        {
          char *current = popCurrent(aux->tareasPrecedentes);
          free(current);
          break;
        }
        nodo = nextList(aux->tareasPrecedentes);
      }
    }
    if (firstList(aux->tareasPrecedentes) == NULL) aux->procedentes = false; // Si no la encontró luego de recorrer, marca el booleano como falso.
    
    heap_pop(heapAux);
  }
  
  return;
}

void marcarCompletada(Heap *TPH, int *tareasRegistradas)
{
  Tarea *aux = malloc(sizeof(Tarea));
  char *tarea = (char *) malloc(sizeof(char));
  
  bool valido = false;
  char *salida = (char *) malloc(sizeof(char));
  printf("Ingrese el nombre de la tarea:\n");
  scanf(" %[^\n]s", tarea);
  while (getchar() != '\n');

  for(int i = 0; i < get_size(TPH); i++) // Recorremos el heap.
  {
    aux = get_data(TPH, i);
    
    if(strcmp(aux -> nombre, tarea) == 0)
    {
      valido = true;
      printf("¿Seguro que quieres borrar la tarea? (s/n)\n");
      scanf("%s", salida);
      
      getchar();
      if (strcmp(salida, "n") == 0)
      {
        printf("La tarea no fue borrada.\n");
        return;
      }
      else
      {
        if(strcmp(salida, "s") == 0) // Borramos la tarea.
        {
          heap_pop_especifico(TPH, i);
          borrarDePrecedentes(TPH, tarea);
          printf("Se ha borrado la tarea.\n");
          (*tareasRegistradas)--;
        }
      }
    }
  }
  
  if(!valido)
  {
    printf("La tarea no fue encontrada en el sistema\n");
    return;
  }
}
// 5.

void deshacerAccion(Heap *TPH, Pila *pilaGlobal, int *tareasRegistradas)
{
  char *datoRegistro = stack_pop(pilaGlobal -> registro);

  if(strcmp(datoRegistro, "0") == 0)
  {
    printf("\nNo se pueden deshacer más acciones. \n\n");
    stack_push(pilaGlobal -> registro, "0");
  }

  else if(strcmp(datoRegistro, "1") == 0) // Dependiendo de la última acción realizada, entra a su respectivo if.
  {
    char *nombre = stack_pop(pilaGlobal -> registroNombres);
    int cont = get_size(TPH);
    Tarea *aux = malloc(sizeof(Tarea));
    
    for(int i = 0; i < cont; i++)
    {
      aux = get_data(TPH, i);
      
      if(strcmp(aux -> nombre, nombre) == 0)
      { 
        (*tareasRegistradas)--;
        heap_pop_especifico(TPH, i);
        borrarDePrecedentes(TPH, nombre);
      }
    }
    printf("Se ha deshecho la acción y %s fue eliminado\n", nombre);
  }
  //eliminamos la tarea

  else if(strcmp(datoRegistro, "2") == 0)
  {
    //eliminamos solo la precedencia
  }

  else if(strcmp(datoRegistro, "4") == 0)
  {
    //volvemos a agregar tarea
  }
  return;
}

// 6.

void cargarTareas(Heap *TPH, int *tareasRegistradas)
{
  char nombreArchivo[MAX];
  
  printf("Ingrese el nombre del archivo a importar:\n");
  scanf("%30s", nombreArchivo);
  
  FILE *fp = fopen(nombreArchivo, "r");
  
  if (fp == NULL)
  {
    printf("\nNo hay ningún archivo con ese nombre.\n\n");
    return;
  }
  
  char linea[1024];
  int i = 0;
  int k = 0;
  
  fgets(linea, 1023, fp);
  
  while(fgets(linea, 1023, fp) != NULL)
  {
    Tarea *tareaAux = malloc(sizeof(Tarea));
    int prioridad;
    
    tareaAux -> tareasPrecedentes = createList();
    tareaAux -> completada = false;
    tareaAux -> procedentes = false;
    
    for(i = 0 ; i < 3 ; i++)
    {
      char *aux = gets_csv_field(linea, i);
      
      if(i == 0) strcpy(tareaAux -> nombre, aux);
      
      if(i == 1) prioridad = atoi(aux) * (-1);
      
      if(i == 2)
      {
        if (strlen(aux) > 1) // Si lo que sigue después de la coma de prioridad no es vacío, agrega cada precedente.
        {
          tareaAux -> procedentes = true;
          char *precedentes = strtok(aux, " ");
          while (precedentes != NULL)
          {
            pushFront(tareaAux -> tareasPrecedentes, precedentes);
            precedentes = strtok(NULL, " ");
          }
        }
        else tareaAux -> procedentes = false;
      }
    }
    
    heap_push(TPH, tareaAux, prioridad);
    k++;
    (*tareasRegistradas)++;
  }
  
  fclose(fp);
  
  printf("\n%i tareas han sido importadas al sistema!\n\n", k);
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

  int tareasRegistradas = 0;

  Heap *tareasPorHacer = createHeap();

  Pila pilaGlobal;
  
  pilaGlobal.registro = createStack(); // Creamos una pila global para cada dato.
  
  pilaGlobal.registroNombres = createStack();
  
  printf("Bienvenido! :D\n");

  while(user_continue)
  {
    menu();
    
    int opcion = 0;
    
    validarOpcion(&opcion); // Validamos que opción sea un número.
    
    if(tareasRegistradas == 0 && opcion != 1 && opcion != 6 && opcion != 7) printf("No hay tareas registradas, registre una primero.\n");
    else
    {
      switch(opcion)
      {
        case 1 :
          agregarTarea(tareasPorHacer, &tareasRegistradas, &pilaGlobal);
          validar(&user_continue);
          break;

        case 2 :
          if(tareasRegistradas < 2)
          {
            printf("Deben registrarse al menos 2 tareas.\n");
            break;
          }
          establecerPrecedencia(tareasPorHacer, &pilaGlobal);
          validar(&user_continue);
          break;

        case 3 :
          mostrarTareas(tareasPorHacer);
          validar(&user_continue);
          break;

        case 4 :
          marcarCompletada(tareasPorHacer, &tareasRegistradas);
          validar(&user_continue);
          break;

        case 5 :
          deshacerAccion(tareasPorHacer, &pilaGlobal, &tareasRegistradas);
          validar(&user_continue);
          break;

        case 6 :
          cargarTareas(tareasPorHacer, &tareasRegistradas);
          validar(&user_continue);
          break;
        
        case 7 :
          printf("Gracias por usar el programa, adiós!");
          
          return 0;
      }
    }
  }
}