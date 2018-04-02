#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#define COULEUR 32
#define CASE   10
#define TAILLE_BLOC         10
#define LARGEUR_FENETRE     1280
#define HAUTEUR_FENETRE     900
#define NB_BLOCS_LARGEUR    LARGEUR_FENETRE / TAILLE_BLOC
#define NB_BLOCS_HAUTEUR    HAUTEUR_FENETRE/ TAILLE_BLOC
#define MINY    TAILLE_BLOC*2 + 5
#define MINX    TAILLE_BLOC + 5
#define MAXY    NB_BLOCS_HAUTEUR - TAILLE_BLOC - 4
#define MAXX    NB_BLOCS_LARGEUR - TAILLE_BLOC - 8
#define VOLUME  3 //15
#define BLUE_BONUS  5
#define RED_BONUS   20
#define ECART 33
#define NB_PLAYER 2         // vous pouvez mettre le nombre de joueurs que vous voulez
#define DEGAT_EPEE 5
#define RECUP_RUBIS 3
#define CENTRE_CERCLE_X  (LARGEUR_FENETRE/2 -50) / TAILLE_BLOC
#define CENTRE_CERCLE_Y  (HAUTEUR_FENETRE/2) / TAILLE_BLOC
#define DEGAT_BOMBE 75
#define RECUP_DEGAT_BOMBE 25
#define RAYON_BOMBE 10

/*
    Ajout de la structure propre à chaque IA
*/
typedef struct Player Player;
struct Player
{
    int x;
    int y;
    int points;
    int item;
    bool bouclier;       // Est ce que l'IA pare les coups.
    int orientation;    // Direction dans laquelle regarde l'IA.
    int anim;           // Boucle qui servira pour les animations.
    int classement;
};

/*
    Ajout de la structure propre à chaque bombe
*/
typedef struct Item Item;
struct Item
{
    int x;
    int y;
    int tours;
    int joueur;
    int anim;
    bool active;
};

enum {UP, DOWN, LEFT, RIGHT,ANIM_UP1,ANIM_UP2,ANIM_UP3,ANIM_DOWN1,ANIM_DOWN2,ANIM_DOWN3,ANIM_LEFT1,ANIM_LEFT2,ANIM_LEFT3,ANIM_RIGHT1,ANIM_RIGHT2,ANIM_RIGHT3, HIT_UP, HIT_DOWN, HIT_LEFT, HIT_RIGHT, SHIELD};
enum {HAUT, BAS, GAUCHE, DROITE, EPEE_HAUT,EPEE_BAS,EPEE_DROITE,EPEE_GAUCHE, PARER,BOMBE};  /// Différentes actions que peuvent faire les IA's
enum{GREEN_RUPEE,BLUE_RUPEE,RED_RUPEE,VIDE,MUR,IA,GANON,POT,BOMBE_MAP,BOMBE_DEFLAG};    /// Différents éléments trouvables dans le tableau en argument.
int play (SDL_Surface* screen);
int movePlayer (int maps[][NB_BLOCS_HAUTEUR], SDL_Rect *position, int direction, Mix_Chunk *s_ruppes);
void SDL_Delay(Uint32 ms);
Uint32 SDL_GetTicks(void);
int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
int Mix_PlayMusic(Mix_Music *music, int loops);
void ganon_move(int maps[][NB_BLOCS_HAUTEUR], Mix_Chunk *s_degat, Player links[NB_PLAYER], SDL_Rect *position, int tours);
void setup_pictures (SDL_Surface *link[21],SDL_Surface *rupees[3],SDL_Surface *ganon[5], SDL_Surface *zelda[6], SDL_Surface *guard[16], SDL_Surface *skull[4],SDL_Surface *daruina[4],SDL_Surface *granma[4],SDL_Surface *koume[4],SDL_Surface *maple [4],SDL_Surface *oldman[4],SDL_Surface *nayru[4],SDL_Surface *saria[4],SDL_Surface *sheik[4], SDL_Surface *ruto[4], SDL_Surface *rauru[4], SDL_Surface *bombes[10]);
void setup_map (int maps[][NB_BLOCS_HAUTEUR]);
void timer (char temps[],char score[],int time,int lastTime,int stime,int mtime,int points);
int win (bool survivant, SDL_Surface* screen, Mix_Music *gerudo, Player links[NB_PLAYER], int continuer, int tours);
void animation(SDL_Surface* screen, SDL_Surface *zelda[6], SDL_Surface *skull[4],SDL_Surface *daruina[4],SDL_Surface *granma[4],SDL_Surface *koume[4],SDL_Surface *maple [4],SDL_Surface *oldman[4],SDL_Surface *nayru[4],SDL_Surface *saria[4],SDL_Surface *sheik[4], SDL_Surface *ruto[4], SDL_Surface *rauru[4], int tours);
void garde(SDL_Surface* screen, SDL_Surface *guard[16], int tours);
int test_ia(int maps[][NB_BLOCS_HAUTEUR]);
void damage(int maps[][NB_BLOCS_HAUTEUR], Player links[NB_PLAYER], int playernow);
int test_ia(int maps[][NB_BLOCS_HAUTEUR], int x, int y, int points, int item, int bouclier, int orientation);
void blit_items(int maps[][NB_BLOCS_HAUTEUR], SDL_Surface* screen, SDL_Surface* rupees[3], SDL_Surface *bombes[10]);
void setup_ia(int maps[][NB_BLOCS_HAUTEUR], Player links[NB_PLAYER]);
bool test_class(int maps[][NB_BLOCS_HAUTEUR], Player links[NB_PLAYER]);
void item(int maps[][NB_BLOCS_HAUTEUR], Player links[NB_PLAYER], int tours, Item bombes [100]);
/*
Fonction qui renvoie un entier pour faire bouger l'IA
*/
int ia_1(int maps_ia[][NB_BLOCS_HAUTEUR], int x, int y, int points, int item, int tours)
{
    int coup; // action retournée à chaque tours
    int i,j; //distance entre link et un rubis
    int depart_x,depart_y,fin_x,fin_y;
    int rubis[2];
    int dep=tours%20;

    // recherche localisation de rubis
    if (tours>=140)
    {
        fprintf(stderr,"\ntour %d, j'ai %d points\n",tours,points);

        depart_x=x-20;
        depart_y=y-20;
        fin_x=x+20;
        fin_y=y+20;

         fprintf(stderr,"y:%d min :%d, max:%d   ",y,depart_y,fin_y);

        if(depart_x<=MINX) depart_x=MINX+10;
        else if (fin_x>=MAXX) fin_x=MAXX-10;
        if (depart_y<=MINY)
        {
            depart_y=MINY+10;
            fprintf(stderr,"depart change ! : %d",depart_y);
        }
        else if (fin_y>=MAXY)
        {
            fin_y=MAXY-10;
            fprintf(stderr,"fin change ! : %d",fin_y);
        }

     //     fprintf(stderr,"plage x : %d - %d je suis a %d, plage y : %d,%d je suis a %d ",depart_x,fin_x,x,depart_y,fin_y,y);


        for (i=depart_x;i<fin_x;i++)
        {
            for(j=depart_y;j<fin_y;j++)
            {
                if(maps_ia[i][j]==2 || maps_ia[i][j]==4 || maps_ia[i][j]==5)
                {
                    rubis[0]=i;   // récupère la position du rubis
                    rubis[1]=j;
                    i=fin_x;
                    j=fin_y;
                         //fprintf(stderr,"le rubis le plus proche %d,%d : et je suis à %d,%d \n",i,j,x,y);
                }
            }
        }


        // déplacement link pour rubis
        if (dep<10)
        {
            if (rubis[0]<x)
            {
                coup=LEFT;
              //  fprintf(stderr,"rubis.x=%d, je suis à %d, je vais à gauche !\n",rubis[0],x);
            }
            else
            {
                coup=RIGHT;
               // fprintf(stderr,"rubis.x=%d, je suis à %d, je vais à droite!\n",rubis[0],x);
            }
        }
        else
        {
            if (rubis[1]<y)
            {
                coup=UP;
               // fprintf(stderr,"rubis.y=%d, je suis à %d, je monte !\n",rubis[1],y);
            }
            else
            {
                coup=DOWN;
              //  fprintf(stderr,"rubis.y=%d, je suis à %d, je descends !\n",rubis[1],y);
            }
        }
    }
    else// tant qu'il n'y a pas de rubis on s'éloigne du centre
    {
         if(x>(MAXX-MINX)/2)
        {
            coup=RIGHT;
        }
        else coup = LEFT;
         if(x==MINX || x==MAXX ||y==MINY)
        {
            coup=UP;
        }
        if(y==MAXY)
            {
                coup=DOWN;
            }
    }
    return coup;
}

int ia_2(int maps_ia[][NB_BLOCS_HAUTEUR], int x, int y, int points, int item, int tours)
{
    if (rand()%25 == 0)
    {
        return  4 + rand()%4;
    }
    return  0 + rand()%4;
}

#endif