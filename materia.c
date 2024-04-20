#include "libprojeto.h"

int checarMateria(Disciplina * materia, char * string){
    if (strcmp(materia->nome,string)) return 1;
    return 0;
}

Disciplina * alocarMateria() {
    Disciplina * materia = (Disciplina *)malloc(sizeof(Disciplina));
    if (materia == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return NULL;
    }
    materia->prox = NULL;
    return materia;
}

Disciplina *buscaBinariaDisciplina(Disciplina *head, const char *nome) {
    Disciplina *left = head;
    Disciplina *right = NULL;
    while (left != NULL) {
        int compare = strcmp(left->nome, nome);
        if (compare == 0) {
            return left; // Disciplina encontrada
        } else if (compare > 0) {
            right = left;
            left = left->prox;
        } else {
            left = left->prox;
        }
    }
    right = left;
    left = right;
    return NULL; // Disciplina não encontrada
}

Disciplina * excluirMateria(Disciplina * materia,char * string){
    Disciplina * cont, * ant = NULL;
    for(cont=materia;cont!= NULL;cont=cont->prox){
		if(strcmp(cont->nome,string)==0){
			break;
		}
        ant = cont;
		} 
		if (cont == NULL){
            printf("\nDisciplina não encontrada\n");
            return materia;
		}
        if (ant == NULL) {
            materia = cont->prox;
        }else{
            ant->prox = cont->prox;
        }
        free(cont);
        return materia;
}

void menuPrincipal(User * users) {
    char r;
    int m = 0,i = 0;
    char inputString[MAX_LENGHT];
    User * temp = recuperarUsers();
    Disciplina * cont;
    Disciplina * auxDisciplina;

    while (m != -1) {
        printf("Lista de matérias cadastradas: \n");
        for(cont=users->materiaHead;cont!=NULL;cont=cont->prox){
            printf("%d - %s\n", i + 1, cont->nome);
            i++;
        }
        printf("========================================================\n");
        printf("== Para acessar uma materia, digite 1 \t\t==\n");
        printf("== Para modificar uma materia, digite 2 \t\t==\n");
        printf("== Para excluir uma materia, digite 3   \t\t==\n");
        printf("== Para sair, digite 4   \t\t==\n");
        printf("== Para encerrar o programa, digite 5   \t\t==\n");
        printf("== Para imprimir as disciplinas, digite 6 \t\t==\n");
        printf("========================================================\n");

        scanf("%c", &r);
        if (verificaNumero(&r) && strlen(&r)==1){
            switch (r) {
                case '1':
                    printf("Digite o nome da matéria deseja acessar: \n");
                    scanf("%[^\n]", inputString);
                    if (!checkName(inputString)){
                        strcpy(inputString, passaMaiuscula(inputString));
                        preListMat(buscaBinariaDisciplina(users->materiaHead,inputString));
                    }
                    break;
                case '2':
                    printf("Digite o nome da matéria deseja editar: \n");
                    scanf("%[^\n]", inputString);
                    if (!checkName(inputString)){
                        strcpy(inputString, passaMaiuscula(inputString));
                        auxDisciplina = buscaBinariaDisciplina(users->materiaHead,inputString);
                        regMat(auxDisciplina);
                    }
                    break;
                case '3':
                    printf("Digite o nome da matéria deseja editar: \n");
                    scanf("%[^\n]", inputString);
                    if (!checkName(inputString)){
                        strcpy(inputString, passaMaiuscula(inputString));
                        users->materiaHead = excluirMateria(users->materiaHead,inputString);
                    }
                    break;
                case '4':
                    printf("Fazendo logoff...\n");
                    enter();
                    m = -1;
                    loginRequest();
                    break;
                case '5':
                    printf("Obrigado por utilizar o Programa!!\n");
                    printf("Salvando...\n");
                    enter();
                    deletarUsuario(temp,users->matricula);
                    saveUserFile(users);
                    printf("Finalizando...\n");
                    freeUsers(users);
                    m = -1;
                    break;
                case '6':
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

int hasMateria(Disciplina * disciplina) {
    if(disciplina != NULL) return 1;
    printf("Nenhuma materia encontrada, partindo para o cadastramento\n");
    return 0;  // Retorna 0 se não encontrar nenhuma matéria
}

void matInsert(Disciplina ** materia) {

    int numDisciplinas;
    do {
        printf("Deseja inserir quantas disciplinas? (Máx: %d)\n", MAX_DISCIPLINES);
        scanf("%d", &numDisciplinas);

        if (numDisciplinas < 1 || numDisciplinas > MAX_DISCIPLINES) {
            printf("Por favor insira um número entre 1 e %d.\n", MAX_DISCIPLINES);
        }
        enter(); // Adicionado para limpar o buffer após a entrada de dados
    } while (numDisciplinas < 1 || numDisciplinas > MAX_DISCIPLINES);
    int i = 0;
    *materia = alocarMateria();
    if (materia == NULL) {
        // Tratar falha na alocação de memória
    } else {
        for (i = 0; i < numDisciplinas; i++) {
            Disciplina *novaDisciplina = alocarMateria();
            if (novaDisciplina == NULL) {
                // Tratar falha na alocação de memória
                printf("Erro ao alocar memória para nova disciplina.\n");
                return;
            }
            regMat(novaDisciplina); // Função regMat para inserir dados na nova disciplina

            // Inserir a nova disciplina na posição correta na lista
            Disciplina *atual = *materia;
            Disciplina *anterior = NULL;

            while (atual != NULL && strcmp(novaDisciplina->nome, atual->nome) > 0) {
                anterior = atual;
                atual = atual->prox;
            }

            if (anterior == NULL) {
                // Inserir no início da lista
                novaDisciplina->prox = *materia;
                *materia = novaDisciplina;
            } else {
                // Inserir no meio ou no final da lista
                anterior->prox = novaDisciplina;
                novaDisciplina->prox = atual;
            }

            printf("Disciplina cadastrada!\n");
    }
    }
    printf("Terminado o cadastro de disciplinas, vamos para o menu principal!\n");
}

void regMat(Disciplina * disciplina) {
    printf("---------------------------------------\n");
    printf("--------Registro de disciplinas--------\n");
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
    printf("Digite o nome da disciplina:");
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

void imprimirMateria(Disciplina materiaAtual) {
    FILE * file = fopen("saida.txt","a");
    fprintf(file,"Nome: %s\n", materiaAtual.nome);
    fprintf(file,"Professor: %s\n", materiaAtual.prof);
    fprintf(file,"Horas totais: %.2f\n", materiaAtual.hora_t);
    fprintf(file,"Horas ministradas: %.2f\n", materiaAtual.hora_c);
    materiaAtual.porc_hora = (materiaAtual.hora_c / materiaAtual.hora_t) * 100;
    fprintf(file,"Porcentagem de aulas ministradas: %.2f%%\n", materiaAtual.porc_hora);
    fprintf(file,"Media da disciplina: %.2f\n", materiaAtual.media);
    if (materiaAtual.notaex || (materiaAtual.nota[0] && materiaAtual.nota[1] && materiaAtual.nota[2])) {
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