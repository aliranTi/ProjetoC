#include "libprojeto.h"

int main(){  
    User * users;
    int current_user, verif_adm;
    users = alocarUser(MAX_USERS);
    criarUsers(users);
    loginRequest(users,&current_user,&verif_adm);

    liberarMateria(users);
    free(users);
    return 0;
}