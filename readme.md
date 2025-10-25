# Sistema de Gerenciamento para Oficina Mecânica (Estrutura de Dados I)

## 1. Descrição do Projeto
Este projeto consiste no desenvolvimento de um sistema em Linguagem C que simula as operações essenciais de uma oficina mecânica. O objetivo principal é permitir o cadastro de clientes, veículos, abertura e gerenciamento de ordens de serviço, além da geração de relatórios. O sistema deve armazenar os dados em arquivos, garantindo persistência.

---

## 2. Requisitos Funcionais (Funcionalidades Obrigatórias)

O sistema implementa as seguintes operações obrigatórias:

### 2.1 Cadastro de Clientes
- **Atributos:** Nome, CPF e Telefone.  
- **Operações:** Inserção, remoção e atualização.  
- **Persistência:** Salvamento e recuperação em arquivo.

### 2.2 Cadastro de Veículos
- **Atributos:** Placa (identificador único), Modelo, Ano e ponteiro para o cliente (dono).  
- **Operações:** Inserção, remoção e atualização.  
- **Persistência:** Salvamento e recuperação em arquivo.

### 2.3 Gerenciamento de Ordens de Serviço (OS)
- **Atributos:** ID da Ordem, ponteiro para o Veículo, Data de Entrada, Descrição do Problema, e Status.  
- **Status (Enum):** AGUARDANDO_AVALIACAO, EM_REPARO, FINALIZADO, ENTREGUE.  
- **Operações:** Abrir, atualizar e encerrar ordens de serviço.  
- **Relatórios:** Listar ordens por cliente, veículo ou status.  
- **Persistência:** Salvamento e recuperação em arquivo.

### 2.4 Relatórios
O sistema gera relatórios por meio do preenchimento de arquivos de texto:
- Histórico de serviços de um veículo.  
- Ordens de serviço abertas em um dia específico.  
- Listar todos os veículos cadastrados de um cliente.  
- Relatório de ordens de serviço por status.  
- Relatório de clientes mais recorrentes.

---

## 3. Especificações Técnicas (Estrutura de Dados)

O projeto cumpre rigorosamente os seguintes requisitos técnicos:

- **Modularização:** Código dividido em módulos lógicos, utilizando `structs` e `enums` para representar dados.  
- **Alocação Dinâmica:** Uso de `malloc` e `free` para alocação dinâmica de memória.  
- **Vetores Dinâmicos:** Vetores dinâmicos para armazenar registros.  
- **Ponteiros:** Uso de ponteiros para manipulação de clientes, veículos e ordens.  
- **Persistência:** Manipulação de arquivos para leitura e escrita de clientes, veículos, ordens de serviço e relatórios.  
- **Busca Eficiente:** Implementação de busca eficiente (ex.: localizar veículo pela placa ou cliente pelo CPF).  
- **Validação:** Validação de dados de entrada (ex.: evitar CPFs inválidos ou placas duplicadas).  
- **Manipulação de Strings:** Manipulação de strings com ponteiros (cópia, concatenação, comparação).

---

## 4. Manual de Uso (Compilação e Execução)

### 4.1 Estrutura de Diretórios
O projeto utiliza uma estrutura modular, onde arquivos de implementação (`.c`) e arquivos de cabeçalho de protótipo (`.h`) estão separados.

### 4.2 Compilação
Para compilar o projeto, utilize o compilador GCC, listando todos os arquivos de código-fonte (`.c`) na linha de comando:

```bash
gcc main.c modules/client.c modules/car.c modules/service_order.c modules/reports.c -o oficina_app
```

### 4.3 Execução
Após a compilação, execute o programa gerado:

```bash
./oficina_app
```