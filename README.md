# Aeroportos em Grafo – Matriz Esparsa de Adjacência
**Disciplina:** Estrutura e Recuperação de Dados II  
**Instituição:** PUC-Campinas  
**Entrega:** 03/junho

---

## Estrutura do projeto

```
aeroportos/
├── sparse_matrix.h   ← Matriz esparsa genérica (interface)
├── sparse_matrix.c   ← Implementação da matriz esparsa
├── airport_vector.h  ← Vetor dinâmico de aeroportos (interface)
├── airport_vector.c  ← Implementação do vetor dinâmico
├── airport_graph.h   ← Grafo de aeroportos (interface)
├── airport_graph.c   ← Implementação do grafo (5 operações)
├── main.c            ← Menu interativo
└── Makefile
```

## Compilar e executar

```bash
# Compilar
make

# Executar
make run

# ou
./aeroportos

# Limpar arquivos objeto
make clean
```

## Operações implementadas

| # | Operação |
|---|----------|
| 1 | Cadastrar novo aeroporto |
| 2 | Cadastrar voo (número) entre dois aeroportos |
| 3 | Remover voo pelo número |
| 4 | Listar voos que partem de um aeroporto |
| 5 | Listar todos os trajetos entre dois aeroportos (DFS) |

## Decisões de projeto

- **Matriz esparsa:** lista encadeada de linhas → lista encadeada de colunas  
  (sem alocação de `n×n`, só armazena células com dados)
- **Genérica:** `void *data` + `size_t data_size`, copiado via `memcpy`
- **Vetor dinâmico:** dobra ao atingir capacidade, encolhe abaixo de 25%
- **Busca de trajetos:** DFS com controle de visitados (evita ciclos)
