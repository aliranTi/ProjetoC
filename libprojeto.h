#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISCIPLINES 6
#define MAX_USERS 5
#define MAX_NOTES 3

struct disciplina{
    char nome[100];
    char prof[100];
    float hora_t, hora_c;
    float porc_hora, media, notaex;
    float nota[MAX_NOTES];
};

struct user{
    int matricula;
    char email[85];
    char password[85];
    struct disciplina *materia;
};

typedef struct user User;
typedef struct disciplina Disciplina;

int hasMateria(User *users);
void matInsert(User *users, int * h);
void regMat(Disciplina * disciplina,int i);
void preListMat(Disciplina * materia);
void cadNota(Disciplina * materia);
void listMat(Disciplina * materia);
char* verMedia(Disciplina * materia);
void menuPrincipal(User * users, int * quant_users, int *verifadm, int * quant_materias);
void imprimirMateria(Disciplina materiaAtual);
User * alocarUser(int quant);
void alocarMateria(User *users, int quant);
void enter();
int containsAtSymbol(char *string);
int countUsers();
User * recuperarUsers(int * quant_users);
int checkMat(User *users,int matInput, int * quant_users);
int checkEmail(User * users,char *string, int * quant_users);
void freeUser(User * user);
void cadUser(int * quant_users);
User * checkUser(char * email, char * password, User * users);
User * loginVerif(char *email, char *password, int *verifadm, int * quant_users);
void loginRequest(int * quant_users, int *verifadm);