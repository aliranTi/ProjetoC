#include "libprojeto.h"

User * alocarUser(int quant){
    User * usuario = (User *) malloc(quant*sizeof(User));
    if (usuario == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return NULL;
    }
    alocarMateria(usuario,1);
    return usuario;
}

void alocarMateria(User *users, int quant) {
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].materia = calloc(quant, sizeof(Disciplina));
        if (users[i].materia == NULL) {
            // Tratar falha na alocação de memória, se necessário
        }
    }
}

void enter(){
    printf("\nAperte 'ENTER' para continuar\n");
    while (getchar() != '\n');      // Limpa o buffer, consumindo o '\n' anterior (se houver)
    getchar(); // Aguarda o usuario pressionar ENTER
    system("cls");
}

void loginRequest(User *users, int *currentUser, int *verifadm) {
    char inputUsername[50];
    char inputPassword[50];
    int escolha;
    static int atual = 0;
    int *atualp= &atual;

    printf("Deseja fazer login ou cadastrar? \n[1] Para login \n[2] Para cadastro\n");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            do {
                printf("Digite seu nome de usuário: ");
                scanf("%49s", inputUsername); // Lê o nome de usuário do teclado
                printf("Digite sua senha: ");
                scanf("%49s", inputPassword); // Lê a senha do teclado

                if (loginVerif(users, inputUsername, inputPassword, currentUser, verifadm)) {
                    printf("Login bem-sucedido!\n");
                    enter();
                    if (*verifadm != 1 && hasMateria(users, currentUser) == 0) {
                        matInsert(users, currentUser);
                    }
                    printf("teste");
                    menuPrincipal(users, currentUser, verifadm);
                } else {
                    printf("Nome de usuário ou senha incorretos!\n");
                }
            } while (!loginVerif(users, inputUsername, inputPassword, currentUser, verifadm));
            break;

        case 2:
            if (atual <= MAX_USERS) {
                users[atual] = cadUser(&users[atual], *atualp);
                atual++;
                printf("\nretornou\n");
                loginRequest(users, currentUser, verifadm);
            } else {
                printf("Número máximo de usuários cadastrados atingido,\npor favor contate o administrador para resolver\n");
                loginRequest(users, currentUser, verifadm);
            }
            break;

        default:
            printf("Escolha inválida!!\n");
            loginRequest(users, currentUser, verifadm);
            break;
    }
}

User cadUser(User *users, int atual) { 
    char inputEmail[85];
    char inputPass[85];
    int cademail = 0;
    do {
        printf("Digite o email para realizar o cadastro: \n");
        scanf(" %84s", inputEmail); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (!containsAtSymbol(inputEmail) || strlen(inputEmail) > 80 || !checkEmail(users, inputEmail)) {
            printf("Por favor insira um email válido!\nEste email já foi cadastrado ou é inválido!\n");
            cademail = 0;
        } else {
            cademail = 1;
        }
    } while (cademail != 1);

    do {
        printf("Digite a senha: \n");
        scanf(" %84s", inputPass); // Ajustado o tamanho máximo para 84 para evitar overflow
        if (strlen(inputPass) < 8 || strlen(inputPass) > 80) {
            printf("Insira uma senha válida! \nContendo no mínimo 8 e no máximo 80 caracteres.\n");
        }
    } while (strlen(inputPass) < 8 || strlen(inputPass) > 80);

    strcpy(users[atual].email, inputEmail);
    strcpy(users[atual].password, inputPass);
    saveUserFile(&users[atual]);
    enter(); // Adicionado para limpar o buffer após a entrada de dados
    return users[atual];
}

void saveUserFile(User * users){
    FILE * arquivo = fopen("UserDB.b","ab");
    fwrite(users,sizeof(User),1,arquivo);
    fclose(arquivo);
}

int loginVerif(User *users, char *email, char *password, int *currentUser, int *verifadm) {
    const char login_adm[] = "admin";
    const char senha_adm[] = "admin123";
    if (strcmp(email, login_adm) == 0 && strcmp(password, senha_adm) == 0) {
        *verifadm = 1;
        *currentUser = MAX_USERS + 1;
        return 1;
    } else {
    for (int i = 0; i < MAX_USERS; i++) {

        if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0) {
            *currentUser = i; // Define o usuário atual após o login bem-sucedido
            *verifadm = 0;    // reseta a tag adm
            return 1;         // Retorna 1 se o login for bem-sucedido
        }
    }
    }
    
    return 0; // Retorna 0 se o login falhar
}


int containsAtSymbol(char *string){ 
    return strchr(string, '@') != NULL;  
    // Retorna 1 (verdadeiro) se '@' estiver na string, caso contrario retorna 0 (falso)
}

int checkEmail(User *users,char *string){ 
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(string,users[i].email)) {
            return 1; //retorna 1 quando os emails forem iguais
        } 
    }
    return 0; //retorna 0 se nao encontrar um email igual
}

int hasMateria(User *users, int *currentUser) {
    for (int i = 0; i < MAX_DISCIPLINES; i++) {
        if (users[*currentUser].materia[i].nome[0] != '\0') {
            printf("Materias ja cadastradas, partindo para o menu\n");
            return 1;  // Retorna 1 se encontrar alguma matéria
        }
    }
    printf("Nenhuma materia encontrada, partindo para o cadastramento\n");
    return 0;  // Retorna 0 se não encontrar nenhuma matéria
}

void matInsert(User *users, int *currentUser) {
    int h;

    do {
        printf("Deseja inserir quantas disciplinas? (Máx: %d)\n", MAX_DISCIPLINES);
        scanf("%d", &h);

        if (h < 1 || h > MAX_DISCIPLINES) {
            printf("Por favor insira um número entre 1 e %d.\n", MAX_DISCIPLINES);
        }
        enter(); // Adicionado para limpar o buffer após a entrada de dados
    } while (h < 1 || h > MAX_DISCIPLINES);

    Disciplina *temp = realloc(users[*currentUser].materia, h * sizeof(Disciplina));
    if (temp == NULL) {
        // Tratar falha na alocação de memória
    } else {
        users[*currentUser].materia = temp;
    }

    for (int i = 0; i < h; i++) {
        regMat(&users[*currentUser].materia[i], i);
    }
}


void regMat(Disciplina * disciplina,int i) {
    printf("---------------------------------------\n");
    printf("--------Registro de disciplinas--------\n");
    printf("---------------------------------------\n");
    printf("---N da matéria a ser registrada: %d---\n", i + 1);
    printf("---------------------------------------\n");
    printf("Digite o nome da disciplina n %d: ", i + 1);
    scanf(" %99[^\n]", disciplina->nome); // Lê até 99 caracteres ou até encontrar uma nova linha.
    printf("Digite o nome do Professor: ");
    scanf(" %99[^\n]", disciplina->prof); // Mesmo procedimento para o nome do professor.
    printf("Digite a quantidade de aulas totais da disciplina: ");
    scanf("%f", &disciplina->hora_t);
    disciplina->hora_t = (disciplina->hora_t * 5) / 6;
    printf("Digite a quantidade de aulas já realizadas da disciplina: ");
    scanf("%f", &disciplina->hora_c);
    disciplina->hora_c = (disciplina->hora_c * 5) / 6;
    printf("Disciplina cadastrada com sucesso!\n");
    enter();
}


void menuPrincipal(User *users, int *currentUser, int *verifadm) {
    int r;
    int m = 0;

    while (m != -1) {
        if (!*verifadm)
        {
            printf("Lista de matérias cadastradas: \n");
            for (int i = 0; i < MAX_DISCIPLINES; i++) {
                printf("%d - %s\n", i + 1, users[*currentUser].materia[i].nome);
            }
        }
        printf("Digite o número da matéria para obter mais detalhes.\n");
        printf("Caso queira adicionar outra matéria, digite [%d]\n",MAX_DISCIPLINES+1);
        printf("Caso queira fazer logoff, digite [%d]\n",MAX_DISCIPLINES+2);
        printf("Digite [%d] para sair\n",MAX_DISCIPLINES+3);
        
        if (*verifadm) {
            printf("Para acessar o menu de administrador digite [6373]\n");
        }

        scanf("%d", &r);

        if (r >= 1 && r <= MAX_DISCIPLINES) {
            r = r - 1;  // Ajuste do índice de linguagem humana para linguagem de máquina
            preListMat(users, currentUser, &r);
        } else {
            switch (r) {
                case 6373:
                    system("cls");
                    adminMenu(users);
                    break;
                case MAX_DISCIPLINES+1:
                    printf("Digite qual matéria deseja inserir ou modificar: \n");
                    scanf("%d", &r);
                    r--;
                    system("cls");
                    regMat(&users[*currentUser].materia[r],r);
                    break;
                case MAX_DISCIPLINES+2:
                    printf("Fazendo logoff...\n");
                    enter();
                    m = -1;
                    loginRequest(users, currentUser, verifadm);
                    break;
                case MAX_DISCIPLINES+3:
                    printf("Obrigado por utilizar o Programa!!\n");
                    printf("Finalizando...\n");
                    enter();
                    m = -1;
                    break;
                default:
                    printf("Opção inválida!!\n");
                    enter();
                    break;
            }
        }
    }
}

void preListMat(User *users, int *currentUser, int *atualMat) {
    int h;
    do {
        Disciplina materia = users[*currentUser].materia[*atualMat];
        system("cls");
        listMat(users, currentUser, atualMat);
        printf("Selecione uma opção \n");
        printf("[1] para inserir notas\n");
        printf("[2] para Imprimir a materia atual");
        printf("[3] para sair\n");
        scanf("%d", &h);

        switch (h) {
            case 1:
                cadNota(users, currentUser, atualMat);
                break;
            case 2:
                imprimirMateria(materia);
                break;
            case 3:
                enter();
                break;
            default:
                printf("Dado inválido!\n");
                enter();
                break;
        }
    } while (h != 2);  
}


void listMat(User *users, int *currentUser, int *atualMat){
    printf("Nome: %s\n", users[*currentUser].materia[*atualMat].nome);
    printf("Professor: %s\n", users[*currentUser].materia[*atualMat].prof);
    printf("Horas totais: %.2f\n", users[*currentUser].materia[*atualMat].hora_t);
    printf("Horas ministradas: %.2f\n", users[*currentUser].materia[*atualMat].hora_c);
    users[*currentUser].materia[*atualMat].porc_hora = (users[*currentUser].materia[*atualMat].hora_c / users[*currentUser].materia[*atualMat].hora_t) * 100;
    printf("Porcentagem de aulas ministradas: %.2f%%\n", users[*currentUser].materia[*atualMat].porc_hora);
    printf("Media da disciplina: %.2f\n", users[*currentUser].materia[*atualMat].media);
    if (users[*currentUser].materia[*atualMat].nota[0] && users[*currentUser].materia[*atualMat].nota[1] && users[*currentUser].materia[*atualMat].nota[2]) {
        printf("Voce esta %s\n", verMedia(users, currentUser, atualMat));
    }
}

void cadNota(User *users, int *currentUser, int *atualMat) {
    int l = 0;

    do {
        system("cls"); // limpa a tela
        for (int i = 0; i < MAX_NOTES; i++) {
            printf("Nota %d: %.1f\n", i + 1, users[*currentUser].materia[*atualMat].nota[i]);
        }
        printf("Deseja inserir qual nota? \n");
        printf("Digite [4] para sair\n");
        scanf("%d", &l);

        if (l >= 1 && l < 4) {
            printf("Digite a nota %d: ", l);
            scanf("%f", &users[*currentUser].materia[*atualMat].nota[l-1]); // Os índices em C começam em 0
            printf("Nota inserida: %.1f\n", users[*currentUser].materia[*atualMat].nota[l-1]);
            users[*currentUser].materia[*atualMat].media = (users[*currentUser].materia[*atualMat].nota[0] + users[*currentUser].materia[*atualMat].nota[1] + users[*currentUser].materia[*atualMat].nota[2]) / 3;
            printf("Média: %.1f\n", users[*currentUser].materia[*atualMat].media);
            enter();
        } else if (l == 4) {
            enter();
            break;
        } else {
            printf("Opção Inválida!\n");
            enter();
        }
    } while (l != 4);
}

char* verMedia(User *users, int *currentUser, int *atualMat) {
    float currentMedia = users[*currentUser].materia[*atualMat].media;

    if (currentMedia >= 7) {
        return "Aprovado!";
    }
    else if (currentMedia < 5) {
        return "Reprovado!";
    }
    else {
        printf("Em Exame!\n");
        if (users[*currentUser].materia[*atualMat].notaex == 0) {
            printf("Digite a nota do exame: ");
            scanf("%f", &users[*currentUser].materia[*atualMat].notaex);
            currentMedia = (currentMedia + users[*currentUser].materia[*atualMat].notaex) / 2;
        } else {
            printf("Nota do exame: %.1f\n", users[*currentUser].materia[*atualMat].notaex);
        }

        return (currentMedia >= 5) ? "Aprovado!!" : "Reprovado!!"; // operador ternário para compactar o código
    }
}


void adminMenu(User *users) {
    int escolhaMenu;
    int escolhaUser;
    int confirmacao;
    const char nulo[] = "";
    
    printf("Código de administrador encontrado\n");

    do {
        printf("O que deseja?\n");
        printf("[1] Cadastrar usuário.\n");
        printf("[2] Excluir usuário.\n");
        printf("[0] Sair\n");
        scanf("%d", &escolhaMenu);

        switch (escolhaMenu) {
            case 1:
                for (int i = 0; i < MAX_USERS; i++) {
                    printf("Usuário %d: %s\n", i + 1, users[i].email);
                }
                printf("Ótimo! Qual usuário deseja inserir ou modificar?\n");
                scanf("%d", &escolhaUser);
                
                if (escolhaUser < 1 || escolhaUser > MAX_USERS) {
                    printf("Número de usuário inválido!\n");
                    break;
                }

                printf("O email do Usuário registrado é: %s\n", users[escolhaUser - 1].email);
                cadUser(users, escolhaUser - 1);
                printf("Usuário atualizado com sucesso!\n");
                printf("Email do Usuário: %s\n", users[escolhaUser - 1].email);
                printf("A senha é %s\n", users[escolhaUser - 1].password);
                enter();
                break;

            case 2:
                for (int i = 0; i < MAX_USERS; i++) {
                    printf("Usuário %d: %s\n", i + 1, users[i].email);
                }
                printf("Ótimo! Qual usuário deseja excluir?\n");
                scanf("%d", &escolhaUser);
                
                if (escolhaUser < 1 || escolhaUser > MAX_USERS) {
                    printf("Número de usuário inválido!\n");
                    break;
                }

                printf("Certeza que deseja excluir o usuário %d? \nDigite o número novamente para confirmar: ", escolhaUser);
                scanf("%d", &confirmacao);
                
                if (escolhaUser == confirmacao) {
                    printf("Deletando o usuário %d, de email: %s\n", escolhaUser, users[escolhaUser - 1].email);
                    strcpy(users[escolhaUser - 1].email, nulo);
                    strcpy(users[escolhaUser - 1].password, nulo);
                    printf("Deletado com sucesso!\n");
                    enter();
                } else {
                    printf("A confirmação difere do número escolhido, voltando ao menu principal\n");
                    enter();
                }
                break;

            case 0:
                break;

            default:
                printf("Opção inválida!\n");
                enter();
                break;
        }
    } while (escolhaMenu != 0);
}

/*void criarUsers(User *users) {
    char senha[80], email[80];
    for (int i = 0; i < MAX_USERS; i++) {
        snprintf(email, sizeof(email), "user%d@mail.com", i + 1);
        snprintf(senha, sizeof(senha), "password%d", i + 1);
        strcpy(users[i].email, email);
        strcpy(users[i].password, senha);
    }
}*/

User * recuperarUsers(){
    User * users;
    FILE * arquivo = fopen("UserDB.b","rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo UserDB.b\nContate o administrador!\n");
        return NULL;
    }
    User userAtual;
    int quant=0;
    while (fread(&userAtual,sizeof(User),1,arquivo)) {
        quant++;
    }
    users = alocarUser(quant);
    rewind(arquivo);
    fread(users,sizeof(User),quant,arquivo);
    fclose(arquivo);
    return users;
}

void imprimirMateria(Disciplina materiaAtual){
    FILE * file = fopen("saida.txt","a");
    fprintf(file,"Nome: %s\n", materiaAtual.nome);
    fprintf(file,"Professor: %s\n", materiaAtual.prof);
    fprintf(file,"Horas totais: %.2f\n", materiaAtual.hora_t);
    fprintf(file,"Horas ministradas: %.2f\n", materiaAtual.hora_c);
    materiaAtual.porc_hora = (materiaAtual.hora_c / materiaAtual.hora_t) * 100;
    fprintf(file,"Porcentagem de aulas ministradas: %.2f%%\n", materiaAtual.porc_hora);
    fprintf(file,"Media da disciplina: %.2f\n", materiaAtual.media);
    if (materiaAtual.nota[0] && materiaAtual.nota[1] && materiaAtual.nota[2]) {
        if (materiaAtual.media>=5)
        {
            fprintf(file,"Voce esta Aprovado!!");
        } else {
            fprintf(file,"Voce esta Reprovado!!");
        }
        
    }
    fclose(file);
}

void liberarMateria(User *users){
    for (int i = 0; i < MAX_DISCIPLINES; i++)
    {
        free(users[i].materia);
    }
}
