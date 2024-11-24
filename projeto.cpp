#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct clientes{
    char nome[30];
    char email[30];
    char cpf[12];
    char telefone[15];
    int statusRegistro;
}Clientes;

typedef struct voo {
    char codigo[10]; // Código do voo (letras e números)
    char origem[30];
    char destino[30];
    char dataPartida[11]; // Data no formato dd/mm/aaaa
    char horarioPartida[6]; // Horário no formato hh:mm
    int poltronasExistentes;
    int poltronasDisponiveis;
    char mapaPoltronas[6][6];
    float valorPassagem;
    int statusRegistro;
} Voo;

typedef struct passagem {
    char codigoReserva[15];
    char cpfCliente[12];
    char codigoVoo[10];
    int numeroPoltrona;
    int statusRegistro; // 1 - Ativo, 0 - Deletado
} Passagem;

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

int cpfValido(char* cpf) {
    if (strlen(cpf) != 11) { // Verifica se o CPF tem 11 dígitos
        return 0;
    }
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (!isdigit(cpf[i])) { // Verifica se todos os caracteres são números
            return 0;
        }
    }
    return 1;
}

int consultarCliente(FILE* arq1, char* cpf) {
    Clientes verificar;
    fseek(arq1, 0, SEEK_SET);

    while (fread(&verificar, sizeof(Clientes), 1, arq1) == 1) {
        if (strcmp(verificar.cpf, cpf) == 0) {
            return ftell(arq1) - sizeof(Clientes);
        }
    }
    return -1;
}

void cadastarClientes(FILE* arq1){
    int status;
    Clientes vet;

    printf("Informe o nome:");
    fgets(vet.nome, sizeof(vet.nome), stdin);
    removerEnter(vet.nome);
    // Validação do nome
        while (!nomeValido(vet.nome)) {
            printf("Nome inválido. Deve conter apenas letras e espaços. Tente novamente: ");
            fgets(vet.nome, sizeof(vet.nome), stdin);
            removerEnter(vet.nome);
        }

    printf("Informe o email:");
    fgets(vet.email, sizeof(vet.email), stdin);
    removerEnter(vet.email);

    printf("Informe o CPF (somente números): ");
    fgets(vet.cpf, sizeof(vet.cpf), stdin);
    removerEnter(vet.cpf);
    while (!cpfValido(vet.cpf) || consultarCliente(arq1, vet.cpf) != -1) {
        if (!cpfValido(vet.cpf)) {
            printf("CPF invalido. Deve conter exatamente 11 digitos. Tente novamente: ");
        } else {
            printf("CPF ja cadastrado. Tente novamente: ");
        }
        fgets(vet.cpf, 12, stdin);
        removerEnter(vet.cpf);
    }

    printf("Informe o numero de telefone(Apenas numeros):");
    fflush(stdin);
    fgets(vet.telefone, sizeof(vet.telefone), stdin);
    removerEnter(vet.telefone);
    // Validação do número de telefone
    while (!telefoneValido(vet.telefone)) {
        printf("Numero de telefone invalido(Apenas numeros). Tente novamente: ");
        fflush(stdin);
        fgets(vet.telefone, sizeof(vet.telefone), stdin);
        removerEnter(vet.telefone);
    }

    vet.statusRegistro = 1;

    fseek(arq1, sizeof(Clientes), SEEK_END);
    status = fwrite(&vet, sizeof(Clientes), 1, arq1);
    if (status == 1){
        printf("Registrado! \n");
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
    printf("CPF: %s\n", exibir[0].cpf);
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
    char cpf[12];
    int posicao;
    
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
                fgets(cpf, sizeof(cpf), stdin);
                removerEnter(cpf);
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
                fgets(cpf, sizeof(cpf), stdin);
                removerEnter(cpf);
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
                fgets(cpf, sizeof(cpf), stdin);
                removerEnter(cpf);
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

//////////////////////////////////////////////////////////////////////////////

int codigoValido(char* codigo) {
    for (int i = 0; codigo[i] != '\0'; i++) {
        if (!isalnum(codigo[i])) {
            return 0; // Se algum caractere não for letra ou número
        }
    }
    return 1; // Código válido
}

int consultarVoo(FILE* arq, char* codigo) {
    Voo voo;
    fseek(arq, 0, SEEK_SET);  // Vai para o início do arquivo

    while (fread(&voo, sizeof(Voo), 1, arq) == 1) {
        if (strcmp(voo.codigo, codigo) == 0) {
            return ftell(arq) - sizeof(Voo); // Retorna a posição do voo no arquivo
        }
    }
    return -1; // Não encontrado
}

int vooJaCadastrado(FILE* arq, char* codigo) {
    Voo voo;
    fseek(arq, 0, SEEK_SET);
    while (fread(&voo, sizeof(Voo), 1, arq) == 1) {
        if (strcmp(voo.codigo, codigo) == 0) {
            return 1; // Voo já cadastrado
        }
    }
    return 0; // Voo não cadastrado
}

void alterarValorPassagem(FILE* arq) {
    char codigo[10];
    printf("Digite o codigo do voo para alterar o valor da passagem: ");
    fgets(codigo, sizeof(codigo), stdin);
    removerEnter(codigo);

    Voo voo;
    int posicao = consultarVoo(arq, codigo);
    if (posicao == -1) {
        printf("Voo nao encontrado.\n");
        return;
    }

    printf("Digite o novo valor da passagem: ");
    float novoValor;
    scanf("%f", &novoValor);
    getchar(); // Limpa o buffer
    fseek(arq, posicao, SEEK_SET);
    fread(&voo, sizeof(Voo), 1, arq);

    voo.valorPassagem = novoValor;

    fseek(arq, posicao, SEEK_SET); // Posiciona no registro do voo encontrado
    if (fwrite(&voo, sizeof(Voo), 1, arq)) {
        printf("Valor da passagem alterado com sucesso!\n");
    } else {
        printf("Erro ao alterar o valor da passagem.\n");
    }
}

void cancelarVoo(FILE* arq) {
    char codigo[10];
    printf("Digite o codigo do voo a ser cancelado: ");
    fgets(codigo, sizeof(codigo), stdin);
    removerEnter(codigo);

    Voo voo;
    int posicao = consultarVoo(arq, codigo);

    if (posicao == -1) {
        printf("Voo nao encontrado.\n");
        return;
    }

    fseek(arq, posicao, SEEK_SET); // Posiciona no voo encontrado
    fread(&voo, sizeof(Voo), 1, arq);

    if (voo.poltronasDisponiveis == voo.poltronasExistentes) {
        voo.statusRegistro = 0; // Marca como deletado
        fseek(arq, posicao, SEEK_SET); // Posiciona no registro do voo para atualização
        if (fwrite(&voo, sizeof(Voo), 1, arq)) {
            printf("Voo cancelado com sucesso!\n");
        } else {
            printf("Erro ao cancelar o voo.\n");
        }
    } else {
        printf("Nao e possivel cancelar o voo, pois existem passagens vendidas.\n");
    }
}

void cadastrarVoo(FILE* arq) {
    Voo voo;
    printf("Digite o codigo do voo (somente letras e numeros): ");
    fgets(voo.codigo, 10, stdin);
    removerEnter(voo.codigo);

    while (!codigoValido(voo.codigo) || vooJaCadastrado(arq, voo.codigo)) {
        if (!codigoValido(voo.codigo)) {
            printf("Codigo invalido. Tente novamente: ");
        } else {
            printf("Voo ja cadastrado. Tente outro codigo: ");
        }
        fgets(voo.codigo, 10, stdin);
        removerEnter(voo.codigo);
    }

    printf("Digite a origem do voo: ");
    fgets(voo.origem, 30, stdin);
    removerEnter(voo.origem);

    printf("Digite o destino do voo: ");
    fgets(voo.destino, 30, stdin);
    removerEnter(voo.destino);

    printf("Digite a data de partida (dd/mm/aaaa): ");
    fgets(voo.dataPartida, 11, stdin);
    removerEnter(voo.dataPartida);

    printf("Digite o horario de partida (hh:mm): ");
    scanf("%5s", voo.horarioPartida);
    removerEnter(voo.horarioPartida);

    voo.poltronasExistentes = 36;
    voo.poltronasDisponiveis = 36; // No momento do cadastro, todas as poltronas estão disponíveis

    getchar();
    printf("Digite o valor da passagem: ");
    fflush(stdin);
    scanf("%f", &voo.valorPassagem);

    voo.statusRegistro = 1; // Status ativo

    fseek(arq, 0, SEEK_END); // Vai para o final do arquivo
    if (fwrite(&voo, sizeof(Voo), 1, arq)) {
        printf("Voo cadastrado com sucesso!\n");
    } else {
        printf("Erro ao cadastrar voo.\n");
    }
}

void buscarVoos(FILE* arq, char* origem, char* destino, char* data) {
    Voo voo;
    int encontrado = 0; // Flag para indicar se algum voo foi encontrado

    fseek(arq, 0, SEEK_SET);  // Vai para o início do arquivo

    while (fread(&voo, sizeof(Voo), 1, arq) == 1) {
        // Verifica se o voo corresponde aos critérios (origem, destino, data)
        if (strcmp(voo.origem, origem) == 0 && strcmp(voo.destino, destino) == 0 && strcmp(voo.dataPartida, data) == 0) {
            // Verifica se há poltronas disponíveis
            if (voo.poltronasDisponiveis > 0) {
                // Exibe o voo com poltronas disponíveis
                printf("Codigo do voo: %s\n", voo.codigo);
                printf("Horario de partida: %s\n", voo.horarioPartida);
                printf("Poltronas disponiveis: %d\n", voo.poltronasDisponiveis);

                // Marca que ao menos um voo foi encontrado
                encontrado = 1;
            }
        }
    }

    // Se nenhum voo foi encontrado
    if (!encontrado) {
        printf("Nenhum voo encontrado com as características solicitadas.\n");
    }
}

void buscarVoosMenu(FILE* arq2) {
    char origem[30], destino[30], data[11];

    printf("Informe a origem do voo: ");
    fgets(origem, sizeof(origem), stdin);
    removerEnter(origem);

    printf("Informe o destino do voo: ");
    fgets(destino, sizeof(destino), stdin);
    removerEnter(destino);

    printf("Informe a data de partida (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    removerEnter(data);

    buscarVoos(arq2, origem, destino, data);
}

void funcVoos(FILE* arq2) {
    int escolha, continuar = 1;

    while (continuar == 1) {
        printf("\nQual funcionalidade deseja acessar? \n");
        printf("1 - Cadastrar Voo \n");
        printf("2 - Buscar Voo \n");
        printf("3 - Alterar valor de passagem \n");
        printf("4 - Cancelar um voo \n");
        printf("5 - Voltar \n");
        printf("Digite sua escolha: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                cadastrarVoo(arq2);
                break;
            case 2:
                buscarVoosMenu(arq2);
                break;
            case 3:
                alterarValorPassagem(arq2);
                break;
            case 4:
                cancelarVoo(arq2);
                break;
            case 5:
                continuar = 0;
                break;
            default:
                printf("Digite uma opção valida. \n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////

int verificarPoltronaDisponivel(Voo* voo, int poltrona) {
    int linha = (poltrona - 1) / 6;
    int coluna = (poltrona - 1) % 6;
    return voo->mapaPoltronas[linha][coluna] == 'L';
}

void comprarPassagem(FILE* arqClientes, FILE* arqVoos, FILE* arqPassagens) {
    Passagem novaPassagem;
    Voo voo;
    int posicaoVoo, poltronaEscolhida;

    // Código da reserva (deve ser único)
    printf("Digite o codigo da reserva: ");
    fgets(novaPassagem.codigoReserva, sizeof(novaPassagem.codigoReserva), stdin);
    removerEnter(novaPassagem.codigoReserva);

    fseek(arqPassagens, 0, SEEK_SET);
    Passagem passagemTemp;
    while (fread(&passagemTemp, sizeof(Passagem), 1, arqPassagens)) {
        if (strcmp(passagemTemp.codigoReserva, novaPassagem.codigoReserva) == 0) {
            printf("Código de reserva já utilizado. Tente novamente.\n");
            return;
        }
    }

    // CPF do cliente
    printf("Digite o CPF do cliente: ");
    fgets(novaPassagem.cpfCliente, sizeof(novaPassagem.cpfCliente), stdin);
    removerEnter(novaPassagem.cpfCliente);

    if (consultarCliente(arqClientes, novaPassagem.cpfCliente) == -1) {
        printf("Cliente não cadastrado.\n");
        return;
    }

    // Código do voo
    printf("Digite o codigo do voo: ");
    fflush(stdin);
    fgets(novaPassagem.codigoVoo, sizeof(novaPassagem.codigoVoo), stdin);
    removerEnter(novaPassagem.codigoVoo);

    posicaoVoo = consultarVoo(arqVoos, novaPassagem.codigoVoo);
    if (posicaoVoo == -1) {
        printf("Voo não encontrado.\n");
        return;
    }

    // Leitura do voo e verificação de disponibilidade
    fseek(arqVoos, posicaoVoo, SEEK_SET);
    fread(&voo, sizeof(Voo), 1, arqVoos);

    if (voo.poltronasDisponiveis == 0) {
        printf("Não há poltronas disponíveis nesse voo.\n");
        return;
    }

    // Escolha da poltrona
    printf("Escolha uma poltrona (1-36): ");
    scanf("%d", &poltronaEscolhida);
    getchar();

    while (poltronaEscolhida < 1 || poltronaEscolhida > 36 || verificarPoltronaDisponivel(&voo, poltronaEscolhida)) {
        printf("Poltrona invalida ou ja ocupada. Escolha novamente: ");
        scanf("%d", &poltronaEscolhida);
        getchar();
    }

    novaPassagem.numeroPoltrona = poltronaEscolhida;
    novaPassagem.statusRegistro = 1;

    // Atualização do voo
    int linha = (poltronaEscolhida - 1) / 6;
    int coluna = (poltronaEscolhida - 1) % 6;
    voo.mapaPoltronas[linha][coluna] = 'O';
    voo.poltronasDisponiveis--;

    fseek(arqVoos, posicaoVoo, SEEK_SET);
    fwrite(&voo, sizeof(Voo), 1, arqVoos);

    // Gravação da passagem
    fseek(arqPassagens, 0, SEEK_END);
    fwrite(&novaPassagem, sizeof(Passagem), 1, arqPassagens);

    printf("Passagem comprada com sucesso! Codigo da reserva: %s\n", novaPassagem.codigoReserva);
}

void consultarPassagensCliente(FILE* arqClientes, FILE* arqPassagens) {
    char cpfCliente[12]; // CPF formatado como string
    Clientes cliente;
    Passagem passagem;
    int encontrado = 0;

    // Solicitar CPF ao usuário
    printf("Digite o CPF do cliente (apenas numeros): ");
    scanf("%11s", cpfCliente);
    getchar();

    // Usar a função consultarCliente para validar e obter o cliente
    if (!consultarCliente(arqClientes, cpfCliente)) {
        printf("CPF nao encontrado. Verifique o CPF e tente novamente.\n");
        return;
    }

    // Mostrar informações do cliente
    printf("Cliente encontrado: %s\n", cliente.nome);

    // Consultar passagens relacionadas ao CPF
    fseek(arqPassagens, 0, SEEK_SET);
    while (fread(&passagem, sizeof(Passagem), 1, arqPassagens)) {
        if (strcmp(passagem.cpfCliente, cpfCliente) == 0 && passagem.statusRegistro == 1) {
            printf("Codigo da Reserva: %s, Voo: %s, Poltrona: %d\n",
                   passagem.codigoReserva, passagem.codigoVoo, passagem.numeroPoltrona);
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("Nenhuma passagem encontrada para o CPF informado.\n");
    }
}

void cancelarPassagem(FILE* arqVoos, FILE* arqPassagens) {
    char codigoReserva[15];
    Passagem passagem;
    Voo voo;
    int encontrado = 0;

    printf("Digite o codigo da reserva: ");
    fgets(codigoReserva, sizeof(codigoReserva), stdin);
    removerEnter(codigoReserva);

    fseek(arqPassagens, 0, SEEK_SET);
    while (fread(&passagem, sizeof(Passagem), 1, arqPassagens)) {
        if (strcmp(passagem.codigoReserva, codigoReserva) == 0 && passagem.statusRegistro == 1) {
            passagem.statusRegistro = 0;

            // Liberar a poltrona no voo
            int posicaoVoo = consultarVoo(arqVoos, passagem.codigoVoo);
            fseek(arqVoos, posicaoVoo, SEEK_SET);
            fread(&voo, sizeof(Voo), 1, arqVoos);

            int linha = (passagem.numeroPoltrona - 1) / 6;
            int coluna = (passagem.numeroPoltrona - 1) % 6;
            voo.mapaPoltronas[linha][coluna] = 'L';
            voo.poltronasDisponiveis++;

            fseek(arqVoos, posicaoVoo, SEEK_SET);
            fwrite(&voo, sizeof(Voo), 1, arqVoos);

            encontrado = 1;

            // Atualizar status da passagem
            fseek(arqPassagens, -sizeof(Passagem), SEEK_CUR);
            fwrite(&passagem, sizeof(Passagem), 1, arqPassagens);
            break;
        }
    }

    if (encontrado) {
        printf("Passagem cancelada com sucesso.\n");
    } else {
        printf("Reserva nao encontrada ou ja cancelada.\n");
    }
}

void consultarPassageirosVoo(FILE* arqClientes, FILE* arqVoos, FILE* arqPassagens) {
    char codigoVoo[10]; // Buffer para armazenar o código do voo
    Passagem passagem;
    Clientes cliente;
    Voo voo;
    int encontrado = 0;

    // Solicitar o código do voo ao usuário
    printf("Digite o codigo do voo: ");
    scanf("%9s", codigoVoo);
    getchar();

    // Procurar o voo no arquivo de voos
    fseek(arqVoos, 0, SEEK_SET);
    int vooEncontrado = 0;
    while (fread(&voo, sizeof(Voo), 1, arqVoos)) {
        if (strcmp(voo.codigo, codigoVoo) == 0) {
            vooEncontrado = 1;
            break;
        }
    }

    // Validar se o voo foi encontrado
    if (!vooEncontrado) {
        printf("Voo nao encontrado. Verifique o codigo e tente novamente.\n");
        return;
    }

    // Exibir detalhes do voo
    printf("Voo encontrado: Codigo: %s, Origem: %s, Destino: %s\n",
           voo.codigo, voo.origem, voo.destino);

    // Procurar passageiros associados ao voo
    fseek(arqPassagens, 0, SEEK_SET);
    while (fread(&passagem, sizeof(Passagem), 1, arqPassagens)) {
        if (strcmp(passagem.codigoVoo, codigoVoo) == 0 && passagem.statusRegistro == 1) {
            // Verificar cliente associado à passagem
            fseek(arqClientes, 0, SEEK_SET);
            int clienteEncontrado = 0;
            while (fread(&cliente, sizeof(Clientes), 1, arqClientes)) {
                if (strcmp(cliente.cpf, passagem.cpfCliente) == 0) {
                    clienteEncontrado = 1;
                    printf("CPF: %s, Nome: %s, Poltrona: %d\n", cliente.cpf, cliente.nome, passagem.numeroPoltrona);
                    encontrado = 1;
                    break;
                }
            }

            if (!clienteEncontrado) {
                printf("Erro: Cliente com CPF %s nao encontrado.\n", passagem.cpfCliente);
            }
        }
    }

    if (!encontrado) {
        printf("Nenhum passageiro encontrado para o voo informado.\n");
    }
}

void funcPassagens(FILE* arq1, FILE* arq2, FILE* arq3) {
    int escolha, continuar = 1;

    while (continuar) {
        printf("\nMenu de Passagens\n");
        printf("1 - Comprar Passagem\n");
        printf("2 - Consultar Passagens de um Cliente\n");
        printf("3 - Cancelar Passagem\n");
        printf("4 - Consultar Passageiros de um Voo\n");
        printf("5 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                comprarPassagem(arq1, arq2, arq3);
                break;
            case 2:
                consultarPassagensCliente(arq1, arq3);
                break;
            case 3:
                cancelarPassagem(arq2, arq2);
                break;
            case 4:
                consultarPassageirosVoo(arq1, arq2, arq3);
                break;
            case 5:
                continuar = 0;
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void removerFisicamenteClientes(FILE* arq1) {
    FILE* temp = fopen("tempClientes.dat", "w+b");
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

    remove("Clientes.dat");
    rename("tempClientes.dat", "Clientes.dat");

    printf("Remocao fisica concluida! Registros removidos foram excluidos permanentemente.\n");
}

int main() {
    char nomeArq1[100] = "Clientes.dat",nomeArq2[100] = "Voos.dat", nomeArq3[100] = "Passagens.dat";
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
                            funcVoos(arq2);
                            break;
                        case 3:
                            funcPassagens(arq1, arq2, arq3);
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