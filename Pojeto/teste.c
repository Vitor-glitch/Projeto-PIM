#include <stdlib.h>  // Necessário para usar system()
#include <stdio.h>
#include <windows.h>
# include <string.h>
# include <conio.h>


//*************************VARIÁVEIS GLOBAIS****************************************************** */
    #define N 20                   
    char USUARIO[N] = {"admin"};   //VARIÁVEIS PARA A FUNÇÃO "acesso_padrao"
    char SENHA[N] = {"admin"};
    const char *nomeArquivo = "produtos.bin";
 //*********************************STRUCT PARA CADASTO DE PRODUTOS********************************************************* */

struct Prod {
    int cod;
    char nome[20];
    float preco;
};

typedef struct Prod prod;

   
  /************************DEclaração das Funções*********************************/
   
    void menu_principal();
    void caixa(const char *nomeArquivo);
    void cadastroDeProdutos(const char *nomeArquivo);
    void estoque();
    void cadastro_usuario();
    void acesso_padrao();
    void logo();
    void cadastro_fornecedor();

/******************FUNÇÃO PRINCIPAL***************************/

   int main() {

  
  SetConsoleOutputCP(CP_UTF8); // Habilita o teclado para português
  
   logo();
   acesso_padrao();

 
        
   }

   void acesso_padrao()
   
   
   
   {
         

        system("cls");

       

        char usuario[10], senha[10], ch;
        int i = 0;

    printf("Insira o nome de usuário: ");
    scanf("%s",usuario);
    fflush;
   

    printf("Insira a senha: ");
    
    // Ocultando a senha com *
    while ((ch = getch()) != '\r') {  // '\r' é o Enter no Windows
        senha[i++] = ch;
        printf("*");
    }
    senha[i] = '\0';  // Finalizando a string da senha
    fflush;
    

    // Validando o login
    if (strcmp(usuario, USUARIO) == 0 && strcmp(senha, SENHA) == 0) {
        menu_principal();

    } else {
        printf("\nCredenciais incorretas!\n");
        Sleep(1000);
        acesso_padrao();
    }

   }


    void logo()
    {
        FILE *Logo;  // Ponteiro para o arquivo
    char linha[256];  // Buffer para armazenar cada linha

    // Abre o arquivo ASCII_art.txt em modo de leitura
    Logo = fopen("Logo.txt", "r");
 

    // Lê o arquivo linha por linha e imprime no console
    while (fgets(linha, sizeof(linha), Logo) != NULL) {
        printf("%s", linha);  // Imprime cada linha lida
    }

    // Fecha o arquivo
    fclose(Logo);
    Sleep(2000);
   system("cls");
   

             }

 void menu_principal(){
    fflush;
    system("cls");
    int op;
    do
    {
      printf("********** MENU PRINCIPAL **********\n");
      printf(" 1. Caixa\n 2. Cadastro de produtos\n 3. Estoque e Fornecedores\n 4. Cadastro de usuários\n 0. Sair");
      printf("\nInsira uma opção: ");
      scanf("%d",&op);
      fflush;

     switch (op) {
            case 1:
                caixa(nomeArquivo);
                break;
            case 2:
                cadastroDeProdutos(nomeArquivo);
                break;
            case 3:
                estoque();
                break;
            case 4:
                cadastro_usuario();
                break;   
            case 0:
                exit(0);
                break;     
            default:
                printf("Opção inválida!\n");
                Sleep(1000);
                system("cls");
            }
        
    } while (op <0 || op > 5);
    




 }  

 

void caixa(const char *nomeArquivo){

    // Abre o arquivo para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Arquivo de produtos não encontrado.\n");
        return;
    }

    // Descobre o número de produtos armazenados
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    rewind(arquivo);
    int numProdutos = tamanhoArquivo / sizeof(prod);

    // Carrega os produtos para memória
    prod *produtos = (prod *)malloc(numProdutos * sizeof(prod));
    if (produtos == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        exit(1);
    }

    fread(produtos, sizeof(prod), numProdutos, arquivo);
    fclose(arquivo);

    // Módulo de caixa
    float total = 0.0;
    int codigo, encontrado;
    char continuar;

    printf("\n--- Módulo de Caixa ---\n");
    do {
        printf("Digite o código do produto (ou 0 para encerrar): ");
        scanf("%d", &codigo);

        if (codigo == 0) {
            break;
        }

        // Busca pelo produto no array
        encontrado = 0;
        for (int i = 0; i < numProdutos; i++) {
            if (produtos[i].cod == codigo) {
                printf("Produto encontrado: %s, Preço: %.2f\n", produtos[i].nome, produtos[i].preco);
                total += produtos[i].preco;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Produto com código %d não encontrado.\n", codigo);
        }

        printf("Deseja continuar? (s/n): ");
        scanf(" %c", &continuar);

    } while (continuar == 's' || continuar == 'S');

    // Exibe o total final
    printf("\n--- Finalização da Compra ---\n");
    printf("Total a pagar: %.2f\n", total);

    // Libera a memória alocada
    free(produtos);
    menu_principal();


  
    
}

void cadastroDeProdutos(const char *nomeArquivo){

    int n;
    printf("Quantos produtos deseja cadastrar? ");
    scanf("%d", &n);

    // Aloca memória para os produtos
    prod *produtos = (prod *)malloc(n * sizeof(prod));
    if (produtos == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Cadastro dos produtos
    for (int i = 0; i < n; i++) {
        printf("\nProduto %d:\n", i + 1);
        printf("Código: ");
        scanf("%d", &produtos[i].cod);

        printf("Nome: ");
        scanf(" %19[^\n]", produtos[i].nome);

        printf("Preço: ");
        scanf("%f", &produtos[i].preco);
    }

    // Salva os produtos no arquivo binário
    FILE *arquivo = fopen(nomeArquivo, "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(produtos);
        exit(1);
    }

    fwrite(produtos, sizeof(prod), n, arquivo);
    fclose(arquivo);

    printf("\nProdutos cadastrados e salvos no arquivo com sucesso.\n");
    free(produtos);
    menu_principal();




    }


void estoque(){
    
    int op2;
    do
    {
        system("cls");
        printf("1.Cadastrar Fornecedor\n2. Consultar Estoque\n3. Voltar ao Menu Principal\nInsira uma opção: ");
        scanf("%d",&op2);
        switch (op2)
        {
        case 1:
            printf("Teste");
            break;

        case 2:
            printf("Teste");
            break;

        case 3:
            system("cls");
            menu_principal();
            break;
        
        default:
        printf("Opção inválida!\n");
                Sleep(1000);
                system("cls");    
            break;
        }


        
    } while (op2 < 0 || op2 > 3);
    

}


void cadastro_usuario(){

    printf("Módulo cadastro de usuáruos");
}












