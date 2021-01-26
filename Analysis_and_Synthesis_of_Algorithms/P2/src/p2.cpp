#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue> 

using namespace std; 

typedef struct vertex {
    int el, visited, augmented;
    int tipo; /*tipo = { 0:Hiper, 1:Fornecedor, 2:Posto, 3:Source}*/
} *vertexLink;

typedef struct connection {
    vertexLink v;
    int flow, cap, residual, inverted;
} *connectionLink;

typedef struct list {
    connectionLink connection;
    int visited;
	struct list *prev, *next, *tail;
} *ListLink;

int parseArgs(vertexLink* fListEl, ListLink** fListAdj, int* edges, int* capForn, int* nPost, int* nFornecedor);
void insertConnection(ListLink* listAdj, int o, connectionLink connection);
int* BFS(ListLink* listAdj, vertexLink listEl, int size, int source, int target, int markFound);
int maximumFlow(ListLink* listAdj, vertexLink listEl, int size);
int findToImprove(ListLink* listAdj, vertexLink listEl, int size, int edges, int nPostos, int nFornecedor);
int* DFS(ListLink* listAdj, vertexLink listEl, int size, int source, int target);
void invertConnections(ListLink* listAdj, vertexLink listEl, int size, int contrary);

int main(int argc, char **argv){
    int size = 0, flow=0, edges = 0, capForn=0, nPostos=0, nForn=0;
    int source = 1, target = 0;
	vertexLink listEl;                         /*Estrutura com informacao dos fornecedores e postos de abastecimentos e Hiper*/
    ListLink* listAdj;                         /*Estrutura com as ligacoes*/
	size = parseArgs(&listEl, &listAdj, &edges, &capForn, &nPostos, &nForn); 
    flow = maximumFlow(listAdj, listEl, size);
    if (flow < capForn || BFS(listAdj, listEl, size, source, target, 0) == NULL) findToImprove(listAdj, listEl, size, edges, nPostos, nForn);
    return 0;
}

int parseArgs(vertexLink* fListEl, ListLink** fListAdj, int* edges, int* capForn, int* nPost, int* nFornecedor){
    int nForn, nPostos, nLigacoes, size;

    scanf("%d %d %d", &nForn, &nPostos, &nLigacoes);
    size = nForn + 2*nPostos + 1 + 1; /*+1 -> Hiper(target), +1 -> Source*/

    vertexLink listEl = (vertexLink) malloc(sizeof(struct vertex) * size);
    ListLink *listAdj = (ListLink*) malloc(sizeof(ListLink) * size);

    listEl[0].el = 0; listEl[0].tipo = 3; listEl[0].visited = 0; listEl[0].augmented = 0; listAdj[0] = NULL; /*Source*/
    listEl[1].el = 1; listEl[1].tipo = 0; listEl[1].visited = 0; listEl[1].augmented = 0; listAdj[1] = NULL; /*Hiper (Target)*/

    int currentI = 2, i=currentI;

    /*Introduz capacidade dos fornecedores*/
    for (; i<(nForn+currentI); i++){
        int cap;
        scanf("%d", &cap);
        *capForn += cap;
        listEl[i].el = i;
        listEl[i].tipo = 1;
        listEl[i].visited = 0;
        listEl[i].augmented = 0;
        listAdj[i] = NULL;

        /*Liga a source aos fornecedores*/
        connectionLink connection = (connectionLink) malloc(sizeof(struct connection));
        connection->v = &(listEl[i]);
        connection->cap = cap;
        connection->residual = cap;
        connection->flow = 0;
        connection->inverted = 0;
        insertConnection(listAdj, 0, connection);
    }

    currentI = i;

    /*Introduz capacidade dos postos de abastecimento*/
    for (; i<(+nPostos+currentI); i++){
        int cap;
        scanf("%d", &cap);
        listEl[i].el = i;
        listEl[i].tipo = 2;
        listEl[i].visited = 0;
        listEl[i].augmented = 0;
        listAdj[i] = NULL;

        listEl[i+nPostos].el = i+nPostos;
        listEl[i+nPostos].tipo = 2;
        listEl[i+nPostos].visited = 0;
        listEl[i].augmented = 0;
        listAdj[i+nPostos] = NULL;

        /*Liga posto de abastecimento a posto linha (simboliza a sua capacidade)*/
        connectionLink connection = (connectionLink) malloc(sizeof(struct connection));
        connection->v = &(listEl[i+nPostos]);
        connection->cap = cap;
        connection->residual = cap;
        connection->flow = 0;
        connection->inverted = 0;
        insertConnection(listAdj, i, connection);
    }

    /*Insere conexões*/
    for (i=0; i<nLigacoes; i++){
        connectionLink connection = (connectionLink) malloc(sizeof(struct connection));
        int o, d, c;
        scanf("%d %d %d", &o, &d, &c);
        connection->v = &(listEl[d]);
        connection->cap = c;
        connection->residual = c;
        connection->flow = 0;
        connection->inverted = 0;
        if ( o <= (nForn+1)) insertConnection(listAdj, o, connection);
        else insertConnection(listAdj, o+nPostos, connection);
    }

    *nFornecedor = nForn;
    *nPost = nPostos;
    *edges = nLigacoes;
    *fListAdj = listAdj;
    *fListEl = listEl;
    return size;
}

void insertConnection(ListLink* listAdj, int o, connectionLink connection){
    ListLink new1 = (ListLink) malloc(sizeof(struct list));
    ListLink head = listAdj[o];
    new1->prev = NULL;
    new1->connection = connection;
    new1->visited = 0;

    /*Insere ordenado*/
    if (head == NULL){
        listAdj[o] = new1;
        new1->next = NULL;
        new1->tail = new1;
    }
    else {
        ListLink aux;
        for (aux = head; aux != NULL; aux = aux->next){
            if (aux->connection->v->el > new1->connection->v->el){
                new1->next = aux;
                new1->prev = aux->prev;
                if (aux->prev != NULL) aux->prev->next = new1;
                aux->prev = new1;
                new1->tail = head->tail;
                if (aux == head) listAdj[o] = new1;
                break;
            }
        }
        if (aux == NULL){
            new1->next = NULL;
            new1->prev = head->tail;
            head->tail->next = new1;
            head->tail = new1;
        }
    }

}

void invertConnections(ListLink* listAdj, vertexLink listEl, int size, int contrary){
    int i;
    for (i=0; i<size; i++){
        ListLink adj = listAdj[i];
        while (adj != NULL){
            ListLink aux = adj->next;
            int el = adj->connection->v->el;
            if (adj->connection->inverted == 0){
                connectionLink connection = (connectionLink) malloc(sizeof(struct connection));
                connection->v = &listEl[i];
                connection->cap = adj->connection->cap;
                connection->residual = adj->connection->residual;
                connection->flow = adj->connection->flow;
                connection->inverted = 1;
                insertConnection(listAdj, el, connection);

                adj->connection->residual = adj->connection->flow;
                adj->connection->flow = adj->connection->residual;
                adj->connection->inverted = 1;
            }
            adj = aux;
        }
    }
}

int* BFS(ListLink* listAdj, vertexLink listEl, int size, int source, int target, int markFound){
    /*int* vertexesQueue = (int*) malloc(sizeof(int)*size);*/
    int* visited = (int*) malloc(sizeof(int)*size);
    int* parent = (int*) malloc(sizeof(int)*size), *depth = (int*) malloc(sizeof(int)*size);
    
    int i, currentV=0, numV = 1;

    queue <int> vertexesQueue;


    /*Inicializa a queue*/
    for (i=0; i<size; i++){
        /*vertexesQueue[i] = -1;*/
        visited[i] = 0;
        depth[i] = 0;
    }

    /*Insere a source*/
    /*vertexesQueue[0] = source;*/ vertexesQueue.push(source);
    visited[currentV] = 0;
    
    i=0;
    int queueSize = 1;
    while (!vertexesQueue.empty()){
        /*currentV = vertexesQueue[i++];*/ currentV = vertexesQueue.front(); vertexesQueue.pop();
        if (currentV == -1) break;

        ListLink adj = listAdj[currentV];

        while (adj != NULL){
            int u = adj->connection->v->el, res = adj->connection->residual;

            /*Verifica se consegue passar*/
            if (visited[u] == 0 && res != 0){
                if (u > numV) numV = u;
                /*parent = (int*) realloc(parent, sizeof(int)*(numV+1));*/
                if (markFound) listEl[u].visited = 1;
                /*vertexesQueue[queueSize++] = u;*/ vertexesQueue.push(u);
                visited[u] = 1;
                parent[u] = currentV;
                depth[u] = depth[currentV]+1;

                /*Se chegamos ao target fazemos o caminho de volta*/
                if (u == target && !markFound){
                    int *path = (int*) malloc(sizeof(int)*(depth[u]+1));
                    while (u != source){
                        path[depth[u]] = u;
                        u = parent[u];
                    }
                    path[0] = source;
                    return path;
                }
            }
            adj = adj->next;
        }
    }
    /*Nao chegamos ao target*/
    return NULL;
}

int maximumFlow(ListLink* listAdj, vertexLink listEl, int size){
    int target = 1; /*O target tem numero 1*/
    int source = 0;
    int totalFlow = 0;

    int* path = BFS(listAdj, listEl, size, source, target, 0);
    while (path != NULL){
        int maxFlow = -1, i=0;

        while (path[i] != target){
            ListLink adj = listAdj[path[i]];
            /*printf("%d - ", path[i]);*/
            while (adj != NULL){
                if (adj->connection->v->el == path[i+1]){
                    int currentRes = adj->connection->residual;
                    if (currentRes<maxFlow || maxFlow == -1) maxFlow = currentRes;
                    break;
                }
                adj = adj->next;
            }
            i++;
        }
        /*printf("%d -> %d\n", target, maxFlow);*/

        i=0;
        while (path[i] != target){
            ListLink adj = listAdj[path[i]];
            while (adj != NULL){
                if (adj->connection->v->el == path[i+1]){
                    adj->connection->residual -= maxFlow;
                    adj->connection->flow += maxFlow;
                    break;
                }
                adj = adj->next;
            }
            i++;
        }
        totalFlow += maxFlow;

        path = BFS(listAdj, listEl, size, 0, 1, 0);
    }
    printf("%d\n", totalFlow); /*Fluxo maximo*/
    return totalFlow;
}


int findToImprove(ListLink* listAdj, vertexLink listEl, int size, int edges, int nPostos, int nFornecedor) {
    int* vertixesToAugment = (int*) malloc(sizeof(int) * 1);
    int** edgesToAugment = (int**) malloc(sizeof(int*) * 1);
    int vertixesPointer = 0, edgesPointer = 0, i;

     /*for (i=0; i<size; i++) {
        printf("%d: ", i);
        ListLink adj = listAdj[i];
        while(adj!=NULL) {
            printf("%d ", adj->connection->v->el);
            adj = adj->next;
        }
        printf("\n");
    }
    printf("\n");*/

    invertConnections(listAdj, listEl, size, 1);
    
    listEl[1].visited = 1;
    BFS(listAdj, listEl, size, 1, 0, 1);

    for (i=1; i<size; i++){
        /*printf("%d visitado? %d\n", i, listEl[i].visited);*/
        if (!listEl[i].visited){
            ListLink adj = listAdj[i];
            while (adj != NULL){
                int target = adj->connection->v->el;
                if (adj->connection->v->visited == 1){
                    if ( (i + nPostos) == target && target > (nPostos+nFornecedor+1)){
                        vertixesToAugment[vertixesPointer++] = i;
                        listEl[i].augmented = 1;
                        vertixesToAugment = (int*) realloc(vertixesToAugment, (vertixesPointer+1)*sizeof(int));
                    }
                    else if (listEl[i].augmented == 0) {
                        int source = i;
                        if (i > (nPostos+nFornecedor+1)) source -= nPostos;
                        int* edges = (int*) malloc(sizeof(int)*2);
                        edges[0] = source; edges[1] = target;
                        edgesToAugment[edgesPointer++] = edges;
                        edgesToAugment = (int**) realloc(edgesToAugment, (edgesPointer+1)*sizeof(int*));
                    }
                }
                adj = adj->next;
            }
        }
    }

    for (i=0; i < vertixesPointer; i++) {
        if (i == (vertixesPointer - 1)) printf("%d\n", vertixesToAugment[i]);
        else printf("%d ", vertixesToAugment[i]);
    }

    if(vertixesPointer==0) printf("\n");
    
    for (i=0; i < edgesPointer; i++) {
        if ((i == (edgesPointer - 1)) && listEl[edgesToAugment[i][0]].augmented == 0) printf("%d %d\n", edgesToAugment[i][0], edgesToAugment[i][1]);
        else if (listEl[edgesToAugment[i][0]].augmented == 0) printf("%d %d\n", edgesToAugment[i][0], edgesToAugment[i][1]);
    }
    
    return 0;
}


