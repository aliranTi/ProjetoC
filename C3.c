#include "libprojeto.h"

int main(){  

    int verif_adm;
    checkFile();
    unsigned int quant_users = (unsigned int)countUsers();
    // users = recuperarUsers(&quant_users);
    //criarUsers(users);
    loginRequest(&quant_users,&verif_adm);
    
    return 0;
}