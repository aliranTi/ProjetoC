#include "libprojeto.h"

int main(){  

    checkFile();
    
    unsigned int quant_users = (unsigned int)countUsers();

    loginRequest(&quant_users);
    
    return 0;
}