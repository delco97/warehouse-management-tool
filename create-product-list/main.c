//           Andrea Del Corto - Gabriele Giovannelli - creazione listino
#include <stdio.h>
#include <stdlib.h>
#define MAXC 15
struct Prodotto
{
  char name[MAXC];
  float cost;
  unsigned int quantTot;
  unsigned char quantMin;
};
void controllNum(unsigned char *x, unsigned int *y, long mini, long mas)
{
  int cont;
  cont = 0;
  if (x != NULL)
  {
    do
    {
      if (cont > 0)
        printf("\nValore inserito fuori range 1: ");
      scanf("%d", x);
      cont++;
    } while (*x < mini || *x > mas);
  }
  else
  {
    do
    {
      if (cont > 0)
        printf("\nValore inserito fuori range 2: ");
      scanf("%d", y);
      cont++;
    } while (*y < mini || *y > mas);
  }
}
void defVetStruct(struct Prodotto *v, unsigned int dim)
{
  int i;
  for (i = 0; i < dim; i++)
  {
    printf("\n\nProdotto %d di %d", i + 1, dim);
    printf("\n  - nome: ");
    scanf("%s", v[i].name);
    printf("\n  - costo: ");
    scanf("%f", &v[i].cost);
    printf("\n  - quantTot: ");
    controllNum(NULL, &v[i].quantTot, 1, 999999);
    printf("\n  - quantMin: ");
    controllNum(&v[i].quantMin, NULL, 1, 999999);
  }
}
int main()
{
  FILE *f;
  unsigned int nProd;
  struct Prodotto *allProd;

  printf("Inserisci il numero dei prodotti: ");
  controllNum(NULL, &nProd, 1, 15);

  allProd = (struct Prodotto *)malloc(nProd * sizeof(struct Prodotto));
  defVetStruct(allProd, nProd); // Inserimento caratteristiche prodotti.
  f = fopen("Prodotti.bin", "wb");
  fwrite(allProd, sizeof(struct Prodotto), nProd, f);
  fclose(f);
  free(allProd);
  return 0;
}
