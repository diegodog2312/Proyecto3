// Dijkstra.c
// usage: dijkstra nv print

// where nv is the size of the graph, and print is 1 if graph and min
// distances are to be printed out, 0 otherwise

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// global variables, shared by all threads by default

int nv,            // number of vertices
    *notdone,      // vertices not checked yet
    md,            // current min over all threads
    mv,            // vertex which achieves that min
    largeint = -1; // max possible unsigned int

unsigned *ohd, // 1-hop distances between vertices; "ohd[i][j]" is
    // ohd[i*nv+j]
    *mind; // min distances found so far

void init(int ac, char **av)
{
    srand(2);
    int i, j, tmp;
    nv = atoi(av[1]);
    ohd = malloc(nv * nv * sizeof(int));
    mind = malloc(nv * sizeof(int));
    notdone = malloc(nv * sizeof(int));
    // random graph
    for (i = 0; i < nv; i++)
        for (j = i; j < nv; j++)
        {
            if (j == i)
                ohd[i * nv + i] = 0;
            else
            {
                ohd[nv * i + j] = rand() % 20;     // Llena el grafo con numeros aleatorios
                ohd[nv * j + i] = ohd[nv * i + j]; //El valor de ida es el mismo que de regreso
            }
        }
    for (i = 1; i < nv; i++)
    {
        notdone[i] = 1;   //Ningun verrtice está revisado aún
        mind[i] = ohd[i]; //La distancia mínima encontrada hasta ahora es
    }
}

void dowork()
{
    int step, // whole procedure goes nv steps
        mymv, // vertex which attains that value
        i;
    unsigned mymd; // min value found by this thread

    for (step = 0; step < nv; step++)
    {
        // closest in its group, then we find overall closest
        md = largeint;
        mv = 0;
        mymd = largeint;
        for (i = 1; i < nv; i++)
        {
            if (notdone[i] && mind[i] < mymd)
            {
                mymd = ohd[i];
                mymv = i;
            }            
        }
        // update overall min if mine is smaller
        if (mymd < md)
        {
            md = mymd;
            mv = mymv;
        }
        // mark new vertex as done
        notdone[mv] = 0;
        // now update ohd
        for (i = 1; i < nv; i++)
            if (mind[mv] + ohd[mv * nv + i] < mind[i])
                mind[i] = mind[mv] + ohd[mv * nv + i];    
    }
}

int main(int argc, char **argv)
{
    int i, j, print;
    double startime, endtime;
    init(argc, argv);
    startime = omp_get_wtime();
    dowork();
    endtime = omp_get_wtime();
    print = atoi(argv[2]);
    if (print)
    {
        printf("graph weights:\n");
        for (i = 0; i < nv; i++)
        {
            for (j = 0; j < nv; j++)
                printf("%u ", ohd[nv * i + j]);
            printf("\n");
        }
        printf("minimum distances:\n");
        for (i = 1; i < nv; i++)
            printf("%u\n", mind[i]);
        printf("\n\n");
    }
    printf("Tiempo de ejecucion: %f", endtime - startime);
}