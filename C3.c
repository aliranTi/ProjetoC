#include "libprojeto.h"

int main(){  

    int verif_adm;
    int quant_users = countUsers();
    // users = recuperarUsers(&quant_users);
    //criarUsers(users);
    loginRequest(&quant_users,&verif_adm);

    
    return 0;
}