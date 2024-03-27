#include "libprojeto.h"

void enter() {
    printf("\nAperte 'ENTER' para continuar\n");
    while (getchar() != '\n');      // Limpa o buffer, consumindo o '\n' anterior (se houver)
    getchar(); // Aguarda o usuario pressionar ENTER
    // system("cls");
}

int hasMateria(User *users) {
    if(users->materia != NULL) return 1;
    printf("Nenhuma materia encontrada, partindo para o cadastramento\n");
    return 0;  // Retorna 0 se não encontrar nenhuma matéria
}

void matInsert(User *users, unsigned int * h) {

    do {
        printf("Deseja inserir quantas disciplinas? (Máx: %d)\n", MAX_DISCIPLINES);
        scanf("%u", h);

        if (*h < 1 || *h > MAX_DISCIPLINES) {
            printf("Por favor insira um número entre 1 e %d.\n", MAX_DISCIPLINES);
        }
        enter(); // Adicionado para limpar o buffer após a entrada de dados
    } while (*h < 1 || *h > MAX_DISCIPLINES);

    users->materia = (Disciplina*) calloc(*h,sizeof(struct disciplina));
    if (users->materia == NULL) {
        // Tratar falha na alocação de memória
    } else {
        for (unsigned int i = 0; i < *h; i++) {
            users->materia[i] = regMat(i);
            printf("materia cadastrada!\n");
        }
    }
    printf("Terminado o cadastro de disciplinas, vamos para o menu principal!\n");
}

Disciplina regMat(unsigned int i) {
    Disciplina * disciplina = malloc(sizeof(struct disciplina));
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
    return *disciplina;
}

void preListMat(Disciplina * materia) {
    int h;
    do {
        system("cls");
        listMat(materia);
        printf("Selecione uma opção \n");
        printf("[1] para inserir notas\n");
        printf("[2] para Imprimir a materia atual\n");
        printf("[3] para sair\n");
        scanf("%d", &h);

        switch (h) {
            case 1:
                cadNota(materia);
                break;
            case 2:
                imprimirMateria(*materia);
                break;
            case 3:
                enter();
                break;
            default:
                printf("Dado inválido!\n");
                enter();
                break;
        }
    } while (h != 3);  
}

void cadNota(Disciplina * materia) {
    int l = 0;

    do {
        // system("cls"); // limpa a tela
        for (int i = 0; i < MAX_NOTES; i++) {
            printf("Nota %d: %.1f\n", i + 1, materia->nota[i]);
        }
        printf("Deseja inserir qual nota? \n");
        printf("Digite [4] para sair\n");
        scanf("%d", &l);

        if (l >= 1 && l < 4) {
            printf("Digite a nota %d: ", l);
            scanf("%f", &materia->nota[l-1]); // Os índices em C começam em 0
            printf("Nota inserida: %.1f\n", materia->nota[l-1]);
            materia->media = (materia->nota[0] + materia->nota[1] + materia->nota[2]) / 3;
            printf("Média: %.1f\n", materia->media);
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

void listMat(Disciplina * materia) {
    printf("Nome: %s\n", materia->nome);
    printf("Professor: %s\n", materia->prof);
    printf("Horas totais: %.2f\n", materia->hora_t);
    printf("Horas ministradas: %.2f\n", materia->hora_c);
    materia->porc_hora = (materia->hora_c / materia->hora_t) * 100;
    printf("Porcentagem de aulas ministradas: %.2f%%\n", materia->porc_hora);
    printf("Media da disciplina: %.2f\n", materia->media);
    if (materia->nota[0] && materia->nota[1] && materia->nota[2]) {
        printf("Voce esta %s\n", verMedia(materia));
    }
}

char* verMedia(Disciplina * materia) {
    float currentMedia = materia->media;

    if (currentMedia >= 7) {
        return "Aprovado!";
    }
    else if (currentMedia < 5) {
        return "Reprovado!";
    }
    else {
        printf("Em Exame!\n");
        if (materia->notaex == 0) {
            printf("Digite a nota do exame: ");
            scanf("%f", &materia->notaex);
            currentMedia = (currentMedia + materia->notaex) / 2;
        } else {
            printf("Nota do exame: %.1f\n", materia->notaex);
        }

        return (currentMedia >= 5) ? "Aprovado!!" : "Reprovado!!"; // operador ternário para compactar o código
    }
}

void menuPrincipal(User * users, unsigned int * quant_users, int *verifadm, unsigned int * quant_materias) {
    int r;
    int m = 0;
    User * temp = recuperarUsers(quant_users);


    while (m != -1) {
        if (!*verifadm)
        {
            printf("Lista de matérias cadastradas: \n");
            for (unsigned int i = 0; i < *quant_materias; i++) {
                printf("%d - %s\n", i + 1, users->materia[i].nome);
            }
        }
        printf("Digite o número da matéria para obter mais detalhes.\n");
        printf("Caso queira adicionar outra matéria, digite [%d]\n",MAX_DISCIPLINES+1);
        printf("Caso queira fazer logoff, digite [%d]\n",MAX_DISCIPLINES+2);
        printf("Digite [%d] para salvar e sair\ndigite [%d] para sair sem salvar\n",MAX_DISCIPLINES+3,MAX_DISCIPLINES+4);
        
        if (*verifadm) {
            printf("Para acessar o menu de administrador digite [6373]\n");
        }

        scanf("%d", &r);

        if (r >= 1 && r <= MAX_DISCIPLINES) {
            r = r - 1;  // Ajuste do índice de linguagem humana para linguagem de máquina
            preListMat(&users->materia[r]);
        } else {
            switch (r) {
                case 6373:
                    system("cls");
                    adminMenu(quant_users);
                    break;
                case MAX_DISCIPLINES+1:
                    printf("Digite qual matéria deseja inserir ou modificar: \n");
                    scanf("%d", &r);
                    r--;
                    system("cls");
                    users->materia[r] = regMat((unsigned)r);
                    break;
                case MAX_DISCIPLINES+2:
                    printf("Fazendo logoff...\n");
                    enter();
                    m = -1;
                    loginRequest(quant_users, verifadm);
                    break;
                case MAX_DISCIPLINES+3:
                    printf("Obrigado por utilizar o Programa!!\n");
                    printf("Salvando...\n");
                    enter();
                    deletarUsuario(temp,*users,quant_users);
                    saveUserFile(users);
                    printf("Finalizando...\n");
                    freeUsers(users);
                    m = -1;
                    break;
                case MAX_DISCIPLINES + 4:
                    printf("Obrigado por utilizar o Programa!!\n");
                    enter();
                    printf("Finalizando...\n");
                    freeUsers(users);
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

void imprimirMateria(Disciplina materiaAtual) {
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
    fprintf(file,"\n");
    fclose(file);
}

void adminMenu(unsigned int * quant_users) {
    int escolhaMenu;
    int escolhaUser;
    int confirmacao;
    User * users = recuperarUsers(quant_users);

    User * temp = &users[0];

    printf("Código de administrador encontrado\n");

    do {
        printf("O que deseja?\n");
        printf("[1] Excluir usuário.\n");
        printf("[0] Sair\n");
        scanf("%d", &escolhaMenu);

        switch (escolhaMenu) {
            
            case 1:
                for (int i = 0; i < (int)*quant_users; i++) {
                    printf("Usuário %d de matricula: %d e email: %s\n", i + 1,users[i].matricula, users[i].email);
                }
                printf("Ótimo! Qual usuário deseja excluir?\n");
                scanf("%d", &escolhaUser);
                
                if (escolhaUser < 1 || escolhaUser > (int)*quant_users) {
                    printf("Número de usuário inválido!\n");
                    break;
                }

                printf("Certeza que deseja excluir o usuário %d? \nDigite o número novamente para confirmar: ", escolhaUser);
                scanf("%d", &confirmacao);
                *temp = users[escolhaUser - 1];
                if (escolhaUser == confirmacao) {
                    printf("Deletando o usuario %d de matricula: %d e email: %s\n", escolhaUser,users[escolhaUser - 1].matricula, users[escolhaUser - 1].email);
                    deletarUsuario(users,*temp,quant_users);
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