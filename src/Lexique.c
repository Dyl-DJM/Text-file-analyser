/* Auteur : Dylan DE JESUS
 * Date creation : 22-02-2021
 * Date modification : 12-03-2021*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TAILLE_MAX 16   /*Taille maximum definie d'un mot*/
#define TAILLE 513  /*Taille d'une chaîne à 512 caractères sans \0*/


typedef struct noeud{
    char lettre;         /* étiquette du nœud */
    struct noeud *fils;   /* adresse du fils  */
    struct noeud *freregauche;   /* adresse du fils gauche */
    struct noeud *freredroit;   /* adresse du fils droit */
}Noeud, *Arbre;


/*Cette fonction renvoie l'addresse du noeud ayant pour étiquette la valeur 'val'*/

Arbre AlloueNoeud(char val){
    Arbre tmp;

    /*Allocation dynamique de la mémoire*/
    tmp = (Noeud*)malloc(sizeof(Noeud));

    /*Si l'allocation ne s'est pas faite on retourne NULL*/
    if(NULL == tmp)
        return tmp;

    /*Sinon on affecte une vlaeur au champ 'lettre' et les freres sont tous NULL*/
    if(NULL != tmp){
        tmp->lettre = val;
        tmp->fils = NULL;
        tmp->freregauche = NULL;
        tmp->freredroit = NULL;
    }
    /*Retourne l'adresse du noeud*/
    return tmp;
}


/*Cette fonction permet la libération memoire des noeuds de l'arbre 'lexique'*/

void libereArbre(Arbre lexique){

    if(lexique != NULL){
        libereArbre(lexique->freregauche);
        libereArbre(lexique->fils);
        libereArbre(lexique->freredroit);

        /*Libere noeud*/
        free(lexique);
    }
}


/*Cette fonction renvoie 1 si tout s'est bien passé
 * Elle permet  de copier un tableau 'tab2' dans 'tab' en supprimant le
 * le premier caractère de 'tab2'*/

int enlevePremCar(char tab[],char tab2[], int taille){
    int i;

    for(i = 1; i < taille; i++){
        tab[i - 1] = tab2[i];
    }
    return 1;
}


/*Cette fonction permet l'ajout d'une lettre 'lettre' dans une chaîne de caractères 'orefixe'*/
void ajouteLettre(char prefixe[], char lettre){
    int i;
    i = 0;

    /*On s'assure que le tableau prefixe est différent de null*/
    assert(prefixe != NULL);
    assert(strlen(prefixe) < TAILLE);

    /*Tant que l(on arrive pas à la fin de la chaîne on incrémente l'indice*/
    while(prefixe[i] != '\0'){
        i++;
    }
    prefixe[i] = lettre;
    prefixe[i+1] = '\0';
}


/*Cette fonction permet l'affichage en sortie standard du menu*/

void AfficheMenu(){
    printf("l) Affiche les mots du lexique du fichier en ordre alphabetique\n");
    printf("s) Sauvegarde l'affichage en ordre alphabetique du lexique du fichier\n");
    printf("r) Recherche un mot \n");
    printf("S) Sauvegarde l'affichage en ordre prefixe du lexique du fichier\n");
    printf("q) Quitter\n");
}


/* Cette fonction permet l'affichage en sortie standard de la liste des mots dans l'ordre alphabetique
 * Elle parcourt les différents noeuds et leurs fils
 * et affiche à l'aide de la chaîne 'radical' de taille 'taille' à laquelle elle ajoute au fur et à mesure des caractères.*/

void AfficheOrdreAlpha(Arbre lexique,char radical[], int taille){

    if(lexique == NULL){
        return;
    }

    AfficheOrdreAlpha(lexique->freregauche, radical, taille);

    /*Marqueur de fin de mot on ecrit le radical engendré recursivement*/
    if (lexique->lettre == '\0'){
        printf("%s ", radical);
    }
    else {
        /*On écrase l'ancien marqueur fin de mot et on rajoute la lettre puis on remet le marqueur fin de mot àla fin*/
        radical[taille] = lexique->lettre;
        radical[taille + 1] = '\0';
        AfficheOrdreAlpha(lexique->fils, radical, taille + 1);
    }
    AfficheOrdreAlpha(lexique->freredroit, radical, taille);

}


/*Cette fonction sauvegarde l'affichage en ordre alphabetique de l'arbre 'lexique' dans le fichier 'sauvegarde'*/

void SauvOrdreAlpha(Arbre lexique, char radical[], int taille ,FILE* sauvegarde){

    if(lexique == NULL){
        return;
    }

    SauvOrdreAlpha(lexique->freregauche, radical, taille, sauvegarde);

    if (lexique->lettre == '\0'){
        /*Affiche le radical*/
        fprintf(sauvegarde, "%s ", radical);
    }

    else {
        radical[taille] = lexique->lettre;
        radical[taille + 1] = '\0';
        SauvOrdreAlpha(lexique->fils, radical, taille + 1, sauvegarde);
    }
    SauvOrdreAlpha(lexique->freredroit, radical, taille, sauvegarde);
}


/*Fonction permettant la sauvegarde du fichier 'sauvegarde' au format '.DIC' des mots de l'arbre 'lexique' */

void SauvFichier(Arbre lexique, FILE* sauvegarde){

    if (NULL == lexique){
        fprintf(sauvegarde, " ");
        return ;
    }
    if(lexique->lettre == '\0'){
        /*On remplace ce symbole par un saut de ligne*/
        fprintf(sauvegarde ,"\n");
    }
    else{
        /*Affichage du caractere*/
        fprintf(sauvegarde ,"%c", lexique->lettre);
    }

    SauvFichier(lexique->freregauche, sauvegarde);
    SauvFichier(lexique->fils, sauvegarde);
    SauvFichier(lexique->freredroit, sauvegarde);
}


/*Cette fonction renvoie 1 si le mot 'mot' de taille 'taille' est présent dans l'arbre 'lexique' sinon renvoie 0*/

int RechercheMot(Arbre lexique, char mot[], unsigned taille){
    char nouv_mot[TAILLE_MAX];

    /*On s'assure que le mot est différent de NULL*/
    assert(mot != NULL);

    /*mot vide donc toutes ses lettre on été trouvées dans l'arbre*/
    if(taille == 0){
        return 1;
    }

    /*L'arbre est NULL on renvoie 0*/
    if(lexique == NULL){
        return 0;
    }

    /*la première lettre du mot est celle du noeud pointée*/
    if(lexique->lettre == mot[0]){

        /*On enleve la première lettre pour tester les suivantes dans l'arbre*/
        enlevePremCar(nouv_mot, mot, taille);
        return RechercheMot(lexique->fils, nouv_mot, taille - 1);
    }

    /*la première lettre du mot est plus petite que celle du noeud pointée*/
    if(lexique->lettre > mot[0]){
        return RechercheMot(lexique->freregauche, mot, taille);
    }

    /*la première lettre du mot est plus grande que celle du noeud pointée*/
    if(lexique->lettre < mot[0]){
        return RechercheMot(lexique->freredroit, mot, taille);
    }
    return 0;
}


/*Cette fonction permet d'incrémenter récursivement et par adresse la variable 'entier' pour chaque
pour chaque mot de l'arbre 'lexique'*/

void NbMots(Arbre lexique, int *entier){

    /*Arbre null on renvoie aucun type*/
    if(lexique == NULL){
        return ;
    }

    /*Un mot = un marqueur de fin de mot, on incrémente*/
    if(lexique->lettre == '\0')
        *entier = *entier + 1;

    /*Parcours des noeuds de l'arbre*/
    NbMots(lexique->freregauche, entier);
    NbMots(lexique->fils, entier);
    NbMots(lexique->freredroit, entier);

}


/*Cette fonction renvoie 1 si l'ajout à l'arbre '*lexique' du mot 'mot' de taille 'taille' a bien été fait sinon renvoie 0*/

int AjouteArbre(Arbre *lexique, char mot[], int taille){
    char nouv_mot[TAILLE_MAX];
    Arbre tmp = *lexique;

    assert(taille >= 0);

    /*On a inséré toutes les lettres du mot*/
    if(taille == 0)
        return 1;

    /*Arbre vide on alloue la première lettre*/
    if(tmp == NULL){
        *lexique = AlloueNoeud(mot[0]);

        /*Le nouveau mot est l'ancien moins la première lettre*/
        enlevePremCar(nouv_mot, mot, taille);
        AjouteArbre(&((*lexique)->fils), nouv_mot, taille-1);
    }

    /*On recherche la place de la lettre à insérer*/

    if(mot[0] > (*lexique)->lettre){ /*Plus grand dans la table ASCII signifie plus loin dans l'alphabet*/
        AjouteArbre(&((*lexique)->freredroit), mot, taille);
    }

    if(mot[0] < (*lexique)->lettre){ /*Plus petit dans la table ASCII signifie avant dans l'alphabet*/
        AjouteArbre(&((*lexique)->freregauche), mot, taille);
    }

    if((mot[0]) == (*lexique)->lettre){ /*l'étiquette existe déjà*/
        /*Le nouveau mot est l'ancien moins la première lettre*/
        enlevePremCar(nouv_mot, mot, taille);
        AjouteArbre(&((*lexique)->fils), nouv_mot, taille-1);
    }

    return 0;
}


/*Cette fonction renvoie 1 une fois l'ajout de tous les mots du fichier 'in' dans l'arbre 'lexique'*/

int FichierEnArbre(Arbre *lexique, FILE *in){
    char mot[TAILLE];

    /*Tant que l'on est pas à la fin du fichier on ajoute mot pas mot à l'arbre*/
    while (EOF != fscanf(in, " %513[a-zA-Zà-ö]%*[^a-zA-Zà-ö]", mot)){   /*Un mot est composé de lettres uniquement*/
        AjouteArbre(lexique, mot, strlen(mot) + 1);
    }
    return 1;
}



int main(int argc,char *argv[]){
    int i, nb_mots;
    char prefixe[TAILLE] = {'\0'};

    /*Arbre dans lequel on mettra le lexique*/
    Arbre test = NULL;

    FILE* fichier_init;
    FILE* fichier_sauv;


    /*Ouverture du fichier donné*/
    fichier_init = fopen(argv[argc - 1],"r");


    /*Gestion de la ligne de commande*/

    /*Il y a moins de 3 arguments --> Erreur*/
    if(argc < 3){

        if(argc > 1){

            if(argv[1][0] == '-'){
                fprintf(stderr, "Veuillez entrer un fichier dans la ligne de commande !\n");
                return 0;
            }
            else{
                if(fichier_init != NULL){
                    AfficheMenu();
                    return 0;
                }
            }
        }
    }

    /*On regarde si le fichier s'est bien ouvert*/
    if(fichier_init == NULL){
        fprintf(stderr, "Erreur d'ouverture de fichier !\n");
        return 0;
    }
    else{
        /*Il s'est bien ouvert on va pouvoir traduire le fichier en arbre lexicale*/
        FichierEnArbre(&test, fichier_init);
    }

    /*On choisit quelles options traiter selon les arguments de la ligne de commandes
     * argc - 2 représente le nombre d'options (Nombre total d'arguments dans la ligne de commande - (Executable + Fichier))*/
    for(i = 1; i < argc - 1; i++){

        if(argv[i][0]=='-'){

            switch(argv[i][1]){

                case 'l':{
                    /*Affichage en ordre alphabetique*/

                    printf("Affichage du fichier '%s' en ordre alphabetique :\n\n   -", argv[argc - 1]);
                    strcpy(prefixe, "");
                    AfficheOrdreAlpha(test, prefixe, 0);
                    printf("\n\n");

                    break;
                }
                case 's':{
                    /*Sauvegarde dans l'ordre alphabetique .L*/

                    fichier_sauv = fopen(strcat(argv[argc - 1],".L"),"w");

                    if(fichier_sauv == NULL){
                        fprintf(stderr, "Erreur d'ouverture du fichier de suavegarde '%s'", strcat(argv[1],".DIC"));
                        libereArbre(test);
                        return 0;
                    }
                    else{
                        printf("Sauvegarde du lexique en ordre alphabetique en format .L....effectuée\n\n");
                    }
                    strcpy(prefixe, "");
                    SauvOrdreAlpha(test, prefixe, 0, fichier_sauv);
                    break;
                }
                case 'r':{
                    /*Recherche d'un mot*/

                    printf("Recherche du mot '%s' dans le fichier :\n\n   -", argv[i + 1]);

                    if(RechercheMot(test, argv[i + 1], strlen(argv[i + 1])) == 0){
                        printf("Le mot '%s' n'est pas présent dans le fichier\n\n", argv[i + 1]);
                    }
                    else{
                        printf("Le mot '%s' est présent dans le fichier\n\n", argv[i + 1]);
                    }
                    break;
                }
                case 'S':{
                    /*Cas de la sauvegarde .DIC*/

                    fichier_sauv = fopen(strcat(argv[argc - 1],".DIC"),"w");

                    if(fichier_sauv == NULL){
                        fprintf(stderr, "Erreur d'ouverture du fichier de suavegarde '%s'", strcat(argv[1],".DIC"));
                        libereArbre(test);
                        return 0;
                    }
                    else{
                        printf("Sauvegarde du lexique en format .DIC....effectuée\n\n");
                    }

                    SauvFichier(test, fichier_sauv);
                    break;
                }
                case 'o':{
                    /*Affichage du nombre de mots dans le fichier*/
                    nb_mots = 0;
                    NbMots(test, &nb_mots);
                    printf("Le fichier contient %d mots\n\n", nb_mots);
                }
                default:{
                    break;
                }
            }
        }
    }

    /*On libere la memoire*/

    libereArbre(test);
    return 0;
}
