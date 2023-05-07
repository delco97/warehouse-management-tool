//           AndreaDelCorto_GabrieleGiovannelli_3IA - Gestione magazzino 2.5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//Per controllo stringhe.
#include <termios.h>
#include <unistd.h>
#define MAXC 16
#define N_SCELTE 5


int getch() {
    struct termios oldtc, newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}

struct Prodotto
{
  char name[MAXC];
  float cost;
  unsigned int quantTot;
  unsigned char quantMin;
};
struct Voce_Menu
{
  char scelta;
  char descrizione[30];
  void (*pf)(struct Prodotto *, int);
};
int dinamicSerch(char *sSerch, struct Prodotto *allProd, int nProd)
{
  int i, posStr = -1, leng = 0;
  char *caratUguali, repet = 0;
  caratUguali = (char *)malloc(nProd * sizeof(char));
  leng = strlen(sSerch);
  // printf("\nsSerch= %s\nlen = %d",sSerch,leng);system("pause");
  for (i = 0; i < nProd; i++)
    caratUguali[i] = memcmp(allProd[i].name, sSerch, leng);
  for (i = 0; i < nProd; i++)
  {
    if (caratUguali[i] == 0)
    { // le str sono uguali
      repet++;
      posStr = i;
    }
  }
  free(caratUguali);
  if (posStr != -1 && repet == 1)
    return posStr;
  else
    return -1;
}
char controlSelectMenu(char *vScelte, unsigned char dim)
{
  char carat;
  unsigned int i;
  do
  {
    carat = getch();
    for (i = 0; i < dim; i++)
    {
      if (carat == vScelte[i])
      {
        printf("%c\n", carat);
        return carat;
      }
    }
  } while (1);
}
void listProd(struct Prodotto *allProd, int nProd)
{
  int i;
  printf("\n  Lista prodotti:\n\n");
  printf("  ----------------------------------------------\n");
  printf("  | nProd | nome Prodotto  | costo | quantita' |");
  for (i = 0; i < nProd; i++)
  {
    printf("\n  ----------------------------------------------\n");
    printf("  |%3d/%3d|%16s|%7.2f|%5d/%5d|", i + 1, nProd, allProd[i].name, allProd[i].cost, allProd[i].quantTot, allProd[i].quantMin);
    if (allProd[i].quantTot < allProd[i].quantMin)
      printf("   **da ordinare**");
  }
  printf("\n  ----------------------------------------------\n");
  printf("\n  Premi un tato per proseguire.\n");
  getch();
}
void valMaga(struct Prodotto *allProd, int nProd)
{
  float valTot = 0;
  int i, pezziTot = 0;
  for (i = 0; i < nProd; i++)
  {
    valTot += allProd[i].cost * allProd[i].quantTot;
    pezziTot += allProd[i].quantTot;
  }
  printf("\n  - Contiene %d diversi tipi di prodotti", nProd);
  printf("\n  - Contiene %d pezzi totali", pezziTot);
  printf("\n  - Valore complessivo: %.2f euro", valTot);
  printf("\n\n  Premi un tasto qualunque per tornare al menu principale. . .");
  getch();
}
int controlInsStr(struct Prodotto *allProd, int nProd)
{
  char ch, ap[MAXC];
  int i, j, pos = -1;
  ch = '0';
  for (i = 0; i < nProd; i++)
    ap[i] = ' ';
  ap[0] = '\0';
  printf("\n  Nome prodotto(maxCarat %d): ", MAXC - 2);
  for (i = 0; ch != '\n';)
  {
    ch = getch();
    if (ch == '\b' && i > 0)
    { // Cancella carettere.
      i--;
      printf("\b");
      ap[i] = ' ';
      ap[i + 1] = '\0';
      pos = dinamicSerch(ap, allProd, nProd);
      printf("%c", ap[i]);
      printf("\b");
    }
    else
    {
      if (ch != '\b' && ch != '\n' && i < MAXC - 1)
      { // Caratteri validi per la stringa.
        ap[i] = ch;
        ap[i + 1] = '\0';
        printf("%c", ap[i]);
        i++;
        pos = dinamicSerch(ap, allProd, nProd);
        if (pos != -1)
        { // Solo una stringa puo' essere individuata a questo punto!
          for (j = 0; j < i; j++)
            printf("\b");
          i = strlen(allProd[pos].name);
          printf("%s", allProd[pos].name);
          strcpy(ap, allProd[pos].name);
          // printf("\nstr = %s\npos= %d",ap,pos);
        }
      }
      if (ch == '\n')
        return pos;
      if (ch == 27)
        return -2;
    }
  }
}
void gestOrdMaga(struct Prodotto *allProd, int nProd)
{
  FILE *f;
  int i, cont = 0, nOrdini;
  listProd(allProd, nProd);
  do
  { // ricerca nome prodotto
    if (cont > 0)
      printf("\n  Prodotto non trovato nella lista.\n");
    i = controlInsStr(allProd, nProd);
    // i = dinamicSerch(nomeProd,allProd,nProd);
    cont++;
  } while (i == -1);
  cont = 0;
  if (i != -2)
  { // i=-2-->ESC
    do
    { // lettura numero pezzi
      if (cont > 0)
        printf("  Valore non valido.\n");
      printf("\n  Numero di pezzi da ordinare (max ordine: %d ): ", allProd[i].quantMin * 5 - allProd[i].quantTot);
      scanf("%d", &nOrdini);
      cont++;
    } while (nOrdini > allProd[i].quantMin * 5 - allProd[i].quantTot || nOrdini < 0);
    if (nOrdini > 0)
    {
      printf("\n  I prodotti sono stati ordinati.");
      allProd[i].quantTot += nOrdini;
    }
    else
      printf("\n  Non sono stati ordinati prodotti.");
    // AGGIORNAMENTO LISTINO
    f = fopen("Prodotti.bin", "wb");
    fwrite(allProd, sizeof(struct Prodotto), nProd, f);
    fclose(f);
  }
  printf("\n  Premi un tasto qualsiasi per tornare al menu'. . .");
  getch();
}
void vendMaga(struct Prodotto *allProd, int nProd)
{
  FILE *f;
  int i, cont = 0, nAcquisti;
  listProd(allProd, nProd);
  do
  { // ricerca nome prodotto
    if (cont > 0)
      printf("\n  Prodotto non trovato nella lista.\n");
    i = controlInsStr(allProd, nProd);
    // i = dinamicSerch(nomeProd,allProd,nProd);
    cont++;
  } while (i == -1);
  cont = 0;
  if (i != -2)
  { // i=-2-->ESC
    do
    { // lettura numero pezzi
      if (cont > 0)
        printf("  Valore non valido.\n");
      printf("\n  Numero di pezzi da acquistare (max: %d): ", allProd[i].quantTot);
      scanf("%d", &nAcquisti);
      cont++;
    } while (nAcquisti < 0 || nAcquisti > allProd[i].quantTot);
    if (nAcquisti > 0)
    {
      printf("\n  I prodotti sono stati venduti.");
      allProd[i].quantTot -= nAcquisti;
    }
    else
      printf("\n  Non sono stati venduti prodotti.");
    // AGGIORNAMENTO LISTINO
    f = fopen("Prodotti.bin", "wb");
    fwrite(allProd, sizeof(struct Prodotto), nProd, f);
    fclose(f);
  }
  printf("\n  Premi un tasto qualsiasi per tornare al menu'. . .");
  getch();
}
void uscita(struct Prodotto *allProd, int nProd)
{
  printf("\n\n                          ARRIVEDERCI :)\n\n");
  exit(0);
}
unsigned char scletaMenu(struct Voce_Menu *v, unsigned char dim)
{
  unsigned int i;
  char c, *vScelte;
  vScelte = (char *)malloc(dim * sizeof(char));
  for (i = 0; i < dim; i++)
    vScelte[i] = v[i].scelta;
  do
  {
    printf("\nInserisci ");
    for (i = 0; i < dim - 1; i++)
    {
      if (i != dim - 2)
        printf("%c, ", v[i].scelta);
      else
        printf("%c o %c: ", v[i].scelta, v[i + 1].scelta);
    }
    c = controlSelectMenu(vScelte, dim);
    for (i = 0; i < dim; i++)
    {
      if (c == v[i].scelta)
      {
        free(vScelte);
        return i;
      }
    }
  } while (1);
}
void stampMenu(char *tit, struct Voce_Menu *v, int dim, struct Prodotto *allProd, int nProd)
{
  int i, j, daOrdinare;
  daOrdinare = 0;
  system("clear");
  printf("                      >------------------------------------<\n");
  printf("                      >                                    <\n");
  printf("                                 %s\n", tit);
  printf("                      >                                    <\n");
  printf("                      >------------------------------------<\n\n");
  printf("Menu':\n");
  for (j = 0; j < nProd; j++)
  { // Guarda nel listno se ci sono ordini da fare.
    if (allProd[j].quantTot < allProd[j].quantMin)
    {
      daOrdinare++;
      break;
    }
  }
  for (i = 0; i < dim; i++)
  {
    printf(" %c - %s", v[i].scelta, v[i].descrizione);
    if (i == 1 && daOrdinare != 0)
      printf(" <---- Ordini da effettuare!");
    printf("\n");
  }
}
int main()
{
  FILE *f;
  int nProd;
  // void (*pf)();
  unsigned char i;
  struct Prodotto *allProd;
  struct Voce_Menu Menu[N_SCELTE] = {
      {'1', "Valorizzazione Magazzino", valMaga},
      {'2', "Gestione Ordini", gestOrdMaga},
      {'3', "Vendita prodotti", vendMaga},
      {'4', "Visualizza listino", listProd},
      {'x', "Termina il programma", uscita},
  };
  system("TITLE Gestione Magazzino");
  system("color F");
  do
  {
    f = fopen("Prodotti.bin", "rb");
    if (f != NULL)
    { // File trovato.
      // LETTURA LISTINO
      fseek(f, 0, SEEK_END);                                                // Posiziona la testina di lettura in fondo al file.
      nProd = ftell(f) / sizeof(struct Prodotto);                           // Numero dei record.
      allProd = (struct Prodotto *)malloc(nProd * sizeof(struct Prodotto)); // Alocazione dinamica vettore dei prodotti.
      fseek(f, 0, SEEK_SET);                                                // Posiziona la testina di lettura a inizio FILE
      fread(allProd, sizeof(struct Prodotto), nProd, f);                    // Inserisce dentro allProd il contenuto del listino ("prodotti.bin").
      // MENU E SCELTA
      stampMenu("Gestione Magazzino", Menu, N_SCELTE, allProd, nProd);
      i = scletaMenu(Menu, N_SCELTE); // SCELTA UTENTE
      system("clear");
      printf("\n --->%s<---\n", Menu[i].descrizione);
      Menu[i].pf(allProd, nProd); // Viene lanciata la funzione scelta dall'utente.
      fflush(stdin);
      fclose(f);
    }
    else
    {
      printf("\n  Listino prodotti non trovato!");
      printf("\n  Premi un tasto qualsiasi per chiudere il programma. . .\n");
      getch();
      exit(0);
    }
  } while (1);
  return 0;
}
