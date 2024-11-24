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

int telefoneValido(char* telefone) {
    // Verifica se contém apenas dígitos
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (!isdigit(telefone[i])) {
            return 0; // Contém caractere não numérico
        }
    }
    return 1; // Telefone válido
}

int consultarCliente(FILE* arq1, int cpf) {
    Clientes verificar;
    fseek(arq1, 0, SEEK_SET);

    while (fread(&verificar, sizeof(Clientes), 1, arq1) == 1) {
        if (verificar.cpf == cpf) {
            return ftell(arq1) - sizeof(Clientes);
        }
    }
    return -1;
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
    while (consultarCliente(arq1, vet[i].cpf) != -1) {
        printf("Usuário com este CPF já registrado. Tente novamente: ");
        scanf("%d", &vet[i].cpf);
        getchar();
    }

    printf("Informe o número de telefone(Apenas numeros): ");
    fgets(vet[i].telefone, 15, stdin);
    removerEnter(vet[i].telefone);

    // Validação do número de telefone
    while (!telefoneValido(vet[i].telefone)) {
        printf("Número de telefone inválido(Apenas numeros). Tente novamente: ");
        fgets(vet[i].telefone, 15, stdin);
        removerEnter(vet[i].telefone);
    }

    vet[i].statusRegistro = 1;

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

    fread(&alterar[0], sizeof(Clientes), 1, arq1);
    fseek(arq1, -sizeof(Clientes), SEEK_CUR);
    int escolha;
    int status;

    printf("Qual informacao deseja alterar?\n");
    printf("1 - Nome \n");
    printf("2 - Telefone \n");
    printf("3 - Email \n");
    printf("4 - Voltar \n");
    printf("Digite sua escolha: ");
    scanf("%d", &escolha);
    getchar();

    switch (escolha){
        case 1:
            printf("Digite o novo nome: ");
            fgets(alterar[0].nome, 30, stdin);
            removerEnter(alterar[0].nome);
            break;
        case 2:
            printf("Digite o novo telefone: ");
            fgets(alterar[0].telefone, 30, stdin);
            removerEnter(alterar[0].telefone);
            while (!telefoneValido(alterar[0].telefone)) {
                printf("Número de telefone inválido(Apenas numeros). Tente novamente: ");
                fgets(alterar[0].telefone, 15, stdin);
                removerEnter(alterar[0].telefone);
            }
            break;
        case 3:
            printf("Digite o novo email: ");
            fgets(alterar[0].email, 30, stdin);
            removerEnter(alterar[0].email);
            break;
        case 4:
            break;
        default:
            printf("Digite uma opção válida. \n");
    }
    
    status = fwrite(&alterar[0], sizeof(Clientes), 1, arq1);
    fflush(arq1);
    if (status == 1){
        printf("Alterado! \n");
    } else {
        printf("Erro ao alterar! \n");
    }
}

void ExibirDados(FILE* arq1, int posicao){
    Clientes exibir[1];

    fread(&exibir[0], sizeof(Clientes), 1, arq1);
    fseek(arq1, -sizeof(Clientes), SEEK_CUR);

    fseek(arq1, posicao, SEEK_SET); 
    printf("Nome: %s\n", exibir[0].nome);
    printf("Email: %s\n", exibir[0].email);
    printf("Telefone: %s\n", exibir[0].telefone);
    printf("CPF: %d\n", exibir[0].cpf);
    printf("Status: %d\n", exibir[0].statusRegistro);
}

void removerCLiente(FILE* arq1, int posicao){
    Clientes remover[1];
    int status;

    fread(&remover[0], sizeof(Clientes), 1, arq1);
    fseek(arq1, -sizeof(Clientes), SEEK_CUR);

    fseek(arq1, posicao, SEEK_SET);
    remover[0].statusRegistro = 0;

    status = fwrite(&remover[0], sizeof(Clientes), 1, arq1);
    fflush(arq1);
    if (status == 1){
        printf("Usuario sera removido ao final da aplicacao!\n");
    } else {
        printf("Erro ao remover usuario!\n");
    }

}

void funcClientes(FILE* arq1){
    int escolha, continuar = 1;
    int cpf, posicao;
    
    while (continuar == 1){

        printf("\nQual funcionalidades deseja acessar? \n");
        printf("1 - Cadastrar Cliente \n");
        printf("2 - Alterar Dados \n");
        printf("3 - Exibir Dados \n");
        printf("4 - Remover Cliente \n");
        printf("5 - Voltar \n");
        printf("Digite sua escolha: ");
        scanf("%d", &escolha);
        getchar();
     
        switch (escolha){
            case 1:
                cadastarClientes(arq1);
                break;
            case 2:
                printf("Digite o CPF do usuario: ");
                scanf("%d", &cpf);
                getchar();
                posicao = consultarCliente(arq1, cpf);
                if (posicao != -1) {
                    fseek(arq1, posicao, SEEK_SET);
                    alterarDados(arq1);
                } else {
                    printf("Nenhum usuario com este CPF cadastrado.\n");
                }
                break;
            case 3:
                printf("Digite o CPF do usuario: ");
                scanf("%d", &cpf);
                getchar();
                posicao = consultarCliente(arq1, cpf);
                if (posicao != -1) {
                    fseek(arq1, posicao, SEEK_SET);
                    ExibirDados(arq1, posicao);
                } else {
                    printf("Nenhum usuario com este CPF cadastrado.\n");
                }
                break;
            case 4:
                printf("Digite o CPF do usuario: ");
                scanf("%d", &cpf);
                getchar();
                posicao = consultarCliente(arq1, cpf);
                if (posicao != -1) {
                    fseek(arq1, posicao, SEEK_SET);
                    removerCLiente(arq1, posicao);
                } else {
                    printf("Nenhum usuario com este CPF cadastrado.\n");
                }
                break;
                break;
            case 5:
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

void removerFisicamenteClientes(FILE* arq1) {
    FILE* temp = fopen("tempClientes.bin", "w+b");
    if (!temp) {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    Clientes cliente;
    fseek(arq1, 0, SEEK_SET);

    while (fread(&cliente, sizeof(Clientes), 1, arq1)) {
        if (cliente.statusRegistro == 1) {
            fwrite(&cliente, sizeof(Clientes), 1, temp);
        }
    }

    fclose(arq1);
    fclose(temp);

    remove("Clientes.bin");
    rename("tempClientes.bin", "Clientes.bin");

    printf("Remoção física concluída! Registros removidos foram excluídos permanentemente.\n");
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
                removerFisicamenteClientes(arq1);
                fecharArq(arq2);
                fecharArq(arq3);
            }
        }
    } 
    return 0;
}