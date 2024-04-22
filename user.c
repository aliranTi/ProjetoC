#include "libprojeto.h"

User * alocarUser() {
    User *usuario = (User *)malloc(sizeof(User));
    if (usuario == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return NULL;
    }
    iniciaListaMateria(&usuario->materiaHead);
    usuario->proxUser = NULL;
    return usuario;
}

void saveUserFile(User * users){
    FILE * arquivo = fopen("UserDB.b","ab");
    if (arquivo == NULL){
        erroArquivo();
    }
    fwrite(users,sizeof(User),1,arquivo);
    Disciplina * temp = users->materiaHead;
    for (temp = users->materiaHead; temp != NULL; temp = temp->prox){
        saveDisciplinaFile(temp);
    }
    fclose(arquivo);
}

int countUsers(){
    FILE * arquivo = fopen("UserDB.b","rb");
    if (arquivo == NULL){
        erroArquivo();
    }
    int quant=0;
    User aux;
    while (fread(&aux,sizeof(User),1,arquivo)) {
        quant++;
    }
    fclose(arquivo);
    return quant;
}

void freeUsers(User *user) {
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

void menuPrincipal(User * users) {
    char r;
    int m = 0,i = 0;
    User * temp = recuperarUsers();
    Disciplina * cont;

    while (m != -1) {
        printf("Lista de matérias cadastradas: \n");
        i = 0;
        cont = users->materiaHead;
        while(cont!=NULL){
            printf("%d - %s\n", i + 1, cont->nome);
            i++;
            cont = cont->prox;
        }
        printf("========================================================\n");
        printf("== Para acessar uma materia, digite 1 \t\t==\n");
        printf("== Para modificar uma materia, digite 2 \t\t==\n");
        printf("== Para excluir uma materia, digite 3   \t\t==\n");
        printf("== Para inserir uma materia, digite 4   \t\t==\n");
        printf("== Para sair, digite 5   \t\t==\n");
        printf("== Para encerrar o programa, digite 6   \t\t==\n");
        printf("== Para imprimir as disciplinas, digite 7 \t\t==\n");
        printf("========================================================\n");

        scanf("%c", &r);
        if (verificaNumero(&r)){
            switch (r) {
                case '1':
                    acessarMateria(users->materiaHead);
                    break;
                case '2':
                    editarMateria(users->materiaHead);
                    break;
                case '3':
                    deletarMateria(&users->materiaHead);
                    break;
                case '4':
                    cadDisciplina(&users->materiaHead);
                    break;
                case '5':
                    printf("Fazendo logoff...\n");
                    enter();
                    deletarUsuario(temp,users->matricula);
                    saveUserFile(users);
                    m = -1;
                    loginRequest();
                    break;
                case '6':
                    printf("Obrigado por utilizar o Programa!!\n");
                    printf("Salvando...\n");
                    enter();
                    deletarUsuario(temp,users->matricula);
                    saveUserFile(users);
                    printf("Finalizando...\n");
                    freeUsers(users);
                    m = -1;
                    break;
                case '7':
                    printf("Imprimindo...");
                    for(cont=users->materiaHead;cont!=NULL;cont=cont->prox){
                        imprimirMateria(*cont);
                    }
                    break;
                default:
                    printf("Opção inválida!!\n");
                    enter();
                    break;
                }
        } else {
            printf("Entrada Invalida!\n");
        }
    }
}

User * recuperarUsers() {
    FILE *arquivoUser = fopen("UserDB.b", "rb");
    if (arquivoUser == NULL) {
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

        if (fread(newUser, sizeof(User), 1, arquivoUser) != 1) {
            // Se não conseguir ler um usuário, sai do loop
            free(newUser);
            break;
        }

        newUser->materiaHead = recuperarDisciplina();
        // Adicionar novo usuário à lista de usuários
        if (head == NULL) {
            head = newUser;
        } else {
            temp->proxUser = newUser;
        }
        temp = newUser;
    }
    fclose(arquivoUser);
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
    User * aux = users;
    
    while(aux != NULL) {
        temp = checkUser(email,password,users);
        if (temp != NULL) {
            return temp;         // Retorna 1 se o login for bem-sucedido
        }
        aux = aux->proxUser;
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