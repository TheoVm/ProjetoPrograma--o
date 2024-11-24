#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct clientes{
    char nome[30];
    char email[30];
    int cpf;
    char telefone[15];
    int statusRegistro;
}Clientes;

typedef struct voos{
    char codigo[30];
    char origem[100];
    char destino[100];
    int dataPartida;
    int horaPartida;
    int poltronasExis[6][6];
    int poltronasDisp[6][6];
    int valorPassagem;
    int statusRegistro;
}Voo;

FILE* prepararArquivos(char* nome) {
    FILE* temp;
    temp = fopen (nome,"r+b");
    if (temp == NULL) {
        printf("Criando arquivo... ");
        temp = fopen (nome, "w+b");
    }
    return temp;
}

void fecharArq (FILE* arq) {
    int status = fclose (arq);
    if (status == 0) {
        printf ("Arquivo fechado! \n");
    }
    else {
        printf ("Erro fechar arquivo.\n");
    }
}

void removerEnter(char* str) {
	int ultimoChar = strlen(str) - 1;
	if (str[ultimoChar] == '\n') {
		str[ultimoChar] = '\0';
	}
}

int nomeValido(char* nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') {
            return 0; // Nome inválido
        }
    }
    return 1; // Nome válido
}

int telefoneValido(const char* telefone) {
    // Verifica se contém apenas dígitos
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (!isdigit(telefone[i])) {
            return 0; // Contém caractere não numérico
        }
    }
    return 1; // Telefone válido
}

void cadastarClientes(FILE* arq1){
    int i = 0;
    int status;
    Clientes vet[1];

    printf("Informe o nome:");
    fgets(vet[i].nome, 30, stdin);
    removerEnter(vet[i].nome);
    // Validação do nome
        while (!nomeValido(vet[i].nome)) {
            printf("Nome inválido. Deve conter apenas letras e espaços. Tente novamente: ");
            fgets(vet[i].nome, 30, stdin);
            removerEnter(vet[i].nome);
        }

    printf("Informe o email:");
    fgets(vet[i].email, 30, stdin);
    removerEnter(vet[i].email);

    printf("Informe o cpf:"); 
    scanf("%d", &vet[i].cpf);
    getchar();

    printf("Informe o número de telefone(Apenas numeros): ");
    fgets(vet[i].telefone, 15, stdin);
    removerEnter(vet[i].telefone);

    // Validação do número de telefone
    while (!telefoneValido(vet[i].telefone)) {
        printf("Número de telefone inválido(Apenas numeros). Tente novamente: ");
        fgets(vet[i].telefone, 15, stdin);
        removerEnter(vet[i].telefone);
    }

    printf("Informe o status de registro:");
    scanf("%d", &vet[i].statusRegistro);
    getchar();

    fseek(arq1, sizeof(Clientes), SEEK_END);
    status = fwrite(&vet[i], sizeof(Clientes), 1, arq1);
    if (status == 1){
        printf("Regsitrado! \n");
    } else {
        printf("Erro ao tentar registrar! \n");
    }
}

void alterarDados(FILE* arq1){
    Clientes alterar[1];
    int procurar;
    // int escolha;

    printf("Digite o CPF do usuario: ");
    scanf("%d",&procurar);
    getchar();

    fseek(arq1, 0, SEEK_SET);
    while (!feof(arq1)){
        fread(&alterar, sizeof(Clientes), 1, arq1);
        if (alterar[0].cpf == procurar){
            printf("Qual informação deseja alterar?");
            printf("1 - Nome \n");
            printf("2 - Telefone \n");
            printf("3 - Email \n");
            printf("4 - Voltar \n");
            break;
        }
    }
}

void ExibirDados(FILE* arq1){
    Clientes exibir[1];
    int procurar;

    printf("Digite o CPF do usuario: ");
    scanf("%d",&procurar);
    getchar();

    fseek(arq1, 0, SEEK_SET);
    while (!feof(arq1)){
        fread(&exibir, sizeof(Clientes), 1, arq1);
        if (exibir[0].cpf == procurar){
            printf("%s\n", exibir[0].nome);
            printf("%s\n", exibir[0].email);
            printf("%s\n", exibir[0].telefone);
            printf("%d\n", exibir[0].cpf);
            printf("%d\n", exibir[0].statusRegistro);
            break;
        } else {
        }
    }

}

void funcClientes(FILE* arq1){
    int escolha, continuar = 1;
    
    while (continuar == 1){
        printf("\nQual funcionalidades deseja acessar? \n");
        printf("1 - Cadastrar Cliente \n");
        printf("2 - Alterar Dados \n");
        printf("3 - Exibir Dados \n");
        printf("4 - Remover Cliente \n");
        printf("5 - Consultar Cliente \n");
        printf("6 - Voltar \n");
        printf("Digite sua escolha: ");
        scanf("%d", &escolha);
        getchar();
                    
        switch (escolha){
            case 1:
                cadastarClientes(arq1);
                break;
            case 2:
                alterarDados(arq1);
                break;
            case 3:
                ExibirDados(arq1);
                break;
            case 4:
                printf("Implementar funcionalidade. \n");
                break;
            case 5:
                printf("Implementar funcionalidade. \n");
                break;
            case 6:
                continuar = 0;
                break;
            default:
                printf("Digite uma opção válida. \n");
        }
    }
}

void funcVoos(){

}

void funcControle(){
    
}

int main() {
    char nomeArq1[100] = "Clientes.bin",nomeArq2[100] = "Voos.bin", nomeArq3[100] = "Passagens.bin";
    int escolha, continuar = 1;
    FILE *arq1, *arq2, *arq3;
    
    arq1 = prepararArquivos(nomeArq1);
    
    if (arq1 == NULL) {
        printf ("Erro ao tentar criar/abrir arquivo %s \n",nomeArq1);
    } else {
        arq2 = prepararArquivos(nomeArq2);
        if (arq2 == NULL) {
            printf ("Erro ao tentar criar/abrir arquivo %s \n",nomeArq2);
            fecharArq(arq1);
        } else {
            arq3 = prepararArquivos(nomeArq3);
            if (arq3 == NULL) {
                printf ("Erro ao tentar criar/abrir arquivo %s \n",nomeArq3);
                fecharArq(arq1);
                fecharArq(arq2);
            } else { 
                while (continuar == 1){
                    printf("Qual funcionalidades deseja acessar? \n");
                    printf("1 - Cliente \n");
                    printf("2 - Voos \n");
                    printf("3 - Controle de Voos \n");
                    printf("4 - Sair \n");
                    printf("Digite sua escolha: ");
                    scanf("%d", &escolha);
                    getchar();
                    
                    switch (escolha){
                        case 1:
                            funcClientes(arq1);
                            break;
                        case 2:
                            funcVoos();
                            break;
                        case 3:
                            funcControle();
                            break;
                        case 4:
                            continuar = 0;
                            break;
                        default:
                            printf("Digite uma opção válida. \n");
                    }
                }
                fecharArq(arq1);
                fecharArq(arq2);
                fecharArq(arq3);
            }
        }
    } 
    return 0;
}