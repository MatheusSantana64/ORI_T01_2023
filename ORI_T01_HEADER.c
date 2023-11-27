#include "ORI_T01_HEADER.h"

void* busca_binaria_corridas(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho) {
    const char *base = base0;
    size_t low = 0;
    size_t high = nmemb;
    size_t mid;
    int cond;

    if (exibir_caminho) {
        printf(REGS_PERCORRIDOS);
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
            if (exibir_caminho) {
                printf("\n");
            }
            return (void *)(base + mid * size);
        }
    }
    if (exibir_caminho) {
        printf("\n");
    }
    if (low < nmemb) {
        return (void *)(base + low * size);
    }
    return NULL;
}

// Funções auxiliares para o qsort:
int qsort_corredores_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice corredores_idx
    return strcmp( ( (corredores_index *)a )->id_corredor, ( (corredores_index *)b )->id_corredor);
}

int qsort_veiculos_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice veiculos_idx
    return strcmp(((veiculos_index *)a)->id_veiculo, ((veiculos_index *)b)->id_veiculo);
}

int qsort_pistas_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice pistas_idx
    return strcmp(((pistas_index *)a)->id_pista, ((pistas_index *)b)->id_pista);
}

int qsort_corridas_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice corridas_idx
    int cmp = strcmp(((corridas_index *)a)->ocorrencia, ((corridas_index *)b)->ocorrencia);
    if(cmp == 0)
        cmp = strcmp(((corridas_index *)a)->id_pista, ((corridas_index *)b)->id_pista);
    return cmp;
}

int qsort_data_idx(const void *a, const void *b) {
    return strcmp(((corridas_index *)a)->ocorrencia, ((corridas_index *)b)->ocorrencia);
}

int qsort_nome_pista_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice nome_pista_idx
    int cmp = strcmp(((nome_pista_index *)a)->nome, ((nome_pista_index *)b)->nome);
    if(cmp == 0)
        cmp = strcmp(((nome_pista_index *)a)->id_pista, ((nome_pista_index *)b)->id_pista);
    return cmp;
}

int qsort_preco_veiculo_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice preco_veiculo_idx
    double diff = ((preco_veiculo_index *)a)->preco - ((preco_veiculo_index *)b)->preco;
    if(diff == 0)
        return strcmp(((preco_veiculo_index *)a)->id_veiculo, ((preco_veiculo_index *)b)->id_veiculo);
    return diff < 0 ? -1 : 1;
}

int qsort_corredor_veiculos_secundario_idx(const void *a, const void *b) {
// Função de comparação entre chaves do índice secundário de corredor_veiculos_secundario_idx
    return strcmp(((corredor_veiculos_secundario_index *)a)->chave_secundaria, ((corredor_veiculos_secundario_index *)b)->chave_secundaria);
}

// Cria o índice respectivo
void criar_corredores_idx() { // NÃO modificar criar_corredores_idx()
// corredores_idx: ı́ndice primário que contém o ID do corredor (chave primária) e o RRN do respectivo registro no arquivo de dados, ordenado pelo ID do corredor (id_corredor)
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
// veiculos_idx: ı́ndice primário que contém o ID do veı́culo (chave primária) e o RRN do respectivo registro no arquivo de dados, ordenado pelo ID do veı́culo (id_veiculo)
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
// pistas_idx: ı́ndice primário que contém o ID da pista (chave primária) e o RRN respectivo do registro no arquivo de pistas, ordenado pelo ID da pista (id_pista)
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
// corridas_idx: ı́ndice primário que consiste na ocorrência (data e horário) da corrida, o ID da pista em que os corredores se inscreveram e o RRN relativo ao registro no arquivo de corridas, ordenado pela ocorrência (ocorrencia) e o ID da pista (id_pista)
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
// nome_pista_idx: ı́ndice secundário que contém as pistas ordenadas por nome e a chave primária (id_pista) da pista especı́fica
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
// preco_veiculo_idx: ı́ndice secundário que contém os veı́culos ordenados por preço e a chave primária (id_veiculo) do veı́culo especı́fico
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
// corredor_veiculos_idx: ı́ndice secundário do tipo lista invertida. Será necessário manter dois ı́ndices (corredor_veiculos_primario_idx e corredor_veiculos_secundario_idx), sendo que o primário possui os IDs de corredores (id_corredor) que possuem certo modelo de veı́culo e o apontador para o próximo corredor proprietário do modelo nesse mesmo ı́ndice primário. Se não houver um próximo corredor, esse apontador deve possuir o valor -1. No ı́ndice secundário estão os modelos, assim como a referência do primeiro corredor proprietário daquele modelo no ı́ndice primário
// O ı́ndice primário não precisa estar organizado, pois cada registro já possui uma referência direta para o próximo (assim como em uma lista encadeada)
    // Inicializar os índices
    corredor_veiculos_idx.corredor_veiculos_primario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_primario_index));
    corredor_veiculos_idx.corredor_veiculos_secundario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_secundario_index));
    corredor_veiculos_idx.qtd_registros_primario = 0;
    corredor_veiculos_idx.qtd_registros_secundario = 0;

    // Percorrer todos os corredores
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        Corredor c = recuperar_registro_corredor(i);

        // Percorrer todos os veículos do corredor
        for (int j = 0; j < QTD_MAX_VEICULO; ++j) {
            if (c.veiculos[j][0] != '\0') {
                inverted_list_insert(c.veiculos[j], c.id_corredor, &corredor_veiculos_idx);
            }
        }
    }

    printf(INDICE_CRIADO, "corredor_veiculos_idx");
}

// Exibe um registro com base no RRN
bool exibir_corredor(int rrn) { // NÃO modificar exibir_corredor()
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
    if (rrn < 0)
        return false;

    Corrida c = recuperar_registro_corrida(rrn);

    printf("%s, %s, %s, %s\n", c.id_pista, c.ocorrencia, c.id_corredores, c.id_veiculos);
    return true;
}

// Recupera do arquivo o registro com o RRN informado e retorna os dados nas structs
Corredor recuperar_registro_corredor(int rrn) { // NÃO modificar recuperar_registro_corredor()
// ARQUIVO DE CORREDORES organizado em registro de tamanho fixo de 160 bytes. 
// Campos de tamanho variável: nome (max 44 bytes), apelido (max 40 bytes) e veiculo (max 45 bytes, com no máximo 3 valores, onde cada modelo de veiculo pode ter no máximo 14 bytes). O campo multi-valorado veiculo deve ter os seus valores separados por ‘|’.
// Campos de tamanho fixo: id_corredor (11 bytes), cadastro (12 bytes) e saldo (13 bytes).
// Campos separados pelo delimitador ‘;’, e cada registro tem 6 delimitadores (um para cada campo). 
// Caso registro tenha menos de 149 bytes, espaço restante deve ser preenchido com o caractere ‘#’.
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
// ARQUIVO DE VEICULOS organizado em registros de tamanho fixo de 128 bytes.
// Campos de tamanho variável: marca (max 23 bytes), modelo (max 14 bytes) e poder (max 51 bytes).
// Campos de tamanho fixo: id_veiculo (7 bytes), velocidade (4 bytes), aceleracao (4 bytes), peso (4 bytes) e valor (13 bytes).
// Campos separados pelo delimitador ‘;’, cada registro tem 8 delimitadores para os campos.
// Caso registro tenha menos que 124 bytes, espaço restante deve ser preenchido com o caractere ‘#’.
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
// ARQUIVO DE PISTAS organizado em registros de tamanho fixo de 56 bytes.
// Campo de tamanho variável: nome (max 31 bytes).
// Campos de tamanho fixo: id_pista (8 bytes), dificuldade (4 bytes), distancia (4 bytes) e recorde (4 bytes).
// Campos separados pelo delimitador ‘;’, cada registro tem 5 delimitadores para os campos.
// Caso registro tenha menos que 56 bytes, o espaço restante deve ser preenchido com o caractere ‘#’
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
// ARQUIVO DE CORRIDAS organizado em registros de tamanho fixo de 128 bytes.
// Campos de tamanho fixo: id_pista (8 bytes), ocorrencia (12 bytes), id_corredores (66 bytes), id_veiculos (42 bytes)
// Não é necessário a presença de delimitadores pois todos os campos tem tamanho fixo.
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

// Escreve em seu respectivo arquivo na posição informada (RRN)
void escrever_registro_corredor(Corredor c, int rrn) { // NÃO modificar escrever_registro_corredor()
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

// Funções principais
void cadastrar_corredor_menu(char *id_corredor, char *nome, char *apelido){
// INSERT INTO corredores VALUES ('<id_corredor>', '<nome>', '<apelido>');
// Criar uma nova conta de corredor: Ler os campos cpf, nome e apelido. Conta será criada sem saldo (00000000000.00). O campo cadastro recebe a data em que o cadastro foi realizado. Atualizar todos os ı́ndices necessários durante a inserção. Exibir a mensagem padrão SUCESSO.
// Caso tente inserir um corredor com um id_corredor (CPF) já cadastrado: imprimir mensagem ERRO_PK_REPETIDA.

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
// DELETE FROM corredores WHERE id_corredor = '<id_corredor>';
// Usuário é capaz de remover uma conta dado um CPF de um corredor (ı́ndice primário id_corredor). Remoção deverá colocar o marcador *| nas duas primeiras posições do registro removido. O espaço do registro removido não deverá ser reutilizado para novas inserções. O registro deverá continuar ocupando exatamente 160 bytes. No ı́ndice primário, o RRN correspondente ao registro removido deverá ser substituı́do por -1. Exibir mensagem SUCESSO.
// Caso conta não existe: exibir mensagem ERRO_REGISTRO_NAO_ENCONTRADO.

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
// UPDATE corredores SET saldo = saldo + '<valor>' WHERE id_corredor = '<id_corredor>';
// Usuário é capaz de adicionar valor na conta de um corredor dado seu CPF (id_corredor) e o valor desejado. O saldo deverá ser atualizado, seguido da impressão da mensagem SUCESSO.
// Caso corredor não cadastrado: imprimir mensagem ERRO_REGISTRO_NAO_ENCONTRADO. 
// Caso valor adicionado menor ou igual a zero: imprimir mensagem ERRO_VALOR_INVALIDO.

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
// UPDATE corredores SET veiculos = array_append(veiculos, '<veiculos>') WHERE id_corredor = '<id_corredor>';
// Usuário pode adicionar veı́culo a um corredor dado seu CPF (id_corredor) e o ID do veı́culo desejado (caso o corredor possua saldo para comprá-lo). Máximo de três modelos de veı́culos por corredor.
// Saldo será descontado da conta do corredor baseado no valor do veı́culo.
// O programa deve atribuir o veı́culo ao corredor, atualizar todos os ı́ndices e arquivos necessários, e imprimir a mensagem SUCESSO.
// Caso corredor não possui saldo: imprimir mensagem ERRO_SALDO_NAO_SUFICIENTE.
// Caso corredor ou veı́culo não existe: imprimir ERRO_REGISTRO_NAO_ENCONTRADO.
// Caso modelo do veı́culo novo já está presente nos veı́culos do corredor: imprimir ERRO_VEICULO_REPETIDO.

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

    // Recuperar os registros do corredor e do veículo
    Corredor corredor = recuperar_registro_corredor(corredor_encontrado->rrn);
    Veiculo veiculo = recuperar_registro_veiculo(veiculo_encontrado->rrn);

    // Verificar se o corredor tem saldo suficiente para comprar o veículo
    if (corredor.saldo < veiculo.preco) {
        printf(ERRO_SALDO_NAO_SUFICIENTE);
        return;
    }

    // Verificar se o corredor já possui o veículo
    for (int i = 0; i < QTD_MAX_VEICULO; ++i) {
        if (strcmp(corredor.veiculos[i], veiculo.modelo) == 0) {
            printf(ERRO_VEICULO_REPETIDO, id_corredor, id_veiculo);
            return;
        }
    }

    // Adicionar o veículo à lista de veículos do corredor
    for (int i = 0; i < QTD_MAX_VEICULO; ++i) {
        if (corredor.veiculos[i][0] == '\0') {
            sprintf(corredor.veiculos[i], "%s", veiculo.modelo);
            break;
        }
    }

    // Atualizar o saldo do corredor
    corredor.saldo -= veiculo.preco;

    // Escrever o registro do corredor de volta no arquivo de dados
    escrever_registro_corredor(corredor, corredor_encontrado->rrn);

    // Atualizar o índice corredor_veiculos_idx
    inverted_list_insert(veiculo.modelo, corredor.id_corredor, &corredor_veiculos_idx);

    printf(SUCESSO);
}

void cadastrar_veiculo_menu(char *marca, char *modelo, char *poder, int velocidade, int aceleracao, int peso, double preco) {
// INSERT INTO veiculos VALUES ('<marca>', '<modelo>', '<poder>', '<velocidade>', '<aceleracao>', '<peso>', '<preco>');
// Para um veı́culo ser adicionado no banco de dados: ler os campos que contém marca, modelo, poder, velocidade, aceleração, peso e preço. Campo 'id veiculo' segue a ordem de cadastro dos veı́culos. Imprimir mensagem SUCESSO.
// Caso ID do veiculo já está presente no banco de dados: imprimir mensagem ERRO_PK_REPETIDA.

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
// INSERT INTO pistas VALUES ('<nome>', '<dificuldade>', '<distancia>', '<recorde>');
// Criar nova pista: ler os campos nome, dificuldade, recorde e distancia. O campo id_pista deve ser preenchido de acordo com a quantidade de pistas cadastradas no sistema (é um valor incremental). Imprimir a mensagem SUCESSO.
// Campo dificuldade é opcional. Caso não informado, receberá o valor padrão 1.

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
// INSERT INTO corridas VALUES ('<id_pista>', '<ocorrencia>', '<id_corredores>', '<id_veiculos>');
// Executar uma nova corrida: ler os campos id_pista, ocorrencia, id_corredores e id_veiculos.
// Valor total arrecadado em uma corrida é calculado pela expressão: "V = 6 * (T * D)", sendo T a taxa de inscrição fixa (TX_FIXA) e D a dificuldade da pista em que a corrida foi executada. 
// Ordenar id_corredores pela sua colocação na corrida: 3 primeiros colocados receberão premiação. Primeiro colocado receberá: 40% de V; Segundo: 30% de V; Terceiro: 20% de V.
// Imprimir mensagem SUCESSO.
// Caso inserir uma corrida cuja chave composta (id_pista e ocorrencia) já está presente no banco de dados: imprimir mensagem ERRO_PK_REPETIDA.
// Caso executar a função com uma pista, corredores ou veı́culos que não estejam presente no banco de dados: imprimir mensagem ERRO_REGISTRO_NAO_ENCONTRADO.
// Caso corredor não possui o veı́culo atribuı́do a ele: imprimir mensagem ERRO_CORREDOR_VEICULO.

    // Verificar se a chave composta (id_pista e ocorrencia) já existe
    corridas_index index_corrida;
    strcpy(index_corrida.id_pista, id_pista);
    strcpy(index_corrida.ocorrencia, ocorrencia);
    corridas_index *found_corrida = bsearch(&index_corrida, corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx);
    
    char pk[TAM_CHAVE_CORRIDAS_IDX];
    strcpy(pk, ocorrencia);
    strcat(pk, id_pista);

    if (found_corrida) {
        
        printf(ERRO_PK_REPETIDA, pk);
        
        return;
    }              

    // Verificar se a pista existe
    pistas_index index_pista;
    strcpy(index_pista.id_pista, id_pista);
    pistas_index *found_pista = bsearch(&index_pista, pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx);

    if (!found_pista) {
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
        return;
    }
    
    // Verificar se os corredores e veículos existem e se os corredores possuem os veículos
    int j = 0;
    bool retorno_possui_veiculo = false;
    for (int i = 0; i < strlen(id_corredores); i += TAM_ID_CORREDOR, j++) {
        // Verificar se o corredor existe
        Corredor corredor;

        char id_corredor[TAM_ID_CORREDOR], *p;
        p = id_corredores + j*(TAM_ID_CORREDOR-1);
        
        corredores_index index_corredor;
        strncpy(id_corredor, p, TAM_ID_CORREDOR-1);
        id_corredor[TAM_ID_CORREDOR-1] = '\0';
        strcpy(index_corredor.id_corredor, id_corredor);
        
        //corredores_index *found_corredor = busca_binaria((void*)&index_corredor, veiculos_idx, qtd_registros_veiculos, sizeof(corredores_index), qsort_corredores_idx, false, 0);
        corredores_index *found_corredor = bsearch(&index_corredor, corredores_idx, qtd_registros_corredores, sizeof(corredores_index) , qsort_corredores_idx);

        if (!found_corredor) {
            printf(ERRO_REGISTRO_NAO_ENCONTRADO);
            return;
        }
        
        corredor = recuperar_registro_corredor(found_corredor->rrn);

        // Verificar se o veículo existe
        char id_veiculo[TAM_ID_VEICULO];
        p = id_veiculos + j*(TAM_ID_VEICULO-1);

        strncpy(id_veiculo, p, TAM_ID_VEICULO-1);
        id_veiculo[TAM_ID_VEICULO-1] = '\0';

        //Buscar veículo no índice veiculos_idx
        veiculos_index index_veiculo;
        strcpy(index_veiculo.id_veiculo, id_veiculo);
        veiculos_index *found_veiculo = bsearch(&index_veiculo, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);

        if (found_veiculo) {
            //Verificar se corredor possui o veículo
            Veiculo veiculo;
            veiculo = recuperar_registro_veiculo(found_veiculo->rrn);

            bool possui_veiculo = false;
            for (int k=0; k<QTD_MAX_VEICULO; k++) {
                if (!strcmp(veiculo.modelo, corredor.veiculos[k])) {
                    possui_veiculo = true;
                }

            }
            if (!possui_veiculo) {
                retorno_possui_veiculo = true;
                printf(ERRO_CORREDOR_VEICULO, found_corredor->id_corredor, veiculo.id_veiculo);
            }
        }
        else {
            printf(ERRO_REGISTRO_NAO_ENCONTRADO);
            return;
        }

    }
    if (retorno_possui_veiculo) {
        return;
    }

    // Calcular o valor total arrecadado na corrida e distribuir os prêmios
    Pista pista = recuperar_registro_pista(found_pista->rrn);

    double valor_total = 6 * (TX_FIXA * pista.dificuldade);
    double premio_primeiro = valor_total * 0.4;
    double premio_segundo = valor_total * 0.3;
    double premio_terceiro = valor_total * 0.2;

    // Adicionar os prêmios ao saldo dos corredores
    char *p;
    for (int i=0; i<3; i++) {
        Corredor c;
        
        p = id_corredores + i*(TAM_ID_CORREDOR-1);

        corredores_index index_corredor;
        strncpy(index_corredor.id_corredor, p, TAM_ID_CORREDOR-1);
        index_corredor.id_corredor[TAM_ID_CORREDOR-1] = '\0';
        
        corredores_index *found_corredor = bsearch(&index_corredor, corredores_idx, qtd_registros_corredores, sizeof(corredores_index) , qsort_corredores_idx);

        c = recuperar_registro_corredor(found_corredor->rrn);
        
        if (i==0)
            c.saldo += premio_primeiro;

        if (i==1)
            c.saldo += premio_segundo;

        if (i==2)
            c.saldo += premio_terceiro;

        escrever_registro_corredor(c, found_corredor->rrn);

    }

    if (retorno_possui_veiculo) {
        return;
    }
    

    // Inserir a nova corrida no banco de dados
    Corrida nova_corrida;
    strcpy(nova_corrida.id_pista, id_pista);
    strcpy(nova_corrida.ocorrencia, ocorrencia);
    strcpy(nova_corrida.id_corredores, id_corredores);
    strcpy(nova_corrida.id_veiculos, id_veiculos);
    escrever_registro_corrida(nova_corrida, qtd_registros_corridas);

    // Atualizar o índice de corridas
    strcpy(corridas_idx[qtd_registros_corridas].id_pista, id_pista);
    strcpy(corridas_idx[qtd_registros_corridas].ocorrencia, ocorrencia);
    corridas_idx[qtd_registros_corridas].rrn = qtd_registros_corridas;
    qtd_registros_corridas++;

    // Reordenar o índice de corridas
    qsort(corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx);

    printf(SUCESSO);
}

// Busca
// As seguintes operações de busca utilizam a busca binária e mostram o caminho percorrido nos ı́ndices da seguinte maneira: "Registros percorridos: 3 2 0 1". Os números representam o RRN dos registros que foram percorridos durante a busca até encontrar o registro de interesse ou esgotar as possibilidades.
// Caso número de elementos par: Há 2 possibilidades para a posição da mediana dos elementos (se o total for 10 posição da mediana: 5 ou 6). Neste caso, sempre escolha a posição mais à direita (posição 6 caso o total for 10).

void buscar_corredor_id_menu(char *id_corredor) { // NÃO modificar buscar_corredor_id_menu()
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, true, 0);
	if (found == NULL || found->rrn < 0)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else
		exibir_corredor(found->rrn);
}

void buscar_pista_id_menu(char *id_pista) {
// Usuário pode buscar pistas pelo ID da pista.
// SELECT * FROM pistas WHERE id_pista = '<id_pista>';
// Todos os dados da pista devem ser impressos na tela de forma formatada.
// Caso id_pista não existe: imprimir mensagem ERRO_REGISTRO_NAO_ENCONTRADO.

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
// Usuário pode buscar pistas pelo nome da pista.
// SELECT * FROM pistas WHERE nome = '<nome da pista>';
// Todos os dados devem ser impressos na tela de forma formatada.
// Como a busca é realizada em dois ı́ndices, é necessário exibir o caminho da busca binária para ambos: primeiro para o ı́ndice secundário e depois para o ı́ndice primário.
// Caso pista não encontrada: imprimir mensagem ERRO_REGISTRO_NAO_ENCONTRADO.

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

// Listagem
void listar_corredores_id_menu() { // NÃO modificar listar_corredores_id_menu()
	if (qtd_registros_corredores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	else
		for (unsigned int i = 0; i < qtd_registros_corredores; i++)
			exibir_corredor(corredores_idx[i].rrn);
}

void listar_corredores_modelo_menu(char *modelo) {
// SELECT * FROM veiculos WHERE '<veiculo>'= ANY (veiculos) ORDER BY id_veiculo ASC
// Exibe corredores que possuem determinado modelo de veı́culo, em ordem crescente de ID.
// Caso nenhum registro retornado: imprimir mensagem AVISO_NENHUM_REGISTRO_ENCONTRADO.
// Antes da listagem dos corredores: imprimir os registros do ı́ndice da lista invertida (corredor_veiculos_primario_idx).

    // Buscar o modelo de veículo no índice secundário
    int indice;
    if (!inverted_list_secondary_search(&indice, false, modelo, &corredor_veiculos_idx)) {
        printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
        return;
    }

    // Percorrer a lista encadeada no índice primário
    char id_corredores[MAX_REGISTROS][TAM_ID_CORREDOR];
    int qtd = inverted_list_primary_search(id_corredores, true, indice, NULL, &corredor_veiculos_idx);
    printf("\n");

    // Buscar e exibir os corredores
    for (int i = 0; i < qtd; ++i) {
        corredores_index chave;
        
        //Antes de listar dar um qsort
        qsort(id_corredores, qtd, sizeof(char) * TAM_ID_CORREDOR, qsort_string_modelo);

        strcpy(chave.id_corredor, id_corredores[i]);
        corredores_index *corredor_encontrado = bsearch(&chave, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);
        if (corredor_encontrado != NULL && corredor_encontrado->rrn >= 0) {
            exibir_corredor(corredor_encontrado->rrn);
        }
    }
}

void listar_veiculos_compra_menu(char *id_corredor) {
// SELECT * FROM veiculos WHERE preco <= ('SELECT saldo FROM corredores WHERE id_corredor = <id_corredor> ');
// Ler o ID de um corredor e exibir todos os veı́culos que o corredor pode comprar baseado em seu saldo.
// Caso nenhum registro for retornado: imprimir mensagem AVISO_NENHUM_REGISTRO_ENCONTRADO.

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
// SELECT * FROM corridas WHERE ocorrencia BETWEEN '<data_inicio>' AND '<data_fim>' ORDER BY ocorrencia ASC;
// Exibe corridas realizadas em determinado perı́odo de tempo (data entre <data_inicio> e <data_fim>), em ordem cronológica. Ambas as datas estarão no formato <AAAAMMDDHHMM>.
// Para cada registro encontrado na listagem: imprimir caminho percorrido.
// Caso nenhum registro for retornado: imprimir mensagem AVISO_NENHUM_REGISTRO_ENCONTRADO.
// Antes de imprimir a lista de corridas: imprimir caminho percorrido durante a busca binária para encontrar o registro cuja <ocorrencia> seja igual à <data_inicio> informada pelo usuário ou data posterior mais próxima.
    corridas_index chave;
    strcpy(chave.ocorrencia, data_inicio);
    corridas_index *corrida_inicio = busca_binaria_corridas(&chave, corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx, true);

    if (corrida_inicio == NULL || corrida_inicio->rrn < 0) {
        printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
        return;
    }

    for (int i = corrida_inicio - corridas_idx; i < qtd_registros_corridas; ++i) {
        if (strcmp(corridas_idx[i].ocorrencia, data_fim) > 0) {
            break;
        }

        if (!exibir_corrida(corridas_idx[i].rrn)) {
            printf(ERRO_REGISTRO_NAO_ENCONTRADO);
            return;
        }
    }
}

// Liberar espaço
void liberar_espaco_menu() {
// VACUUM corredores;
// ARQUIVO_CORREDORES deve ser reorganizado com a remoção fı́sica de todos os registros marcados como excluı́dos e os ı́ndices deverão ser atualizados. A ordem dos registros no arquivo atualizado deverá ser igual do arquivo original. Imprimir mensagem SUCESSO.


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

// Imprimir arquivos de dados (Não modificar)
// Caso vazio: imprimir mensagem ERRO_ARQUIVO_VAZIO.

void imprimir_arquivo_corredores_menu() {
// \echo file ARQUIVO_CORREDORES

	if (qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDORES);
}

void imprimir_arquivo_veiculos_menu() {
// \echo file ARQUIVO_CORREDORES

	if (qtd_registros_veiculos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_VEICULOS);
}

void imprimir_arquivo_pistas_menu() {
// \echo file ARQUIVO_CORREDORES

	if (qtd_registros_pistas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PISTAS);
}

void imprimir_arquivo_corridas_menu() {
// \echo file ARQUIVO_CORREDORES

	if (qtd_registros_corridas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORRIDAS);
}

// Imprimir índices primários
// Caso vazio: imprimir ERRO_ARQUIVO_VAZIO;

void imprimir_corredores_idx_menu() { // NÃO modificar imprimir_corredores_idx_menu()
// \echo index veiculos_idx

	if (corredores_idx == NULL || qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_corredores; ++i)
			printf("%s, %d\n", corredores_idx[i].id_corredor, corredores_idx[i].rrn);
}

void imprimir_veiculos_idx_menu() {
// \echo index veiculos_idx

    if (veiculos_idx == NULL || qtd_registros_veiculos == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_veiculos; ++i)
            printf("%s, %d\n", veiculos_idx[i].id_veiculo, veiculos_idx[i].rrn);
}

void imprimir_pistas_idx_menu() {
// \echo index pistas_idx

    if (pistas_idx == NULL || qtd_registros_pistas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_pistas; ++i)
            printf("%s, %d\n", pistas_idx[i].id_pista, pistas_idx[i].rrn);
}

void imprimir_corridas_idx_menu() {
// \echo index corridas_idx

    if (corridas_idx == NULL || qtd_registros_corridas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_corridas; ++i)
            printf("%s, %s, %d\n", corridas_idx[i].ocorrencia, corridas_idx[i].id_pista, corridas_idx[i].rrn);
}

// Imprimir índices secundários
void imprimir_nome_pista_idx_menu() {
// \echo index nome_pista_idx

    if (nome_pista_idx == NULL || qtd_registros_pistas == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_pistas; ++i)
            printf("%s, %s\n", strupr(nome_pista_idx[i].nome), nome_pista_idx[i].id_pista);
}

void imprimir_preco_veiculo_idx_menu() {
// \echo index preco_veiculo_idx

    if (preco_veiculo_idx == NULL || qtd_registros_veiculos == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < qtd_registros_veiculos; ++i){
            printf("%.2lf, %s\n", preco_veiculo_idx[i].preco, preco_veiculo_idx[i].id_veiculo);
		}
}

void imprimir_corredor_veiculos_secundario_idx_menu() {
// \echo index corredor_veiculos_secundario_idx
// Imprime structs de ı́ndice secundário com os modelos de veı́culos (corredor_veiculos_secundario_idx) e o número de ı́ndice para o primeiro corredor proprietário desse modelo.

    if (corredor_veiculos_idx.corredor_veiculos_secundario_idx == NULL || corredor_veiculos_idx.qtd_registros_secundario == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_secundario; ++i)
            printf("%s, %d\n", strupr(corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria), corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].primeiro_indice);
}

void imprimir_corredor_veiculos_primario_idx_menu() {
// \echo index corredor_veiculos_primario_idx
// Imprime structs de ı́ndice secundário com o ID de um corredor proprietário de um modelo de veı́culo (corredor_veiculos_primario_idx) e o número de ı́ndice para o próximo corredor proprietário.
    if (corredor_veiculos_idx.corredor_veiculos_primario_idx == NULL || corredor_veiculos_idx.qtd_registros_primario == 0)
        printf(ERRO_ARQUIVO_VAZIO);
    else
        for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_primario; ++i)
            printf("%s, %d\n", corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria, corredor_veiculos_idx.corredor_veiculos_primario_idx[i].proximo_indice);
}

void liberar_memoria_menu() {
// "/q" Libera memória e encerra programa.
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
// inverted_list_insert é responsável por inserir duas chaves (chave_secundaria e chave_primaria) em uma Lista Invertida (t).
    corredor_veiculos_secundario_index *result;
    int indice_primario = t->qtd_registros_primario;
    int indice_secundario = t->qtd_registros_secundario;

    result = (corredor_veiculos_secundario_index*)bsearch(chave_secundaria, t->corredor_veiculos_secundario_idx, t->qtd_registros_secundario, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx);

    
    //Elemento não existe no índice secundário
    if (!result) {
        //Adiciona ao índice secundário
        strcpy(t->corredor_veiculos_secundario_idx[indice_secundario].chave_secundaria, chave_secundaria);
        t->corredor_veiculos_secundario_idx[indice_secundario].primeiro_indice = indice_primario;
        t->qtd_registros_secundario++;
        qsort(t->corredor_veiculos_secundario_idx, t->qtd_registros_secundario, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx);

        //Adiciona ao índice primário 
        strcpy(t->corredor_veiculos_primario_idx[indice_primario].chave_primaria, chave_primaria);
        t->corredor_veiculos_primario_idx[indice_primario].proximo_indice = -1;
        t->qtd_registros_primario++;

        return;
    }

    //Elemento existe no índice secundário
    else {        
        int cont = result->primeiro_indice;        

        while (t->corredor_veiculos_primario_idx[cont].proximo_indice != -1) {
            cont = t->corredor_veiculos_primario_idx[cont].proximo_indice;
        }

        indice_secundario = cont;
        
        t->corredor_veiculos_primario_idx[indice_secundario].proximo_indice = t->qtd_registros_primario;
        indice_secundario = t->qtd_registros_primario;
        strcpy(t->corredor_veiculos_primario_idx[indice_secundario].chave_primaria, chave_primaria);
        t->corredor_veiculos_primario_idx[indice_secundario].proximo_indice = -1;
        t->qtd_registros_primario++;
    }
}

bool inverted_list_secondary_search(int *result, bool exibir_caminho, char *chave_secundaria, inverted_list *t) {
// inverted_list_secondary_search é responsável por buscar uma chave no índice secundário de uma Lista invertida (T). O valor de retorno indica se a chave foi encontrada ou não.    
    corredor_veiculos_secundario_index *secundario_index;
    corredor_veiculos_secundario_index chave;

    strcpy(chave.chave_secundaria, chave_secundaria);
    
    secundario_index = (corredor_veiculos_secundario_index *) bsearch(&chave, corredor_veiculos_idx.corredor_veiculos_secundario_idx, corredor_veiculos_idx.qtd_registros_secundario, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx);
    
    if (secundario_index) {
        *result = secundario_index->primeiro_indice;
        return true;
    }
    else
        *result = -1;
    return false;
}

int inverted_list_primary_search(char result[][TAM_ID_CORREDOR], bool exibir_caminho, int indice, int *indice_final, inverted_list *t) {
// inverted_list_primary_search é responsável por percorrer o índice primário de uma Lista invertida (T). O valor de retorno indica a quantidade de chaves encontradas.
    int count = 0;
    if (exibir_caminho) {
        printf(REGS_PERCORRIDOS);
    }
    while (indice != -1) {
        if (result != NULL) {
            strcpy(result[count], t->corredor_veiculos_primario_idx[indice].chave_primaria);
        }
        count++;
        if (exibir_caminho) {
            printf(" %d", indice);
        }
        indice = t->corredor_veiculos_primario_idx[indice].proximo_indice;
    }
    if (indice_final != NULL) {
        *indice_final = indice;
    }
    return count;
}

void* busca_binaria_com_reps(const void *key, const void *base0, size_t nmemb, size_t size, 
// Função Genérica de busca binária que aceita parâmetros genéricos (assinatura baseada na função bsearch da biblioteca C).
    int (*compar)(const void *, const void *), bool exibir_caminho, int posicao_caso_repetido, int retorno_se_nao_encontrado) {
    const char *base = base0;
    size_t low = 0;
    size_t high = nmemb;
    size_t mid;
    int cond;
    
    if (exibir_caminho) {
        printf(REGS_PERCORRIDOS);
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

int qsort_string_modelo(const void *a, const void *b) {
    return strcmp((char *)a, (char *)b);
}