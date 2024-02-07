#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISCIPLINES 6
#define MAX_USERS 5
#define MAX_NOTES 3

typedef struct{
    char nome[100];
    char prof[100];
    float hora_t, hora_c;
    float porc_hora, media, notaex;
    float nota[MAX_NOTES];
} Disciplina;

typedef struct{
    char email[85];
    char password[85];
    Disciplina materia[MAX_DISCIPLINES];
} User;

User users[MAX_USERS];

// Disciplina materia[MAX_USERS][MAX_DISCIPLINES];
// float nota[MAX_USERS][MAX_NOTES][MAX_DISCIPLINES];
int currentUser = -1; // -1 indica que nenhum usuario esta logado
int verifadm;

//Prototipos...
void enter(); // apenas para o usuario ter tempo de ler
void loginRequest(); // pede o login ou cadastro para o usuario
void cadUser(int g); // cadastro de usuarios
int loginVerif(char *email, char *password); // função que verifica se o usuario digitado consta no banco de dados
int containsAtSymbol(char *string); // verifica se o email digitado contem o caracter '@'
int checkEmail(char *string); // verifica se o email digitado ja foi cadastrado
int hasMateria(); // verifica se o usuario atual contem alguma materia cadastrada
void matInsert(); // insere materias
void regMat(int i); // recebe os valores para registrar na materia
void menuPrincipal(); // nome autoexplicativo
void preListMat(); // determina quantas materias listar
void listMat(int atualMat); // lista as materias 
void cadNota(int atualMat); // cadastra nota digitada pelo usuario
char* verMedia(int atualMat); // determina se o usuario ta aprovado ou reprovado
void adminMenu(); // nome autoexplicativo

void enter(){
    printf("\nAperte 'ENTER' para continuar\n");
    while (getchar() != '\n');      // Limpa o buffer, consumindo o '\n' anterior (se houver)
    getchar(); // Aguarda o usuario pressionar ENTER
    system("cls");
}

void loginRequest(){
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
                if(!verifadm && !hasMateria()){
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

void cadUser(int atual){ 
    char inputEmail[85];
    char inputPass[85];
    int cademail = 0;
    do {
        printf("Digite o email para realizar o cadastro: \n");
        scanf(" %99s", inputEmail);
        if (!containsAtSymbol(inputEmail) || strlen(inputEmail) > 80 || !checkEmail(inputEmail)) {
            printf("Por favor insira um email valido!\nProvavelmente, este email ja foi cadastrado ou eh invalido!\n");
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
    strcpy(users[atual].email, inputEmail);
    strcpy(users[atual].password, inputPass);
    enter();
}

int loginVerif(char *email, char *password){ 
    const char login_adm[] = "admin";
    const char senha_adm[] = "admin123";
    for (int i = 0; i < MAX_USERS; i++)
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

int containsAtSymbol(char *string){ 
    return strchr(string, '@') != NULL;  
    // Retorna 1 (verdadeiro) se '@' estiver na string, caso contrario retorna 0 (falso)
}

int checkEmail(char *string){ 
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(string,users[i].email)) {
            return 1; //retorna 1 quando os emails forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar um email igual
}

int hasMateria(){ // Letícia
    for (int i = 0; i < MAX_DISCIPLINES; i++) {
        if (strlen(users[currentUser].materia[i].nome) > 0) {
            return 1;  // Retorna 1 se encontrar alguma materia
        }
    }
    return 0;  // Retorna 0 se nao encontrar nenhuma materia
}

void matInsert(){
    int h;
    do {
        printf("Deseja inserir quantas disciplinas? (Max: 6)\n");
        scanf("%d", &h);
        if (h < 1 || h > MAX_DISCIPLINES) {
                printf("Por favor insira um numero entre 1 e 6.\n");
            }
        system("cls");
    } while (h < 1 || h > MAX_DISCIPLINES);
    for (int i = 0; i < h; i++) {
        regMat(i);
    }
}

void regMat(int i){
    printf("---------------------------------------\n");
    printf("--------Registro de disciplinas--------\n");
    printf("---------------------------------------\n");
    printf("---N da materia a ser registrada: %d---\n", i + 1);
    printf("---------------------------------------\n");
    printf("Digite o nome da disciplina n %d: ", i + 1);
    scanf(" %99[^\n]", users[currentUser].materia[i].nome); // Lê ateh 99 caracteres ou ateh encontrar uma nova linha.
    printf("Digite o nome do Professor: ");
    scanf(" %99[^\n]", users[currentUser].materia[i].prof); // Mesmo procedimento para o nome do professor.
    printf("Digite a quantidade de aulas totais da disciplina: ");
    scanf("%f", &users[currentUser].materia[i].hora_t);
    users[currentUser].materia[i].hora_t = (users[currentUser].materia[i].hora_t * 5) / 6;
    printf("Digite a quantidade de aulas ja realizadas da disciplina: ");
    scanf("%f", &users[currentUser].materia[i].hora_c);
    users[currentUser].materia[i].hora_c = (users[currentUser].materia[i].hora_c * 5) / 6;
    printf("Disciplina cadastrada com sucesso!\n");
    enter();
}

void menuPrincipal(){
    const int EXIT_CODE = 14323;  // Usando uma constante em vez de um numero magico
    int m = 0;
    int r;
    while (m != EXIT_CODE) {
        printf("Lista de materias cadastradas: \n");
        for (int i = 0; i < 6; i++) {
            printf("%d - %s\n", i + 1, users[currentUser].materia[i].nome);
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
            r = r - 1;  // Ajuste do índice de linguagem humana para linguagem de maquina
            preListMat(r);
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
                    scanf("%d", &r);
                    r--;
                    system("cls");
                    regMat(r);
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

void preListMat(int atualMat){ 
    int h;
    do{
        system("cls");
        listMat(atualMat);
        printf("Selecione uma opcao \n");
        printf("[1] para inserir notas\n");
        printf("[2] para sair\n");
        scanf("%d", &h);
        switch (h){
        case 1:
            cadNota(atualMat);
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

void listMat(int atualMat){
    printf("Nome: %s\n", users[currentUser].materia[atualMat].nome);
    printf("Professor: %s\n", users[currentUser].materia[atualMat].prof);
    printf("Horas totais: %.2f\n", users[currentUser].materia[atualMat].hora_t);
    printf("Horas ministradas: %.2f\n", users[currentUser].materia[atualMat].hora_c);
    users[currentUser].materia[atualMat].porc_hora = (users[currentUser].materia[atualMat].hora_c / users[currentUser].materia[atualMat].hora_t) * 100;
    printf("Porcentagem de aulas ministradas: %.2f%%\n", users[currentUser].materia[atualMat].porc_hora);
    printf("Media da disciplina: %.2f\n", users[currentUser].materia[atualMat].media);
    if (users[currentUser].materia[atualMat].nota[0] && users[currentUser].materia[atualMat].nota[1] && users[currentUser].materia[atualMat].nota[2]) {
        printf("Voce esta %s\n", verMedia(atualMat));
    }
}

void cadNota(int atualMat){
    int l = 0;
    do {
        system("cls"); // limpatela
        for (int i = 0; i < MAX_NOTES; i++) {
            printf("Nota %d: %.1f\n", i + 1, users[currentUser].materia[atualMat].nota[i]);
        }
        printf("Deseja inserir qual nota? \n");
        printf("Digite [4] para sair\n");
        scanf("%d", &l);
        if (l >= 1 && l < 4) {
            printf("Digite a nota %d: ", l);
            scanf("%f", &users[currentUser].materia[atualMat].nota[l-1]); // Os índices em C comecam em 0
            printf("Nota inserida: %.1f\n", users[currentUser].materia[atualMat].nota[l-1]);
            users[currentUser].materia[atualMat].media = (users[currentUser].materia[atualMat].nota[0] + users[currentUser].materia[atualMat].nota[1] + users[currentUser].materia[atualMat].nota[0]) / 3;
            printf("Media: %.1f\n", users[currentUser].materia[atualMat].media);
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

char* verMedia(int atualMat){// Joaquim
    float currentMedia = users[currentUser].materia[atualMat].media;
    if (currentMedia >= 7) {
        return "Aprovado!";
    }
    else if (currentMedia < 5) {
        return "Reprovado!";
    }
    else {
        printf("Em Exame!\n");
        if (users[currentUser].materia[atualMat].notaex == 0) {
            printf("Digite a nota do exame: ");
            scanf("%f", &users[currentUser].materia[atualMat].notaex);
            currentMedia = (currentMedia + users[currentUser].materia[atualMat].notaex) / 2;
        } else {
            printf("Nota do exame: %f\n", users[currentUser].materia[atualMat].notaex);
        }

        return (currentMedia >= 5) ? "Aprovado!!" : "Reprovado!!"; //operador ternario para compactar o codigo;
    }
}

void adminMenu(){ 
    int escolhaMenu;
    int escolhaUser; 
    int confirmacao;
    const char nulo[] = "";
    printf("Codigo de administrador encontrado\n");
    do {
        printf("O que deseja?\n");
        printf("[1] Cadastrar usuario.\n");
        printf("[2] Excluir usuario.\n");
        printf("[0] Sair\n");
        scanf("%d", &escolhaMenu);
        switch (escolhaMenu) {
        case 1:
            for (int i = 0; i < MAX_USERS; i++)
            {
                printf("Usuario %d: %s\n", i + 1, users[i].email);
            }
            printf("Otimo! Qual usuario deseja inserir ou modificar?\n");
            scanf("%d", &escolhaUser);
            printf("O email do Usuario registrado eh: %s\n", users[escolhaUser-1].email);
            cadUser(escolhaUser - 1);
            printf("Usuario atualizado com sucesso!\n");
            printf("email do Usuario: %s\n", users[escolhaUser - 1].email);
            printf("A senha eh %s\n", users[escolhaUser - 1].password);
            enter();
            break;

        case 2:
            for (int i = 0; i < MAX_USERS; i++)
            {
                printf("Usuario %d: %s\n", i + 1, users[i].email);
            }
            printf("Otimo! Qual usuario deseja excluir?\n");
            scanf("%d", &escolhaUser);
            printf("Certeza que deseja excluir o usuario %d? \ndigite o numero novamente para confirmar: ",escolhaUser);
            scanf("%d", &confirmacao);
            if(escolhaUser==confirmacao){
                printf("Deletando o usuario %d, de email: %s\n",escolhaUser,users[escolhaUser - 1].email);
                strcpy(users[escolhaUser - 1].email,nulo);
                strcpy(users[escolhaUser - 1].password,nulo);
                printf("Deletado com sucesso!\n");
                enter();
                adminMenu();
            } 
            else{
                printf("A confirmacao difere do numero escolhido, voltando ao menu principal\n");
                enter();
                adminMenu();
            }
            break;

        case 0:
            break;

        default:
            printf("Opcao invalida!\n");
            enter();
            break;
        }
    } while (escolhaMenu != 0);
}
void criarUsers(){
    char senha[80],email[80];
    for (int i = 0; i < MAX_USERS; i++)
    {
        snprintf(email, sizeof(email), "user%d@mail.com", i+1);
        snprintf(senha, sizeof(senha), "password%d", i+1);
        strcpy(users[i].email, email);
        strcpy(users[i].password, senha);
    }
}
int main(){  
    criarUsers();
    loginRequest();
    return 0;
}