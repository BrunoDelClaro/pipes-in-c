#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>


enum PIPES { READ, WRITE }; /* Constants 0 and 1 for READ and WRITE */



#define OK 1
#define ERR 0

#define NO_ITEM 2

#define NO_DIN 3


typedef struct{
    unsigned int prc;
    char nome[24];
    char desc[256];
    char prop_mag[256];
    char raridade[12];
}_t_item;

_t_item cria_item(unsigned int prc, char* nome,char* desc, char* prop_mag, char* raridade);


void print_item(char* buff,_t_item in_item){
    char* aux[256];
    memset(aux,0,strlen(aux));
    sprintf(aux,"\nNome:%s\nDescricao:%s\nPropriedade Magica:%s\nRaridade:%s\nPreco:%d\n",in_item.nome,in_item.desc,in_item.prop_mag,in_item.raridade,in_item.prc);
    strcat(buff,aux);
}

int buy_item(const char* nome){

    char c;
    char aux[1024];
    char* tmp[1024];

    int pos = 0;
    int found = 0;
    int rs=0;

    FILE *arq_loja;

    FILE *f_tmp;

    _t_item *item = malloc(sizeof(_t_item));

    printf("Procurando:%s\n",nome);




    arq_loja = fopen("loja.csv","r");

    if(arq_loja == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    f_tmp = fopen("auxLoja_buy.csv","w");

    if(f_tmp == NULL){
        printf ("Erro ao criar arquivo de backup\n");
        exit(1);
    }

    fseek(arq_loja,0,SEEK_SET);


    while((c = getc(arq_loja)) !=  EOF){



        if(c == ',' || c == ';'|| c == '\n'){
            switch(pos){
            case 0:
                strcpy(item->nome,aux);
                break;
            case 1:
                strcpy(item->desc,aux);
                break;
            case 2:
                strcpy(item->prop_mag,aux);
                break;
            case 3:
                strcpy(item->raridade,aux);
                break;
            case 4:
                item->prc = atoi(aux);
                break;

            }
            memset(aux,0,strlen(aux));
            pos++;

            if(c == ';'){

                if(strcmp(item->nome,nome) == 0 && found != 1){
                    printf("item : %s\n",item->nome);
                    found = 1;
                    rs =add_item_inv(*item);
                    if(rs == NO_DIN){
                        return NO_DIN;

                    }
                }else{
                    sprintf(tmp,"%s,%s,%s,%s,%d;\n",item->nome,item->desc,item->prop_mag,item->raridade,item->prc);
                    fputs(tmp,f_tmp);

                }

            }else if( c == '\n'){
                pos = 0;
            }

        }else{
            strncat(aux,&c,1);

        }



    }

    if(found){



        fclose(arq_loja);
        fclose(f_tmp);

        change_din(rs);

        remove("loja.csv");
        rename("auxLoja_buy.csv","loja.csv");

        return OK;

    }else{
        fclose(arq_loja);
        fclose(f_tmp);
        return NO_ITEM;

    }

}

int sell_item(const char* nome){

    char c;
    char aux[1024];
    char* tmp[1024];
    int pos = 0;
    int rs= 0;
    int found = 0;

    FILE *arq_inv;

    FILE *f_tmp;

    _t_item *item = malloc(sizeof(_t_item));

    printf("Procurando:%s\n",nome);


    arq_inv = fopen("inventario.csv","r");

    if(arq_inv == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }
    f_tmp = fopen("auxInventario_sell.csv","w");

    if(f_tmp == NULL){
        printf ("Erro ao criar arquivo de backup\n");
        exit(1);
    }

    fseek(arq_inv,0,SEEK_SET);


    while((c = getc(arq_inv)) !=  EOF){


        if(c == ',' || c == ';'|| c == '\n'){
            switch(pos){
            case 0:
                strcpy(item->nome,aux);
                break;
            case 1:
                strcpy(item->desc,aux);
                break;
            case 2:
                strcpy(item->prop_mag,aux);
                break;
            case 3:
                strcpy(item->raridade,aux);
                break;
            case 4:
                item->prc = atoi(aux);
                break;

            }
            memset(aux,0,strlen(aux));
            pos++;

            if(c == ';'){
                if(strcmp(item->nome,nome) == 0 && found != 1){
                    printf("item:%s\n",item->nome);
                    found = 1;
                    rs = add_item_loja(*item);

                }else{
                    sprintf(tmp,"%s,%s,%s,%s,%d;\n",item->nome,item->desc,item->prop_mag,item->raridade,item->prc);
                    fputs(tmp,f_tmp);

                }

            }else if( c == '\n'){
                pos = 0;
            }

        }else{
            strncat(aux,&c,1);

        }



    }



    if(found){



        fclose(arq_inv);
        fclose(f_tmp);

        change_din(rs);

        remove("inventario.csv");
        rename("auxInventario_sell.csv","inventario.csv");

        return OK;

    }else{
        fclose(arq_inv);
        fclose(f_tmp);
        return NO_ITEM;

    }




}

int get_din(){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_inv;

     _t_item *item = malloc(sizeof(_t_item));

    arq_inv = fopen("inventario.csv","r");

    if(arq_inv == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    fseek(arq_inv,0,SEEK_SET);


    while((c = getc(arq_inv)) !=  EOF){

        if(c == ',' || c == ';'|| c == '\n'){
            switch(pos){
            case 0:
                strcpy(item->nome,aux);
                break;
            case 1:
                strcpy(item->desc,aux);
                break;
            case 2:
                strcpy(item->prop_mag,aux);
                break;
            case 3:
                strcpy(item->raridade,aux);
                break;
            case 4:
                item->prc = atoi(aux);
                break;

            }
            memset(aux,0,strlen(aux));
            pos++;

            if(c == ';'){

                fclose(arq_inv) ;
                return item->prc;

            }else if( c == '\n'){
                pos = 0;
            }

        }else{
            strncat(aux,&c,1);

        }

    }


    fclose(arq_inv);

    return ERR;



}


int change_din(int din){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_inv;
    FILE *f_tmp;

    _t_item *item = malloc(sizeof(_t_item));



    arq_inv = fopen("inventario.csv","r");

    if(arq_inv == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    fseek(arq_inv,0,SEEK_SET);


    while((c = getc(arq_inv)) !=  EOF){



        if(c == '\n'){

            f_tmp = fopen("auxInventario.csv","w");

            if(f_tmp == NULL){
                printf ("Erro ao criar arquivo de backup\n");
                exit(1);
            }

            *item = cria_item(din,"CareCoins","Usado pra compra bugigangas","Vale mais do que dinheiro","Comum");

            sprintf(aux,"%s,%s,%s,%s,%d;\n",item->nome,item->desc,item->prop_mag,item->raridade,item->prc);

            fputs(aux,f_tmp);

            while((c = getc(arq_inv)) !=  EOF){

                fputc(c,f_tmp);

            }
            fclose(arq_inv);
            fclose(f_tmp);
            remove("inventario.csv");
            rename("auxInventario.csv","inventario.csv");

            return OK;


        }
    }
    fclose(arq_inv);

    return ERR;



}

int add_item_loja(_t_item item){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_loja;


    int din = get_din();

    int rs = din+item.prc;


    arq_loja = fopen("loja.csv","a");

    if(arq_loja == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    sprintf(aux,"%s,%s,%s,%s,%d;\n",item.nome,item.desc,item.prop_mag,item.raridade,item.prc);

    fputs(aux,arq_loja);

    fclose(arq_loja);

    return rs;





}

int add_item_inv(_t_item item){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_inv;


    int din = get_din();

    int rs = din-item.prc;

    if(rs<0){
        printf("rs %d\n",rs);
        return NO_DIN;

    }else{



        arq_inv = fopen("inventario.csv","a");

        if(arq_inv == NULL){
            printf("Erro ao abrir arquivo\n");
            return;

        }

        sprintf(aux,"%s,%s,%s,%s,%d;\n",item.nome,item.desc,item.prop_mag,item.raridade,item.prc);

        fputs(aux,arq_inv);

        fclose(arq_inv);

        return rs;
    }




}

void get_inventario(char* buff){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_inv;

    _t_item *item = malloc(sizeof(_t_item));

    memset(buff,0,strlen(buff));


    arq_inv = fopen("inventario.csv","r");

    if(arq_inv == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    fseek(arq_inv,0,SEEK_SET);


    while((c = getc(arq_inv)) !=  EOF){



        if(c == ',' || c == ';'|| c == '\n'){
            switch(pos){
            case 0:
                strcpy(item->nome,aux);
                break;
            case 1:
                strcpy(item->desc,aux);
                break;
            case 2:
                strcpy(item->prop_mag,aux);
                break;
            case 3:
                strcpy(item->raridade,aux);
                break;
            case 4:
                item->prc = atoi(aux);
                break;

            }
            memset(aux,0,strlen(aux));
            pos++;

            if(c == ';'){

                print_item(buff,*item);


            }else if( c == '\n'){
                strcat(buff,"---------------- ------------------");
                pos = 0;
            }

        }else{
            strncat(aux,&c,1);

        }



    }

    fclose(arq_inv);



}

void get_loja(char* buff){

    char c;
    char aux[1024];
    int pos = 0;

    FILE *arq_loja;

    _t_item *item = malloc(sizeof(_t_item));

    memset(buff,0,strlen(buff));


    arq_loja = fopen("loja.csv","r");

    if(arq_loja == NULL){
        printf("Erro ao abrir arquivo\n");
        return;

    }

    fseek(arq_loja,0,SEEK_SET);


    while((c = getc(arq_loja)) !=  EOF){



        if(c == ',' || c == ';'|| c == '\n'){
            switch(pos){
            case 0:
                strcpy(item->nome,aux);
                break;
            case 1:
                strcpy(item->desc,aux);
                break;
            case 2:
                strcpy(item->prop_mag,aux);
                break;
            case 3:
                strcpy(item->raridade,aux);
                break;
            case 4:
                item->prc = atoi(aux);
                break;

            }
            memset(aux,0,strlen(aux));
            pos++;

            if(c == ';'){

                print_item(buff,*item);


            }else if( c == '\n'){
                strcat(buff,"---------------- ------------------");
                pos = 0;
            }

        }else{
            strncat(aux,&c,1);

        }



    }

    fclose(arq_loja);



}


_t_item cria_item(unsigned int prc, char* nome,char* desc, char* prop_mag, char* raridade){


    _t_item *item = malloc(sizeof(_t_item));

    strcpy(item->nome,nome);
    strcpy(item->desc,desc);
    strcpy(item->prop_mag,prop_mag);
    strcpy(item->raridade,raridade);
    item->prc = prc;

    return *item;



}

