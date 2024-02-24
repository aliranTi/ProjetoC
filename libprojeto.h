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
    char email[85];
    char password[85];
    struct disciplina *materia;
};

typedef struct user User;
typedef struct disciplina Disciplina;

User *alocarUser(int quant);
User cadUser(User *users, int atual); // cadastro de usuarios
User *recuperarUsers();
void testar();
void imprimirMateria(User *users, int *currentUser, int *atualMat);
void alocarMateria(User *users, int quant);
void enter(); // apenas para o usuario ter tempo de ler
void loginRequest(User *users, int *currentUser, int *verifadm); // pede o login ou cadastro para o usuario
void matInsert(User *users, int *currentUser); // insere materias
void regMat(Disciplina * disciplina,int i); // recebe os valores para registrar na materia
void menuPrincipal(User *users, int *currentUser, int *verifadm); // nome autoexplicativo
void preListMat(User *users, int *currentUser, int *atualMat); // determina quantas materias listar
void listMat(User *users, int *currentUser, int *atualMat); // lista as materias 
void cadNota(User *users, int *currentUser, int *atualMat); // cadastra nota digitada pelo usuario
void adminMenu(User *users); // nome autoexplicativo
void criarUsers(User *users); // Cria os usuarios
void liberarMateria(User *users);
void saveUserFile(User * users);
char* verMedia(User *users, int *currentUser, int *atualMat); // determina se o usuario ta aprovado ou reprovado
int hasMateria(User *users, int *currentUser); // verifica se o usuario atual contem alguma materia cadastrada
int containsAtSymbol(char *string); // verifica se o email digitado contem o caracter '@'
int loginVerif(User *users, char *email, char *password, int *currentUser, int *verifadm); // função que verifica se o usuario digitado consta no banco de dados
int checkEmail(User *users, char *string); // verifica se o email digitado ja foi cadastrado
