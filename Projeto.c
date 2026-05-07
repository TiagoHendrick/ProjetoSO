#include <iostream>
#include <vector>
#include <cstring>  // Para strcmp, strtok, strcspn
#include <cstdlib>  // Para system(), exit(), atoi()
#include <filesystem> // Para validar se ficheiros existem
#include <stdbool.h>

using namespace std;
namespace fs = std::filesystem;

// Função para validar se o ficheiro existe antes de ler/remover/copiar
bool ficheiroExiste(char* nome) {
    if (nome == NULL) return false;
    return fs::exists(nome);
}

int main() {
    char array[256];  
    char *palavras[33]; 

    cout << "===============================================" << endl;
    cout << "   UESH - INTERPRETADOR DE COMANDOS (TESTE)    " << endl;
    cout << "   Pronto para correr na VM Linux              " << endl;
    cout << "===============================================" << endl;

    while (true) {
        cout << "\nUEsh> ";
        
        // 1. LEITURA SEGURA
        if (fgets(array, sizeof(array), stdin) == NULL) break;
        array[strcspn(array, "\n")] = 0; // Remove o \n final

        // 2. TOKENIZAÇÃO (Lógica de Ponteiros)
        char* stringSegment = strtok(array, " ");
        int slot = 0;
        while (stringSegment != NULL && slot < 32) {
            palavras[slot] = stringSegment;
            slot++;
            stringSegment = strtok(NULL, " ");
        }
        palavras[slot] = NULL; // Fim do array de ponteiros

        if (palavras[0] == NULL) continue; // Se o utilizador só carregou no Enter

        // --- EXECUTOR DE COMANDOS ---

        // [EXIT]
        if (strcmp(palavras[0], "exit") == 0 || strcmp(palavras[0], "Exit") == 0) {
            printf_s("A encerrar... Ate a proxima!");
            exit(0);
        }

        // [LS] - Listar ficheiros
        else if (strcmp(palavras[0], "ls") == 0) {
            system("ls -l --color=auto");
        }

        // [PWD] - Onde estou?
        else if (strcmp(palavras[0], "pwd") == 0) {
            system("pwd");
        }

        // [ECHO] - Repetir o que foi escrito
        else if (strcmp(palavras[0], "echo") == 0) {
            for (int i = 1; palavras[i] != NULL; i++) {
                cout << palavras[i] << " ";
            }
            cout << endl;
        }

        // [RM] - Remover (com validação)
        else if (strcmp(palavras[0], "rm") == 0) {
            if (palavras[1] == NULL) {
                cout << "Erro: Use 'rm [ficheiro]'" << endl;
            } else if (!ficheiroExiste(palavras[1])) {
                cout << "Erro: Ficheiro nao encontrado." << endl;
            } else {
                char confirma;
                cout << "Apagar " << palavras[1] << "? (s/n): ";
                cin >> confirma; cin.ignore();
                if (confirma == 's' || confirma == 'S') {
                    string cmd = string("rm ") + palavras[1];
                    system(cmd.c_str());
                    cout << "Removido com sucesso." << endl;
                }
            }
        }

        
        // [CP] - Copiar
        else if (strcmp(palavras[0], "cp") == 0) {
            if (palavras[1] == NULL || palavras[2] == NULL) {
                cout << "Erro: Use 'cp [origem] [destino]'" << endl;
            } else if (!ficheiroExiste(palavras[1])) {
                cout << "Erro: Origem nao existe." << endl;
            } else {
                string cmd = string("cp ") + palavras[1] + " " + palavras[2];
                system(cmd.c_str());
                cout << "Copia concluida." << endl;
            }
        }

        // [SORT] - Ordenar ficheiro
        else if (strcmp(palavras[0], "sort") == 0) {
            if (palavras[1] == NULL) {
                cout << "Erro: Indique o ficheiro." << endl;
            } else if (!ficheiroExiste(palavras[1])) {
                cout << "Erro: Ficheiro nao existe." << endl;
            } else {
                string cmd = string("sort ") + palavras[1];
                cout << "--- Ordenado ---" << endl;
                system(cmd.c_str());
            }
        }

        // [KILL] - Matar processo
        else if (strcmp(palavras[0], "kill") == 0) {
            if (palavras[1] == NULL) {
                cout << "Erro: Indique o PID." << endl;
            } else {
                // Validação de número (atoi devolve 0 se falhar)
                int pid = atoi(palavras[1]);
                if (pid == 0 && strcmp(palavras[1], "0") != 0) {
                    cout << "Erro: PID invalido." << endl;
                } else {
                    string cmd = string("kill -9 ") + palavras[1];
                    system(cmd.c_str());
                    cout << "Comando enviado para PID " << pid << endl;
                }
            }
        }

        // COMANDO DESCONHECIDO (Tenta enviar direto para o Linux)
        else {
            cout << "UEsh: comando interno nao encontrado. A tentar sistema..." << endl;
            system(array); // Tenta rodar comandos que nao programaste (ex: mkdir)
        }
    }

    return 0;
}
