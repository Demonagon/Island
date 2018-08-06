#ifndef MIND_NODE__H
#define MIND_NODE__H
#include "mind_memory.h"

/** Créé le 6 août 2018 par Pacôme
* Une MindNode est un noeud dans le "réseau de neurone" d'une IA. Un tel réseau
* est différent des réseaux de neurones habituels dans le sens qu'ils n'ont
* pas la vocation d'apprendre. Ces réseaux sont des réseaux construits à la
* main, ce choix étant justifié que nous voulons ici créer une expérience
* de jeu vidéo ; le besoin de maîtriser le comportement des IAs est donc
* nécessaire.
* Cependant, afin de rendre ce comportement riche, nous allons essayer la
* structure suivante : chaque noeud reçoit une information de temps à autre
* d'une nature qui varie de noeud en noeud. Considérant certain paramètres,
* le noeud doit constament concevoir un produit (dont la nature dépend du noeud)
* qui soit optimale selont une fonction d'estimation qui peut varier au cours
* du temps. De plus, chaque information reçue en entrée a vocation d'être
* potentiellement rétractée ou modifiée à tout moment.
* Nous avons choisi d'utiliser des arbres de recherche binaires afin de stocker
* les solutions construites au fur et à mesure sous une forme triée. À chaque
* fois qu'un ingrédient est ajouté, mis à jour ou retiré, l'arbre contenant
* les produits se met à jour.
* La capacité maximale de l'arbre des produits est fixée à une constante. De
* plus, l'IA va régulièrement oublier les solutions les moins optimale. Par
* exemple, un animal peut en dormant conserver uniquement la moitié des objets
* utiles dans tout les noeuds de son système.
*
* Bien évidemment ces noeuds ont la vocation de fonctionner en réseau. Ainsi
* Chaque noeud possède une "destination" : qui va généralement être soit un
* autre noeud, soit une destination finale comme l'action qui sera effectuée
* par un animal.
*/

struct MindNode;

typedef MemoryTokenEvaluator MindProductEvaluator;
// 1 : data, 2 : solution, 3 : solution value
typedef void (*MindNodeOutputCallback) (void *, void *, int);
// 1 : node, 2 : ingredient, 3 : ingredient value
typedef void (*MindNodeIngredientProcessorFunction) (struct MindNode *, void *, int);

typedef struct MindNodeIngredientProcessor {
	MindNodeIngredientProcessorFunction on_new;
	MindNodeIngredientProcessorFunction on_deleted;
} MindNodeIngredientProcessor;

typedef struct MindNodeParent {
	void * data;
	MindNodeOutputCallback on_new_solution;
	MindNodeOutputCallback on_deleted_solution;
} MindNodeParent;

MindNodeParent mindNodeParentOutputCreate(void * data,
	MindNodeOutputCallback on_new_solution,
	MindNodeOutputCallback on_deleted_solution);
MindNodeParent mindNodeParentNodeCreate(struct MindNode * node);

typedef struct MindNode {
	MindNodeParent parent;
	MindNodeIngredientProcessor ingredient_processor;
	MindMemory product_memory;
} MindNode;

void mindNodeInit(
	MindNode * node, 
	MindNodeParent parent,
	MindNodeIngredientProcessor ingredient_processor,
	int max_products,
	MindProductEvaluator memory_evaluator);

void mindChildNodeInit(
	MindNode * node, 
	MindNode * parent_node,
	MindNodeIngredientProcessor ingredient_processor,
	int max_products,
	MindProductEvaluator memory_evaluator);

void mindNodeAddProduct(MindNode * node, void * product);
void mindNodeForgetHalf(MindNode * node);

#endif
