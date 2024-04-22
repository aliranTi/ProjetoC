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

// Function prototypes
int checarMateria(Disciplina * materia, char * string);
Disciplina * alocarMateria();
Disciplina * buscaBinariaDisciplina(Disciplina *head, const char *nome);
void saveDisciplinaFile(Disciplina *materia);
void iniciaListaMateria(Disciplina **materiaHead);
Disciplina * excluirMateria(Disciplina * materia,char * string);
void cadDisciplina(Disciplina ** materia);
void regMat(Disciplina * disciplina);
void preListMat(Disciplina * materia);
void cadNota(Disciplina * materia);
void listMat(Disciplina * materia);
char* verMedia(Disciplina * materia);
void imprimirMateria(Disciplina materiaAtual);
int containsAtSymbol(char *string);
void erroArquivo();
char *passaMaiuscula(char *palavra);
int verificaCaracteresEspeciais(char *string);
int verificaNumero(char *string);
int checkName(char *string);
int countUsers();
void checkFile();
void enter();
void saveUserFile(User *users);
void menuPrincipal(User *users);
int hasMateria(Disciplina *materia);
int existeDisciplina(Disciplina *materia, const char *nomeDisciplina);
User *alocarUser();
void freeUsers(User *user);
void matInsert(Disciplina **materia);
Disciplina * recuperarDisciplina();
User *recuperarUsers();
int checkMat(User *users, char *matInput);
int checkEmail(User *users, char *string);
void cadUser();
User *checkUser(char *email, char *password, User *users);
User *loginVerif(char *email, char *password);
void deletarUsuario(User *users, char *matricula);
void loginRequest();