#include "libprojeto.h"

int main(){  

    int verif_adm;
    checkFile();
    
    unsigned int quant_users = (unsigned int)countUsers();

    loginRequest(&quant_users,&verif_adm);
    
    return 0;
}