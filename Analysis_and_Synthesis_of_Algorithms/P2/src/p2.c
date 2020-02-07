#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex {
    int el, visited, augmented, parent, visitedBFS;
    int tipo; /*tipo = { 0:Hiper, 1:Fornecedor, 2:Posto, 3:Source}*/
    struct connection* parentEdge;
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

typedef struct queueItem {
    int value;
    struct queueItem *prev, *next, *tail;
} *QueueLink;

int parseArgs(vertexLink* fListEl, ListLink** fListAdj, int* edges, int* capForn, int* nPost, int* nFornecedor);
void insertConnection(ListLink* listAdj, int o, connectionLink connection);
connectionLink* BFS(ListLink* listAdj, vertexLink listEl, int size, int source, int target, int markFound, int* startPath);
int maximumFlow(ListLink* listAdj, vertexLink listEl, int size);
int findToImprove(ListLink* listAdj, vertexLink listEl, int size, int edges, int nPostos, int nFornecedor);
void invertConnections(ListLink* listAdj, vertexLink listEl, int size, int contrary);
void push(QueueLink* queue, int value);
int pop(QueueLink* queue);
int isEmpty(QueueLink* queue);

int main(int argc, char **argv){
    int size = 0, flow=0, edges = 0, capForn=0, nPostos=0, nForn=0;
    int source = 1, target = 0;
	vertexLink listEl;                         /*Estrutura com informacao dos fornecedores e postos de abastecimentos e Hiper*/
    ListLink* listAdj;                         /*Estrutura com as ligacoes*/
	size = parseArgs(&listEl, &listAdj, &edges, &capForn, &nPostos, &nForn); 
    flow = maximumFlow(listAdj, listEl, size);
    if (flow < capForn || BFS(listAdj, listEl, size, source, target, 0, NULL) == NULL) findToImprove(listAdj, listEl, size, edges, nPostos, nForn);
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
        listEl[i+nPostos].augmented = 0;
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
    ListLink new = (ListLink) malloc(sizeof(struct list));
    ListLink head = listAdj[o];
    new->prev = NULL;
    new->connection = connection;
    new->visited = 0;

    /*Insere ordenado*/
    if (head == NULL){
        listAdj[o] = new;
        new->next = NULL;
        new->tail = new;
    }
    else {
        ListLink aux;
        for (aux = head; aux != NULL; aux = aux->next){
            if (aux->connection->v->el > new->connection->v->el){
                new->next = aux;
                new->prev = aux->prev;
                if (aux->prev != NULL) aux->prev->next = new;
                aux->prev = new;
                new->tail = head->tail;
                if (aux == head) listAdj[o] = new;
                break;
            }
        }
        if (aux == NULL){
            new->next = NULL;
            new->prev = head->tail;
            head->tail->next = new;
            head->tail = new;
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

connectionLink* BFS(ListLink* listAdj, vertexLink listEl, int size, int source, int target, int markFound, int* startPath){
    /*int* visited = (int*) malloc(sizeof(int)*size);*/
    int i, currentV=0, maxDepth=0;

    /*Inicializa a queue*/
    for (i=0; i<size; i++){
        listEl[i].visitedBFS = 0;
    }

    /*Insere a source*/
    QueueLink queue = (QueueLink) malloc(sizeof(struct queueItem));
    queue->next = NULL;
    queue->tail = queue;
    queue->prev = NULL;
    queue->value = source;

    i=0;
    
    while (!isEmpty(&queue)){
        currentV = pop(&queue);

        ListLink adj = listAdj[currentV];

        while (adj != NULL){
            int u = adj->connection->v->el, res = adj->connection->residual;

            /*Verifica se consegue passar*/
            if (listEl[u].visitedBFS == 0 && res != 0){
                maxDepth++;
                if (markFound) listEl[u].visited = 1;
                push(&queue, u);
                listEl[u].visitedBFS = 1;
                listEl[u].parent = currentV;
                listEl[u].parentEdge = adj->connection;

                /*Se chegamos ao target fazemos o caminho de volta*/
                if (u == target && !markFound){
                    connectionLink* path = (connectionLink*) malloc(sizeof(connectionLink) * (maxDepth + 1));
                    while (u != source){
                        path[maxDepth--] = listEl[u].parentEdge;
                        u = listEl[u].parent;
                    }
                    if (startPath != NULL) *startPath = maxDepth+1;
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
    int startPath = 0;

    connectionLink* path = BFS(listAdj, listEl, size, source, target, 0, &startPath);
    while (path != NULL){
        int maxFlow = -1, i=startPath;

        while (path[i] != NULL){
            /*printf("%d->%d com %d , ", path[i]->v->parent, path[i]->v->el, path[i]->residual);   */ 
            connectionLink adj = path[i];
            int currentRes = adj->residual;
            if (currentRes<maxFlow || maxFlow == -1) maxFlow = currentRes;
            if(path[i]->v->el == target) break;
            i++;
        }
        /*printf("%d -> %d\n", target, maxFlow);*/


        i = startPath;
        while (path[i] != NULL){
            connectionLink adj = path[i];
            adj->residual -= maxFlow;
            adj->flow += maxFlow;
            if(path[i]->v->el == target) break;
            i++;
        }
        totalFlow += maxFlow;

        path = BFS(listAdj, listEl, size, 0, 1, 0, &startPath);
    }
    printf("%d\n", totalFlow); /*Fluxo maximo*/
    return totalFlow;
}


int findToImprove(ListLink* listAdj, vertexLink listEl, int size, int edges, int nPostos, int nFornecedor) {
    int* vertixesToAugment = (int*) malloc(sizeof(int) * 1);
    int** edgesToAugment = (int**) malloc(sizeof(int*) * 1);
    int vertixesPointer = 0, edgesPointer = 0, i;

    invertConnections(listAdj, listEl, size, 1);
    
    listEl[1].visited = 1;
    BFS(listAdj, listEl, size, 1, 0, 1, NULL);

    for (i=1; i<size; i++){
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

void push(QueueLink* queue, int value){
    QueueLink new1 = (QueueLink) malloc(sizeof(struct queueItem));
    new1->value = value;
    new1->next = NULL;
    new1->tail = new1;
    
    if ((*queue) == NULL){
        new1->prev = NULL;
        *queue = new1;
    }
    else {
        new1->prev = (*queue)->tail;
        if ((*queue)->tail == (*queue)) {
            (*queue)->next = new1;
            new1->prev = *queue;
        }
        (*queue)->tail->next = new1;
        (*queue)->tail->tail = new1;
        (*queue)->tail = new1;
    }
}

int pop(QueueLink* queue){
    QueueLink adj = *queue;
    if (adj == NULL) return -1;
    else {
        int value = adj->value;
        if ((*queue)->next != NULL) {
            (*queue)->next->prev = NULL;
            adj->next->tail = (*queue)->tail;
        }
        (*queue) = adj->next;
        free(adj);
        return value;
    }
}

int isEmpty(QueueLink* queue){
    return ((*queue) == NULL)?1:0;
}