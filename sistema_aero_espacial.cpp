#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum EstadoVoo {
    PLANEJADO,
    EM_CURSO,
    SUCESSO,
    EXPLOSAO
};


class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;
    vector<int> voosParticipados;

    Astronauta(string cpf, int idade, string nome) {
        this->cpf = cpf;
        this->idade = idade;
        this->nome = nome;
        this->vivo = true;
        this->disponivel = true;
    }
};


class Voo {
public:
    int codigo;
    vector<string> cpfs;
    EstadoVoo estado;

    Voo(int codigo) {
        this->codigo = codigo;
        this->estado = PLANEJADO;
    }
};


vector<Astronauta> astronautas;
vector<Voo> voos;


Astronauta* buscarAstronauta(string cpf) {
    for (auto &a : astronautas) {
        if (a.cpf == cpf) return &a;
    }
    return nullptr;
}

Voo* buscarVoo(int codigo) {
    for (auto &v : voos) {
        if (v.codigo == codigo) return &v;
    }
    return nullptr;
}

string estadoToString(EstadoVoo estado) {
    switch (estado) {
        case PLANEJADO: return "planejado";
        case EM_CURSO: return "em curso";
        case SUCESSO: return "finalizado com sucesso";
        case EXPLOSAO: return "finalizado com explosao";
    }
    return "";
}

void cadastrarAstronauta(string cpf, int idade, string nome) {
    if (buscarAstronauta(cpf)) {
        cout << "Erro: astronauta com CPF " << cpf << " ja existe\n";
        return;
    }

    astronautas.push_back(Astronauta(cpf, idade, nome));
    cout << "Astronauta cadastrado\n";
}

void cadastrarVoo(int codigo) {
    if (buscarVoo(codigo)) {
        cout << "Erro: voo ja existe\n";
        return;
    }

    voos.push_back(Voo(codigo));
    cout << "Voo cadastrado\n";
}

void adicionarAstronauta(string cpf, int codigo) {
    Astronauta* a = buscarAstronauta(cpf);
    Voo* v = buscarVoo(codigo);

    if (!a) {
        cout << "Erro: astronauta nao encontrado\n";
        return;
    }

    if (!v) {
        cout << "Erro: voo nao encontrado\n";
        return;
    }

    if (v->estado != PLANEJADO) {
        cout << "Erro: voo nao esta planejado\n";
        return;
    }

    if (!a->vivo) {
        cout << "Erro: astronauta morto\n";
        return;
    }

    for (auto c : v->cpfs) {
        if (c == cpf) {
            cout << "Erro: astronauta ja esta no voo\n";
            return;
        }
    }

    v->cpfs.push_back(cpf);
    cout << "Astronauta adicionado ao voo\n";
}

void removerAstronauta(string cpf, int codigo) {
    Astronauta* a = buscarAstronauta(cpf);
    Voo* v = buscarVoo(codigo);

    if (!a || !v) {
        cout << "Erro: dados invalidos\n";
        return;
    }

    if (v->estado != PLANEJADO) {
        cout << "Erro: voo nao esta planejado\n";
        return;
    }

    for (int i = 0; i < v->cpfs.size(); i++) {
        if (v->cpfs[i] == cpf) {
            v->cpfs.erase(v->cpfs.begin() + i);
            cout << "Astronauta removido do voo\n";
            return;
        }
    }

    cout << "Erro: astronauta nao esta no voo\n";
}

void lancarVoo(int codigo) {
    Voo* v = buscarVoo(codigo);

    if (!v) {
        cout << "Erro: voo nao encontrado\n";
        return;
    }

    if (v->estado != PLANEJADO) {
        cout << "Erro: voo nao esta planejado\n";
        return;
    }

    if (v->cpfs.empty()) {
        cout << "Erro: voo sem astronautas\n";
        return;
    }

    for (auto cpf : v->cpfs) {
        Astronauta* a = buscarAstronauta(cpf);
        if (!a->vivo || !a->disponivel) {
            cout << "Erro: astronauta indisponivel ou morto\n";
            return;
        }
    }

    for (auto cpf : v->cpfs) {
        Astronauta* a = buscarAstronauta(cpf);
        a->disponivel = false;
        a->voosParticipados.push_back(codigo);
    }

    v->estado = EM_CURSO;
    cout << "Voo lancado\n";
}

void explodirVoo(int codigo) {
    Voo* v = buscarVoo(codigo);

    if (!v) {
        cout << "Erro: voo nao encontrado\n";
        return;
    }

    if (v->estado != EM_CURSO) {
        cout << "Erro: voo nao esta em curso\n";
        return;
    }

    v->estado = EXPLOSAO;

    for (auto cpf : v->cpfs) {
        Astronauta* a = buscarAstronauta(cpf);
        a->vivo = false;
        a->disponivel = false;
    }

    cout << "Voo explodiu\n";
}

void finalizarVoo(int codigo) {
    Voo* v = buscarVoo(codigo);

    if (!v) {
        cout << "Erro: voo nao encontrado\n";
        return;
    }

    if (v->estado != EM_CURSO) {
        cout << "Erro: voo nao esta em curso\n";
        return;
    }

    v->estado = SUCESSO;

    for (auto cpf : v->cpfs) {
        Astronauta* a = buscarAstronauta(cpf);
        if (a->vivo) {
            a->disponivel = true;
        }
    }

    cout << "Voo finalizado com sucesso\n";
}

void listarVoos() {
    for (int estado = 0; estado <= 3; estado++) {
        cout << "\n=== " << estadoToString((EstadoVoo)estado) << " ===\n";

        for (auto &v : voos) {
            if (v.estado == estado) {
                cout << "Voo " << v.codigo << "\n";

                for (auto cpf : v.cpfs) {
                    Astronauta* a = buscarAstronauta(cpf);
                    cout << " - " << cpf << " " << a->nome << "\n";
                }
            }
        }
    }
}

void listarMortos() {
    for (auto &a : astronautas) {
        if (!a.vivo) {
            cout << a.cpf << " " << a.nome << " Voos: ";

            for (auto v : a.voosParticipados) {
                cout << v << " ";
            }

            cout << "\n";
        }
    }
}


int main() {
    string comando;

    while (cin >> comando) {
        if (comando == "FIM") break;

        if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);
            cadastrarAstronauta(cpf, idade, nome);
        }

        else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            cadastrarVoo(codigo);
        }

        else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            adicionarAstronauta(cpf, codigo);
        }

        else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            removerAstronauta(cpf, codigo);
        }

        else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            lancarVoo(codigo);
        }

        else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            explodirVoo(codigo);
        }

        else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            finalizarVoo(codigo);
        }

        else if (comando == "LISTAR_VOOS") {
            listarVoos();
        }

        else if (comando == "LISTAR_MORTOS") {
            listarMortos();
        }
    }

    return 0;
}
