////////////////////////////////
/// Inclusï¿½o das bibliotecas ///
////////////////////////////////
#include <string>
#include <iostream>

// Se estiver rodando no windows
// precisamos incluir a biblioteca
// de modo diferente
#ifdef __WIN32
	#include <mysql.h>
#else
	// Em outros sistemas, creio eu,
	// ï¿½ desse outro jeito
	#include <mysql/mysql.h>
#endif

///////////////////
/// Protï¿½tipos ////
///////////////////
void exibeMenu();
void desconectar();
void verCompromissos();
void mostrarErroDoMysql();
void adicionarCompromisso();
void editarCompromisso();

/////////////////////////////
/// Variï¿½veis globais X<( ///
/////////////////////////////

/**
 * Contï¿½m a conexï¿½o com o banco de dados
 */
MYSQL *connexao;
bool conectado = false;

//////////////////
/// Constantes ///
//////////////////

const unsigned int ADICIONAR_COMPROMISSO = 0;
const unsigned int REMOVER_COMPROMISSO = 1;
const unsigned int MOSTRAR_COMPROMISSO = 2;
const unsigned int EDITAR_COMPROMISSO = 3;
const unsigned int SAIR = 4;

/**
 * Funï¿½ï¿½o principal
 */
int main(int argc, char **argv) {

	// Exibe a mensagem de boas vindas
	std::cout << "Boas vindas da agenda!" << std::endl;

	exibeMenu();

	// Guarda a opï¿½ï¿½o escolhida
	int opcao;

	// Entra em um loop infinito perguntando
	// o que o usuï¿½rio deseja fazer
	while (true) {

		// Pergunta a opï¿½ï¿½o do usuï¿½rio
		std::cin >> opcao;

		// Executa a aï¿½ï¿½o escolhida
		switch (opcao) {
			case ADICIONAR_COMPROMISSO:
				adicionarCompromisso();
				break;
			case REMOVER_COMPROMISSO:
				// TODO implementar
				std::cout << "Falta implementar." << std::endl;
				break;
			case MOSTRAR_COMPROMISSO:
				verCompromissos();
				break;
			case EDITAR_COMPROMISSO:
				editarCompromisso();
				break;
		}

		// Exibe o menu novamente
		exibeMenu();
	}

	desconectar();
	return 0;
}

/**
 * Exibe um menu para o usuï¿½rio
 */
void exibeMenu() {
	std::cout << "Escolha uma opï¿½ï¿½o:" << std::endl;
	std::cout << ADICIONAR_COMPROMISSO << " - Adicionar compromisso" << std::endl;
	std::cout << REMOVER_COMPROMISSO << " - Remover compromisso" << std::endl;
	std::cout << MOSTRAR_COMPROMISSO << " - Mostrar compromisso" << std::endl;
	std::cout << EDITAR_COMPROMISSO << " - Editar compromisso" << std::endl;
	std::cout << SAIR << " - SAIR" << std::endl;
}

/**
 * Mostra os erros que podem ocorrer na conexï¿½o
 * @param mysql
 */
void mostrarErroDoMysql(MYSQL *mysql) {
	printf("Erro: (%d) [%s] \"%s\"", mysql_errno(mysql), mysql_sqlstate(mysql), mysql_error(mysql));
	mysql_close(mysql);
	exit(-1);
}

/**
 * Conecta no banco de dados
 * @return - true: Conectou, false: Falha ao conectar
 */
bool conectar() {

	if (conectado) return true;

	connexao = mysql_init(NULL);
	if (!mysql_real_connect(connexao, "127.0.0.1", "root", "", "Agenda", 3306, NULL, 0)) {
		mostrarErroDoMysql(connexao);
		conectado = false;
		return false;
	}

	conectado = true;
	return true;
}

/**
 * Desconecta do banco de dados
 */
void desconectar() {
	mysql_close(connexao);
	conectado = false;
}

/**
 * Adiciona um compromisso
 */
void adicionarCompromisso() {

	// Verifica se a conexÃ£o foi realizada com sucesso
	if (!conectar()) {
		std::cout << "Falha ao conectar ao banco de dados!" << std::endl;
		return;
	}

	// VariÃ¡veis usadas para montar o query
	std::string dia, mes, ano, descricao;

	// Solicita o dia do compromisso
	std::cout << "Informe o dia:";
	std::cin >> dia;

	// Solicita o mï¿½s do compromisso
	std::cout << "Informe o mï¿½s:";
	std::cin >> mes;

	// Solicita o ano do compromisso
	std::cout << "Informe o ano:";
	std::cin >> ano;

	// Solicita a descriï¿½ï¿½o do compromisso
	std::cout << "Descreva o compromisso:";
	std::cin >> descricao;

	// Monta a query
	std::string sql = "insert into Compromisso (data, descricao)values('" + ano + "-" + mes + "-" + dia + "','" + descricao + "')";
	// Executa a query
	int statusDeExecucao = mysql_query(connexao, sql.data());

	// Verifica se deu tudo certo
	if (statusDeExecucao == 0) {
		// Deu certo!
		std::cout << "Compromisso inserido" << std::endl;
	} else {
		// Algo deu errado!
		mostrarErroDoMysql(connexao);
		std::cout << "Falha ao inserir compromisso!" << std::endl;
	}

}

void verCompromissos() {

	// Verifica se a conexï¿½o foi realizada com sucesso
	if (!conectar()) {
		std::cout << "Falha ao conectar ao banco de dados!" << std::endl;
		return;
	}

	// Guarda um registro
	MYSQL_ROW registro;

	// Guarda todos os registros retornados
	MYSQL_RES *resultados;

	// Executa a query
	int statusDeExecucao = mysql_query(connexao, "SELECT cod, data, descricao FROM Compromisso");

	// Verifica se deu tudo certo
	if (statusDeExecucao == 0) {

		// Deu certo! Recupera os registros retornados;
		resultados = mysql_store_result(connexao);

		// Mostra os registros retornados
		while ((registro = mysql_fetch_row(resultados))) {
			std::cout << registro[0] << "\t" << registro[1] << "\t" << registro[2] << std::endl;
		}
	} else {
		// Algo deu errado!
		mostrarErroDoMysql(connexao);
		std::cout << "Falha ao recuperar os registros!" << std::endl;
	}
	exit;
}
void editarCompromisso(){
	unsigned int op;
	std::string cod,dia,mes,ano,descricao;
	verCompromissos();
	std::cout<< "Digite o codigo do compromisso que deseja alterar" << std::endl;
	std::cin>>cod;
	std::cout<< "Deseja alterar 1- Data 2- Descrição ?";
	std::cin>>op;
	switch(op){
		case 1:{
			// Solicita o dia do compromisso
			std::cout << "Informe o dia:";
			std::cin >> dia;
		
			// Solicita o mï¿½s do compromisso
			std::cout << "Informe o mï¿½s:";
			std::cin >> mes;
		
			// Solicita o ano do compromisso
			std::cout << "Informe o ano:";
			std::cin >> ano;
		
			// Monta a query
			std::string sql = "update Compromisso set data =('" + ano + "-" + mes + "-" + dia+"') where cod = "+cod+"";
			std::cout << sql << std::endl;
			// Executa a query
			int statusDeExecucao = mysql_query(connexao, sql.data());
		
			// Verifica se deu tudo certo
			if (statusDeExecucao == 0) {
				// Deu certo!
				std::cout << "Compromisso inserido" << std::endl;
			} else {
				// Algo deu errado!
				mostrarErroDoMysql(connexao);
				std::cout << "Falha ao inserir compromisso!" << std::endl;
			}
			break;
		}
		case 2:{
			// Solicita a descriï¿½ï¿½o do compromisso
			std::cout << "Descreva o compromisso:" << std::endl;
			std::cin >> descricao;
			// Monta a query
			std::string sql = "update Compromisso set descricao ='"+descricao+"' where cod = '"+cod+"'";
			std::cout << sql << std::endl;
			// Executa a query
			int statusDeExecucao = mysql_query(connexao, sql.data());
		
			// Verifica se deu tudo certo
			if (statusDeExecucao == 0) {
				// Deu certo!
				std::cout << "Compromisso inserido" << std::endl;
			} else {
				// Algo deu errado!
				mostrarErroDoMysql(connexao);
				std::cout << "Falha ao inserir compromisso!" << std::endl;
			}
			break;
		}
		default:{
			std::cout<<"Codigo inválido" << std::endl;
			break;
		}
	}
}
