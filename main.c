/*********************************************************************/
/**                                                                 **/
/**   Raphael Chypriades Junqueira Amarante    Numero USP 9348856   **/
/**   Sistemas de Programacao - PCS 3216                            **/
/**   Professor: Joao Jose Neto                                     **/
/**                                                                 **/
/*********************************************************************/

// bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// definicao de constantes
#define MAX 150
#define TAMANHO 1024

// definicao de tipos

// programas a serem deletados
typedef struct deletado {
   char nome[MAX];
   struct deletado* proximo;
} deletado_t;

// simbolos do montador
typedef struct simbolo {
   char nome[10];
   uint16_t valor;
   struct simbolo* proximo;
} simbolo_t;

// prototipos
void show_map (uint8_t *memoria);
void loop_instrucao (uint16_t CI, uint8_t *memoria, char *nome_arquivo, char *diretorio, uint16_t *numero_bytes);
void push_del(deletado_t* *head, char *nome);
void push_simbolo(simbolo_t* *head, char *nome);
int busca_simbolo(simbolo_t* *head, char *nome);
uint16_t get_valor(simbolo_t* *head, char *leitor);
void montador_1(char *nome_arquivo, uint16_t *inicio, uint8_t *numero_bytes);
void montador_2(char *nome_arquivo, uint16_t *inicio, uint8_t *numero_bytes, simbolo_t* *head);
void carga_loader(char *nome_arquivo, uint8_t *memoria);

// funcoes

// primeiro passo do montador
void montador_1(char *nome_arquivo, uint16_t *inicio, uint8_t *numero_bytes) {

    FILE *programa;
    char leitor[10];
    int aux = 0;
    uint16_t posicao;

    // iniciando lista vazia
    simbolo_t* head = NULL;

    // abre o o arquivo de entrada para leitura
    programa = fopen(nome_arquivo, "r");
    if (programa == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    while (1) {

        aux = fscanf(programa, "%s", leitor);
        if (aux != 1) printf("Erro na leitura do arquivo\n");

        // sai do loop quando acha simbolo de fim de programa
        if(!strcmp(leitor, "#")) break;


        if(!strcmp(leitor, "@")) { // ORIGIN: define endereco inicial
            aux = fscanf(programa, "%04X", inicio);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            posicao = *inicio;
        }

        else if(!strcmp(leitor, "JP")) { // jump incondicional
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "JZ")) { // jump if zero
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "JN")) { // jump if negative
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "CN")) { // instrucao de controle
            // nao faz nada no primeiro passo, apenas avanca na leitura e incrementa posicao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            posicao += 0x1;
        }

        else if(!strcmp(leitor, "+")) { // soma
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "-")) { // subtracao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "*")) { // multiplicacao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "/")) { // divisao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "LD")) { // carrega valor da memoria
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "MM")) { // guarda valor na memoria
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "SC")) { // chamda de subrotina
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            push_simbolo(&head, leitor); // add simbolo ao qual a operacao se refere na lista
            posicao += 0x2;
        }

        else if(!strcmp(leitor, "OS")) { // chamada de sistema operacional
            // nao faz nada no primeiro passo, apenas incrementa posicao
            posicao += 0x1;
        }

        else if(!strcmp(leitor, "IO")) { // input/output/interrupcao
            // nao faz nada no primeiro passo, apenas avanca na leitura e incrementa posicao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            posicao += 0x1;
        }

        else if(!strcmp(leitor, "K")) { // constante
            // nao faz nada no primeiro passo, apenas avanca na leitura e incrementa posicao
            aux = fscanf(programa, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");
            posicao += 0x1;
        }

        else { // simbolos ou labels
            push_simbolo(&head, leitor);
            // associa endereco ao simbolo correto
            simbolo_t* atual = head;
            while(strcmp(atual->nome, leitor)) atual = atual->proximo;
            atual->valor = posicao;
        }
    }

    *numero_bytes = posicao - *inicio;

    // fecha programa
    fclose(programa);

    // chamada do passo 2
    montador_2(nome_arquivo, inicio, numero_bytes, &head);
}

// segundo passo do montador
void montador_2(char *nome_arquivo, uint16_t *inicio, uint8_t *numero_bytes, simbolo_t* *head) {

    FILE *programa_hex;
    FILE *programa_simb;
    char leitor[10];
    char novo_nome[MAX];
    int aux = 0;

    // abre o o arquivo de entrada para leitura
    programa_simb = fopen(nome_arquivo, "r");
    if (programa_simb == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    // monta nome correto do arquivo de escrita
    strncpy(novo_nome, nome_arquivo, strlen(nome_arquivo) - 4);
    strcat(novo_nome, "_hex.txt");

    // abre arquivo para escrita
    programa_hex = fopen(novo_nome, "w");
    if (programa_hex == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    // escreve endereco inicial e numero de bytes
    fprintf(programa_hex, "%02X\n", (*inicio / 0x100));
    fprintf(programa_hex, "%02X\n", (*inicio % 0x100));
    fprintf(programa_hex, "%02X\n", *numero_bytes);

    // descarta primeiras leituras, referentes ao simbolo @ e ao endereco inicial
    aux = fscanf(programa_simb, "%s", leitor);
    if (aux != 1) printf("Erro na leitura do arquivo\n");
    aux = fscanf(programa_simb, "%s", leitor);
    if (aux != 1) printf("Erro na leitura do arquivo\n");


    while (1) {

        aux = fscanf(programa_simb, "%s", leitor);
        if (aux != 1) printf("Erro na leitura do arquivo\n");

        // sai do loop quando acha simbolo de fim de programa
        if(!strcmp(leitor, "#")) break;


        if(!strcmp(leitor, "JP")) { // jump incondicional
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x00 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "JZ")) { // jump if zero
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x10 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "JN")) { // jump if negative
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x20 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "CN")) { // instrucao de controle
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime o byte da operacao
            if(!strcmp(leitor, "HM")) fprintf(programa_hex, "%02X\n", 0x30);
            else if(!strcmp(leitor, "IN")) fprintf(programa_hex, "%02X\n", 0x32);
        }

        else if(!strcmp(leitor, "+")) { // soma
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x40 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "-")) { // subtracao
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x50 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "*")) { // multiplicacao
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x60 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "/")) { // divisao
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x70 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "LD")) { // carrega valor da memoria
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x80 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "MM")) { // guarda valor na memoria
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0x90 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "SC")) { // chamda de subrotina
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime os dois bytes da operacao
            fprintf(programa_hex, "%02X\n", 0xA0 + (get_valor(head, leitor) / 0x100));
            fprintf(programa_hex, "%02X\n", (get_valor(head, leitor) % 0x100));
        }

        else if(!strcmp(leitor, "OS")) { // chamada de sistema operacional
            // imprime o byte da operacao
            fprintf(programa_hex, "%02X\n", 0xB0);
        }

        else if(!strcmp(leitor, "IO")) { // input/output
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime o byte da operacao
            if(!strcmp(leitor, "GD")) fprintf(programa_hex, "%02X\n", 0xC0);
            else if(!strcmp(leitor, "PD")) fprintf(programa_hex, "%02X\n", 0xC4);
        }

        else if(!strcmp(leitor, "K")) { // constante
            // le proximo simbolo
            aux = fscanf(programa_simb, "%s", leitor);
            if (aux != 1) printf("Erro na leitura do arquivo\n");

            // imprime o byte da constante
            fprintf(programa_hex, "%s\n", leitor);
        }

        // else { } -> ignora labels e simbolos quando nao precedidos por mneumonico
    }

    // limpa lista ligda de simbolos para nao conflitar com outros programas
    simbolo_t* atual = *head;
    simbolo_t* proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *head = NULL;

    // fecha programas
    fclose(programa_hex);
    fclose(programa_simb);
}

// retorna valor associado ao simbolo
uint16_t get_valor(simbolo_t* *head, char *leitor) {
    simbolo_t* atual = *head;
    while(strcmp(atual->nome, leitor)) atual = atual->proximo;
    return atual->valor;
}

// adiciona simbolo no final da lista
void push_simbolo(simbolo_t* *head, char *nome) {

    // primeiro da lista
    if(*head == NULL) {
        simbolo_t* novo = (simbolo_t*)malloc(sizeof(simbolo_t));
        if(novo == NULL){
            printf("Nao foi possivel alocar memoria\n");
            exit(-1);
        }
        strcpy(novo->nome, nome);
        novo->proximo = NULL;
        *head = novo;
        return;
    }

    // criando novo item somente se ele ainda nao existir na lista
    if (busca_simbolo(head, nome)) return;

    simbolo_t* novo = (simbolo_t*)malloc(sizeof(simbolo_t));
    if(novo == NULL){
        printf("Nao foi possivel alocar memoria\n");
        exit(-1);
    }

    // definindo novo item
    strcpy(novo->nome, nome);
    novo->proximo = NULL;

    // adicionando no final da lista
    simbolo_t* atual = *head;
    while (atual->proximo != NULL) atual = atual->proximo;
    atual->proximo = novo;
}

// busca simbolo. retorna 1 se ja existe e 0 caso contrario
int busca_simbolo(simbolo_t* *head, char *nome) {
    simbolo_t* atual = *head;
    while (atual != NULL) {
        if(!strcmp(atual->nome, nome)) return 1;
        atual = atual->proximo;
    }
    return 0;
}

// exibe conteudo da memoria em hexadecimal
void show_map (uint8_t *memoria) {

    printf ("Mapa de memoria: \n");
    printf ("      0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F\n");
    int i = 0;
    for (int linha = 0; i < TAMANHO; linha++) {
        printf ("%03X  ", linha);
        for (int coluna = 0; coluna < 16; coluna++, i++) {
            printf ("%02X  ", memoria[i]);
        }
        printf ("\n");
    }
}

// executa instrucao (de 1 ou 2 bytes) de acordo com seu tipo e seu operando
void loop_instrucao (uint16_t CI, uint8_t *memoria, char *nome_arquivo, char *diretorio, uint16_t *numero_bytes) {

    FILE *programa;
    FILE *saida;
    int8_t operacao;
    int16_t operando;
    int8_t acumulador = 0x00; // inicialmente contem zero
    int aux;
    char auxiliar[10];
    int HALT = 0;
    int modo_indireto = 0;
    char arquivo_saida[MAX];

     // abre o arquivo de entrada para leitura
    programa = fopen(nome_arquivo, "r");
    if (programa == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    // monta nome correto do arquivo de escrita
    strcpy(arquivo_saida, diretorio);
    strcat(arquivo_saida, "saida.txt");

    // abre arquivo para escrita
    saida = fopen(arquivo_saida, "w");
    if (saida == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    // descarta primeiras leituras, referentes ao endereco inicial e pega numero de bytes
    aux = fscanf(programa, "%02X", numero_bytes);
    aux = fscanf(programa, "%02X", numero_bytes);
    aux = fscanf(programa, "%02X", numero_bytes);
    if (aux != 1) printf("Erro na leitura do arquivo\n");
    rewind(programa);

    while(!HALT) {

        operacao = (memoria[CI] / 0x10); // identifica o tipo da instrucao
        // identifica o operando para instrucoes de 1 byte
        if (operacao == 0x3 || operacao == 0xB || operacao == 0xC)
            operando = (memoria[CI] % 0x10);
        // identifica o operando para instrucoes de 2 bytes
        else operando = (memoria[CI] % 0x10)*0x100 + memoria[CI + 0x1];

        switch (operacao) {

        case 0x0: // jump incondicional
            if(!modo_indireto) CI = operando;
            else CI = memoria[operando]*0x100 + memoria[operando + 0x1];
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x1: // jump se acumulador for 0
            if (acumulador == 0 && !modo_indireto) CI = operando;
            else if(acumulador == 0) CI = memoria[operando]*0x100 + memoria[operando + 0x1];
            else CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x2: // jump se acumulador for negativo
            if (acumulador < 0 && !modo_indireto) CI = operando;
            else if(acumulador < 0) CI = memoria[operando]*0x100 + memoria[operando + 0x1];
            else CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x3: // instrucao de controle
            switch (operando) {

            case 0x0: // halt machine
                printf("Halt Machine\n\n");
                HALT = 1;
                break;

            case 0x2: // indirect
                modo_indireto = 1;
                CI += 1;
                break;
            }
            break;

        case 0x4: // soma
            if(!modo_indireto) acumulador += memoria[operando];
            else acumulador += memoria[memoria[operando]*0x100 + memoria[operando + 0x1]];
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x5: // subtracao
            if(!modo_indireto) acumulador -= memoria[operando];
            else acumulador -= memoria[memoria[operando]*0x100 + memoria[operando + 0x1]];
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x6: // multiplicacao
            if(!modo_indireto) acumulador *= memoria[operando];
            else acumulador *= memoria[memoria[operando]*0x100 + memoria[operando + 0x1]];
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x7: // divisao
            if(!modo_indireto) acumulador /= memoria[operando];
            else acumulador /= memoria[memoria[operando]*0x100 + memoria[operando + 0x1]];
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x8: // load
            if(!modo_indireto) acumulador = memoria[operando];
            else acumulador = memoria[memoria[operando]*0x100 + memoria[operando + 0x1]];
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0x9: // store
            if(!modo_indireto) memoria[operando] = acumulador;
            else memoria[memoria[operando]*0x100 + memoria[operando + 0x1]] = acumulador;
            CI += 2;
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0xA: // chamada de sub-rotina
            if(!modo_indireto) {
                memoria[operando] = (CI + 0x2) / 0x100;
                memoria[operando + 0x1] = (CI + 0x2) % 0x100;
                CI = operando + 0x2;
            }
            else {
                memoria[memoria[operando]*0x100 + memoria[operando + 0x1]] = (CI + 0x2) / 0x100;
                memoria[(memoria[operando]*0x100 + memoria[operando + 0x1]) + 0x1] = (CI + 0x2) % 0x100;
                CI = (memoria[operando]*0x100 + memoria[operando + 0x1]) + 0x2;
            }
            modo_indireto = 0; // sempre retorna ao enderecamento direto
            break;

        case 0xB: // chamada do sistema operacional
            HALT = 1; // devolve controle de volta ao interpretador
            break;

        case 0xC: // entrada, saida e interrupcao
            switch (operando) {

            case 0x0: // get data (do arquivo.txt para o acumulador)
                fgets(auxiliar, sizeof(auxiliar)-1, programa);
                sscanf(auxiliar, "%02X", &acumulador);
                CI += 1;
                break;

            case 0x4: // put data (do acumulador para o arquivo.txt)
                fprintf(saida, "%02X\n", acumulador);
                CI += 1;
                break;
            }
            break;
        }
    }

    // fecha programas
    fclose(programa);
    fclose(saida);
}

// adiciona programa a ser deletado no fim da lista
void push_del(deletado_t* *head, char *nome) {

    // primeiro da lista
    if(*head == NULL) {
        deletado_t* novo = (deletado_t*)malloc(sizeof(deletado_t));
        if(novo == NULL){
            printf("Nao foi possivel alocar memoria\n");
            exit(-1);
        }
        strcpy(novo->nome, nome);
        novo->proximo = NULL;
        *head = novo;
        return;
    }

    // criando novo item
    deletado_t* novo = (deletado_t*)malloc(sizeof(deletado_t));
    if(novo == NULL){
        printf("Nao foi possivel alocar memoria\n");
        exit(-1);
    }

    // definindo novo item
    strcpy(novo->nome, nome);
    novo->proximo = NULL;

    // adicionando no final da lista
    deletado_t* atual = *head;
    while (atual->proximo != NULL) atual = atual->proximo;
    atual->proximo = novo;
}

// faz a carga do loader na memoria
void carga_loader(char *nome_arquivo, uint8_t *memoria) {
    FILE *programa;

    programa = fopen(nome_arquivo, "r");
    if (programa == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    // le arquivo e coloca na memoria
    for (int i = 0; i < 44; i++) fscanf(programa, "%02X", &memoria[0x0 + i]);

    // fecha programa
    fclose(programa) ;
}

int main()
{
    // criando variaveis necessarias
    uint8_t numero_bytes = 0x00;
    uint16_t inicio = 0x0000;

    char comando[MAX];
    char nome_arquivo[MAX];
    char diretorio[MAX];

    uint8_t memoria[TAMANHO] = {0x00}; // cada posicao de memoria representa um byte e todas sao iniciadas com 0
    uint16_t CI = 0x0000; // contador de instrucoes (16 bits); inicialmente aponta pra 0

    // iniciando lista vazia
    deletado_t* head = NULL;

    printf("Identifique o usuario (nome do diretorio completo onde estao os programas): \n");
    printf("***Ex.: /Users/raphael-amarante/Desktop/projeto/*** \n");

    fgets(diretorio, MAX, stdin);
    if(diretorio[strlen(diretorio) - 1] == '\n')
        diretorio[strlen(diretorio) - 1] = NULL; // necessario retirar newline

    // inicializacao: carregando o loader na memoria
    strcpy(nome_arquivo, diretorio);
    strcat(nome_arquivo, "loader_hex.txt");
    carga_loader(nome_arquivo, memoria);

    while (1) {

        printf("\nDigite um dos seguintes comandos:\n");
        printf("$DIR - lista programas disponiveis no sistema\n");
        printf("$DEL <nome> - remove programa do sistema\n");
        printf("$RUN <nome> - carrega na memoria e executa programa hexadecimal\n");
        printf("$MTD <nome> - montador: gera programa hexadecimal a partir do simbolico\n");
        printf("$MAP - exibe mapa de memoria\n");
        printf("$END - encerra sistema\n");
        printf("***Ex. de <nome>: programa.txt*** \n\n");

        fgets(comando, MAX, stdin);
        if(comando[strlen(comando) - 1] == '\n')
            comando[strlen(comando) - 1] = NULL; // necessario retirar newline


        if (!strncmp(comando, "$DIR", 4)) {

            printf("Arquivos iniciados com 0 sao os que serao apagados quando o sistema for finalizado\n");

            struct dirent *de;  // ponteiro de entrada do diretorio

            DIR *dr = opendir(diretorio);

            if (dr == NULL) {
                printf("Nao foi possivel abrir o diretorio\n" );
                return 0;
            }

            while ((de = readdir(dr)) != NULL) {
                if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || !strcmp(de->d_name, ".DS_Store")) {
                    /*NOP*/
                }
                // "." representa o diretorio atual
                //".." representa o diretorio "pai"
                // ".DS_Store" representa um arquivo gerado pelo OS da maquina hospedeira
                else printf("%s\n", de->d_name);
            }

            closedir(dr);
        }

        else if (!strncmp(comando, "$DEL", 4)) {

            // prepara nome completo do arquivo + diretorio
            strcpy(nome_arquivo, diretorio);
            strcat(nome_arquivo, comando + 5);

            // prepara novo nome completo (adiciona-se "0" antes do nome do arquivo)
            char novo_nome[MAX];
            strcpy(novo_nome, diretorio);
            strcat(novo_nome, "0");
            strcat(novo_nome, comando + 5);

            // renome para posterior remocao definitiva do programa
            if(!rename(nome_arquivo, novo_nome)) {
                printf("Nome do arquivo a ser deletado foi modificado com sucesso\n");
                push_del(&head, novo_nome);
            }
            else printf("Erro na operacao\n");
        }

        else if (!strncmp(comando, "$RUN", 4)) {

            // prepara nome completo do arquivo + diretorio
            strcpy(nome_arquivo, diretorio);
            strcat(nome_arquivo, comando + 5);

            // executa loader para carga do programa na memoria
            loop_instrucao (CI, memoria, nome_arquivo, diretorio, &numero_bytes);

            // seta CI para inicio do programa a executar
            CI = memoria[0x0028]*0x100 + memoria[0x0029] - numero_bytes;

            // executa programa
            loop_instrucao (CI, memoria, nome_arquivo, diretorio, &numero_bytes);
        }

        else if (!strncmp(comando, "$MTD", 4)) {
            // prepara nome completo do arquivo + diretorio
            strcpy(nome_arquivo, diretorio);
            strcat(nome_arquivo, comando + 5);
            montador_1(nome_arquivo, &inicio, &numero_bytes);
        }

        else if (!strncmp(comando, "$MAP", 4)) {
            show_map(memoria);
        }

        else if (!strncmp(comando, "$END", 4)) {
            deletado_t* atual = head;
            while (atual != NULL) {
                if (!remove(atual->nome))
                    printf("Programa removido com sucesso\n");
                else printf("Nao foi possivel deletar o arquivo\n");

                atual = atual->proximo;
            }
            break;
        }
    }
    return 0;
}
