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

void saveDisciplinaFile(Disciplina *materia){
    FILE * arquivo = fopen("DiscDB.b","ab");
    if (arquivo == NULL){
        erroArquivo();
    }
    fwrite(materia,sizeof(Disciplina),1,arquivo);
    fclose(arquivo);
}

Disciplina * recuperarDisciplina(){
    FILE *arquivoDisc = fopen("DiscDB.b", "rb");
    if (arquivoDisc == NULL) {
        erroArquivo();
        return NULL;
    }
    // Ler e reconstruir a lista de disciplinas de cada usuário
    Disciplina *materiaHead = NULL;
    Disciplina *tempDisciplina = NULL;
    while (1) {
        Disciplina *novaDisciplina = (Disciplina *)malloc(sizeof(Disciplina));
        if (novaDisciplina == NULL) {
            // Tratar falha na alocação de memória, se necessário
            break;
        }
        if (fread(novaDisciplina, sizeof(Disciplina), 1, arquivoDisc) != 1) {
            // Se não conseguir ler uma disciplina, sai do loop
            free(novaDisciplina);
            break;
        }
        if(existeDisciplina(materiaHead, novaDisciplina->nome)){
            free(novaDisciplina);
            continue;
        }
        // Adicionar nova disciplina à lista do usuário
        if (materiaHead == NULL) {
            materiaHead = novaDisciplina;
        } else {
            tempDisciplina->prox = novaDisciplina;
        }
        tempDisciplina = novaDisciplina;
    }
    fclose(arquivoDisc);
    return materiaHead;
}

void iniciaListaMateria(Disciplina **materiaHead) {
    Disciplina *materia = (Disciplina *) malloc(sizeof(Disciplina));
    if (materia == NULL) {
        // Tratar falha na alocação de memória, se necessário
        return;
    }
    materia->prox = NULL;
    *materiaHead = materia;
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

int existeDisciplina(Disciplina *materia, const char *nomeDisciplina) {
    Disciplina *atual = materia;
    while (atual != NULL) {
        if (strcmp(atual->nome, nomeDisciplina) == 0) {
            return 1; // A disciplina já existe na lista
        }
        atual = atual->prox;
    }
    return 0; // A disciplina não existe na lista
}

void acessarMateria(Disciplina * materiaHead){
    char inputString[MAX_LENGHT];
    Disciplina * auxDisciplina;
    printf("Digite o nome da matéria deseja acessar: \n");
    scanf(" %[^\n]", inputString);
    if (!checkName(inputString)){
        strcpy(inputString, passaMaiuscula(inputString));
        auxDisciplina = buscaBinariaDisciplina(materiaHead,inputString);
        if(auxDisciplina != NULL) {
            preListMat(auxDisciplina);
        } else {
            printf("Disciplina não encontrada!\n");
            enter();
        }
    }
}

void editarMateria(Disciplina * materiaHead){
    char inputString[MAX_LENGHT];
    Disciplina * auxDisciplina;
    printf("Digite o nome da matéria deseja editar: \n");
    scanf(" %[^\n]", inputString);
    if (!checkName(inputString)){
        strcpy(inputString, passaMaiuscula(inputString));
        auxDisciplina = buscaBinariaDisciplina(materiaHead,inputString);
        if(auxDisciplina != NULL) {
            regMat(auxDisciplina);
        } else {
            printf("Disciplina não encontrada!\n");
            enter();
        }
    }
}

void deletarMateria(Disciplina ** materiaHead){
    char inputString[MAX_LENGHT];
    printf("Digite o nome da matéria deseja excluir: \n");
    scanf(" %[^\n]", inputString);
    if (!checkName(inputString)){
        strcpy(inputString, passaMaiuscula(inputString));
        *materiaHead = excluirMateria(*materiaHead,inputString);
    } else {
        printf("Nome inválido!\n");
        enter();
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
    for (i = 0; i < numDisciplinas; i++) {
        cadDisciplina(materia);
    }
    printf("Terminado o cadastro de disciplinas, vamos para o menu principal!\n");
}

void cadDisciplina(Disciplina ** materia) {
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
        saveDisciplinaFile(novaDisciplina);
        printf("Disciplina cadastrada!\n");
}

void regMat(Disciplina * disciplina) {
    printf("---------------------------------------\n");
    printf("--------Registro de disciplinas--------\n");
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
    printf("Digite o nome da disciplina: ");
    scanf(" %99[^\n]", disciplina->nome); // Lê até 99 caracteres ou até encontrar uma nova linha.
    strcpy(disciplina->nome, passaMaiuscula(disciplina->nome));
    printf("Digite o nome do Professor: ");
    scanf(" %99[^\n]", disciplina->prof); // Mesmo procedimento para o nome do professor.
    strcpy(disciplina->prof, passaMaiuscula(disciplina->prof));
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