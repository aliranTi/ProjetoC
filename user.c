#include "libprojeto.h"

User * alocarUser() {
    User *usuario = (User *)malloc(sizeof(User));
    if (usuario == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return NULL;
    }
    iniciaListaMateria(usuario);
    usuario->proxUser = NULL;
    return usuario;
}

void iniciaListaMateria(User *user) {
    Disciplina *materia = (Disciplina *) malloc(sizeof(Disciplina));
    if (materia == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return;
    }
    materia->prox = NULL
    user->materiaHead = materia;
}

void freeUser(User *user) {
    if (user != NULL){
    Disciplina *temp = user->materiaHead;
    while (temp != NULL) {
        Disciplina *next = temp->prox;
        free(temp);
        temp = next;
    }
    free(user);
    }
}

void freeUsers(User *user) {
    Disciplina *temp = user->materiaHead;
    while (temp != NULL) {
        Disciplina *next = temp->prox;
        free(temp);
        temp = next;
    }
    printf("free");
    free(user);
}

User * recuperarUsers() {
    FILE *arquivo = fopen("UserDB.b", "rb");
    if (arquivo == NULL) {
        erroArquivo();
        return NULL;
    }
    User *head = NULL;
    User *temp = NULL;
    User *newUser = NULL;
    while (1) {
        newUser = alocarUser();
        if (newUser == NULL) {
            // Tratar falha na alocação de memória, se necessário
            break;
        }

        if (fread(newUser, sizeof(User), 1, arquivo) != 1) {
            // Se não conseguir ler um usuário, sai do loop
            free(newUser);
            break;
        }

        if (head == NULL) {
            head = newUser;
        } else {
            temp->proxUser = newUser;
        }
        temp = newUser;
    }
    fclose(arquivo);
    return head;
}

int checkMat(User *users,char * matInput){ 
    User * aux = users;
    for (aux = users;aux != NULL;aux = aux->proxUser) {
        if (strcmp(matInput,aux->matricula)) {
            return 1; //retorna 1 quando as matriculas forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar uma matricula igual
}

int checkEmail(User * users,char *string){ 
    User * aux = users;
    for (aux = users;aux != NULL;aux = aux->proxUser) {
        if (strcmp(string,aux->email)) {
            return 1; //retorna 1 quando os emails forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar um email igual
}

void cadUser() {
    char inputEmail[85];
    char inputPass[85];
    char inputMat[DOC_SIZE];
    int cadEmail = 0, cadMat = 0;
    User *users = alocarUser();
    User *usersCheck = recuperarUsers();


    do {
        printf("Digite o email para realizar o cadastro:\n");
        scanf(" %84s", inputEmail); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (!containsAtSymbol(inputEmail) || strlen(inputEmail) > 80 || checkEmail(usersCheck, inputEmail)) {
            printf("Por favor insira um email válido!\nEste email já foi cadastrado ou é inválido!\n");
            cadEmail = 0;
        } else {
            cadEmail = 1;
        }
    } while (!cadEmail);

    do {
        printf("Digite a senha:\n");
        scanf(" %84s", inputPass); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (strlen(inputPass) < 8 || strlen(inputPass) > 80) {
            printf("Insira uma senha válida!\nContendo no mínimo 8 e no máximo 80 caracteres.\n");
        }
    } while (strlen(inputPass) < 8 || strlen(inputPass) > 80);

    printf("Agora, digite sua matricula para completar o cadastro:\n");
    do {
        scanf("%s", inputMat);
        cadMat = checkMat(usersCheck, inputMat);
        if (cadMat) {
            printf("Essa matricula ja foi cadastrada!\nInsira uma matricula diferente:\n");
        }
    } while (cadMat);

    freeUsers(usersCheck);
    printf("Usuario cadastrado com sucesso!\n");
    strcpy(users->matricula,inputMat);
    strcpy(users->email, inputEmail);
    strcpy(users->password, inputPass);
    users->materiaHead = NULL;
    saveUserFile(users);
    free(users);
    enter(); // Adicionado para limpar o buffer após a entrada de dados
}

User * checkUser(char * email, char * password, User * users){
    if (strcmp(users->email, email) == 0 && strcmp(users->password, password) == 0) {
        printf("usuario encontrado\n");
        return users;         // Retorna o usuario se o login for bem-sucedido
    }
    else {
        printf("usuario nao encontrado\n");
        return NULL;
    }
}

User * loginVerif(char *email, char *password) {
    User * users = recuperarUsers();
    User * temp;
    User * aux;
    
    for (aux = users;aux != NULL;aux = aux->proxUser) {
        temp = checkUser(email,password,users);
        if (temp != NULL) {
            return temp;         // Retorna 1 se o login for bem-sucedido
        }
    }
    free(users);
    return NULL; // Retorna 0 se o login falhar
}

void deletarUsuario(User * users,char * matricula){

    User * aux;
    FILE * arquivo = fopen("UserDB.b","wb");
    if(arquivo == NULL){
        erroArquivo();
    }
    for (aux = users;aux != NULL;aux = aux->proxUser)
    {
        if (strcmp(aux->matricula,matricula) != 0)
        {
            fwrite(aux,sizeof(User),1,arquivo);
        }
    }
    fclose(arquivo);
}

void loginRequest() {
    char inputUsername[50];
    char inputPassword[50];
    int escolha;
    User * temp;
    User * users;

    printf("Deseja fazer login ou cadastrar? \n[1] Para login \n[2] Para cadastro\n");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            printf("Digite seu nome de usuário: ");
            scanf("%49s", inputUsername); // Lê o nome de usuário do teclado
            printf("Digite sua senha: ");
            scanf("%49s", inputPassword); // Lê a senha do teclado
            temp = loginVerif(inputUsername, inputPassword);
            if (temp != NULL) {
                users = temp;
                printf("Login bem-sucedido!\n");
                enter();
                if (!hasMateria(users->materiaHead)) {
                    matInsert(&users->materiaHead);
                }
                menuPrincipal(users);
            } else {
                printf("Nome de usuário ou senha incorretos!\n");
                enter();
                loginRequest();
            }
            break;

        case 2:
            cadUser();
            printf("Usuario cadastrado com sucesso!!\n");
            loginRequest();
            break;

        default:
            printf("Escolha inválida!!\n");
            loginRequest();
            break;
    }
}