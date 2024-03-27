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
void matInsert(User *users, unsigned int * h);
Disciplina regMat(unsigned int i);
void preListMat(Disciplina * materia);
void cadNota(Disciplina * materia);
void listMat(Disciplina * materia);
char* verMedia(Disciplina * materia);
void menuPrincipal(User * users, unsigned int * quant_users, int *verifadm, unsigned int * quant_materias);
void imprimirMateria(Disciplina materiaAtual);
User * alocarUser(unsigned int quant);
void alocarMateria(User *users, unsigned int quant);
void enter();
void checkFile();
int containsAtSymbol(char *string);
void freeUsers(User * users);
void saveUserFile(User * users);
int countUsers();
User * recuperarUsers(unsigned int * quant_users);
int checkMat(User *users,int matInput, unsigned int * quant_users);
int checkEmail(User * users,char *string, unsigned int * quant_users);
void cadUser(unsigned int * quant_users);
User * checkUser(char * email, char * password, User * users);
User * loginVerif(char *email, char *password, int *verifadm, unsigned int * quant_users);
void loginRequest(unsigned int * quant_users, int *verifadm);
void adminMenu(unsigned int * quant_users);
void deletarUsuario(User * users,User userExcluido, unsigned int * quant_users);