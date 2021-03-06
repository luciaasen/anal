/**
 *
 * Descripcion: Implementacion de funciones de tiempo
 *
 * Fichero: tiempos.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2016
 *
 */

#include "permutaciones.h"
#include "tiempos.h"
#include "ordenacion.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

/***************************************************/
/* Funcion: tiempo_medio_ordenacion Fecha:         */
/*                                                 */
/* Vuestra documentacion (formato igual            */
/* que en el primer apartado):                     */
/***************************************************/
short tiempo_medio_ordenacion(pfunc_ordena metodo, 
                              int n_perms,
                              int tamanio, 
                              PTIEMPO ptiempo)
{   int i, obBuff, obMed=0;
    float t1, t2, tDiff, tMed=0; 
    int **ArrPerm;
    
    ptiempo->n_perms=n_perms;
    ptiempo->tamanio= tamanio;
	ptiempo->min_ob= INT_MAX;
	ptiempo->max_ob= 0;
    
    if(!metodo || n_perms<1 || tamanio<1 || !ptiempo) return ERR;
    
    ArrPerm= genera_permutaciones( n_perms,  tamanio);
    if(!ArrPerm) return ERR;
    
    for(i=0; i<n_perms; i++){
       t1=clock(); 
       obBuff= metodo(ArrPerm[i], 0, tamanio-1);
       if(obBuff == ERR) return ERR;
       t2=clock();
       if(obBuff<ptiempo->min_ob){
           ptiempo->min_ob=obBuff;
       }
       if(ptiempo->max_ob<obBuff){
           ptiempo->max_ob=obBuff;
       }
       obMed+= obBuff;
       
       tDiff= (t2-t1)/CLOCKS_PER_SEC;
       tMed+=tDiff;
       
    } 
    ptiempo->tiempo=tMed/n_perms;
    ptiempo->medio_ob=obMed/n_perms;

	for(i=0; i<n_perms; i++){
		if(ArrPerm[i])
			free(ArrPerm[i]);

	}
	free(ArrPerm);

    return OK;
}

/***************************************************/
/* Funcion: genera_tiempos_ordenacion Fecha:       */
/*                                                 */
/* Vuestra documentacion                           */
/***************************************************/
short genera_tiempos_ordenacion(pfunc_ordena metodo, char* fichero, 
                                int num_min, int num_max, 
                                int incr, int n_perms)
{
    
    int min= num_min, iter=0, i, j, tamanio;

	    iter=((num_max-num_min)/incr)+1;
	PTIEMPO * pptiempo= (PTIEMPO *)malloc(sizeof(PTIEMPO)*iter);
 
    if(!pptiempo) return ERR;
    
    if(!fichero || !metodo || num_min<1 || num_min>num_max || incr<1 || n_perms<1)
    return ERR;
    

   
    
    
     for(i=0; i<iter; i++){
		pptiempo[i]= (PTIEMPO) malloc(sizeof(TIEMPO));
		if(!pptiempo[i]){
			for(j = 0; j<i; j++){
				free(pptiempo[j]);
			}
       		free(pptiempo);
       		return ERR;
       }
    }
   
    
    for(i=0, tamanio=num_min; i<iter; i++, tamanio+=incr){
       tiempo_medio_ordenacion( metodo, n_perms, tamanio, pptiempo[i]);
       guarda_tabla_tiempos(fichero, pptiempo[i], 1);
    }    
    
    for(i = 0; i<iter; i++){
    	if(pptiempo[i])
			free(pptiempo[i]);
			
		}
 	free(pptiempo);
    
    return OK;

}

/***************************************************/
/* Funcion: guarda_tabla_tiempos Fecha:            */
/*                                                 */
/* Vuestra documentacion (formato igual            */
/* que en el primer apartado):                     */
/***************************************************/
short guarda_tabla_tiempos(char* fichero, PTIEMPO tiempo, int N)
{
  FILE *f;
  if(!fichero || !tiempo || N<1) return ERR;
  f = fopen(fichero, "a");
  
  fprintf(f,"%d %f %f %d %d \n", tiempo->tamanio, tiempo->tiempo, tiempo->medio_ob, tiempo->min_ob, tiempo->max_ob);
       
    fclose(f);
    return OK;
}
