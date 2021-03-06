#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "functions.h"
#include "graphique.h"


/* N'ont été ajouté à ce fichier que les commentaires relatifs aux ajouts graphiques vu qu'il reprend le même schéma que le rendu 1*/

int initialiserMonde(Monde *monde){
    monde->tour = 0;
    initialiserPlateau(monde->plateau);
    (monde->rouge) = calloc(1, sizeof(*(monde->rouge)));
    if((monde->rouge) == NULL) {
      return 0;
    }
    (monde->rouge)->couleur = ROUGE;
    (monde->rouge)->unites = NULL;
    (monde->bleu) = calloc(1, sizeof(*(monde->bleu)));
    if((monde->bleu) == NULL) {
      return 0;
    }
    (monde->bleu)->couleur = BLEU;
    (monde->bleu)->unites = NULL;
    return 1;
}

void initialiserPlateau(Unite *plateau[LONG][LARG]) {
  size_t i, j;
  for(i = 0; i < LONG; ++i) {
    for(j = 0; j < LARG; ++j) {
      plateau[i][j] = NULL;
    }
  }
}

Unite *creerUnite(char genre, UListe *uliste) {
  Unite *temp;
  temp = calloc(1, sizeof(*(uliste->unites)));
  if(temp != NULL) {
    temp->genre = genre;
    temp->couleur = uliste->couleur;
    temp->suiv = NULL;
    if(uliste->unites == NULL) {
      uliste->unites = temp;
    } else {
      temp->suiv = uliste->unites;
      uliste->unites = temp;
    }
  }

  return temp;

}

Unite *dernier(UListe uliste) {
  Unite *unite;
  unite = uliste.unites;
  while(unite != NULL && unite->suiv != NULL) {
    unite = (unite->suiv);
  }
  return unite;
}

int placerAuMonde(Unite *unite, Monde *monde, size_t posX, size_t posY) {
  if(unite == NULL || monde->plateau[posY][posX] != NULL || posY >= LONG || posX >= LARG) {
    return 0;
  } else {
    unite->posX = posX;
    unite->posY = posY;
    monde->plateau[posY][posX] = unite;
    return 1;
  }
}



char *getSymbol(char genre) {
  char *string;
  string = malloc(3 * sizeof(*string));
  switch(genre) {
    case(GUERRIER):
      strcpy(string, "()");
      break;
    case(SERF):
      strcpy(string, "00");
      break;
    default:
      strcpy(string, "xx");
      break;
  }
  return string;
}


void deplacerUnite(Unite *unite, Monde *monde, int destX, int destY){
    if(monde->plateau[destY][destX] == NULL && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ) /* On verifie que la destination existe et est vide et que c'est un déplacement adjacent */
    {
                    monde->plateau[unite->posY][unite->posX]=NULL;
                    unite->posX = destX;
                    unite->posY = destY;
                    monde->plateau[destY][destX] = unite;

        }
dessinerplateau(*monde);
}

void enleverUnite(Unite *unite, Monde *monde) {
  UListe *uliste = getUListe(unite->couleur, monde);
  Unite *unitePrec = getUnitePrec(unite, uliste);
  if(unitePrec == unite) {
    uliste->unites = unite->suiv;
  } else if(unitePrec != NULL) {
    unitePrec->suiv = unite->suiv;
  }
  monde->plateau[unite->posY][unite->posX] = NULL;
  free(unite);
}

UListe *getUListe(char couleur, Monde *monde) {
  if(couleur == (monde->rouge)->couleur) {
    return (monde->rouge);
  }
  else if(couleur == (monde->bleu)->couleur) {
    return (monde->bleu);
  }
  else {
    return NULL;
  }

}

Unite *getUnitePrec(Unite *unite, UListe *uliste) {
  Unite *search;
  search = uliste->unites;
  while(search != NULL && search->suiv != unite && search != unite) {
    search = search->suiv;
  }
  return search;
}



/* On gere les evenements au clic du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
void gererTourJoueur(char couleur, Monde *monde) {
  int selection;
  char c='N';
  UListe uliste = *getUListe(couleur, monde);
  int nUnite = nombreUnite(uliste);
  Unite **uniteSelect = creerSelection(uliste);
  int mouseX=414;
  int mouseY=590;
  if(nUnite!=0) {
    dessinerplateau(*monde);
    do {
      selection = parcourirUniteSelect(uniteSelect, nUnite, *monde);
      if(selection != -1) {
        actionUnite(uniteSelect[selection], monde);
        nUnite = enleverSelect(uniteSelect, selection, nUnite);
        dessinerplateau(*monde);
        
       
        arreterTour();  
        while (!(mouseX<315 && mouseX>215 && mouseY<641 && mouseY>591) && !(mouseX<515 && mouseX>415 && mouseY<641 && mouseY>591)){
        MLV_wait_mouse(&mouseX, &mouseY);  
            
        if((mouseX<315 && mouseX>215 && mouseY<641 && mouseY>591)){
            c='O';
      }
        else if((mouseX<515 && mouseX>415 && mouseY<641 && mouseY>591))
        {
            c='N';
        }
    }
    }}
      while( (c=='N') && (selection > -1) && (nombreUnite(*(monde->rouge)) > 0 && nombreUnite(*(monde->bleu)) > 0));
         
          free(uniteSelect);
      
}
}

Unite **creerSelection(UListe uliste) {
  int n = nombreUnite(uliste);
  int i;
  Unite **tab = calloc(n, sizeof(*tab));
  Unite *unite = uliste.unites;
  for(i = 0; i < n && unite != NULL; ++i) {
    tab[i] = unite;
    unite = unite->suiv;
  }

  return tab;
}

int nombreUnite(UListe uliste) {
  Unite *unite;
  int n = 0;
  unite = uliste.unites;
  n += (unite != NULL);
  while(unite != NULL && unite->suiv != NULL) {
    unite = (unite->suiv);
    n++;
  }
  return n;
}

int parcourirUniteSelect(Unite **tab, int length, Monde monde) {
    
  int i = -1;
  char c='N';    
  int mouseX, mouseY; /* On gere ici aussi les evenements au click du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
   
  
  if(length <= 0) {
   
  } 
    else {

          if(i + 1 < length) {
            ++i;  
          } else {
            i = 0;
          }
          hoverSelection(*tab[i],monde);  
         
           MLV_wait_mouse(&mouseX, &mouseY);  

    while(c=='N') 
    {
 
        if(mouseX<515 && mouseX>415 && mouseY<641 && mouseY>591) {

          if(i + 1 < length) {
            ++i;  
          } else {
            i = 0;
          }
          hoverSelection(*tab[i],monde);  
           MLV_wait_mouse(&mouseX, &mouseY);  

        } else if(mouseX<315 && mouseX>215 && mouseY<641 && mouseY>591) {
            c='O';
        } else {
            MLV_wait_mouse(&mouseX, &mouseY); 
        }
            
    }
              
  }

   
  return i;

}

int enleverSelect(Unite **tab, size_t indice, size_t length) {
  if(indice < length - 1) {
    decaleSelect(tab, indice, length);
  }
  return (length - 1);
}

void decaleSelect(Unite **tab, size_t debut, size_t length) {
  size_t i;
  for(i = debut; i < length - 1; ++i) {
    tab[i] = tab[i + 1];
  }
}

/* On gere ici aussi les evenements au clic du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
void actionUnite(Unite *unite, Monde *monde) {
  char c='P';       
  troisActions();
  int mouseX, mouseY;
  
    while (c=='P'){
        MLV_wait_mouse(&mouseX, &mouseY);    
      if(mouseX>100 && mouseX<200 && mouseY<641 && mouseY>591){ 
      c='D';
      action(c);
    int posX, posY;
    MLV_wait_mouse(&posX, &posY);
    deplacerUnite(unite, monde, posX/40, posY/40);
  } 
        else if(mouseX>300 && mouseX<400 && mouseY<641 && mouseY>591) {
     
      c='A';
      action(c);
     int attX,attY;
   MLV_wait_mouse(&attX, &attY);
    attaquer(unite, monde, attX/40, attY/40);
  }
        
        
       else if(mouseX>500 && mouseX<600 && mouseY<641 && mouseY>591)  {
           c='T';
   
           
        }   
    }
}



int attaquer(Unite *unite, Monde *monde, int destX, int destY){
    if(monde->plateau[destY][destX] !=NULL && unite->couleur!= monde->plateau[destY][destX]->couleur ){
                if (unite->genre==GUERRIER || unite->genre==monde->plateau[destY][destX]->genre){
                    enleverUnite(monde->plateau[destY][destX],monde);
                    deplacerUnite(unite,monde,destX,destY);
                    dessinerplateau(*monde);
                    return 1;
                        }
                    enleverUnite(unite,monde);
                    dessinerplateau(*monde);
                    return 0;
    }
dessinerplateau(*monde);
   return 0;

}

int deplacerOuAttaquer(Unite *unite, Monde *monde, int destX, int destY){
    if( destX >= LONG || destY >= LARG){
        return -1;
    }

    if( destX <= LONG && destY <= LARG && abs(destX-(unite->posX))>=1 && abs(destY-(unite->posY))>=1 ){
        return -2;
    }

       if(monde->plateau[destY][destX]->genre == unite->genre){
        return -3;
    }

    if(monde->plateau[destY][destX] == NULL && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ) /* On verifie que la destination existe et est vide et que c'est un déplacement adjascent */
    {   deplacerUnite(unite,monde,destX,destY);

        return 1;
    }

    if(monde->plateau[destY][destX]->genre != unite->genre  && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ){
        attaquer(unite,monde,destX,destY);
        if ((attaquer(unite,monde,destX,destY))==1){
            return 2;
        }
        return 3;
    }
    return 0;
}

void viderMonde(Monde *monde) {
  viderUListe(monde->rouge);
  viderUListe(monde->bleu);
  initialiserPlateau(monde->plateau);
}

void viderUListe(UListe *uliste) {
  Unite *temp, *unite;
  temp = uliste->unites;
  if(temp != NULL) {
    while(temp->suiv != NULL) {
      unite = temp;
      temp = temp->suiv;
      free(unite);
    }
    free(uliste);
  }

}

void gererTour(Monde *monde) {
    
    ++(monde->tour);
    
    if (nombreUnite(*(monde->rouge)) > 0){
    gererTourJoueur(ROUGE, monde);
    }
    if (nombreUnite(*(monde->bleu)) > 0){
    gererTourJoueur(BLEU, monde);
        }

}

/* On place au click du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
void placerUnite(Monde *monde, UListe *uliste, char genre){
    int posX, posY;
  	MLV_wait_mouse(&posX, &posY);
    while (!placerAuMonde(creerUnite(genre, uliste), monde, posX/40, posY/40)){
      	MLV_wait_mouse(&posX, &posY);
    }
    
    dessinerplateau(*monde);

}

void placementParJoueur(Monde *monde, char couleur){
    UListe *uliste=getUListe(couleur,monde);

    placerUnite(monde,uliste,SERF);
    placerUnite(monde,uliste,SERF);
    placerUnite(monde,uliste,GUERRIER);
}

/* On gere les evenements au clic du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
void placementInitial(Monde *monde){
    char couleur='N';
    int posX, posY;
  	MLV_wait_mouse(&posX, &posY);
      if(posX<250 && posX>50 && posY<691 && posY>591){   
        reinitialiseInterface();
        couleur='R';
    
    }
     if(posX<650 && posX>450 && posY<691 && posY>591){   
         reinitialiseInterface();
        couleur='B';
          
   
    }
    while(couleur != ROUGE && couleur != BLEU) {
        MLV_wait_mouse(&posX, &posY);  
    if(posX<250 && posX>50 && posY<691 && posY>591){   
        reinitialiseInterface();
        couleur='R';
    
    }
     if(posX<650 && posX>450 && posY<691 && posY>591){   
         reinitialiseInterface();
        couleur='B';
          
   
    }
    
    }
     Pos(couleur);
     placementParJoueur(monde,couleur);
   
    if('B' == couleur){
        Pos('R');
    placementParJoueur(monde,ROUGE);
          
  } else {
        Pos('B');
    placementParJoueur(monde,BLEU);
  }
}

/* On gere les evenements au clic du joueur, il est aussi à exclure l'espace où le joueur ne peux pas cliquer*/ 
int arreterPartie(Monde monde){
    if (nombreUnite(*(monde.rouge)) > 0 && nombreUnite(*(monde.bleu)) > 0){
    arreterPartieGr();
    char c='N';
    int mouseX, mouseY;
    MLV_wait_mouse(&mouseX, &mouseY);  
    
     while(c=='N') 
    {
         
    if((mouseX<515 && mouseX>415 && mouseY<641 && mouseY>591)){
    return 0;
        }
    else if(mouseX<315 && mouseX>215 && mouseY<641 && mouseY>591) {
            c='O';
        return 1;

        } 
    else {
           
    MLV_wait_mouse(&mouseX, &mouseY); 
        }
}

}
    return 0;
    }


/* On crée dans cette fonction la fenêtre et on la dessine, le plateau et/ou l'interface sera redessinée par la suite dans les fonctions qui le necessitent plus haut. Dans gérer tour on s'occupe aussi de l'affichage de l'écran de fin en fonction du gagnant */ 
void gererPartie(void){
    Monde mondejeu;
    int arret = 0;
    
    char gagnant;
    initialiserMonde(&mondejeu);
    creerFenetre();
    
    quiCommence(); 
    dessinerplateau(mondejeu);
    placementInitial(&mondejeu);
    dessinerplateau(mondejeu);
    
    
    while( arret==0 && (nombreUnite(*(mondejeu.rouge)) > 0 && nombreUnite(*(mondejeu.bleu)) > 0)) {
    gererTour(&mondejeu);
    arret = arreterPartie(mondejeu);
    }


     if (nombreUnite(*(mondejeu.bleu)) <= 0)
      {   	gagnant='R';
            Fin(gagnant);
         MLV_wait_seconds(30);

	   MLV_free_window();
      } 
    
      if (nombreUnite(*(mondejeu.rouge)) <= 0) 
    {     gagnant='B';
            Fin(gagnant);
          
          MLV_wait_seconds(30);

	   MLV_free_window();
      }
    
         gagnant='N';
         Fin(gagnant);
         MLV_wait_seconds(30);

	   MLV_free_window();
        
      
    
    
}


