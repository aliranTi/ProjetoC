#include "libprojeto.h"

int containsAtSymbol(char *string){ 
    return strchr(string, '@') != NULL;  
    // Retorna 1 (verdadeiro) se '@' estiver na string, caso contrario retorna 0 (falso)
}

void erroArquivo(){
    printf("erro no arquivo!\n");
    exit(1);
}

char *passaMaiuscula(char *palavra) {
    int i;
    for (i = 0; (unsigned)i < strlen(palavra); i++) {
        palavra[i] = (char)toupper(palavra[i]);
    }
    return palavra;
}

int verificaCaracteresEspeciais(char *string)
{
    int i;
    for (i = 0; (unsigned)i < strlen(string); i++)
    {
        if ((string[i] >= 33 && string[i] <= 47) || (string[i] >= 58 && string[i] <= 64) || (string[i] >= 91 && string[i] <= 96) || (string[i] >= 123 && string[i] <= 126))
        {
            return 1;
        }
    }
    return 0;
}

int verificaNumero(char *string)
{
    int i;
    for (i = 0; (unsigned)i < strlen(string); i++)
    {
        if (string[i] >= 48 && string[i] <= 57)
        {
            return 1;
        }
    }
    return 0;
}

int checkName(char * string){
    return verificaNumero(string) || verificaCaracteresEspeciais(string);
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

void checkFile(){
    FILE * arquivo = fopen("UserDB.b","rb");
    
    if (arquivo == NULL) {
        printf("Arquivo Invalido!\nRecriando...\n");
        fclose(arquivo);
        arquivo = fopen("UserDB.b","wb");
    }
    fclose(arquivo);
    arquivo = fopen("DiscDB.b","rb");
    
    if (arquivo == NULL) {
        printf("Arquivo Invalido!\nRecriando...\n");
        fclose(arquivo);
        arquivo = fopen("DiscDB.b","wb");
    }
    fclose(arquivo);
}

void enter(){
    printf("\nAperte 'ENTER' para continuar\n");
    while (getchar() != '\n');      // Limpa o buffer, consumindo o '\n' anterior (se houver)
    getchar(); // Aguarda o usuario pressionar ENTER
    #if __linux__
	    system("clear");
	#elif _WIN32
	    system("cls");
	#endif
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