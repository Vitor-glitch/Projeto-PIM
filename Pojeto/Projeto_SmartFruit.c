#include <stdlib.h>  // Necessário para usar system()
#include <stdio.h>
#include <windows.h>
# include <string.h>
# include <conio.h>
#include <stdbool.h>


//*************************VARIÁVEIS GLOBAIS****************************************************** */
    #define N 20    
    int quant=0;               
    char USUARIO[N] = {"admin"};   //VARIÁVEIS PARA A FUNÇÃO "acesso_padrao"
    char SENHA[N] = {"admin"};
    
    const char *nomeArquivo = "registros.bin";
    bool p_acesso = true;
    int quantusuarios = 0;
    int MAX_USUARIOS = 3;
 //*********************************STRUCT PARA CADASTO DE PRODUTOS********************************************************* */

struct Prod {
    int cod;
    char nome[20];
    float preco;
    int quantidade;
    char fornecedor[50]; 
    
};

typedef struct Prod prod;

//********************************************STRUCT PARA FORNECEDORES************************************************************* */
    typedef struct {
    char nome[50];   // Nome do fornecedor
    char cnpj[15];   // CNPJ do fornecedor
} fornecedor;
/********************************************STRUCT PARA USUÁRIOS************************************************************************ */
 struct Usuarios {
    char usuario_[50]; // Nome do usuário
    char senha_[50];   // Senha do usuário
};
typedef struct Usuarios usuarios;
 
  /************************Protótipos das Funções*********************************/
   
    void menu_principal();
    void caixa(const char *nomeArquivo);
    void cadastroDeProdutos(const char *nomeArquivo);
    void atualizarEstoque(const char *nomeArquivo);
    void estoque();
    void cadastro_usuario();
    void acesso_padrao();
    void logo();
    void cadastro_fornecedor();
    void login();
    void cadastroDeFornecedores(const char *nomeArquivo);
    void busca_produtos(const char *nomeArquivo);

/******************FUNÇÃO PRINCIPAL***************************/

   int main() {

  
  SetConsoleOutputCP(CP_UTF8); // Habilita o teclado para português
  
   logo();

   FILE *arquivo = fopen("usuarios.bin", "rb");
   fread(&p_acesso, sizeof(bool), 1, arquivo);
   fclose(arquivo);
   if(p_acesso == false || arquivo == NULL){
   acesso_padrao();
   }
    else
    {
        login();
    }
    
        
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

    system("cls");
    int op;
    do
    {
      printf("********** MENU PRINCIPAL **********\n");
      printf(" 1. Caixa\n 2. Cadastro de produtos\n 3. Estoque e Fornecedores\n 4. Cadastro de usuários\n 0. Sair");
      printf("\nInsira uma opção: ");
      scanf("%d",&op);
      while (getchar() != '\n');

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

    system("cls");
    printf("\n--- Módulo de Caixa ---\n");
    do {
        printf("Digite o código do produto (ou 0 para encerrar): ");
        scanf("%d", &codigo);
        while (getchar() != '\n');

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
        while (getchar() != '\n');


    } while (continuar == 's' || continuar == 'S');

    
    system("cls");
    printf("\n--- Finalização da Compra ---\n");
    printf("Total a pagar: R$ %.2f\n", total);
    free(produtos);
    
    int c;
    do {
        c = getchar();
    } while (c != '\n'); 
    
    menu_principal();
     
}

void cadastroDeProdutos(const char *nomeArquivo){

     int n;
    printf("Quantos produtos deseja cadastrar? ");
    scanf("%d", &n);
    while (getchar() != '\n'); 

    prod *produtos = (prod *)malloc(n * sizeof(prod));
    if (produtos == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        printf("\nProduto %d:\n", i + 1);
        printf("Código: ");
        scanf(" %d", &produtos[i].cod);
        while (getchar() != '\n'); 

        printf("Nome: ");
        scanf(" %19[^\n]", produtos[i].nome);
        while (getchar() != '\n'); 

        printf("Preço: ");
        scanf(" %f", &produtos[i].preco);
        while (getchar() != '\n'); 

        printf("Quantidade no estoque: ");
        scanf(" %d", &produtos[i].quantidade);
        while (getchar() != '\n'); 

        printf("Fornecedor (nome): ");
        scanf(" %49[^\n]", produtos[i].fornecedor);
        while (getchar() != '\n'); 
    }

    FILE *arquivo = fopen(nomeArquivo, "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(produtos);
        exit(1);
    }

    // Escreve os produtos no arquivo
    for (int i = 0; i < n; i++) {
        char tipo = 'P'; // Tipo para indicar que é um produto
        fwrite(&tipo, sizeof(char), 1, arquivo);
        fwrite(&produtos[i], sizeof(prod), 1, arquivo);
    }

    fclose(arquivo);
    system("cls");
    printf("\nProdutos cadastrados e salvos no arquivo com sucesso.\n");
    Sleep(1500);
    free(produtos);
}


void atualizarEstoque(const char *nomeArquivo){

 int codigo, novaQuantidade;
    int encontrado = 0;

    printf("Digite o código do produto que deseja atualizar: ");
    scanf("%d", &codigo);

    FILE *arquivo = fopen(nomeArquivo, "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    prod produto;
    while (fread(&produto, sizeof(prod), 1, arquivo)) {
        if (produto.cod == codigo) {
            encontrado = 1;
            printf("Produto encontrado: %s\n", produto.nome);
            printf("Quantidade atual: %d\n", produto.quantidade);
            printf("Digite a nova quantidade: ");
            scanf("%d", &novaQuantidade);

            produto.quantidade = novaQuantidade;

            // Move o ponteiro do arquivo para reescrever o registro
            fseek(arquivo, -sizeof(prod), SEEK_CUR);
            fwrite(&produto, sizeof(prod), 1, arquivo);
            system("cls");
            printf("\nQuantidade atualizada com sucesso!\n");
            Sleep(1500);
            break;
            
        }
    }

    if (!encontrado) {
        system("cls");
        printf("\nProduto com o código %d não encontrado.\n", codigo);
        Sleep(1500);
    }

    fclose(arquivo);









}

void estoque(){
    
    int op2;
    do
    {
        system("cls");
        printf("1.Cadastrar Fornecedor\n2. Consultar Estoque\n3. Voltar ao Menu Principal\n4. Atualizar estoque\nInsira uma opção: ");
        scanf("%d",&op2);
        switch (op2)
        {
        case 1:
            cadastroDeFornecedores(nomeArquivo);
            break;

        case 2:
             busca_produtos(nomeArquivo);
            break;

        case 3:
            system("cls");
            menu_principal();
            break;
        case 4:
            system("cls");
            atualizarEstoque(nomeArquivo);
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
   
    FILE *arquivo;

    // Abre o arquivo binário para leitura e escrita (modo "r+b" para binário)
    arquivo = fopen("usuarios.bin", "r+b");

    // Se o arquivo não existir (primeira execução), cria o arquivo binário
    if (arquivo == NULL) {
        arquivo = fopen("usuarios.bin", "wb");
        if (arquivo == NULL) {
            printf("Erro ao criar arquivo de usuarios.\n");
            return;
        }
        // Inicializa as variáveis e grava no arquivo
        p_acesso = false;
        quant = 0;
        fwrite(&p_acesso, sizeof(bool), 1, arquivo); // Salva p_acesso
        fwrite(&quant, sizeof(int), 1, arquivo);     // Salva quant
    } else {
        // Se o arquivo existir, lê os dados de p_acesso e quant
        fread(&p_acesso, sizeof(bool), 1, arquivo); // Recupera p_acesso
        fread(&quant, sizeof(int), 1, arquivo);    // Recupera quant
    }

    // Atualiza p_acesso para indicar que o usuário acessou a função
    p_acesso = true;

    // Verifica se o limite de usuários foi alcançado
    if (quant >= MAX_USUARIOS) {
        system("cls");
        printf("Limite de usuarios alcançado.\n");
        Sleep(1500);
        fclose(arquivo);
        menu_principal();
    }

    // Array para armazenar os dados dos usuários
    usuarios usuario_[MAX_USUARIOS];

    // Lê os usuários existentes no arquivo
    fseek(arquivo, sizeof(bool) + sizeof(int), SEEK_SET); // Move o ponteiro após p_acesso e quant
    fread(usuario_, sizeof(usuarios), quant, arquivo);

    // Solicita o nome de usuário
    char nome_usuario[50];
    printf("Insira o nome de usuário (máximo 49 caracteres): ");
    scanf("%49s", nome_usuario);

    // Verifica se o nome de usuário já existe
    for (int i = 0; i < quant; i++) {
        if (strcmp(usuario_[i].usuario_, nome_usuario) == 0) {
            // Se o nome de usuário já existir, exibe uma mensagem de erro
            system("cls");
            printf("Nome de usuário já existente. Tente novamente.\n");
            Sleep(1500);
            fclose(arquivo);
            menu_principal();  // Ou repete o cadastro dependendo da sua lógica
            return;
        }
    }

    // Solicita a senha do novo usuário
    printf("Insira a senha do usuário (máximo 49 caracteres): ");
    scanf("%49s", usuario_[quant].senha_);

    // Atribui o nome de usuário ao novo usuário
    strcpy(usuario_[quant].usuario_, nome_usuario);

    // Incrementa o contador de usuários
    quant++;

    // Volta o ponteiro do arquivo para o início e grava os dados atualizados
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&p_acesso, sizeof(bool), 1, arquivo); // Atualiza p_acesso no arquivo
    fwrite(&quant, sizeof(int), 1, arquivo);    // Atualiza quant no arquivo
    fwrite(usuario_, sizeof(usuarios), quant, arquivo); // Grava os usuários no arquivo

    // Limpa a tela e exibe uma mensagem de sucesso
    system("cls");
    printf("Usuário cadastrado com sucesso!\n");
    Sleep(1500);

    fclose(arquivo); // Fecha o arquivo
    menu_principal();





}

void login(){


    system("cls");

    FILE *arquivo;

    // Abre o arquivo binário para leitura (modo "rb" para binário)
    arquivo = fopen("usuarios.bin", "rb");
   
    // Lê os dados de p_acesso e quant (quantidade de usuários cadastrados)
    fread(&p_acesso, sizeof(bool), 1, arquivo); // Recupera p_acesso
    fread(&quant, sizeof(int), 1, arquivo);     // Recupera quant

    // Array para armazenar os dados dos usuários
    usuarios usuario_[MAX_USUARIOS];

    // Lê os usuários existentes no arquivo
    fseek(arquivo, sizeof(bool) + sizeof(int), SEEK_SET); // Move o ponteiro após p_acesso e quant
    fread(usuario_, sizeof(usuarios), quant, arquivo);

    // Variáveis para o login
    char nome_usuario[50], senha_usuario[50];
    bool usuario_encontrado = false;

    // Solicita o nome de usuário e a senha
    printf("Insira o nome de usuário: ");
    scanf("%49s", nome_usuario);
    printf("Insira a senha: ");
    scanf("%49s", senha_usuario);

    // Verifica se o nome de usuário e a senha são válidos
    for (int i = 0; i < quant; i++) {
        if (strcmp(usuario_[i].usuario_, nome_usuario) == 0) {
            // Usuário encontrado, agora verifica a senha
            if (strcmp(usuario_[i].senha_, senha_usuario) == 0) {
                // Se a senha também for correta                
                fclose(arquivo);
                menu_principal();
               // Retorna true se o acesso for autorizado
            } else {
                // Senha incorreta
                printf("Senha incorreta!\n");
                Sleep(1500);
                fclose(arquivo);
                login();
                
            }
        }
    }

    // Se o usuário não for encontrado
    if (!usuario_encontrado) {
        system("cls");
        printf("Usuário não encontrado.\n");
        Sleep(1500);
        fclose(arquivo);
        

    login();



    
}

}

void cadastroDeFornecedores(const char *nomeArquivo){

 int n;
    printf("Quantos fornecedores deseja cadastrar? ");
    scanf("%d", &n);
    while (getchar() != '\n');  // Limpa o buffer de entrada

    fornecedor *fornecedores = (fornecedor *)malloc(n * sizeof(fornecedor));
    if (fornecedores == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Cadastro dos fornecedores
    for (int i = 0; i < n; i++) {
        printf("\nFornecedor %d:\n", i + 1);
        printf("Nome: ");
        scanf(" %49[^\n]", fornecedores[i].nome);
        while (getchar() != '\n');  // Limpa o buffer de entrada

        printf("CNPJ: ");
        scanf(" %19[^\n]", fornecedores[i].cnpj);
        while (getchar() != '\n');  // Limpa o buffer de entrada
    }

    // Salvar os fornecedores no arquivo binário
    FILE *arquivo = fopen(nomeArquivo, "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(fornecedores);
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        fwrite("F", sizeof(char), 1, arquivo);  // Identificador de fornecedor
        fwrite(&fornecedores[i], sizeof(fornecedor), 1, arquivo);
    }

    fclose(arquivo);
    printf("\nFornecedores cadastrados e salvos no arquivo com sucesso.\n");
    free(fornecedores);
}

void busca_produtos(const char *nomeArquivo){

     FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    prod p;
    fornecedor f;
    char tipo;

    // Exibir cabeçalho da tabela
    printf("\n%-10s %-20s %-10s %-15s %-20s %-25s\n", "Código", "Nome", "Preço", "Quantidade", "CNPJ Fornecedor", "Fornecedor");
    printf("-------------------------------------------------------------------------------\n");

    // Exibir os produtos uma vez
    while (fread(&tipo, sizeof(char), 1, arquivo)) {
        if (tipo == 'P') { // Produto
            fread(&p, sizeof(prod), 1, arquivo);

            // Procurar o fornecedor correspondente dentro do arquivo
            rewind(arquivo); // Reposiciona o ponteiro do arquivo para o início
            char cnpjFornecedor[30] = "Fornecedor nao encontrado"; // Default
            while (fread(&tipo, sizeof(char), 1, arquivo)) {
                if (tipo == 'F') { // Fornecedor
                    fread(&f, sizeof(fornecedor), 1, arquivo);
                    if (strcmp(p.fornecedor, f.nome) == 0) {
                        strcpy(cnpjFornecedor, f.cnpj); // Atribui o CNPJ
                        break;
                    }
                }
            }

            // Exibir dados do produto com o CNPJ do fornecedor
            printf("%-10d %-20s %-10.2f %-15d %-20s %-25s\n", p.cod, p.nome, p.preco, p.quantidade, cnpjFornecedor, p.fornecedor);
            system("pause");
        }
    }

    fclose(arquivo);

    

}



















