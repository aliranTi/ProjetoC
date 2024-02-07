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



void enter(); // apenas para o usuario ter tempo de ler
void loginRequest(); // pede o login ou cadastro para o usuario
void cadUser(int g); // cadastro de usuarios
int loginVerif(char *email, char *password); // função que verifica se o usuario digitado consta no banco de dados
int containsAtSymbol(char *string); // verifica se o email digitado contem o caracter '@'
void adminMenu(); // nome autoexplicativo
int checkEmail(char *string); // verifica se o email digitado ja foi cadastrado
int hasMateria(); // verifica se o usuario atual contem alguma materia cadastrada
void matInsert(); // insere materias
void regMat(int i); // recebe os valores para registrar na materia
void menuPrincipal(); // nome autoexplicativo
void preListMat(); // determina quantas materias listar
void listMat(int atualMat); // lista as materias 
void cadNota(int atualMat); // cadastra nota digitada pelo usuario
char* verMedia(int atualMat); // determina se o usuario ta aprovado ou reprovado