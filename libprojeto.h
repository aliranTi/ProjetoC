#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_DISCIPLINES 6
#define MAX_NOTES 3
#define MAX_LENGHT 100
#define DOC_SIZE 12


struct disciplina{
    char nome[100];
    char prof[100];
    float hora_t, hora_c;
    float porc_hora, media, notaex;
    float nota[MAX_NOTES];
    struct disciplina * prox;
};

struct user{
    char matricula[DOC_SIZE];
    char email[85];
    char password[85];
    struct disciplina *materiaHead;
    struct user * proxUser;
};

typedef struct user User;
typedef struct disciplina Disciplina;

void freeMat(Disciplina *materia);
int checarMateria(Disciplina * materia, char * string);
Disciplina * alocarMateria();
Disciplina *buscaBinariaDisciplina(Disciplina *head, const char *nome);
void menuPrincipal(User * users);
int hasMateria(Disciplina * disciplina);
void matInsert(Disciplina ** materia);
void regMat(Disciplina * disciplina);
void preListMat(Disciplina * materia);
void cadNota(Disciplina * materia);
void listMat(Disciplina * materia);
char* verMedia(Disciplina * materia);
void imprimirMateria(Disciplina materiaAtual);
void loginRequest();
void deletarUsuario(User * users,char * matricula);
User * loginVerif(char *email, char *password);
User * checkUser(char * email, char * password, User * users);
void cadUser();
int checkEmail(User * users,char *string);
int checkMat(User *users,char * matInput);
User * recuperarUsers();
void freeUsers(User *user);
void iniciaListaMateria(User *user);
User * alocarUser();
int containsAtSymbol(char *string);
void erroArquivo();
char *passaMaiuscula(char * palavra);
int verificaCaracteresEspeciais(char *string);
int verificaNumero(char *string);
int checkName(char * string);
int countUsers();
void checkFile();
void enter();
void saveUserFile(User * users);