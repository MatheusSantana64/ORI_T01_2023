/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. Almeida
 *
 * Trabalho 01 - Indexação
 * 
  * ========================================================================== *
 *                             <<< NÃO ALTERAR >>>
 * ========================================================================== */
 
/* Bibliotecas */
#include "ORI_T01_HEADER.h"
#include "ORI_T01_HEADER.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

int main() {
    // variáveis utilizadas pelo interpretador de comandos
    char input[500];
    uint64_t seed = 2;
    char datetime[TAM_DATETIME] = "202103181430"; // UTC 18/03/2021 14:30:00
    char id_corredor[TAM_ID_CORREDOR];
    char nome_corredor[TAM_MAX_NOME_CORREDOR];
    char apelido[TAM_MAX_APELIDO];
	double valor;
    char id_veiculo[TAM_ID_VEICULO];
    char marca[TAM_MAX_MARCA];
    char modelo[TAM_MAX_MODELO];
    char poder[TAM_MAX_PODER];
    int velocidade;
    int aceleracao;
    int peso;
    double preco;
    char id_pista[TAM_ID_PISTA];
    char nome_pista[TAM_MAX_NOME_PISTA];
    int dificuldade;
    int distancia;
    int recorde;
    char ocorrencia[TAM_DATETIME];
    char corredores[TAM_ID_CORREDORES];
    char veiculos[TAM_ID_VEICULOS];
    char data_inicio[TAM_DATETIME];
    char data_fim[TAM_DATETIME];
 
    scanf("SET ARQUIVO_CORREDORES TO '%[^']';\n", ARQUIVO_CORREDORES);
    int temp_len = strlen(ARQUIVO_CORREDORES);
    qtd_registros_corredores = temp_len / TAM_REGISTRO_CORREDOR;
    ARQUIVO_CORREDORES[temp_len] = '\0';
 
    scanf("SET ARQUIVO_VEICULOS TO '%[^']';\n", ARQUIVO_VEICULOS);
    temp_len = strlen(ARQUIVO_VEICULOS);
    qtd_registros_veiculos = temp_len / TAM_REGISTRO_VEICULO;
    ARQUIVO_VEICULOS[temp_len] = '\0';
 
    scanf("SET ARQUIVO_PISTAS TO '%[^']';\n", ARQUIVO_PISTAS);
    temp_len = strlen(ARQUIVO_PISTAS);
    qtd_registros_pistas = temp_len / TAM_REGISTRO_PISTA;
    ARQUIVO_PISTAS[temp_len] = '\0';

    scanf("SET ARQUIVO_CORRIDAS TO '%[^']';\n", ARQUIVO_CORRIDAS);
    temp_len = strlen(ARQUIVO_CORRIDAS);
    qtd_registros_corridas = temp_len / TAM_REGISTRO_CORRIDA;
    ARQUIVO_CORRIDAS[temp_len] = '\0';
 
    // inicialização do gerador de números aleatórios e função de datas
    prng_srand(seed);
    setenv("TZ", "UTC", 1);
    set_time(datetime);
 
    criar_corredores_idx();
    criar_veiculos_idx();
    criar_pistas_idx();
    criar_corridas_idx();
    criar_nome_pista_idx();
    criar_preco_veiculo_idx();
    criar_corredor_veiculos_idx();
 
    while (1) {
        fgets(input, 500, stdin);
        printf("%s", input);
        clear_input(input);
 
        if (strcmp("", input) == 0)
            continue; // não avança o tempo nem imprime o comando este seja em branco
 
        /* Funções principais */
        if (sscanf(input, "INSERT INTO corredores VALUES ('%[^']', '%[^']', '%[^']'');", id_corredor, nome_corredor, apelido) == 3)
            cadastrar_corredor_menu(id_corredor, nome_corredor, apelido);
        else if (sscanf(input, "DELETE FROM corredores WHERE id_corredor = '%[^']';", id_corredor) == 1)
            remover_corredor_menu(id_corredor);
        else if (sscanf(input, "UPDATE corredores SET saldo = saldo + %lf WHERE id_corredor = '%[^']';", &valor, id_corredor) == 2)
            adicionar_saldo_menu(id_corredor, valor);
        else if (sscanf(input, "UPDATE corredores SET veiculos = array_append(veiculos, '%[^']') WHERE id_corredor = '%[^']';", id_veiculo, id_corredor) == 2)
            comprar_veiculo_menu(id_corredor, id_veiculo);
        else if (sscanf(input, "INSERT INTO veiculos VALUES ('%[^']', '%[^']', '%[^']', %d, %d, %d, %lf);", marca, modelo, poder, &velocidade, &aceleracao, &peso, &preco) == 7)
            cadastrar_veiculo_menu(marca, modelo, poder, velocidade, aceleracao, peso, preco);
        else if (sscanf(input, "INSERT INTO pistas VALUES ('%[^']', %d, %d, %d);", nome_pista, &dificuldade, &distancia, &recorde) == 4)
            cadastrar_pista_menu(nome_pista, dificuldade, distancia, recorde);
        else if(sscanf(input, "INSERT INTO corridas VALUES ('%[^']', '%[^']', '%[^']', '%[^']');", id_pista, ocorrencia, corredores, veiculos) == 4)
            executar_corrida_menu(id_pista, ocorrencia, corredores, veiculos);
 
        /* Busca */
        else if (sscanf(input, "SELECT * FROM corredores WHERE id_corredor = '%[^']';", id_corredor) == 1)
            buscar_corredor_id_menu(id_corredor);
        else if (sscanf(input, "SELECT * FROM pistas WHERE id_pista = '%[^']';", id_pista) == 1)
            buscar_pista_id_menu(id_pista);
        else if (sscanf(input, "SELECT * FROM pistas WHERE nome = '%[^']';", nome_pista) == 1)
            buscar_pista_nome_menu(nome_pista);
 
        /* Listagem */
        else if (strcmp("SELECT * FROM corredores ORDER BY id_corredor ASC;", input) == 0)
            listar_corredores_id_menu();
        else if (sscanf(input, "SELECT * FROM corredores WHERE '%[^']' IN array_append(veiculos) ORDER BY id_corredor ASC;", modelo) == 1)
            listar_corredores_modelo_menu(modelo);
        else if (sscanf(input, "SELECT * FROM veiculos WHERE preco <= ('SELECT saldo FROM corredores WHERE id_corredor = '%[^']' ');", id_corredor) == 1)
            listar_veiculos_compra_menu(id_corredor);
        else if (sscanf(input, "SELECT * FROM corridas WHERE ocorrencia BETWEEN '%[^']' AND '%[^']' ORDER BY ocorrencia ASC;", data_inicio, data_fim) == 2)
            listar_corridas_periodo_menu(data_inicio, data_fim);
 
        /* Liberar espaço */
        else if (strcmp("VACUUM corredores;", input) == 0)
            liberar_espaco_menu();
 
        /* Imprimir arquivos de dados */
        else if (strcmp("\\echo file ARQUIVO_CORREDORES", input) == 0)
            imprimir_arquivo_corredores_menu();
        else if (strcmp("\\echo file ARQUIVO_VEICULOS", input) == 0)
            imprimir_arquivo_veiculos_menu();
        else if (strcmp("\\echo file ARQUIVO_PISTAS", input) == 0)
            imprimir_arquivo_pistas_menu();
        else if (strcmp("\\echo file ARQUIVO_CORRIDAS", input) == 0)
            imprimir_arquivo_corridas_menu();
        
        /* Imprimir índices primários */
        else if (strcmp("\\echo index corredores_idx", input) == 0)
            imprimir_corredores_idx_menu();
        else if (strcmp("\\echo index veiculos_idx", input) == 0)
            imprimir_veiculos_idx_menu();
        else if (strcmp("\\echo index pistas_idx", input) == 0)
            imprimir_pistas_idx_menu();
        else if (strcmp("\\echo index corridas_idx", input) == 0)
            imprimir_corridas_idx_menu();
        
        /* Imprimir índices secundários */
        else if (strcmp("\\echo index nome_pista_idx", input) == 0)
            imprimir_nome_pista_idx_menu();
        else if (strcmp("\\echo index preco_veiculo_idx", input) == 0)
            imprimir_preco_veiculo_idx_menu();
        else if (strcmp("\\echo index corredor_veiculos_secundario_idx", input) == 0)
            imprimir_corredor_veiculos_secundario_idx_menu();
        else if (strcmp("\\echo index corredor_veiculos_primario_idx", input) == 0)
            imprimir_corredor_veiculos_primario_idx_menu();
 
        /* Liberar memória eventualmente alocada e encerrar programa */
        else if (strcmp("\\q", input) == 0)
            { liberar_memoria_menu(); return 0; }
        else if (sscanf(input, "SET SRAND %lu;", &seed) == 1)
            { prng_srand(seed); printf(SUCESSO); continue; }
        else if (sscanf(input, "SET TIME '%[^']';", datetime) == 1)
            { if (set_time(datetime)) printf(SUCESSO); else printf(ERRO_VALOR_INVALIDO); continue; }
        else
            printf(ERRO_OPCAO_INVALIDA);
 
        tick_time();
    }
}
