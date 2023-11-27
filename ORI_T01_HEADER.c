/* Bibliotecas */
#include "ORI_T01_HEADER.h"

/* Funções auxiliares para o qsort.
 * Com uma pequena alteração, é possível utilizá-las no bsearch, assim, evitando código duplicado.
 * */

/* Função de comparação entre chaves do índice corredores_idx */
int qsort_corredores_idx(const void *a, const void *b) {
    return strcmp( ( (corredores_index *)a )->id_corredor, ( (corredores_index *)b )->id_corredor);
}

/* Função de comparação entre chaves do índice veiculos_idx */
int qsort_veiculos_idx(const void *a, const void *b) {
    return strcmp(((veiculos_index *)a)->id_veiculo, ((veiculos_index *)b)->id_veiculo);
}

/* Função de comparação entre chaves do índice pistas_idx */
int qsort_pistas_idx(const void *a, const void *b) {
    return strcmp(((pistas_index *)a)->id_pista, ((pistas_index *)b)->id_pista);
}

/* Função de comparação entre chaves do índice corridas_idx */
int qsort_corridas_idx(const void *a, const void *b) {
    int cmp = strcmp(((corridas_index *)a)->ocorrencia, ((corridas_index *)b)->ocorrencia);
    if(cmp == 0)
        cmp = strcmp(((corridas_index *)a)->id_pista, ((corridas_index *)b)->id_pista);
    return cmp;
}

/* Funções de comparação apenas entre data de ocorrencia do índice corridas_idx */
int qsort_data_idx(const void *a, const void *b){
	return strcmp( ( (corridas_index *)a )->ocorrencia, ( (corridas_index *)b )->ocorrencia);
}

/* Função de comparação entre chaves do índice nome_pista_idx */
int qsort_nome_pista_idx(const void *a, const void *b) {
    int cmp = strcmp(((nome_pista_index *)a)->nome, ((nome_pista_index *)b)->nome);
    if(cmp == 0)
        cmp = strcmp(((nome_pista_index *)a)->id_pista, ((nome_pista_index *)b)->id_pista);
    return cmp;
}

/* Função de comparação entre chaves do índice preco_veiculo_idx */
int qsort_preco_veiculo_idx(const void *a, const void *b) {
    double diff = ((preco_veiculo_index *)a)->preco - ((preco_veiculo_index *)b)->preco;
    if(diff == 0)
        return strcmp(((preco_veiculo_index *)a)->id_veiculo, ((preco_veiculo_index *)b)->id_veiculo);
    return diff < 0 ? -1 : 1;
}

/* Função de comparação entre chaves do índice secundário de corredor_veiculos_secundario_idx */
int qsort_corredor_veiculos_secundario_idx(const void *a, const void *b) {
    return strcmp(((corredor_veiculos_secundario_index *)a)->chave_secundaria, ((corredor_veiculos_secundario_index *)b)->chave_secundaria);
}

/* Cria o índice respectivo */
// criar_corredores_idx() não deve ser modificada
void criar_corredores_idx() {
    if (!corredores_idx)
        corredores_idx = malloc(MAX_REGISTROS * sizeof(corredores_index));
 
    if (!corredores_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
 
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        Corredor c = recuperar_registro_corredor(i);
 
        if (strncmp(c.id_corredor, "*|", 2) == 0)
            corredores_idx[i].rrn = -1; // registro excluído
        else
            corredores_idx[i].rrn = i;
 
        strcpy(corredores_idx[i].id_corredor, c.id_corredor);
    }
 
    qsort(corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);
    printf(INDICE_CRIADO, "corredores_idx");
}

void criar_veiculos_idx() {
    if (!veiculos_idx)
        veiculos_idx = malloc(MAX_REGISTROS * sizeof(veiculos_index));

    if (!veiculos_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);

        if (strncmp(v.id_veiculo, "*|", 2) == 0)
            veiculos_idx[i].rrn = -1; // registro excluído
        else
            veiculos_idx[i].rrn = i;

        strcpy(veiculos_idx[i].id_veiculo, v.id_veiculo);
    }

    qsort(veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
    printf(INDICE_CRIADO, "veiculos_idx");
}

void criar_pistas_idx() {
    if (pistas_idx)
        free(pistas_idx);

    pistas_idx = malloc(MAX_REGISTROS * sizeof(pistas_index));

    if (!pistas_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);

        if (strncmp(p.id_pista, "*|", 2) == 0)
            pistas_idx[i].rrn = -1; // registro excluído
        else
            pistas_idx[i].rrn = i;

        strcpy(pistas_idx[i].id_pista, p.id_pista);
    }

    qsort(pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx);
    printf(INDICE_CRIADO, "pistas_idx");
} 

void criar_corridas_idx() {
    if (corridas_idx)
        free(corridas_idx);

    corridas_idx = malloc(MAX_REGISTROS * sizeof(corridas_index));

    if (!corridas_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    for (unsigned i = 0; i < qtd_registros_corridas; ++i) {
        Corrida c = recuperar_registro_corrida(i);

        if (strncmp(c.id_pista, "*|", 2) == 0)
            corridas_idx[i].rrn = -1; // registro excluído
        else
            corridas_idx[i].rrn = i;

        strcpy(corridas_idx[i].ocorrencia, c.ocorrencia);
        strcpy(corridas_idx[i].id_pista, c.id_pista);
    }

    qsort(corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx);
    printf(INDICE_CRIADO, "corridas_idx");
}

void criar_nome_pista_idx() {
    if (nome_pista_idx)
        free(nome_pista_idx);

    nome_pista_idx = malloc(MAX_REGISTROS * sizeof(nome_pista_index));

    if (!nome_pista_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);

        if (strncmp(p.id_pista, "*|", 2) == 0)
            continue; // registro excluído

        strcpy(nome_pista_idx[i].nome, p.nome);
        strcpy(nome_pista_idx[i].id_pista, p.id_pista);
    }

    qsort(nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx);
    printf(INDICE_CRIADO, "nome_pista_idx");
}

void criar_preco_veiculo_idx() {
    if (preco_veiculo_idx)
        free(preco_veiculo_idx);

    preco_veiculo_idx = malloc(MAX_REGISTROS * sizeof(preco_veiculo_index));

    if (!preco_veiculo_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);

        if (strncmp(v.id_veiculo, "*|", 2) == 0)
            continue; // registro excluído

        preco_veiculo_idx[i].preco = v.preco;
        strcpy(preco_veiculo_idx[i].id_veiculo, v.id_veiculo);
    }

    qsort(preco_veiculo_idx, qtd_registros_veiculos, sizeof(preco_veiculo_index), qsort_preco_veiculo_idx);
    printf(INDICE_CRIADO, "preco_veiculo_idx");
}

void criar_corredor_veiculos_idx() {
    printf("DEBUG: Entrou na função criar_corredor_veiculos_idx.\n");

    // Alocação de memória
    if (corredor_veiculos_idx.corredor_veiculos_primario_idx)
        free(corredor_veiculos_idx.corredor_veiculos_primario_idx);
    if (corredor_veiculos_idx.corredor_veiculos_secundario_idx)
        free(corredor_veiculos_idx.corredor_veiculos_secundario_idx);

    corredor_veiculos_idx.corredor_veiculos_primario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_primario_index));
    corredor_veiculos_idx.corredor_veiculos_secundario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_secundario_index));

    printf("DEBUG: Memória alocada para os índices.\n");

    // Verificação de alocação de memória
    if (!corredor_veiculos_idx.corredor_veiculos_primario_idx || !corredor_veiculos_idx.corredor_veiculos_secundario_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }

    // Criação do índice primário
    printf("DEBUG: Iniciando a criação do índice primário.\n");
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        printf("DEBUG: Iteração %u do loop de criação do índice primário.\n", i);
        Corredor c = recuperar_registro_corredor(i);

        if (strncmp(c.id_corredor, "*|", 2) == 0)
            continue; // registro excluído

        printf("DEBUG: Corredor recuperado: %s\n", c.id_corredor);

        strcpy(corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria, c.id_corredor);
        corredor_veiculos_idx.corredor_veiculos_primario_idx[i].proximo_indice = -1; // inicialmente, não há próximo índice

        printf("DEBUG: Chave primária inserida no índice: %s\n", corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria);
    }
    printf("DEBUG: Índice primário criado.\n");

    // Criação do índice secundário
    printf("DEBUG: Iniciando a criação do índice secundário.\n");
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        printf("DEBUG: Iteração %u do loop de criação do índice secundário.\n", i);
        Veiculo v = recuperar_registro_veiculo(i);

        if (strncmp(v.id_veiculo, "*|", 2) == 0)
            continue; // registro excluído

        printf("DEBUG: Veículo recuperado: %s\n", v.id_veiculo);

        strcpy(corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria, v.modelo);
        corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].primeiro_indice = -1; // inicialmente, não há primeiro índice

        printf("DEBUG: Chave secundária inserida no índice: %s\n", corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria);
    }
    printf("DEBUG: Índice secundário criado.\n");

    // Ordenação dos índices
    printf("DEBUG: Iniciando a ordenação dos índices.\n");
    qsort(corredor_veiculos_idx.corredor_veiculos_secundario_idx, qtd_registros_veiculos, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx);
    printf("DEBUG: Índice secundário ordenado.\n");

    printf("DEBUG: Índices criados com sucesso.\n");
    printf("DEBUG: Índice primário:\n");
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        printf("DEBUG: Chave primária: %s, Próximo índice: %d\n", corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria, corredor_veiculos_idx.corredor_veiculos_primario_idx[i].proximo_indice);
    }

    printf("DEBUG: Índice secundário:\n");
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        printf("DEBUG: Chave secundária: %s, Primeiro índice: %d\n", corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria, corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].primeiro_indice);
    }

    printf(INDICE_CRIADO, "corredor_veiculos_idx");
}

/* Exibe um registro com base no RRN */
// exibir_corredor() não deve ser modificada
bool exibir_corredor(int rrn) {
    if (rrn < 0)
        return false;
 
    Corredor c = recuperar_registro_corredor(rrn);
 
    printf("%s, %s, %s, %s, %.2lf\n", c.id_corredor, c.nome, c.apelido, c.cadastro, c.saldo);
    return true;
}

bool exibir_veiculo(int rrn) {
    if (rrn < 0)
        return false;

    Veiculo v = recuperar_registro_veiculo(rrn);

    printf("%s, %s, %s, %s, %d, %d, %d, %.2lf\n", v.id_veiculo, v.marca, v.modelo, v.poder, v.velocidade, v.aceleracao, v.peso, v.preco);
    return true;
}

bool exibir_pista(int rrn) {
    if (rrn < 0)
        return false;

    Pista p = recuperar_registro_pista(rrn);

    printf("%s, %s, %d, %d, %d\n", p.id_pista, p.nome, p.dificuldade, p.distancia, p.recorde);
    return true;
}

bool exibir_corrida(int rrn) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "exibir_corrida()");
	
	return false;
}

/* Recupera do arquivo o registro com o RRN informado
 * e retorna os dados nas structs */
// recuperar_registro_corredor() não deve ser modificada
Corredor recuperar_registro_corredor(int rrn) {
	Corredor c;
	char temp[TAM_REGISTRO_CORREDOR + 1], *p;
	strncpy(temp, ARQUIVO_CORREDORES + (rrn * TAM_REGISTRO_CORREDOR), TAM_REGISTRO_CORREDOR);
	temp[TAM_REGISTRO_CORREDOR] = '\0';

	p = strtok(temp, ";");
	strcpy(c.id_corredor, p);
	p = strtok(NULL, ";");
	strcpy(c.nome, p);
	p = strtok(NULL, ";");
	strcpy(c.apelido, p);
	p = strtok(NULL, ";");
	strcpy(c.cadastro, p);
	p = strtok(NULL, ";");
	c.saldo = atof(p);
	p = strtok(NULL, ";");

	for(int i = 0; i < QTD_MAX_VEICULO; ++i)
		c.veiculos[i][0] = '\0';

	if(p[0] != '#') {
		p = strtok(p, "|");

		for(int pos = 0; p; p = strtok(NULL, "|"), ++pos)
			strcpy(c.veiculos[pos], p);
	}

	return c;
}

Veiculo recuperar_registro_veiculo(int rrn) {
    Veiculo v;
    char temp[TAM_REGISTRO_VEICULO + 1], *p;
    strncpy(temp, ARQUIVO_VEICULOS + (rrn * TAM_REGISTRO_VEICULO), TAM_REGISTRO_VEICULO);
    temp[TAM_REGISTRO_VEICULO] = '\0';

    p = strtok(temp, ";");
    strcpy(v.id_veiculo, p);
    p = strtok(NULL, ";");
    strcpy(v.marca, p);
    p = strtok(NULL, ";");
    strcpy(v.modelo, p);
    p = strtok(NULL, ";");
    strcpy(v.poder, p);
    p = strtok(NULL, ";");
    v.velocidade = atoi(p);
    p = strtok(NULL, ";");
    v.aceleracao = atoi(p);
    p = strtok(NULL, ";");
    v.peso = atoi(p);
    p = strtok(NULL, ";");
    v.preco = atof(p);

    return v;
}

Pista recuperar_registro_pista(int rrn) {
    Pista p;
    char temp[TAM_REGISTRO_PISTA + 1], *token;
    strncpy(temp, ARQUIVO_PISTAS + (rrn * TAM_REGISTRO_PISTA), TAM_REGISTRO_PISTA);
    temp[TAM_REGISTRO_PISTA] = '\0';

    token = strtok(temp, ";");
    strcpy(p.id_pista, token);
    token = strtok(NULL, ";");
    strcpy(p.nome, token);
    token = strtok(NULL, ";");
    p.dificuldade = atoi(token);
    token = strtok(NULL, ";");
    p.distancia = atoi(token);
    token = strtok(NULL, ";");
    p.recorde = atoi(token);

    return p;
}

Corrida recuperar_registro_corrida(int rrn) {
    Corrida c;
    char temp[TAM_REGISTRO_CORRIDA + 1];

    // Copia o registro para a string temporária
    strncpy(temp, ARQUIVO_CORRIDAS + (rrn * TAM_REGISTRO_CORRIDA), TAM_REGISTRO_CORRIDA);
    temp[TAM_REGISTRO_CORRIDA] = '\0';

    // Extrai os dados da string temporária para a estrutura Corrida
    strncpy(c.id_pista, temp, 8);
    c.id_pista[8] = '\0';

    strncpy(c.ocorrencia, temp + 8, 12);
    c.ocorrencia[12] = '\0';

    strncpy(c.id_corredores, temp + 20, 66);
    c.id_corredores[66] = '\0';

    strncpy(c.id_veiculos, temp + 86, 42);
    c.id_veiculos[42] = '\0';

    return c;
}

/* Escreve em seu respectivo arquivo na posição informada (RRN) */
// escrever_registro_corredor() não deve ser modificada
void escrever_registro_corredor(Corredor c, int rrn) {
	char temp[TAM_REGISTRO_CORREDOR + 1], p[100];
	temp[0] = '\0'; p[0] = '\0';

	strcpy(temp, c.id_corredor);
	strcat(temp, ";");
	strcat(temp, c.nome);
	strcat(temp, ";");
	strcat(temp, c.apelido);
	strcat(temp, ";");
	strcat(temp, c.cadastro);
	strcat(temp, ";");
	sprintf(p, "%013.2lf", c.saldo);
	strcat(temp, p);
	strcat(temp, ";");

	for(int i = 0, k = 0; i < QTD_MAX_VEICULO; ++i) {
		if(strlen(c.veiculos[i]) > 0) {
			if (k == 0)
				k = 1;
			else
				strcat(temp, "|");
			strcat(temp, c.veiculos[i]);
		}
	}
	strcat(temp, ";");

	strpadright(temp, '#', TAM_REGISTRO_CORREDOR);

	strncpy(ARQUIVO_CORREDORES + rrn*TAM_REGISTRO_CORREDOR, temp, TAM_REGISTRO_CORREDOR);
}

void escrever_registro_veiculo(Veiculo v, int rrn) {
    char temp[TAM_REGISTRO_VEICULO + 1], p[100];
    temp[0] = '\0'; p[0] = '\0';

    strcpy(temp, v.id_veiculo);
    strcat(temp, ";");
    strcat(temp, v.marca);
    strcat(temp, ";");
    strcat(temp, v.modelo);
    strcat(temp, ";");
    strcat(temp, v.poder);
    strcat(temp, ";");
    sprintf(p, "%04d", v.velocidade);
    strcat(temp, p);
    strcat(temp, ";");
    sprintf(p, "%04d", v.aceleracao);
    strcat(temp, p);
    strcat(temp, ";");
    sprintf(p, "%04d", v.peso);
    strcat(temp, p);
    strcat(temp, ";");
    sprintf(p, "%013.2lf", v.preco);
    strcat(temp, p);
    strcat(temp, ";");

    strpadright(temp, '#', TAM_REGISTRO_VEICULO);

    strncpy(ARQUIVO_VEICULOS + rrn*TAM_REGISTRO_VEICULO, temp, TAM_REGISTRO_VEICULO);
}

void escrever_registro_pista(Pista p, int rrn) {
    char temp[TAM_REGISTRO_PISTA + 1], buffer[100];
    temp[0] = '\0'; buffer[0] = '\0';

    strcpy(temp, p.id_pista);
    strcat(temp, ";");
    strcat(temp, p.nome);
    strcat(temp, ";");
    sprintf(buffer, "%04d", p.dificuldade);
    strcat(temp, buffer);
    strcat(temp, ";");
    sprintf(buffer, "%04d", p.distancia);
    strcat(temp, buffer);
    strcat(temp, ";");
    sprintf(buffer, "%04d", p.recorde);
    strcat(temp, buffer);
    strcat(temp, ";");

    strpadright(temp, '#', TAM_REGISTRO_PISTA);

    strncpy(ARQUIVO_PISTAS + rrn*TAM_REGISTRO_PISTA, temp, TAM_REGISTRO_PISTA);
}

void escrever_registro_corrida(Corrida c, int rrn) {
    char temp[TAM_REGISTRO_CORRIDA + 1];

    // Formata os dados da corrida em uma string
    sprintf(temp, "%s%s%s%s", c.id_pista, c.ocorrencia, c.id_corredores, c.id_veiculos);

    // Preenche o restante da string com o caractere '#'
    strpadright(temp, '#', TAM_REGISTRO_CORRIDA);

    // Copia a string formatada para a posição correta em ARQUIVO_CORRIDAS
    strncpy(ARQUIVO_CORRIDAS + rrn * TAM_REGISTRO_CORRIDA, temp, TAM_REGISTRO_CORRIDA);
}

/* Funções principais */
void cadastrar_corredor_menu(char *id_corredor, char *nome, char *apelido){
    corredores_index index;
    strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);
    if (found != NULL && found->rrn >= 0) {
        printf(ERRO_PK_REPETIDA, id_corredor);
        return;
    }

    Corredor c;
    strcpy(c.id_corredor, id_corredor);
    strcpy(c.nome, nome);
    strcpy(c.apelido, apelido);
    c.saldo = 0.0;
    current_datetime(c.cadastro);
    for(int i = 0; i < QTD_MAX_VEICULO; ++i)
        c.veiculos[i][0] = '\0';

    escrever_registro_corredor(c, qtd_registros_corredores);

    strcpy(corredores_idx[qtd_registros_corredores].id_corredor, id_corredor);
    corredores_idx[qtd_registros_corredores].rrn = qtd_registros_corredores;
    qtd_registros_corredores++;

    qsort(corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);

    printf(SUCESSO);
}

void remover_corredor_menu(char *id_corredor) {
    corredores_index index;
    strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);
    if (found == NULL || found->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    Corredor c = recuperar_registro_corredor(found->rrn);
    memcpy(c.id_corredor, "*|", 2);
    escrever_registro_corredor(c, found->rrn);

    found->rrn = -1;

    printf(SUCESSO);
}

void adicionar_saldo_menu(char *id_corredor, double valor) {
	adicionar_saldo(id_corredor, valor, true);
}

void adicionar_saldo(char *id_corredor, double valor, bool flag){
    // Verificar se o valor a ser adicionado é menor ou igual a zero
    if (valor <= 0) {
        printf(ERRO_VALOR_INVALIDO);
        return;
    }

    // Buscar o corredor pelo id_corredor fornecido
    corredores_index index;
    strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);

    // Se o corredor não for encontrado, imprimir a mensagem de erro
    if (found == NULL || found->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    // Recuperar o registro do corredor
    Corredor c = recuperar_registro_corredor(found->rrn);

    // Adicionar o valor ao saldo do corredor
    c.saldo += valor;

    // Escrever o registro do corredor de volta no arquivo de dados
    escrever_registro_corredor(c, found->rrn);

    // Imprimir a mensagem de sucesso
    printf(SUCESSO);
}

void comprar_veiculo_menu(char *id_corredor, char *id_veiculo) {
    // Buscar o corredor pelo id_corredor fornecido
    corredores_index chave_corredor;
    strcpy(chave_corredor.id_corredor, id_corredor);
    corredores_index *corredor_encontrado = (corredores_index*) bsearch(&chave_corredor, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);

    // Se o corredor não for encontrado, imprimir a mensagem de erro
    if (corredor_encontrado == NULL || corredor_encontrado->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    // Buscar o veículo pelo id_veiculo fornecido
    veiculos_index chave_veiculo;
    strcpy(chave_veiculo.id_veiculo, id_veiculo);
    veiculos_index *veiculo_encontrado = (veiculos_index*) bsearch(&chave_veiculo, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);

    // Se o veículo não for encontrado, imprimir a mensagem de erro
    if (veiculo_encontrado == NULL || veiculo_encontrado->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    // Recuperar o registro do corredor e do veículo
    Corredor corredor = recuperar_registro_corredor(corredor_encontrado->rrn);
    Veiculo veiculo = recuperar_registro_veiculo(veiculo_encontrado->rrn);

    // Verificar se o corredor tem saldo suficiente para comprar o veículo
    if (corredor.saldo < veiculo.preco) {
        printf(ERRO_SALDO_NAO_SUFICIENTE);
        return;
    }

    // Verificar se o corredor já possui o veículo
    for (int i = 0; i < QTD_MAX_VEICULO; i++) {
        if (strcmp(corredor.veiculos[i], veiculo.modelo) == 0) {
            printf(ERRO_VEICULO_REPETIDO, corredor.id_corredor, veiculo.id_veiculo);
            return;
        }
    }

    // Adicionar o veículo à lista de veículos do corredor
    for (int i = 0; i < QTD_MAX_VEICULO; i++) {
        if (corredor.veiculos[i][0] == '\0') {
            strcpy(corredor.veiculos[i], veiculo.modelo);
            break;
        }
    }

    // Atualizar o saldo do corredor
    corredor.saldo -= veiculo.preco;

    // Escrever o registro do corredor de volta no arquivo de dados
    escrever_registro_corredor(corredor, corredor_encontrado->rrn);

    printf(SUCESSO);
}

void cadastrar_veiculo_menu(char *marca, char *modelo, char *poder, int velocidade, int aceleracao, int peso, double preco) {
    Veiculo novo_veiculo;

    // Copia os dados para a nova struct Veiculo
    strcpy(novo_veiculo.marca, marca);
    strcpy(novo_veiculo.modelo, modelo);
    strcpy(novo_veiculo.poder, poder);
    novo_veiculo.velocidade = velocidade;
    novo_veiculo.aceleracao = aceleracao;
    novo_veiculo.peso = peso;
    novo_veiculo.preco = preco;

    // Gera o ID do veículo
    char id_veiculo[TAM_ID_VEICULO];
    sprintf(id_veiculo, "%07d", qtd_registros_veiculos);
    strcpy(novo_veiculo.id_veiculo, id_veiculo);

    // Verifica se o ID do veículo já existe
    veiculos_index chave;
    strcpy(chave.id_veiculo, id_veiculo);
    veiculos_index *veiculo_encontrado = (veiculos_index*) bsearch(&chave, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
    if (veiculo_encontrado != NULL) {
        printf(ERRO_PK_REPETIDA, id_veiculo);
        return;
    }

    // Adiciona o novo veículo ao final do arquivo de veículos
    escrever_registro_veiculo(novo_veiculo, qtd_registros_veiculos);

    // Atualiza o índice de veículos
    strcpy(veiculos_idx[qtd_registros_veiculos].id_veiculo, id_veiculo);
    veiculos_idx[qtd_registros_veiculos].rrn = qtd_registros_veiculos;

    // Atualiza o índice de preços de veículos
    preco_veiculo_idx[qtd_registros_veiculos].preco = preco;
    strcpy(preco_veiculo_idx[qtd_registros_veiculos].id_veiculo, id_veiculo);

    // Incrementa a quantidade de registros de veículos
    qtd_registros_veiculos++;

    // Reordena os índices de veículos e preços de veículos
    qsort(veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
    qsort(preco_veiculo_idx, qtd_registros_veiculos, sizeof(preco_veiculo_index), qsort_preco_veiculo_idx);

    printf(SUCESSO);
}

void cadastrar_pista_menu(char *nome, int dificuldade, int distancia, int recorde) {
    // Verifica se o nome da pista já existe
    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);
        if (strcmp(p.nome, nome) == 0) {
            printf(ERRO_PK_REPETIDA, nome);
            return;
        }
    }

    Pista nova_pista;

    // Gera o id_pista
    sprintf(nova_pista.id_pista, "%08d", qtd_registros_pistas);

    // Copia os dados para a nova struct Pista
    strcpy(nova_pista.nome, nome);
    nova_pista.dificuldade = dificuldade;
    nova_pista.distancia = distancia;
    nova_pista.recorde = recorde;

    // Adiciona a nova pista ao final do arquivo de pistas
    escrever_registro_pista(nova_pista, qtd_registros_pistas);

    // Atualiza o índice de pistas
    strcpy(pistas_idx[qtd_registros_pistas].id_pista, nova_pista.id_pista);
    pistas_idx[qtd_registros_pistas].rrn = qtd_registros_pistas;

    // Atualiza o índice nome_pista_idx
    strcpy(nome_pista_idx[qtd_registros_pistas].nome, nova_pista.nome);
    strcpy(nome_pista_idx[qtd_registros_pistas].id_pista, nova_pista.id_pista);

    // Incrementa a quantidade de registros de pistas
    qtd_registros_pistas++;

    // Reordena os índices de pistas e nome_pista_idx
    qsort(pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx);
    qsort(nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx);

    printf(SUCESSO);
}

void executar_corrida_menu(char *id_pista, char *ocorrencia, char *id_corredores, char *id_veiculos) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "executar_corrida_menu()");
}

/* Busca */
// buscar_corredor_id_menu() não deve ser modificada
void buscar_corredor_id_menu(char *id_corredor) {
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, true, 0);
	if (found == NULL || found->rrn < 0)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else
		exibir_corredor(found->rrn);
}

void buscar_pista_id_menu(char *id_pista) {
    // Criar uma chave de busca com o ID da pista fornecido
    pistas_index chave;
    strcpy(chave.id_pista, id_pista);

    // Realizar a busca binária no índice de pistas
    pistas_index *pista_encontrada = (pistas_index*) busca_binaria((void*)&chave, pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx, true, 0);

    // Verificar se a pista foi encontrada
    if (pista_encontrada == NULL || pista_encontrada->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    } else {
        // Exibir os detalhes da pista
        exibir_pista(pista_encontrada->rrn);
    }
}

void buscar_pista_nome_menu(char *nome_pista) {
    // Criar uma chave de busca com o nome da pista fornecido
    nome_pista_index chave;
    strcpy(chave.nome, nome_pista);

    // Percorrer o índice nome_pista_idx para encontrar o nome da pista
    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        if (strcmp(nome_pista_idx[i].nome, nome_pista) == 0) {
            // Copiar o ID da pista para a chave de busca
            strcpy(chave.id_pista, nome_pista_idx[i].id_pista);
            break;
        }
    }

    // Realizar a busca binária no índice secundário nome_pista_idx
    nome_pista_index *pista_encontrada = (nome_pista_index*) busca_binaria((void*)&chave, nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx, true, 0);

    // Se a pista não for encontrada no índice secundário, retornar
    if (pista_encontrada == NULL) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    // Criar uma chave de busca com o ID da pista encontrada
    pistas_index chave_primaria;
    strcpy(chave_primaria.id_pista, pista_encontrada->id_pista);

    // Realizar a busca binária no índice primário pistas_idx
    pistas_index *pista_final = (pistas_index*) busca_binaria((void*)&chave_primaria, pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx, true, 0);

    // Se a pista for encontrada no índice primário, exibir os detalhes da pista
    // Caso contrário, imprimir a mensagem de erro
    if (pista_final != NULL) {
        exibir_pista(pista_final->rrn);
    } else {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    }
}

/* Listagem */
// listar_corredores_id_menu() não deve ser modificada
void listar_corredores_id_menu() {
	if (qtd_registros_corredores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	else
		for (unsigned int i = 0; i < qtd_registros_corredores; i++)
			exibir_corredor(corredores_idx[i].rrn);
}

void listar_corredores_modelo_menu(char *modelo){
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "listar_corredores_modelo_menu()");
}

void listar_veiculos_compra_menu(char *id_corredor) {
    // Buscar o corredor pelo id_corredor fornecido
    corredores_index chave;
    strcpy(chave.id_corredor, id_corredor);
    corredores_index *corredor_encontrado = (corredores_index*) bsearch(&chave, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);

    // Se o corredor não for encontrado, imprimir a mensagem de erro
    if (corredor_encontrado == NULL || corredor_encontrado->rrn < 0) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }

    // Recuperar o registro do corredor
    Corredor corredor = recuperar_registro_corredor(corredor_encontrado->rrn);

    // Verificar os veículos que o corredor pode comprar
    bool veiculo_encontrado = false;
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        veiculos_index chave_veiculo;
        strcpy(chave_veiculo.id_veiculo, preco_veiculo_idx[i].id_veiculo);
        veiculos_index *veiculo_idx = (veiculos_index*) bsearch(&chave_veiculo, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
        if (veiculo_idx != NULL && veiculo_idx->rrn >= 0) {
            Veiculo veiculo = recuperar_registro_veiculo(veiculo_idx->rrn);
            if (veiculo.preco <= corredor.saldo) {
                exibir_veiculo(veiculo_idx->rrn);
                veiculo_encontrado = true;
            }
        }
    }

    // Se nenhum veículo for encontrado, imprimir a mensagem de aviso
    if (!veiculo_encontrado) {
        printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
    }
}

void listar_corridas_periodo_menu(char *data_inicio, char *data_fim) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "listar_corridas_periodo_menu()");
}

/* Liberar espaço */
void liberar_espaco_menu() {
    char temp[TAM_ARQUIVO_CORREDORES];
    int j = 0;
    int old_to_new_rrn[MAX_REGISTROS];

    for (int i = 0; i < qtd_registros_corredores; i++) {
        Corredor c = recuperar_registro_corredor(i);
        if (strncmp(c.id_corredor, "*|", 2) != 0) {
            escrever_registro_corredor(c, j);
            strncpy(temp + (j*TAM_REGISTRO_CORREDOR), ARQUIVO_CORREDORES + (i*TAM_REGISTRO_CORREDOR), TAM_REGISTRO_CORREDOR);
            old_to_new_rrn[i] = j;
            j++;
        } else {
            old_to_new_rrn[i] = -1;
        }
    }

    strncpy(ARQUIVO_CORREDORES, temp, TAM_ARQUIVO_CORREDORES);
    qtd_registros_corredores = j;

    for (int i = 0; i < qtd_registros_corredores; i++) {
        corredores_idx[i].rrn = old_to_new_rrn[corredores_idx[i].rrn];
    }

    printf(SUCESSO);
}

/* Imprimir arquivos de dados */
// Estas funções de imprimir arquivo não devem ser modificadas 
void imprimir_arquivo_corredores_menu() {
	if (qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDORES);
}

void imprimir_arquivo_veiculos_menu() {
	if (qtd_registros_veiculos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_VEICULOS);
}

void imprimir_arquivo_pistas_menu() {
	if (qtd_registros_pistas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PISTAS);
}

void imprimir_arquivo_corridas_menu() {
	if (qtd_registros_corridas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORRIDAS);
}

/* Imprimir índices primários */
// imprimir_corredores_idx_menu() não deve ser modificada
void imprimir_corredores_idx_menu() {
	if (corredores_idx == NULL || qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_corredores; ++i)
			printf("%s, %d\n", corredores_idx[i].id_corredor, corredores_idx[i].rrn);
}

void imprimir_veiculos_idx_menu() {
    if (veiculos_idx == NULL || qtd_registros_veiculos == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_veiculos; ++i)
            printf("%s, %d\n", veiculos_idx[i].id_veiculo, veiculos_idx[i].rrn);
}

void imprimir_pistas_idx_menu() {
    if (pistas_idx == NULL || qtd_registros_pistas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_pistas; ++i)
            printf("%s, %d\n", pistas_idx[i].id_pista, pistas_idx[i].rrn);
}

void imprimir_corridas_idx_menu() {
    if (corridas_idx == NULL || qtd_registros_corridas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_corridas; ++i)
            printf("%s, %s, %d\n", corridas_idx[i].ocorrencia, corridas_idx[i].id_pista, corridas_idx[i].rrn);
}

/* Imprimir índices secundários */
void imprimir_nome_pista_idx_menu() {
    if (nome_pista_idx == NULL || qtd_registros_pistas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_pistas; ++i)
            printf("%s, %s\n", strupr(nome_pista_idx[i].nome), nome_pista_idx[i].id_pista);
}

void imprimir_preco_veiculo_idx_menu() {
    if (preco_veiculo_idx == NULL || qtd_registros_veiculos == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_veiculos; ++i){
            printf("%.2lf, %s\n", preco_veiculo_idx[i].preco, preco_veiculo_idx[i].id_veiculo);
		}
}

void imprimir_corredor_veiculos_secundario_idx_menu() {
    if (corredor_veiculos_idx.corredor_veiculos_secundario_idx == NULL || corredor_veiculos_idx.qtd_registros_secundario == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_secundario; ++i)
            printf("%s, %d\n", corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria, corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].primeiro_indice);
}

void imprimir_corredor_veiculos_primario_idx_menu() {
    if (corredor_veiculos_idx.corredor_veiculos_primario_idx == NULL || corredor_veiculos_idx.qtd_registros_primario == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_primario; ++i)
            printf("%s, %d\n", corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria, corredor_veiculos_idx.corredor_veiculos_primario_idx[i].proximo_indice);
}

/* Liberar memória e encerrar programa */
void liberar_memoria_menu() {
    if(corredores_idx != NULL) {
        free(corredores_idx);
        corredores_idx = NULL;
    }

    if(veiculos_idx != NULL) {
        free(veiculos_idx);
        veiculos_idx = NULL;
    }

    if(pistas_idx != NULL) {
        free(pistas_idx);
        pistas_idx = NULL;
    }

    if(corridas_idx != NULL) {
        free(corridas_idx);
        corridas_idx = NULL;
    }

    if(nome_pista_idx != NULL) {
        free(nome_pista_idx);
        nome_pista_idx = NULL;
    }

    if(preco_veiculo_idx != NULL) {
        free(preco_veiculo_idx);
        preco_veiculo_idx = NULL;
    }

    if(corredor_veiculos_idx.corredor_veiculos_primario_idx != NULL) {
        free(corredor_veiculos_idx.corredor_veiculos_primario_idx);
        corredor_veiculos_idx.corredor_veiculos_primario_idx = NULL;
    }

    if(corredor_veiculos_idx.corredor_veiculos_secundario_idx != NULL) {
        free(corredor_veiculos_idx.corredor_veiculos_secundario_idx);
        corredor_veiculos_idx.corredor_veiculos_secundario_idx = NULL;
    }
}

void inverted_list_insert(char *chave_secundaria, char *chave_primaria, inverted_list *t) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "inverted_list_insert()");
}

bool inverted_list_secondary_search(int *result, bool exibir_caminho, char *chave_secundaria, inverted_list *t) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "inverted_list_secondary_search()");
	return false;
}

int inverted_list_primary_search(char result[][TAM_ID_CORREDOR], bool exibir_caminho, int indice, int *indice_final, inverted_list *t) {
	/*IMPLEMENTE A FUNÇÃO AQUI*/
	printf(ERRO_NAO_IMPLEMENTADO, "inverted_list_primary_search()");
	return -1;
}


void* busca_binaria_com_reps(const void *key, const void *base0, size_t nmemb, size_t size, 
    int (*compar)(const void *, const void *), bool exibir_caminho, int posicao_caso_repetido, int retorno_se_nao_encontrado) {
    const char *base = base0;
    size_t low = 0;
    size_t high = nmemb;
    size_t mid;
    int cond;
    
    if (exibir_caminho) {
        printf("Registros percorridos:");
    }
    
    while (low < high) {
        mid = (low + high) / 2;
        if (exibir_caminho) {
            printf(" %zu", mid);
        }
        cond = (*compar)(key, base + mid * size);
        if (cond < 0) {
            high = mid;
        } else if (cond > 0) {
            low = mid + 1;
        } else {
            if (posicao_caso_repetido == -1) {
                while (mid > 0 && (*compar)(key, base + (mid - 1) * size) == 0) {
                    mid--;
                    if (exibir_caminho) {
                        printf(" %zu", mid);
                    }
                }
            } else if (posicao_caso_repetido == 1) {
                while (mid < nmemb - 1 && (*compar)(key, base + (mid + 1) * size) == 0) {
                    mid++;
                    if (exibir_caminho) {
                        printf(" %zu", mid);
                    }
                }
            }
            if (exibir_caminho) {
                printf("\n");
            }
            return (void *)(base + mid * size);
        }
    }
    if (exibir_caminho) {
        printf("\n");
    }
    return NULL;
}

// As funções abaixo não devem ser modificadas
void* busca_binaria(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int retorno_se_nao_encontrado) {
	return busca_binaria_com_reps(key, base0, nmemb, size, compar, exibir_caminho, 0, retorno_se_nao_encontrado);
}

char *strpadright(char *str, char pad, unsigned size){
	for (unsigned i = strlen(str); i < size; ++i)
		str[i] = pad;
	str[size] = '\0';
	return str;
}
char *strupr(char *str){
	for (char *p = str; *p; ++p)
		*p = toupper(*p);
	return str;
}
char *strlower(char *str){
	for (char *p = str; *p; ++p)
		*p = tolower(*p);
	return str;
}