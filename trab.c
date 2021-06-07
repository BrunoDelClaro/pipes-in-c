// crt_pipe.c
/* This program uses the _pipe function to pass streams of
* text to spawned processes.
*/
#include "trab.h"

int main( int argc, char *argv[] )
{

    int pipe1[2];
    int pipe2[2];
    char hstr[20];
    char aux[20];
    int pid, problem, c;
    int termstat;
    char buff[2048];





    /* If no arguments, this is the spawning process */
    if( argc == 1 )
    {

        setvbuf( stdout, NULL, _IONBF, 0 );

        /* Open a set of pipes */
        if( _pipe( pipe1, 256, O_BINARY ) == -1 || _pipe( pipe2, 256, O_BINARY ) == -1 )
            exit( 1 );

        /* Convert pipe read descriptor to string and pass as argument
         * to spawned program. Program spawns itself (argv[0]).
         */
        _itoa_s( pipe1[READ], hstr, sizeof(hstr), 10 );
        _itoa_s( pipe2[WRITE], aux, sizeof(aux), 10 );
        if( ( pid = _spawnl( P_NOWAIT, argv[0], argv[0],
                             hstr,aux, NULL ) ) == -1 )
            printf( "Spawn failed" );

        /* Put problem in write pipe. Since spawned program is
         * running simultaneously, first solutions may be done
         * before last problem is given.
         */
        while(1)
        {
            printf("\nInsira um comando:");
            gets(buff);
            _write( pipe1[WRITE], buff, 1024);
            _read( pipe2[READ],buff , 2048);
            printf("\n%s",buff);

        }





        /* Wait until spawned program is done processing. */
        _cwait( &termstat, pid, WAIT_CHILD );
        if( termstat & 0x0 )
            printf( "Child failed\n" );

        _close( pipe1[READ] );
        _close( pipe1[WRITE] );
        _close( pipe2[READ] );
        _close( pipe2[WRITE] );

    }

    /* If there is an argument, this must be the spawned process. */
    else
    {

        /* Convert passed string descriptor to integer descriptor. */
        pipe1[READ] = atoi( argv[1] );
        pipe2[WRITE] = atoi( argv[2] );

        /* Read problem from pipe and calculate solution. */
        while(1)
        {
            char aux2[2048];
            _read( pipe1[READ], buff, sizeof(char) * 1024 );
            if(strcmp(buff,"!loja") == 0){
                printf("Voce selecionou loja\n");
                get_loja(aux2);
                _write( pipe2[WRITE],aux2,sizeof(aux2)*2048);
            }else if (strncmp(buff,"!buy ",5) == 0){
                printf("Voce selecionou comprar\n");
                char* tmp = strtok(buff," ");
                tmp = strtok(NULL," ");
                int res = buy_item(tmp);

                switch(res){
                case ERR:
                    strcpy(aux2,"ERRO");
                    break;
                case OK:
                    strcpy(aux2,"OK");
                    break;
                case NO_ITEM:
                    strcpy(aux2,"ITEM NÃO ENCONTRADO");
                    break;
                case NO_DIN:
                    strcpy(aux2,"SEM GRANA IRMAO");
                    break;
                }
                _write( pipe2[WRITE],aux2,sizeof(aux)*2048);

            }else if (strncmp(buff,"!sell ",6) == 0){
                printf("Voce selecionou vender\n");
                char* tmp = strtok(buff," ");
                tmp = strtok(NULL," ");
                int res = sell_item(tmp);

                switch(res){
                case ERR:
                    strcpy(aux2,"ERRO");
                    break;
                case OK:
                    strcpy(aux2,"OK");
                    break;
                case NO_ITEM:
                    strcpy(aux2,"ITEM NÃO ENCONTRADO");
                    break;
                case NO_DIN:
                    strcpy(aux2,"SEM GRANA IRMAO");
                    break;
                }
                _write( pipe2[WRITE],aux2,sizeof(aux)*2048);

            }else if(strcmp(buff,"!clear") == 0){
                system("cls");
                _write( pipe2[WRITE],"",1);

            }else if(strcmp(buff,"!inv") == 0){
                printf("Voce selecionou Inventario\n");
                get_inventario(aux2);
                _write( pipe2[WRITE],aux2,sizeof(aux2)*2048);

            }else if(strcmp(buff,"!help") == 0){
                printf("Precisa de ajuda? Segue a Lista\n");
                strcpy(aux2,"!loja - Lista a loja\n!inv - Lista o seu inventario\n!buy [NOME] - Compra o item selecionado\n!sell [NOME] - Vende o item selecionado\n!clear - Limpa a tela\n!help - help?");
                _write( pipe2[WRITE],aux2,sizeof(aux2)*2048);

            }else if(strcmp(buff,"!setdin") == 0){
                printf("Comando Proibido\n");
                int res = change_din(2000);

                switch(res){
                case 0:
                    strcpy(aux2,"ERRO");
                    break;
                case 1:
                    strcpy(aux2,"OK");
                    break;
                case 2:
                    strcpy(aux2,"ITEM NÃO ENCONTRADO");
                    break;


                }
                _write( pipe2[WRITE],aux2,sizeof(aux2)*2048);

            }else{

                strcpy(aux2,"Comando nao encontrado, use !help para listar os comandos\n");
                _write( pipe2[WRITE],aux2,sizeof(aux)*2048);

            }


        }
    }
}



