#ifndef MAIN_MEMORY__H
#define MAIN_MEMORY__H

#include "object.h"

/**
* Ce fichier décrit le code de la base de donnée qui contiendra tout les objets
* du jeu qui ont besoin d'une mémoire pour être stockés. Chaque objet répondant
* à cette description devra être défini dans "objet.h".
* Cette base de donnée peut contenir un nombre théoriquement illimité d'objets,
* d'une façon très compacte. Pour cela, elle découpe la liste des objets en
* morceaux (chunks) de taille fixe, qui sont indexés dans le tableau
* "dynamical_array". Dès que la base de donnée est remplie, on augmente la
* taille de "dynamical_array" (en copiant au passage l'adresse de chaque
* morceau), et on alloue dans la nouvelle case un tableau de taille
* MAIN_MEMORY_CHUNK_SIZE.
*
* Le coût maximal d'une opération sur cette base de donnée est de
* nombre d'objets / taille des morceaux. La taille des morceaux doit être
* gardée raisonnable pour que la charge mémoire colle au maximum au nombre
* réel.
*
* Un détail de fonctionnement intéressant est que la base de donnée ne va
* retirer un morceau que lorsque deux morceaux entiers sont vides. Cela est
* conçu pour éviter la situation où un objet serait ajouté puis enlevé en boucle
* autour du seuil de changement ; sans cette précaution la base de donnée
* passerait un temps conséquent à allouer et désallouer des morceaux de 
* mémoire.
*
* Par soucis d'optimisation toute exploration exhaustive ou partiellement
* exhaustive de la base de donnée est interdite. Pour permettre aux objets
* de retrouver leurs propres indices on utilise une fonction contenue dans
* un pointeur dont l'objectif est de signaler tout changement d'indice
* d'un objet.
*/

typedef unsigned int MemoryIndex;

typedef void (*SetupRoutine) (GameObject *, MemoryIndex);
typedef void (*IndexUpdater) (GameObject *, MemoryIndex);

typedef struct MemoryObject {
	IndexUpdater index_updater;
	GameObject object;
} MemoryObject;

#define MAIN_MEMORY_CHUNK_SIZE 100

typedef struct MainMemory {
	MemoryObject ** dynamical_array;
	unsigned int last_chunk_size;
	unsigned int before_last_chunk_size;
	unsigned int dynamical_array_size;
} MainMemory;

MainMemory mainMemoryCreate();

void mainMemoryFree(MainMemory * memory);

void mainMemoryAddArray(MainMemory * memory);
void mainMemoryRemoveArray(MainMemory * memory);
unsigned int mainMemoryGetTotalSize(MainMemory * memory);
unsigned int mainMemoryGetTotalCapacity(MainMemory * memory);

unsigned int mainMemoryGetIndexChunk(MemoryIndex index);
unsigned int mainMemoryGetInChunkIndex(MemoryIndex index);

MemoryIndex mainMemoryAddObject(MainMemory * memory, GameObject object,
								SetupRoutine setup_routine,
								IndexUpdater index_updater);
GameObject mainMemoryRemoveObject(MainMemory * memory, MemoryIndex index);

GameObject * mainMemoryAccessObject(MainMemory * memory, MemoryIndex index);

/*----------------------------------------------------------------------------*/

/**
* Des fonctions qui raccourcisent le boulot pour placer un objet en mémoire.
*/

//void gameObjectCopyAndSetup(MainMemory * memory, GameObject object,
//							SetupRoutine setup_routine);

#endif
