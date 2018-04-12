#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#define COULEUR 32
#define CASE   10
#define TAILLE_BLOC         10
#define LARGEUR_FENETRE     1280
#define HAUTEUR_FENETRE     900
#define NB_BLOCS_LARGEUR    LARGEUR_FENETRE / TAILLE_BLOC
#define NB_BLOCS_HAUTEUR    HAUTEUR_FENETRE/ TAILLE_BLOC
#define MINY    TAILLE_BLOC*2 + 5 //15
#define MINX    TAILLE_BLOC + 5   //110
#define MAXY    NB_BLOCS_HAUTEUR - TAILLE_BLOC - 4  //76
#define MAXX    NB_BLOCS_LARGEUR - TAILLE_BLOC - 8  //25
#define VOLUME  3 //15
#define BLUE_BONUS  5
#define RED_BONUS   20
#define ECART 33
#define NB_PLAYER 6    // vous pouvez mettre le nombre de joueurs que vous voulez
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
    #define RUBIS 1
    #define ENEMY 2

    int i,j; //distance entre link et un rubis
    int depart_x,depart_y,fin_x,fin_y;
    int recherche = 0;

    int rubis[2]={CENTRE_CERCLE_X+4,CENTRE_CERCLE_Y-4}; // si ne trouve pas de rubis, se dirige vers le centre
    int advers[2]; //coordonée des adversaires
    int dx,dy,dmin_x,dmin_y; //conditions pour les rubis
    int ax,ay,amin_x,amin_y; //conditions pour les adversaires

    if (points<=0)
    {
        return 1;
    }

    if (item>0)
    {
        fprintf(stderr, "je pose une bombe");
        return BOMBE;
    }

    // recherche localisation de rubis
   else if (tours>=140)
    {
        fprintf(stderr,"\ntour %d, j'ai %d points\n",tours,points);

    ///Définit la zone de recherche des rubis (dans un rayon de 20 autour de sa position)
        depart_x=x-20; // d
        depart_y=y-20;
        fin_x=x+20;
        fin_y=y+20;

        dmin_x=depart_x; // la plus petite distance trouvée
        dmin_y=depart_y;

    ///Définit la zone de recherche des enemis dans un rayon de 3 cases
        amin_x=x+3;
        amin_y=y+3;

///Recherche des rubis
       // if( x>MINX + 27 && x<MAXX-27)
        //{
            for (i=depart_x;i<fin_x;i++)
            {
                for(j=depart_y;j<fin_y;j++)
                {
                    if(maps_ia[i][j]==GREEN_RUPEE || maps_ia[i][j]==BLUE_RUPEE || maps_ia[i][j]==RED_RUPEE ||maps_ia[i][j]==POT)
                    {
                        dx=abs(x-i); //cacul la distance entre sa potition et la positiion des rubis
                        dy=abs(y-j);

                        if(dx<dmin_x && dy<dmin_y) // si le nouveau rubis trouvé est plus près
                        {
                            rubis[0]=i;   // récupère la position du rubis
                            rubis[1]=j;
                            dmin_x=dx;
                            dmin_y=dy;
                            recherche = RUBIS;
                        }
                    }
                    if (maps_ia[i][j]==BOMBE_MAP)
                        {
                            fprintf(stderr,"J ai trouvé une bombe  ");

                                if(x>i) return RIGHT;
                                else return LEFT;

                                /*if(y>j) return DOWN;
                                else return UP;*/

                        }


                }
            }
      //  }
///Recherche des IA si ne trouve pas de rubis
        if ( recherche =! RUBIS)
        {
            for (i=x-3;i<x+3;i++)
            {
                for (j=y-3;j<y+3;j++)
                {
                     if(maps_ia[i][j]==IA && i!=x && j!=y) //
                        {
                            fprintf(stderr,"trouvé IA");
                            ax=abs(x-i);
                            ay=abs(y-j);

                            if(ax<amin_x && ay<amin_y)
                            {
                                fprintf(stderr,"trouvé IA PLUS PROCHE");
                                rubis[i]=i;
                                rubis[j]=j;
                                amin_x=ax;
                                amin_y=ay;
                                recherche=ENEMY;
                            }
                        }

                }
            }
        }



        /*    if (abs(x-advers[0])>abs(y-advers[1]))
            {
                if (advers[0]<x) return LEFT;
                else  return RIGHT;
            }
            else if(abs(x-advers[0])<=abs(y-advers[1]))
            {
                if (advers[1]<y) return UP;
                else  return DOWN;
            }
            else if (advers[0]-x==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_DROITE;
            }
            else if (x-advers[0]==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_GAUCHE;
            }
            else if (advers[1]-y==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_HAUT;
            }
             else if (y-advers[1]==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_BAS;
            }
        }*/


     ///Déplacement link pour rubis
      /*  if ( recherche == ENEMY)
         {
             if (rubis[0]-x==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_DROITE;
            }
            else if (x-rubis[0]==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_GAUCHE;
            }
            else if (rubis[1]-y==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_HAUT;
            }
             else if (y-rubis[1]==1)
            {
                fprintf(stderr,("touché ! \n"));
                return EPEE_BAS;
            }
         }
         else
         {*/
            if (abs(x-rubis[0])>abs(y-rubis[1]))
            {
                if (rubis[0]<x) return LEFT;
                else  return RIGHT;
            }
            else
            {
                if (rubis[1]<y) return UP;
                else  return DOWN;
            }
        // }
    }// fin du nombre du tours
/// Tant qu'il n'y a pas de rubis on s'éloigne du centre
   else
   {
            if(x>(MAXX-MINX)/2)
            {
                return RIGHT;
            }
            else  return LEFT;
    }

}


int ia_2(int maps_ia[][NB_BLOCS_HAUTEUR], int x, int y, int points, int item, int tours)
{
    if (tours%10==0 || item >0)
    {
        return BOMBE;

    }
   return rand()%4;
}

#endif
