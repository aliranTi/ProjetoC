#include "libprojeto.h"

User * alocarUser(unsigned int quant){
    User * usuario = (User *) malloc(quant*sizeof(User));
    if (usuario == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return NULL;
    }
    alocarMateria(usuario,MAX_DISCIPLINES);
    return usuario;
}

void alocarMateria(User *users, unsigned int quant) {
    users->materia = calloc(quant, sizeof(Disciplina));
    if (users->materia == NULL) {
        // Tratar falha na alocação de memória, se necessário
    }
}

int containsAtSymbol(char *string){ 
    return strchr(string, '@') != NULL;  
    // Retorna 1 (verdadeiro) se '@' estiver na string, caso contrario retorna 0 (falso)
}

void erroArquivo(){
    printf("erro no arquivo!\n");
    exit(1);
}

int countUsers(){
    FILE * arquivo = fopen("UserDB.b","rb");
    if (arquivo == NULL){
        erroArquivo();
    }
    int quant=0;
    User userAtual;
    while (fread(&userAtual,sizeof(User),1,arquivo)) {
        quant++;
    }
    fclose(arquivo);
    return quant;
}

User * recuperarUsers(unsigned int * quant_users){
    User * users;
    FILE * arquivo = fopen("UserDB.b","rb");
    if (arquivo == NULL) {
        erroArquivo();
        return NULL;
    }
    users = alocarUser(*quant_users);
    rewind(arquivo);
    fread(users,sizeof(User),*quant_users,arquivo);
    fclose(arquivo);
    return users;
}

int checkMat(User *users,int matInput, unsigned int * quant_users){ 
    for (unsigned int i = 0; i < *quant_users; i++) {
        if (matInput == users[i].matricula) {
            return 1; //retorna 1 quando as matriculas forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar uma matricula igual
}

void checkFile(){
    FILE * arquivo = fopen("UserDB.b","rb");
    
    if (arquivo == NULL) {
        printf("Arquivo Invalido!\nRecriando...\n");
        fclose(arquivo);
        arquivo = fopen("UserDB.b","wb");
    }
    fclose(arquivo);
}

int checkEmail(User * users,char *string, unsigned int * quant_users){ 
    for (unsigned int i = 0; i < *quant_users; i++) {
        if (strcmp(string,users[i].email)) {
            return 1; //retorna 1 quando os emails forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar um email igual
}

void freeUsers(User * user){
    free(user->materia);
    free(user);
}

void cadUser(unsigned int * quant_users) { 
    char inputEmail[85];
    char inputPass[85];
    int inputMat;
    int cadEmail = 0,cadMat = 0;
    User * users = alocarUser(1);
    User * usersCheck = recuperarUsers(quant_users);
    do {
        printf("Digite o email para realizar o cadastro: \n");
        scanf(" %84s", inputEmail); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (!containsAtSymbol(inputEmail) || strlen(inputEmail) > 80 || checkEmail(usersCheck, inputEmail, quant_users)) {
            printf("Por favor insira um email válido!\nEste email já foi cadastrado ou é inválido!\n");
            cadEmail = 0;
        } else {
            cadEmail = 1;
        }
    } while (!cadEmail);

    do {
        printf("Digite a senha: \n");
        scanf(" %84s", inputPass); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (strlen(inputPass) < 8 || strlen(inputPass) > 80) {
            printf("Insira uma senha válida! \nContendo no mínimo 8 e no máximo 80 caracteres.\n");
        }
    } while (strlen(inputPass) < 8 || strlen(inputPass) > 80);

    printf("Agora, digite sua matricula para completar o cadastro: \n");
    do
    {
        scanf("%d",&inputMat);
        cadMat = checkMat(usersCheck,inputMat, quant_users);
        if (cadMat)
        {
            printf("Essa matricula ja foi cadastrada!\ninsira uma matricula diferente: \n");
        }
        
    } while (cadMat);
    

    freeUsers(usersCheck);
    users->matricula = inputMat;
    strcpy(users->email, inputEmail);
    strcpy(users->password, inputPass);
    users->materia = NULL;
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

User * loginVerif(char *email, char *password, int *verifadm, unsigned int * quant_users) {
    User * users = recuperarUsers(quant_users);
    User * temp;
    User admin = {0, "admin", "admin123", NULL};
    User * adminPointer = &admin;

    if (strcmp(email, admin.email) == 0 && strcmp(password, admin.password) == 0) {
        *verifadm = 1;
        return adminPointer;
    } else {
    for (unsigned int i = 0; i < *quant_users; i++) {
        temp = checkUser(email,password,&users[i]);
        if (temp != NULL) {
            *verifadm = 0;    // reseta a tag adm
            return temp;         // Retorna 1 se o login for bem-sucedido
        }
    }
    }
    free(users);
    return NULL; // Retorna 0 se o login falhar
}

void deletarUsuario(User * users,User userExcluido, unsigned int * quant_users){
    unsigned int i;
    FILE * arquivo = fopen("UserDB.b","wb");
    if(arquivo == NULL){
        erroArquivo();
    }
    for (i = 0; i < *quant_users; i++)
    {
        if (users[i].matricula != userExcluido.matricula)
        {
            fwrite(&users[i],sizeof(User),1,arquivo);
        }
    }
    fclose(arquivo);
}

void saveUserFile(User * users){
    FILE * arquivo = fopen("UserDB.b","ab");
    if (arquivo == NULL){
        erroArquivo();
    }
    fwrite(users,sizeof(User),1,arquivo);
    fclose(arquivo);
}

void loginRequest(unsigned int * quant_users, int *verifadm) {
    char inputUsername[50];
    char inputPassword[50];
    int escolha;
    User * temp;
    User * users;
    unsigned int quant_materias;

    printf("Deseja fazer login ou cadastrar? \n[1] Para login \n[2] Para cadastro\n");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            printf("Digite seu nome de usuário: ");
            scanf("%49s", inputUsername); // Lê o nome de usuário do teclado
            printf("Digite sua senha: ");
            scanf("%49s", inputPassword); // Lê a senha do teclado
            temp = loginVerif(inputUsername, inputPassword, verifadm, quant_users);
            if (temp != NULL) {
                users = loginVerif(inputUsername, inputPassword, verifadm, quant_users);
                printf("Login bem-sucedido!\n");
                enter();
                if (*verifadm != 1 && !hasMateria(users)) {
                    matInsert(users, &quant_materias);
                }
                menuPrincipal(users, quant_users, verifadm, &quant_materias);
            } else {
                printf("Nome de usuário ou senha incorretos!\n");
                loginRequest(quant_users,verifadm);
            }
            break;

        case 2:
            cadUser(quant_users);
            printf("Usuario cadastrado com sucesso!!\n");
            loginRequest(quant_users,verifadm);
            break;

        default:
            printf("Escolha inválida!!\n");
            loginRequest(quant_users, verifadm);
            break;
    }
}