#include "graphics.h"
#include "cgi.h"
#include <string.h>

void abrirLinkABS(char *l)
{
  char link[MAX_BUFFER];
  sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s\n", l);
  ABRIR_LINK(link);
}

void colorscheme (ESTADO *e)
{
  switch(e->tema) {
    case DEFAULT: e->tema = DRACULA;
                  break;
    case DRACULA: e->tema = MONOKAI;
                  break;
    case MONOKAI: e->tema = DEFAULT;
                  break;
  }
}

void colorSchemePath (ESTADO *e, char *path)
{
  switch(e->tema)
  {
    case DEFAULT: sprintf(path, "%s%s/", IMAGE_PATH, "default");
                  break;
    case DRACULA: sprintf(path, "%s%s/", IMAGE_PATH, "dracula");
                  break;
    case MONOKAI: sprintf(path, "%s%s/", IMAGE_PATH, "monokai");
                  break;
  }
}

void getScaleFactor(int *sf, ESTADO *e)
{
  *sf = (e->num_lins >= e->num_cols) ? (float) (DTAB / e->num_lins) : (float) (DTAB / e->num_cols);
}

void calculaCentroPeca(ESTADO *e, int sf, int *fx, int *fy)
{
  int cx = ((ECRA_X-DTAB)/2)-1, cy = ((ECRA_Y-DTAB)/2)-1, fnsx, fnsy; //fns = fix non square
  fnsx = (DTAB - (e->num_cols * sf)) / 2;
  fnsy = (DTAB - (e->num_lins * sf)) / 2;
  *fx = cx + fnsx;
  *fy = cy + fnsy;
}

void butaoProxEstadoJog(ESTADO *e, int i, int j, int sf, char *s, char *user)
{
  int cx, cy;
  char path[60];
  char action[20];
  char link[60];
  calculaCentroPeca(e, sf, &cx, &cy);
  sprintf(action, "@x-%d-y-%d", i, j);
  sprintf(link, "%s%s", user, action);
  colorSchemePath(e, path);
  ABRIR_LINK_ABS(link);
    IMAGEM(i+cx, j+cy, sf, s, path);
  FECHAR_LINK;
}

void drawBloq(ESTADO *e, int i, int j, int sf, char *s)
{
  int cx = 0, cy = 0;
  char path[60];
  colorSchemePath(e, path);
  calculaCentroPeca(e, sf, &cx, &cy);
  IMAGEM(i+cx, j+cy, sf, s, path);
}

void drawBloqPreview(ESTADO *e, int i, int j, int sf, char *s, int tab)
{
  int cx = 0, cy = 0;
  char path[60];
  colorSchemePath(e, path);
  switch(tab % 6)
  {
    case 1: cx = 215;
            cy = 225;
            break;
    case 2: cx = 850;
            cy = 225;
            break;
    case 3: cx = 1515;
            cy = 225;
            break;
    case 4: cx = 215;
            cy = 600;
            break;
    case 5: cx = 850;
            cy = 600;
            break;
    case 0: cx = 1515;
            cy = 600;
            break;
  }
  IMAGEM(i+cx, j+cy, sf, s, path);
}

void drawButton(ESTADO *e, int x, int y, int sx, int sy, char *s, char *action, char *user)
{
  char link[40];
  char path[60];
  colorSchemePath(e, path);
  sprintf(link, "%s%s", user, action);
  ABRIR_LINK_ABS(link);
    IMAGEM_ABS(x, y, sx, sy, s, path);
  FECHAR_LINK;
}

void drawTextButton(int x, int y, int n, char *action, char *user)
{
  char link[40];
  sprintf(link, "%s%s", user, action);
  ABRIR_LINK_ABS(link);
    TEXTO(x, y, "#FFF", n);
  FECHAR_LINK;
}

void drawPreview(ESTADO *e, int id, char *action, char *user, int tab)
{
  int i, j, sf;
  ESTADO d;
  char link[40];
  d.id = id;
  d.tema = DEFAULT;
  sprintf(link, "%s%s", user, action);
  ABRIR_LINK_ABS(link);
   d = le_tabuleiro(&d, tab);
   getScaleFactor(&sf, &d);
   for (i = 0; i < d.num_cols; i++)
    for (j = 0; j < d.num_lins; j++)
    {
      if(e->tema != d.tema) d.tema = e->tema;
      drawPeca(i, j, &d, (sf*0.3), 1, user, 1, tab);
    }

  FECHAR_LINK;
}

void drawPeca(int i, int j, ESTADO *e, int sf, int vPeca, char *user, int preview, int tab)
{
  char s[50];
  switch (e->grelha[i][j]) {
    case VAZIA:
                  if (vPeca == 2) strcpy(s, "wrong.png");
                  else strcpy(s, "vazio.png");
                  break;
    case SOL_X:
                  if (vPeca) strcpy(s, "X.png");
                  else strcpy(s, "wrongX.png");
                  break;
    case SOL_O:
                  if (vPeca) strcpy(s, "O.png");
                  else strcpy(s, "wrongO.png");
                  break;
    case FIXO_X:
                  if (vPeca) strcpy(s, "bloqX.png");
                  else strcpy(s, "wrongbloqX.png");
                  break;
    case FIXO_O:
                  if (vPeca) strcpy(s, "bloqO.png");
                  else strcpy(s, "wrongbloqO.png");
                  break;

    case HINT_X:  strcpy(s, "hintX.gif");
                  break;
    case HINT_O:  strcpy(s, "hintO.gif");
                  break;
    case BLOQUEADA: strcpy(s, "bloq.png");
                    break;
  }

  if (e->grelha[i][j] > FIXO_O && (!preview)) butaoProxEstadoJog(e, i, j, sf, s, user);
  else if (!preview) drawBloq(e, i, j, sf, s);
       else drawBloqPreview(e, i, j, sf, s, tab);
}

void drawSemaforo (ESTADO *e)
{
  char s[20];
  char path[60];
  colorSchemePath(e, path);
  switch (e->validade)
  {
    case VALIDO: strcpy(s, "valido.png");
                 break;
    case IMPOSSIVEL: strcpy(s, "impossivel.png");
                     break;
    case INVALIDO: strcpy(s, "invalido.png");
                   break;
  }
  IMAGEM_ABS(727, 790, 450, 150, s, path);
}
