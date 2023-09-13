#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISCIPLINES 6
#define MAX_USERS 5
#define MAX_NOTES 3

typedef struct{
    char email[85];
    char password[85];
} User;

typedef struct{
    char nome[100];
    char prof[100];
    float hora_t, hora_c;
    float porc_hora, media, notaex;
} Disciplina;

User users[MAX_USERS] = {
    {"user1@mail.com", "password1"},
    {"user2@mail.com", "password2"},
    {"user3@mail.com", "password3"},
    {"user4@mail.com", "password4"},
    {"user5@mail.com", "password5"}
    };

Disciplina materia[MAX_USERS][MAX_DISCIPLINES];
float nota[MAX_USERS][MAX_NOTES][MAX_DISCIPLINES];
int currentUser = -1; // -1 indica que nenhum usuario esta logado
char login[50], senha[50];
int n, i, m, r, h, verifadm;
char ver_media[25];

//Prototipos...
int loginVerif(char *email, char *password); // função que verifica se o usuario digitado consta no banco de dados
void enter(); // apenas para o usuario ter tempo de ler
void preListMat(); // determina quantas materias listar
char* verMedia(); // determina se o usuario ta aprovado ou reprovado
void listMat(); // lista as materias 
void regMat(); // recebe os valores para registrar na materia
int containsAtSymbol(char *string); // verifica se o email digitado contem o caracter '@'
void cadNota(); // cadastra nota digitada pelo usuario
void matInsert(); // insere materias
void menuPrincipal(); // nome autoexplicativo
void cadUser(int g); // cadastro de usuarios
void adminMenu(); // nome autoexplicativo
void loginRequest(); // pede o login ou cadastro para o usuario
int hasMateria(); // verifica se o usuario atual contem alguma materia cadastrada
int checkEmail(char *string); // verifica se o email digitado ja foi cadastrado

int loginVerif(char *email, char *password){
    const char login_adm[] = "admin";
    const char senha_adm[] = "admin123";
    for (i = 0; i < MAX_USERS; i++)
    {
        if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0)
        {
            currentUser = i; // Defina o usuario atual após o login bem-sucedido
            return 1;        // Retorna 1 se o login for bem-sucedido
        }
    }
    if (strcmp(email, login_adm) == 0 && strcmp(password, senha_adm) == 0) {
        verifadm = 1;
        currentUser = MAX_USERS + 1;
        return 1;
    }
    return 0; // Retorna 0 se o login falhar
} // funcao de verificacao de login basico

void enter(){
    printf("\nAperte 'ENTER' para continuar\n");
    while (getchar() != '\n');      // Limpa o buffer, consumindo o '\n' anterior (se houver)
    getchar(); // Aguarda o usuario pressionar ENTER
    system("cls");
}

void preListMat(){
    do{
        system("cls");
        listMat();
        printf("Selecione uma opcao \n");
        printf("[1] para inserir notas\n");
        printf("[2] para sair\n");
        scanf("%d", &h);
        switch (h){
        case 1:
            cadNota();
            break;
        case 2:
            enter();
            break;
        default:
            printf("Dado invalido!\n");
            enter();
            break;
        }
    } while (h != 2);
}

char* verMedia() {
    float currentMedia = materia[currentUser][r].media;

    if (currentMedia >= 7) {
        return "Aprovado!";
    }
    else if (currentMedia < 5) {
        return "Reprovado!";
    }
    else {
        printf("Em Exame!\n");
        if (materia[currentUser][r].notaex == 0) {
            printf("Digite a nota do exame: ");
            scanf("%f", &materia[currentUser][r].notaex);
            currentMedia = (currentMedia + materia[currentUser][r].notaex) / 2;
        } else {
            printf("Nota do exame: %f\n", materia[currentUser][r].notaex);
        }

        return (currentMedia >= 5) ? "Aprovado!!" : "Reprovado!!";
    }
}

void listMat() {
    printf("Nome: %s\n", materia[currentUser][r].nome);
    printf("Professor: %s\n", materia[currentUser][r].prof);
    printf("Horas totais: %.2f\n", materia[currentUser][r].hora_t);
    printf("Horas ministradas: %.2f\n", materia[currentUser][r].hora_c);
    materia[currentUser][r].porc_hora = (materia[currentUser][r].hora_c / materia[currentUser][r].hora_t) * 100;
    printf("Porcentagem de aulas ministradas: %.2f%%\n", materia[currentUser][r].porc_hora);
    printf("Media da disciplina: %.2f\n", materia[currentUser][r].media);
    if (nota[currentUser][0][r] && nota[currentUser][1][r] && nota[currentUser][2][r]) {
        printf("Voce esta %s\n", verMedia());
    }
}

void regMat() {
    printf("---------------------------------------\n");
    printf("--------Registro de disciplinas--------\n");
    printf("---------------------------------------\n");
    printf("---N da materia a ser registrada: %d---\n", i + 1);
    printf("---------------------------------------\n");
    printf("Digite o nome da disciplina n %d: ", i + 1);
    scanf(" %99[^\n]", materia[currentUser][i].nome); // Lê ateh 99 caracteres ou ateh encontrar uma nova linha.
    printf("Digite o nome do Professor: ");
    scanf(" %99[^\n]", materia[currentUser][i].prof); // Mesmo procedimento para o nome do professor.
    printf("Digite a quantidade de aulas totais da disciplina: ");
    scanf("%f", &materia[currentUser][i].hora_t);
    materia[currentUser][i].hora_t = (materia[currentUser][i].hora_t * 5) / 6;
    printf("Digite a quantidade de aulas ja realizadas da disciplina: ");
    scanf("%f", &materia[currentUser][i].hora_c);
    materia[currentUser][i].hora_c = (materia[currentUser][i].hora_c * 5) / 6;
    printf("Disciplina cadastrada com sucesso!\n");
    enter();
}

void cadNota() {
    int l = 0;

    do {
        system("cls"); // limpatela
        for (i = 0; i < MAX_NOTES; i++) {
            printf("Nota %d: %.1f\n", i + 1, nota[currentUser][i][r]);
        }

        printf("Deseja inserir qual nota? \n");
        printf("Digite [4] para sair\n");
        scanf("%d", &l);
        if (l >= 1 && l < 4) {
            printf("Digite a nota %d: ", l);
            scanf("%f", &nota[currentUser][l - 1][r]); // Os índices em C comecam em 0
            printf("Nota inserida: %.1f\n", nota[currentUser][l - 1][r]);
            materia[currentUser][r].media = (nota[currentUser][0][r] + nota[currentUser][1][r] + nota[currentUser][2][r]) / 3;
            printf("Media: %.1f\n", materia[currentUser][r].media);
            enter();
        } else if (l == 4) {
            enter();
            break;
        } else {
            printf("Opcao Invalida!\n");
            enter();
        }
    } while (l != 4);
}

void loginRequest() {
    char inputUsername[50];
    char inputPassword[50];
    int escolha;
    static int atual = 0;
    printf("Deseja fazer login ou cadastrar? \n[1] Para login \n[2] Para cadastro\n");
    scanf("%d", &escolha);
    switch (escolha){
    case 1:
        do {
            printf("Digite seu nome de usuario: ");
            scanf("%49s", inputUsername); // Lê o nome de usuario do teclado
            printf("Digite sua senha: ");
            scanf("%49s", inputPassword); // Lê a senha do teclado
            if (loginVerif(inputUsername, inputPassword)) {
                printf("Login bem-sucedido!\n");
                enter();
                if(verifadm != 1 && !hasMateria()){
                matInsert();
                }
                menuPrincipal();
            } else {
                printf("Nome de usuario ou senha incorretos!\n");
            }
        } while((loginVerif(inputUsername,inputPassword)) == 0);
        break;

    case 2:
        if (atual <= MAX_USERS) {
            cadUser(atual);
            atual++;
            loginRequest();
        } else {
            printf("Numero maximo de usuarios cadastrados atingido, \npor favor contate o administrador para resolver\n");
            loginRequest();
        }
        break;

    default:
        printf("Escolha invalida!!\n");
        loginRequest();
        break;
    }
}

int checkEmail(char *string) {
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(string,users[i].email)) {
            return 1; //retorna 1 quando os emails forem iguais
            break; //caso seja igual, para o for
        } else {
            return 0; //retorna 0 se nao encontrar um email igual
        }
    }
}

void cadUser(int g) {
    char inputEmail[85];
    char inputPass[85];
    int cademail = 0;
    do {
        printf("Digite o email para realizar o cadastro: \n");
        scanf(" %99s", inputEmail);
        if (!containsAtSymbol(inputEmail) || strlen(inputEmail) > 80 || !checkEmail(inputEmail)) {
            printf("Por favor insira um email valido!\nProvavelmente, este email ja foi cadastrado ou nao contem o simbolo '@'\n");
            cademail = 0;
        } else { cademail = 1; };
    } while (cademail != 1);
    do {
        printf("Digite a senha: \n");
        scanf(" %99s", inputPass);
        if (strlen(inputPass) < 8 || strlen(inputPass) > 80) {
            printf("Insira uma senha valida! \nContendo no minimo 8 e no maximo 80 caracteres.\n");
        }
    } while (strlen(inputPass) < 8 || strlen(inputPass) > 80);
    strcpy(users[g].email, inputEmail);
    strcpy(users[g].password, inputPass);
    enter();
}

void adminMenu() {
    printf("Codigo de administrador encontrado\n");
    do {
        printf("Deseja inserir novo usuario ou modificar algum? \n");
        printf("[1] Sim\n");
        printf("[0] Nao\n");
        scanf("%d", &m);
        switch (m) {
        case 1:
            for (i = 0; i < MAX_USERS; i++)
            {
                printf("Usuario %d: %s\n", i + 1, users[i].email);
            }
            printf("Otimo! Qual usuario deseja inserir ou modificar?\n");
            scanf("%d", &r);
            printf("O email do Usuario registrado eh: %s\n", users[r-1].email);
            cadUser(r - 1);
            printf("Usuario atualizado com sucesso!\n");
            printf("email do Usuario: %s\n", users[r-1].email);
            printf("A senha eh %s\n", users[r-1].password);
            enter();
            system("cls");  // limpatela
            break;

        case 0:
            break;

        default:
            printf("Opcao invalida!\n");
            enter();
            break;
        }
    } while (m != 0);
}

int containsAtSymbol(char *string) {
    return strchr(string, '@') != NULL;  
    // Retorna 1 (verdadeiro) se '@' estiver na string, caso contrario retorna 0 (falso)
}

int hasMateria() {
    for (i = 0; i < MAX_DISCIPLINES; i++) {
        if (strlen(materia[currentUser][i].nome) > 0) {
            return 1;  // Retorna 1 se encontrar alguma materia
        }
    }
    return 0;  // Retorna 0 se nao encontrar nenhuma materia
}

void matInsert() {      
    do {
        printf("Deseja inserir quantas disciplinas? (Max: 6)\n");
        scanf("%d", &h);
        if (h < 1 || h > MAX_DISCIPLINES) {
                printf("Por favor insira um numero entre 1 e 6.\n");
            }
        system("cls");
    } while (h < 1 || h > MAX_DISCIPLINES);
    for (i = 0; i < h; i++) {
        regMat();
    }
}

void menuPrincipal() {
    const int EXIT_CODE = 14323;  // Usando uma constante em vez de um numero magico
    m = 0;
    while (m != EXIT_CODE) {
        printf("Lista de materias cadastradas: \n");
        for (i = 0; i < 6; i++) {
            printf("%d - %s\n", i + 1, materia[currentUser][i].nome);
        }
        printf("Digite o numero da materia para obter mais detalhes.\n");
        printf("Caso queira fazer logoff, digite [7]\n");
        printf("Caso queira adicionar outra materia, digite [8]\n");
        printf("Digite [9] para sair\n");
        if (verifadm == 1) {
            printf("Para acessar o menu de administrador digite [6373]\n");
        }
        scanf("%d", &r);
        if (r >= 1 && r <= 6) {
            r = r - 1;  // Ajuste do índice
            preListMat();
        } else {
            switch (r) {
                case 6373:
                    system("cls");
                    adminMenu();
                    break;

                case 9:
                    printf("Obrigado por utilizar o Programa!!\n");
                    printf("Finalizando...\n");
                    enter();
                    m = EXIT_CODE;
                    break;

                case 8:
                    printf("Digite qual materia deseja inserir ou modificar: \n");
                    scanf("%d", &i);
                    i--;
                    system("cls");
                    regMat();
                    break;

                case 7:
                    printf("Fazendo logoff...\n");
                    enter();
                    verifadm = 0;
                    m = EXIT_CODE;
                    loginRequest();
                    break;

                default:
                    printf("Opcao invalida!!\n");
                    enter();
                    break;
            }
        }
    }
}

int main() {
    loginRequest();
    return 0;
}