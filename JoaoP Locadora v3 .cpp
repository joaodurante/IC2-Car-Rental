/**
 *	Locadora Veículos v3
 *
 *	OBJETIVO:
 *		Efetuar o controle eficiente de uma locadora de ve�culos atrav�s do cadastramento de clientes,
 *		assim como sua altera��o, consulta e exclus�o, cadastramento de veiculos com op��es como
 * 		carros, e seus tr�s diferentes tipos (popular, luxo, van), e motocicletas, a movimenta��o
 *		do acervo de ve�culos pelos clientes e tamb�m da realiza��o autom�tica de relat�rios
 *		do acervo e das movimentações ocorridas. Armazenando todos os dados coletados em arquivos.
 */

/**
*	@author João Pedro De Munno Durante
*	@date 07/11/2017
*	@file LocadoraVeiculos.cpp
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<conio.h>

/**
 *Tamanho maximo de loca��es por cliente
 */
#define MAX 50

/**
 *Nome do arquivo que cont�m os dados dos clientes
 */
#define arqC "Clientes.txt"

/**
 *Nome do arquivo que cont�m os dados dos veiculos
 */
#define arqV "Veiculos.txt"

/**
 *Nome do arquivo que cont�m os dados das movimenta��es
 */
#define arqM "Movimentacoes.txt"

/**
 *Struct com as informa��es dos clientes
 */	
struct DadosCliente{
	int Codigo;													/**< Codigo							*/
	char Nome[50];												/**< Nome							*/
	char CPF[12];												/**< CPF							*/
	char Telefone[15];											/**< Telefone						*/
	
	struct{														/**< Struct Endere�o				*/
		char estado[20];										/**< Estado							*/
		char cidade[20];										/**< Cidade							*/
		char bairro[20]; 										/**< Bairro							*/
		char rua[20];											/**< Rua							*/
		char numero[10];										/**< Numero							*/
	}endereco;
														
	int totalLocacao;											/**< Numero de Loca�oes				*/
	DadosCliente *prox;											/**< Apontador para o proximo 		*/
};
typedef struct DadosCliente cliente;

/**
 *Struct com elementos que comp�em a data (dd/mm/aa)
 */
typedef struct{
	int dia;													/**< Dia							*/
	int mes;													/**< Mes							*/
	int ano;													/**< Ano							*/
}Data;

/**
 *Struct com informa�oes do histotico de loca�oes do veiculo
 */
typedef struct{ 
	char nomeCliente[50];										/**< Nome do cliente				*/
	Data dataLocacao;											/**< Data de loca�ao				*/
	Data dataDevolucao;											/**< Data de devolu�ao				*/
}HistoricoLocacoes;

/**
 *Enumera�ao com op��o de SIM ou NAO
 */
enum naosim{
	sim=1,														/**< 1								*/
	nao															/**< 2								*/
};

/**
 *Enumera��o com op��es de central multimidia
 */
enum CentralMultimidia{
	nenhum=1,													/**< 1								*/
	radio,														/**< 2								*/
	GPS															/**< 3								*/
};

/**
 *Enumera��o com os tipos de ve�culos
 */
enum TipoVeiculo{
	CARRO=1, 													/**< 1								*/	
	MOTO														/**< 2								*/
};

/**
 *Enumera��o com os tipos de carros
 */
enum TipoCarro{
	CARROPOPULAR=1, 											/**< 1								*/
	CARROLUXO, 													/**< 2								*/
	VAN															/**< 3								*/
};

/**
 *Struct com as informa��es dos ve�culos
 */
struct DadosVeiculos{
	union{														/**< Uni�o Tipos de Veiculos		*/
		struct{
			int codigo;											/**< Codigo							*/
			char modelo[20];									/**< Modelo							*/
			HistoricoLocacoes historicoLocacoes[MAX];			/**< Array de Historico de Loca�oes */
			int contadorLocacoes; 								/**< Contador de loca��es realizadas*/
		}Motos;													/**< Motos							*/
		struct{
			int codigo;											/**< Codigo							*/
			char modelo[20];									/**< Modelo							*/
			union TiposCarros{ 									/**< Uniao Tipos de Carros			*/
				
				struct{ 
					enum naosim arCondicionado;					/**< Ar-Condicionado				*/
					enum naosim direcaoHidraulica;				/**< Dire��o Hidraulica				*/
					enum CentralMultimidia centralMultimidia;	/**< Central Multimidia				*/
				}carroPopular;									/**< Carro Popular					*/
				
				struct{ 
					enum naosim arCondicionado;					/**< Ar-Condicionado				*/
					enum CentralMultimidia centralMultimidia;	/**< Central Multimidia				*/
					int numeroPassageiros;						/**< Numero de passageiros			*/
				}van;											/**< Van							*/
			}uCarros;
			
			enum TipoCarro tipoCarro;
			
			HistoricoLocacoes historicoLocacoes[MAX];			/**< Array de Historico de Loca�oes */
			int contadorLocacoes; 								/**< Contador de loca��es realizadas*/
		}Carros;												/**< Carros							*/
	}uVeiculos;
	enum TipoVeiculo tipoVeiculo;		
	DadosVeiculos *prox;
};
typedef struct DadosVeiculos veiculo;

/**
 *Struct com as informa��es das movimenta��es
 */
struct DadosMovimentacao{
	int codigoCliente;
	int codigoVeiculo;
	DadosMovimentacao *prox;
};
typedef struct DadosMovimentacao movimentacao;


/**
 *Fun��o com o menu principal
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listaveiculo1 Apontador para o 1 elemento da lista dos veiculos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao 
 */
void menuprincipal(cliente *listacliente1, veiculo *listasveiculo1, movimentacao *listamovimentacao1); 

/**
 *Fun��o que recebe a confirmacao ou nao da opera�ao (cadastro/alteracao/exclusao)
 *
 *@return confirm Variavel que armazena a op�ao escolhida (s/n)
 */
char confirmaropcao(); 

/**
 *Fun��o para o menu de clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimenta��es
 */
void menucliente(cliente *listacliente1, movimentacao *listamovimentacao1); 

/**
 *Fun��o para a inclusao de novos clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void inclusaocliente(cliente *listacliente1); 

/**
 *Fun��o que insere dados num auxiliar de cliente
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@return aux Retorna uma vari�vel do tipo cliente que cont�m os dados informados
 */
cliente incluirdadosCliente(cliente *listacliente1);

/**
 *Fun��o que verifica se a lista de clientes est� vazia ou n�o, retornando um inteiro que indica o mesmo
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *
 */
int vaziacliente(cliente *listacliente1);

/**
 *Fun�ao que printa as informa��es dos clientes de acordo com o c�digo passado
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *
 */
void printarcliente(cliente *listacliente1, int codigo);

/**
 *Fun��o para a alteracao de clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */												   
void alteracaocliente(cliente *listacliente1, movimentacao *listamovimentacao1); 

/**
 *Fun��o para a consulta de clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void consultacliente(cliente *listacliente1);

/**
 *Fun��o para a exclusao de clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimenta��es
 */
void exclusaocliente(cliente *listacliente1, movimentacao *listamovimentacao1);

/**
 *Fun��o que analisa se o codigo (cliente) inserido ja esta sendo usado
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param codigo Codigo que sera analisado
 *@return Retorna inteiro indicando se o codigo esta em uso (sim ou nao/1 ou 0)
 */
int consultarcodC(cliente *listacliente1, int codigo);

/**
 *Fun��o para o menu de veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void menuveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o para a op��o de inclusao de novos veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void inclusaoveiculo(veiculo *listaveiculo1);

/**
 *Fun��o para a op��o de alteracao de veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */
void alteracaoveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o para a consulta de veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void consultaveiculo(veiculo *listaveiculo1);

/**
 *Fun��o para a op��o de exclusao de veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */
void exclusaoveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o que recebe as informa��es passdas pelo usuario do veiculo e a retorna para uma poss�vel inclus�o
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@return aux Variavel do tipo veiculo que cont�m as informa��es passadas pelo usu�rio
 */
veiculo incluirdados(veiculo *listaveiculo1);

/**
 *Fun��o que localiza e printa informacoes de acordo com o codigo passado
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param codigo Codigo que sera analisado e printado
 */
void printarveiculo(veiculo *listaveiculo1, int codigo);

/**
 *Fun��o que analisa se o codigo (veiculo) inserido ja esta sendo usado
 *
 *@param codigo Codigo que sera analisado
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@return Retorna o valor 1(ja utilizado) ou 0(nao utilizado) 
 */
int consultarCodV(veiculo *listaveiculo1, int codigo);
/**
 *Fun��o que verifica se a lista de ve�culos est� vazia
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param codigo Codigo que ser� analisado
 *@return Inteiro indicando sim/n�o
 */
int vaziaveiculo(veiculo *listaveiculo1);

/**
 *Fun��o para o menu de movimentacao
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao 
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void menumovimentacao(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o para a op�ao de locacao de veiculos
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void locacaoveiculo(movimentacao *listamovimentacao1, veiculo *listaveiculo1, cliente *listacliente1);

/**
 *Fun��o para a devolucao de veiculos
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void devolucaoveiculo(movimentacao *listamovimentacao1, veiculo *listaveiculo1, cliente *listacliente1);

/**
 *Fun��o que encontra a posicao do veiculo na lista pelo c�digo do ve�culo
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param codigo Variavel que armazena o codigo do veiculo
 *@param aux Ponteiro de ponteiro que ser� alterado com a correta posi��o
 */
 void encontrarposicaoVeiculo(veiculo *listaveiculo1, int codigo, veiculo **aux);
 
/**
 *Fun��o que encontra a posi��o da movimenta��o na lista pelo c�digo do ve�culo
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimenta��o
 *@param codigoVeiculo Variavel que armazena o codigo do ve�culo
 *@param aux Ponteiro de ponteiro que ser� alterado com a correta posi��o
 *
 */
void encontrarposicaoMovimentacao(movimentacao *listamovimentacao1, int codigoVeiculo, movimentacao **aux);

/**
 *Fun��o que verifica se a lista de movimenta��o est� vazia
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimenta��o
 *@return Retorna um inteiro indicando sim ou n�o
 */
 int vaziamovimentacao(movimentacao *listamovimentacao1);
 
/**
 *Fun��o que checa a disponibilidade de um veiculo
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 *@param codigo Variavel que armazena o codigo do veiculo
 *@return disponibilidade Variavel que armazena se o veiculo est� locado ou n�o
 */
int checardisponibilidade(movimentacao *listamovimentacao1, int codigo);

/**
 *Fun��o para o menu de relatorios
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */
void menurelatorio(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o para a op��o de balan�o do acervo
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void balancodoacervo(veiculo *listaveiculo1);

/**
 *Fun��o que analisa e faz o relatorio dos veiculos do acervo
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void contadorbalanco(veiculo *listaveiculo1);

/**
 *Fun��o que analisa e printa as quantidades de acordo com o tipo do veiculo
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void contadortiposveiculos(veiculo *listaveiculo1);

/**
 *Fun��o para a op��o de balan�o de loca��es
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */
void balancolocacoes(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o que analisa e printa as quantidades de locacoes do mes atual
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void locacoesmes(veiculo *listaveiculo1); 

/**
 *Fun��o que analisa e retorna o atraso na devolucao dos veiculos
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 *@return contadorAtrasoTotal Variavel que armazena o tempo de atraso na devolu�ao do veiculo (em dias)
 */
int atrasodevolucao(veiculo *listaveiculo1);

/**
 *Fun��o que analisa e printa o ranking dos veiculos mais locados
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de veiculos
 */
void rankingveiculos(veiculo *listaveiculo1);

/**
 *Fun��o que analisa e printa o ranking de clientes
 *
 *@param listacliente1 Apontador para o 1 elemento da lista dos clientes
 */
void rankingclientes(cliente *listacliente1);

/**
 *Fun��o que armazena a data atual e a retorna
 *
 *@return data Variavel contendo a data atual
 */
Data dataatual();

/**
 *Fun��o que verifica se o cliente est� com um ve�culo locado
 *
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 *@param codigo Codigo que sera analisado
 *@return Inteiro representando sim ou n�o
 */
int clienteLocacao(movimentacao *listamovimentacao1, int codigo);

/**
 *Fun��o que verifica o total de ve�culos cadastrados
 *
 *@param listaveiculo1 Apontador para o 1 elemento da lista de ve�culos
 *@return nVeiculos Inteiro com o total de ve�culos
 */
int totalveiculos(veiculo *listaveiculo1);

/**
 *Fun��o que verifica o total de ve�culos cadastrados
 *
 *@param listacliente1 Apontador para o 1 elemento da lista de clientes
 *@return nClientes Inteiro com o total de clientes
 */
int totalclientes(cliente *listacliente1);

/**
 *Fun��o que verifica o total de loca��es cadastrados
 *
 *@param listacliente1 Apontador para o 1 elemento da lista de clientes
 *@return nLocacoes Inteiro com o total de loca��es
 */
int totallocacoes(cliente *listacliente1);

/**
 *Fun��o que libera a mem�ria que as listas ocuparam
 *
 *@param listacliente1 Apontador para o 1 elemento da lista de clientes
 *@param listaveiculo1 Apontador para o 1 elemento da lista de ve�culos
 *@param listamovimentacao1 Apontador para o 1 elemento da lista de movimentacao
 */
void freelistas(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1);

/**
 *Fun��o gen�rica que aloca e insire novos elementos (de qualquer tipo) na lista e os preenche com os valores passados
 *
 *@param X � o primeiro elemento da lista passada
 *@param aux � a variavel que armazena os dados
 */
template <typename X> void criarelemento(X *lista, X aux);

/**
 *Fun��o que importa todos os dados armazenados no arquivo para o programa
 *
 *@param X � o primeiro elemento da lista passada
 *@param nomeArq � o nome do arquivo passado 
 */
template<typename X> void importarArq(X *lista, const char *nomeArq);

/**
 *Fun��o que escreve no final do arquivos novos dados
 *
 *@param X � o elemento que ser� escrito no arquivo
 *@param nomeArq � o nome do arquivo passado 
 */
template <typename X> void writeArq(X *item, const char *nomeArq);

/**
 *Fun��o que deleta todo os dados do arquivo e reescreve os dados atualizados
 *
 *@param X � o primeiro elemento da lista passada
 *@param nomeArq � o nome do arquivo passado 
 */
template <typename X> void reWriteArq(X *lista, const char *nomeArq);

int main(){
	
	cliente *listacliente1 = (cliente *) malloc(sizeof(cliente));
	listacliente1->prox=NULL;
	importarArq(listacliente1, arqC);
	
	veiculo *listaveiculo1 = (veiculo *) malloc(sizeof(veiculo));
	listaveiculo1->prox=NULL;
	importarArq(listaveiculo1, arqV);
	
	movimentacao *listamovimentacao1 = (movimentacao *) malloc(sizeof(movimentacao));
	listamovimentacao1->prox=NULL;
	importarArq(listamovimentacao1, arqM);
	
	menuprincipal(listacliente1, listaveiculo1, listamovimentacao1);
}


void menuprincipal(cliente  *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1){
	int resposta=1;
	
	do{
		system("cls");
		puts("MENU PRINCIPAL\n");
		puts("1 - CADASTRO DE CLIENTES");
		puts("2 - CADASTRO DE VEICULOS");
		puts("3 - MOVIMENTACAO");
		puts("4 - RELATORIOS");
		puts("0 - FINALIZAR");
		scanf("%d", &resposta);
		switch(resposta){
			
			case 1: menucliente(listacliente1, listamovimentacao1);
				break;
			
			case 2: menuveiculo(listaveiculo1, listamovimentacao1);
				break;
				
			case 3: menumovimentacao(listacliente1, listaveiculo1, listamovimentacao1);
				break;
				
			case 4: menurelatorio(listacliente1, listaveiculo1, listamovimentacao1);
				break;
				
			case 0: freelistas(listacliente1, listaveiculo1, listamovimentacao1);
				break;
		}
	}while(resposta!=0);
}

void freelistas(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1){
	
	if(!vaziacliente(listacliente1)){
		cliente *atualC, *proxC;
		atualC=listacliente1->prox;
		while(atualC!=NULL){
			proxC=atualC->prox;
			free(atualC);
			atualC=proxC;
		}
	}
	free(listacliente1);
	
	if(!vaziaveiculo(listaveiculo1)){
		veiculo *atualV, *proxV;
		atualV=listaveiculo1->prox;
		while(atualV!=NULL){
			proxV=atualV->prox;
			free(atualV);
			atualV=proxV;
		}
	}
	free(listaveiculo1);
	
	if(!vaziamovimentacao(listamovimentacao1)){
		movimentacao *atualM, *proxM;
		atualM=listamovimentacao1->prox;
		while(atualM!=NULL){
			proxM=atualM->prox;
			free(atualM);
			atualM=proxM;
		}
	}
}

void menucliente(cliente *listacliente1, movimentacao *listamovimentacao1){
	
	int resposta;
	
	do{
		system("cls");
		puts("MENU DE CLIENTES\n");
		puts("1 - INCLUSAO");
		puts("2 - ALTERACAO");
		puts("3 - CONSULTA");
		puts("4 - EXCLUSAO");
		puts("0 - RETORNAR");
		scanf("%d", &resposta);
		
		switch(resposta){
			
			case 1: inclusaocliente(listacliente1);
				break;
				
			case 2: alteracaocliente(listacliente1, listamovimentacao1);
				break;
				
			case 3: consultacliente(listacliente1);
				break;
				
			case 4: exclusaocliente(listacliente1, listamovimentacao1);
				break;
		}
	}while(resposta!=0);
}

cliente incluirdadosCliente(cliente *listacliente1){
	cliente aux;
	int codigo;
	int i;
	do{ //Digita o codigo e analisa se o mesmo ja foi utilizado
		puts("\nDigite o codigo do cliente: ");
		scanf("%d", &codigo);
		i=consultarcodC(listacliente1, codigo);
		if(i==0)
			aux.Codigo=codigo;
		else
			puts("\nCODIGO JA UTILIZADO! DIGITE NOVAMENTE.");
		}while(i==1);
		
		aux.Codigo=codigo;
		fflush(stdin);
		printf("\nDigite o nome do cliente: ");
		gets(aux.Nome);
		fflush(stdin);
		printf("\nDigite o CPF do cliente: ");
		gets(aux.CPF);
		fflush(stdin);
		printf("\nDigite o telefone do cliente: ");
		gets(aux.Telefone);
		fflush(stdin);
		printf("\nDigite o Estado do cliente: ");
		gets(aux.endereco.estado);
		fflush(stdin);
		printf("\nDigite a cidade do cliente: ");
		gets(aux.endereco.cidade);
		fflush(stdin);
		printf("\nDigite o bairro do cliente: ");
		gets(aux.endereco.bairro);
		fflush(stdin);
		printf("\nDigite a rua do cliente: ");
		gets(aux.endereco.rua);
		fflush(stdin);
		printf("\nDigite o numero do cliente: ");
		gets(aux.endereco.numero);
		fflush(stdin);
		return aux;
}

void inclusaocliente(cliente *listacliente1){
	
	char confirm, novoCadastro;
	int i, codigo;
	cliente aux;
	
	do{
		cliente *listacliente = (cliente *) malloc(sizeof(cliente));
		system("cls");
		puts("INCLUSAO DE CLIENTES");
		aux=incluirdadosCliente(listacliente1);
		confirm=confirmaropcao();
		if(confirm=='s'){
			*listacliente=aux;
			listacliente->totalLocacao=0;
			listacliente->prox=NULL;
			if(vaziacliente(listacliente1)){
				listacliente1->prox=listacliente;
			}
			else{
				cliente *apontador = listacliente1->prox;
				while(apontador->prox != NULL){
					apontador=apontador->prox;
				}
				apontador->prox=listacliente;
			}
			listacliente->prox=NULL;
			writeArq(listacliente, arqC);
			system("cls");
			puts("INCLUSAO DE CLIENTES");
			puts("\nINCLUSAO EFETUADA COM SUCESSO");
		}
		else
			free(listacliente);
		
		puts("\nDeseja realizar outro cadastro [s/n]? ");
		scanf("%c", &novoCadastro);
		fflush(stdin);
		
	}while(novoCadastro=='s');
}

int vaziacliente(cliente *listacliente1){ 
	
	if(listacliente1->prox == NULL)
		return 1;
	else
		return 0;
}

void printarcliente(cliente *listacliente1, int codigo){ 
	
	cliente *apontador=listacliente1->prox;
	
	if(apontador->Codigo!=codigo){
		while(apontador->Codigo != codigo){
			apontador=apontador->prox;
		}
	}
	printf("INFORMACOES DO CLIENTE\n\n");
	printf("Codigo: %d", apontador->Codigo);
	printf("\nNome: %s", apontador->Nome);
	printf("\nCPF: %s", apontador->CPF);
	printf("\nTelefone: %s", apontador->Telefone);
	printf("\nEstado: %s", apontador->endereco.estado);
	printf("\nCidade: %s", apontador->endereco.cidade);
	printf("\nBairro: %s", apontador->endereco.bairro);
	printf("\nRua: %s", apontador->endereco.rua);
	printf("\nNumero: %s", apontador->endereco.numero);
}

void consultacliente(cliente *listacliente1){ 
	
	char novaConsulta;
	int codigo, i;
	
	do{
		system("cls");
		printf("CONSULTA DE CLIENTES\n\n");
		do{
			i=0;
			printf("Informe o codigo do cliente para a consulta: ");
			scanf("%d", &codigo);
			fflush(stdin);
			if(consultarcodC(listacliente1, codigo)){
				i=1;
				system("cls");
				printarcliente(listacliente1, codigo);
			}
			else printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n\n");
		}while(i==0);
		fflush(stdin);
		puts("\n\nDeseja fazer uma nova consulta [s/n]? ");
		scanf("%c", &novaConsulta);
		fflush(stdin);
		
	}while(novaConsulta=='s');
}

int consultarcodC(cliente *listacliente1, int codigo){
	
	cliente *aux;
	if(listacliente1->prox!=NULL){
		aux=listacliente1->prox;
		while(aux->prox!=NULL && aux->Codigo!=codigo){
			aux=aux->prox;
		}
		if(aux->Codigo==codigo)
			return 1;
		else 
			return 0;
	}
	else return 0;
}

int clienteLocacao(movimentacao *listamovimentacao1, int codigo){
	
	movimentacao *apontador=listamovimentacao1->prox;
	if(apontador!=NULL){
		if(apontador->codigoCliente!=codigo){
		while(apontador->prox!=NULL){
			if(apontador->codigoCliente==codigo)
				return 1;
			else apontador=apontador->prox;
		}
		if(apontador->codigoCliente!=codigo) return 0;
		}
		else return 1;
	}
	else return 0;
}

void alteracaocliente(cliente *listacliente1, movimentacao *listamovimentacao1){
	
	char confirm, novaAlteracao;
	int codigo;
	unsigned int i=0;
	cliente aux;
	cliente *apontador = listacliente1->prox;
	
	do{
		system("cls");
		printf("ALTERACAO DE CLIENTES");
		do{
			i=0;
			printf("\n\nDigite o codigo do cliente: ");
			scanf("%d", &codigo);
			if(!consultarcodC(listacliente1, codigo))
				printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE");
			else{
				if(!clienteLocacao(listamovimentacao1, codigo))
					i=1;
				else printf("\nO CLIENTE NAO PODE SER ALTERADO, HA UMA LOCACAO EM SEU NOME");
			}
		}while(i==0);
	
		if(apontador->Codigo!=codigo){
			while(apontador->Codigo != codigo)
				apontador=apontador->prox;
		}
		system("cls");
		printarcliente(listacliente1, codigo);
		printf("\n\nNOVAS INFORMACOES\n\n");
		aux=incluirdadosCliente(listacliente1);
		aux.prox=apontador->prox;
		confirm=confirmaropcao();
		
		if(confirm=='s'){
			*apontador=aux;
			reWriteArq(listacliente1, arqC);
			system("cls");
			printf("ALTERACAO DE CLIENTES\n\n");
			printf("ALTERACAO EFETUADA COM SUCESSO\n");
		}
		puts("\nDeseja fazer uma nova alteracao [s/n]? ");
		scanf("%c", &novaAlteracao);
		fflush(stdin);
	}while(novaAlteracao=='s');
}

void exclusaocliente(cliente *listacliente1, movimentacao *listamovimentacao1){ 
	char novaExclusao, confirm;
	int codigo, i;
	cliente *apontador=listacliente1->prox;
	
		do{
			system("cls");
			puts("EXCLUSAO DE CLIENTES");
			do{
			i=0;
				printf("\nDigite o codigo do cliente: ");
				scanf("%d", &codigo);
				if(!consultarcodC(listacliente1, codigo))
					printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n");
				else i=1;
			}while(i==0);
			
			if(clienteLocacao(listamovimentacao1, codigo)){
				printf("\nO VEICULO NAO PODE SER EXCLUIDO, HA UMA LOCACAO EM SEU NOME\n");
				i=0;
			}
			if(i==1){
				printf("\n");
				printarcliente(listacliente1, codigo);
				if(apontador->Codigo!=codigo){
					while(apontador->Codigo != codigo)
						apontador=apontador->prox;
				}
				printf("\n");
				confirm=confirmaropcao();
					
				if(confirm=='s'){
					cliente *aux=listacliente1->prox;
					while(aux->prox!=apontador && aux!=apontador)
						aux=aux->prox;
						
					aux->prox=apontador->prox;
					free(apontador);
					reWriteArq(listacliente1, arqC);
					system("cls");
					printf("EXCLUSAO DE CLIENTES\n\n");
					printf("EXCLUSAO EFETUADA COM SUCESSO\n");
				}
			}
			fflush(stdin);
			puts("\nDeseja fazer uma nova exclusao [s/n]? ");
			scanf("%c", &novaExclusao);
			fflush(stdin);
		
	}while(novaExclusao=='s');
}

int consultarcodV(veiculo *listaveiculo1, int codigo){
	
	if(!vaziaveiculo(listaveiculo1)){
		veiculo *apontador;
		apontador=listaveiculo1->prox;
		while(apontador!=NULL){
			switch(apontador->tipoVeiculo){
				case CARRO:{
					if(apontador->uVeiculos.Carros.codigo==codigo)
						return 1;
					else apontador=apontador->prox;
					break;
				}
				case MOTO:{
					if(apontador->uVeiculos.Motos.codigo==codigo)
						return 1;
					else apontador=apontador->prox;
					break;
				}
			}
		}
	}
	return 0;
}

char confirmaropcao(){ 

	char confirm;	
	fflush(stdin);
	do{
		puts("\nConfirmar operacao [s/n]? ");
		scanf("%c", &confirm);
		fflush(stdin);
		if(confirm!='s' && confirm!='n'){
			puts("OPCAO INVALIDA! DIGITE NOVAMENTE");
		}
	}while(confirm!='s' && confirm!='n');
	return confirm;
}

void menuveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1){

	int resposta;	
	do{
		system("cls");
		puts("MENU DE VEICULOS\n");
		puts("1 - INCLUSAO");
		puts("2 - ALTERACAO");
		puts("3 - CONSULTA");
		puts("4 - EXCLUSAO");
		puts("0 - RETORNAR");
		scanf("%d", &resposta);
		
		switch(resposta){
			
			case 1: inclusaoveiculo(listaveiculo1);
				break;
				
			case 2: alteracaoveiculo(listaveiculo1, listamovimentacao1);
				break;
				
			case 3: consultaveiculo(listaveiculo1);
				break;
				
			case 4: exclusaoveiculo(listaveiculo1, listamovimentacao1);
				break;
		}
	}while(resposta!=0);
}

void inclusaoveiculo(veiculo *listaveiculo1){
	
	char confirm, novoCadastro;
	int posicao;
	veiculo aux;
	
	do{
		system("cls");
		printf("INCLUSAO DE VEICULOS\n\n");
		
		aux=incluirdados(listaveiculo1);
		
		confirm=confirmaropcao();
		if(confirm=='s'){
			
			veiculo *listaveiculo=(veiculo *) malloc(sizeof(veiculo));
			*listaveiculo=aux;
			
			if(vaziaveiculo(listaveiculo1))
				listaveiculo1->prox=listaveiculo;
			else{
				veiculo *aux = listaveiculo1->prox;
				while(aux->prox != NULL){
					aux=aux->prox;
				}
				aux->prox=listaveiculo;
			}
			
			if(listaveiculo->tipoVeiculo==CARRO)
				listaveiculo->uVeiculos.Carros.contadorLocacoes=0;
			else listaveiculo->uVeiculos.Motos.contadorLocacoes=0;
			
			listaveiculo->prox=NULL;
			writeArq(listaveiculo, arqV);
			system("cls");
			puts("INCLUSAO DE VEICULOS");
			puts("\nINCLUSAO EFETUADA COM SUCESSO");
		}
		
		puts("\nDeseja realizar outro cadastro [s/n]? ");
		scanf("%c", &novoCadastro);
		fflush(stdin);
	}while(novoCadastro=='s');
}

int vaziaveiculo(veiculo *listaveiculo1){
	
	if(listaveiculo1->prox == NULL)
		return 1;
	else
		return 0;
}

veiculo incluirdados(veiculo *listaveiculo1){ 
	
	veiculo aux;
	int codigo, i;
	
	printf("Escolha uma das opcoes\n1-Carro\n2-Moto\n");
	scanf("%d", &aux.tipoVeiculo);
	do{
		printf("\nDigite o codigo: ");
		scanf("%d", &codigo);
		i=consultarcodV(listaveiculo1, codigo);
		if(i==1)
			printf("\nCODIGO JA ESTA SENDO UTILIZADO, DIGITE NOVAMENTE\n");
	}while(i==1);
	switch(aux.tipoVeiculo){
		case CARRO:{
			aux.uVeiculos.Carros.codigo=codigo;
			fflush(stdin);
			printf("\nDigite o modelo: ");
			gets(aux.uVeiculos.Carros.modelo);
			fflush(stdin);
			puts("\nDigite o tipo do carro:\n1- Carro popular\n2- Carro de luxo\n3- Van");
			scanf("%d", &aux.uVeiculos.Carros.tipoCarro);
			fflush(stdin);
			switch(aux.uVeiculos.Carros.tipoCarro){
				case CARROPOPULAR:{
					puts("\nO carro possui ar-condicionado:\n1- Sim\n2- Nao");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.carroPopular.arCondicionado);
					fflush(stdin);
					puts("\nQual central multimidia o carro possui:\n1- Nenhum\n2- Radio\n3- GPS ");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.carroPopular.centralMultimidia);
					fflush(stdin);
					puts("\nO carro possui direcao hidraulica:\n1- Sim\n2- Nao ");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.carroPopular.direcaoHidraulica);
					fflush(stdin);
					break;
				}
				case CARROLUXO:{
					break;
				}
				case VAN:{
					puts("\nA van possui ar-condicionado:\n1- Sim\n2- Nao");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.van.arCondicionado);
					fflush(stdin);
					puts("\nQual central multimidia a van possui:\n1- Nenhum\n2- Radio\n3- GPS ");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.van.centralMultimidia);
					fflush(stdin);
					puts("\nQual o numero de passageiros da van? ");
					scanf("%d", &aux.uVeiculos.Carros.uCarros.van.numeroPassageiros);
					fflush(stdin);
					break;
				}
			}
			break;
		}
		case MOTO:{
			aux.uVeiculos.Motos.codigo=codigo;
			printf("\nDigite o modelo: ");
			scanf("%s", &aux.uVeiculos.Motos.modelo);
			aux.uVeiculos.Motos.contadorLocacoes=0;
			break;
		}
	}
	return aux;
}

void alteracaoveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1){
	
	int codigo, i;
	char confirm, novaAlteracao;
	veiculo aux;
	
	do{
		system("cls");
		printf("ALTERACAO DE VEICULOS\n\n");
		do{
			i=0;
			printf("Informe o codigo do veiculo: ");
			scanf("%d", &codigo);
			if(consultarcodV(listaveiculo1, codigo)){
				if(checardisponibilidade(listamovimentacao1, codigo)){
					i=1;
					printarveiculo(listaveiculo1, codigo);
				}
				else printf("\nO VEICULO ESTA LOCADO, A ALTERACAO NAO PODE SER CONCLUIDA\n\n");
			}
			else printf("\n\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n\n");
		}while(i==0);
		printf("\n\n\nNOVAS INFORMACOES\n\n");
		aux=incluirdados(listaveiculo1);
		
		confirm=confirmaropcao();
		if(confirm=='s'){
			veiculo *apontadorVeiculo=listaveiculo1->prox;
			encontrarposicaoVeiculo(listaveiculo1, codigo, &apontadorVeiculo);
			aux.prox=apontadorVeiculo->prox;
			*apontadorVeiculo=aux;
			reWriteArq(listaveiculo1, arqV);
			system("cls");
			puts("ALTERACAO DE VEICULOS");
			puts("\nALTERACAO EFETUADA COM SUCESSO");
		}
		puts("\nDeseja realizar outra alteracao [s/n]? ");
		scanf("%c", &novaAlteracao);
		fflush(stdin);
	}while(novaAlteracao=='s');
}

void printarveiculo(veiculo *listaveiculo1, int codigo){ 
	
	veiculo *apontador=listaveiculo1->prox;
	encontrarposicaoVeiculo(listaveiculo1, codigo, &apontador);
	system("cls");
	printf("INFORMACOES DO VEICULO\n\n");
	switch(apontador->tipoVeiculo){
		case CARRO:{
			printf("Veiculo: CARRO");
			printf("\nCodigo: %d", apontador->uVeiculos.Carros.codigo);
			printf("\nModelo: %s", apontador->uVeiculos.Carros.modelo);
			switch(apontador->uVeiculos.Carros.tipoCarro){
				case CARROPOPULAR:{
					printf("\nTipo: CARRO POPULAR");
					if(apontador->uVeiculos.Carros.uCarros.carroPopular.arCondicionado==1)
						printf("\nAr-Condicionado: SIM");
					else
						printf("\nAr-Condicionado: NAO");
					if(apontador->uVeiculos.Carros.uCarros.carroPopular.centralMultimidia==1)
						printf("\nCentral Multimidia: NENHUM");
					else if(apontador->uVeiculos.Carros.uCarros.carroPopular.centralMultimidia==2)
						printf("\nCentral Multimidia: RADIO");
					else
						printf("\nCentral Multimidia: GPS");
					if(apontador->uVeiculos.Carros.uCarros.carroPopular.direcaoHidraulica==1)
						printf("\nDirecao Hidraulica: SIM");
					else
						printf("\nDirecao Hidraulica: NAO");
					break;
				}
				case CARROLUXO:{
					printf("\nTipo: CARRO DE LUXO");
					break;
				}
				case VAN:{
					printf("\nTipo: VAN");
					if(apontador->uVeiculos.Carros.uCarros.van.arCondicionado==1)
						printf("\nAr-Condicionado: SIM");
					else
						printf("\nAr-Condicionado: NAO");
					if(apontador->uVeiculos.Carros.uCarros.van.centralMultimidia==1)
						printf("\nCentral Multimidia: NENHUM");
					else if(apontador->uVeiculos.Carros.uCarros.van.centralMultimidia==2)
						printf("\nCentral Multimidia: RADIO");
					else
						printf("\nCentral Multimidia: GPS");
					printf("\nNumero de passageiros: %d", apontador->uVeiculos.Carros.uCarros.van.numeroPassageiros);
					break;
				}
			}
			break;
		}
		case MOTO:{
			printf("Veiculo: MOTOCICLETA");
			printf("\nCodigo: %d", apontador->uVeiculos.Motos.codigo);
			printf("\nModelo: %s", apontador->uVeiculos.Motos.modelo);
			break;
		}
	}
}

void consultaveiculo(veiculo *listaveiculo1){ 
	
	int codigo, i;
	char novaConsulta;

	do{
		system("cls");
		printf("CONSULTA DE VEICULOS\n\n");
		do{
			i=0;
			printf("Informe o codigo do veiculo para a consulta: ");
			scanf("%d", &codigo);
			if(consultarcodV(listaveiculo1, codigo)){
				i=1;
				printarveiculo(listaveiculo1, codigo);
			}
			else printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n\n");
		
		}while(i==0);
		fflush(stdin);
		puts("\n\nDeseja realizar outra consulta [s/n]? ");
		scanf("%c", &novaConsulta);
		fflush(stdin);
	}while(novaConsulta=='s');
}

void exclusaoveiculo(veiculo *listaveiculo1, movimentacao *listamovimentacao1){
	
	int i, codigo;
	char confirm, novaExclusao;
	
	do{
	system("cls");
	printf("ALTERACAO DE VEICULOS\n\n");
	do{
		i=0;
		printf("Informe o codigo do veiculo: ");
		scanf("%d", &codigo);
		if(consultarcodV(listaveiculo1, codigo)){
			i=1;
			printarveiculo(listaveiculo1, codigo);
		}
		else printf("\n\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n\n");
	}while(i==0);
	
	printf("\n");
	confirm=confirmaropcao();
	if(confirm=='s'){
		system("cls");
		puts("EXCLUSAO DE VEICULOS\n");
		if(checardisponibilidade(listamovimentacao1, codigo)){
			
			veiculo *apontador=listaveiculo1->prox;
			encontrarposicaoVeiculo(listaveiculo1, codigo, &apontador);
			veiculo *aux=listaveiculo1;
		
			while(aux->prox!=apontador && aux!=apontador)
				aux=aux->prox;
				
			aux->prox=apontador->prox;
			free(apontador);
			reWriteArq(listaveiculo1, arqV);
			puts("EXCLUSAO EFETUADA COM SUCESSO");
		}
		else printf("O VEICULO ESTA LOCADO, EXCLUSAO NAO EFETUADA\n");
	}
	puts("\nDeseja realizar uma nova exclusao [s/n]? ");
	scanf("%c", &novaExclusao);
	fflush(stdin);
	}while(novaExclusao=='s');
}

void encontrarposicaoVeiculo(veiculo *listaveiculo1, int codigo, veiculo **aux){
	
	int i=0;
	*aux=listaveiculo1->prox;
	veiculo *apontador=*aux;
	if(!vaziaveiculo(listaveiculo1)){
		while(i==0){
			switch(apontador->tipoVeiculo){
				case 1:{
					if(apontador->uVeiculos.Carros.codigo==codigo)
						i=1;
					else if(apontador->prox!=NULL)
						apontador=apontador->prox;
					break;
				}
				case 2:{
					if(apontador->uVeiculos.Motos.codigo==codigo)
						i=1;
					else if(apontador->prox!=NULL)
						apontador=apontador->prox;
					break;
				}
			}
		}
	}
	*aux=apontador;
}

Data dataatual(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Data data;
    data.dia = tm.tm_mday;
    data.mes = tm.tm_mon + 1;
    data.ano = tm.tm_year + 1900;
    return data;
}

void menumovimentacao(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1){ 
	
	int resposta=1;
	do{
		system("cls");
		puts("MENU DE MOVIMENTACAO\n");
		puts("1 - LOCACAO");
		puts("2 - DEVOLUCAO");
		puts("0 - RETORNAR");
		scanf("%d", &resposta);
		
		switch(resposta){
			case 1: locacaoveiculo(listamovimentacao1, listaveiculo1, listacliente1);
				break;
			case 2: devolucaoveiculo(listamovimentacao1, listaveiculo1, listacliente1);
				break;
		}
	}while(resposta!=0);
}

void locacaoveiculo(movimentacao *listamovimentacao1, veiculo *listaveiculo1, cliente *listacliente1){
	
	char novoEmprestimo, confirm;
	int i, j; //Variaveis auxiliares que guardar�o valores das consultas
	int codigoVeiculo, codigoCliente;
	veiculo *apontadorVeiculo=listaveiculo1->prox;
	cliente *apontadorCliente=listacliente1->prox;
	movimentacao *aux = listamovimentacao1->prox;
	
	
	do{
		j=0;
		system("cls");
		puts("LOCACAO DE VEICULOS");
		do{
			i=0;
			puts("\nDigite o codigo do veiculo: ");
			scanf("%d", &codigoVeiculo);
			i=consultarcodV(listaveiculo1, codigoVeiculo); //Consulta se o codigo do veiculo existe
			if(i==0)
				printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n");
		}while(i==0);
		
		if(i==1){
			if(checardisponibilidade(listamovimentacao1, codigoVeiculo)){
				puts("\n\nVEICULO");
				printarveiculo(listaveiculo1, codigoVeiculo);
				puts("\n\n\nCLIENTE\n");
				do{
					puts("Digite o codigo do cliente: ");
					scanf("%d", &codigoCliente);
					fflush(stdin);
					j=consultarcodC(listacliente1, codigoCliente);			
					if(j==0)
						puts("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n");
				}while(j==0);
				
				confirm=confirmaropcao();
				
				if(confirm=='s'){
					movimentacao *listamovimentacao=(movimentacao *) malloc(sizeof(movimentacao));
					if(vaziamovimentacao(listamovimentacao1))
						listamovimentacao1->prox=listamovimentacao;
					else{
						aux = listamovimentacao1->prox;
						while(aux->prox != NULL){
							aux=aux->prox;
						}
						aux->prox=listamovimentacao;
					}
					listamovimentacao->prox=NULL;
					
					listamovimentacao->codigoCliente=codigoCliente;
					listamovimentacao->codigoVeiculo=codigoVeiculo;
					writeArq(listamovimentacao, arqM);
					
					apontadorVeiculo=listaveiculo1->prox;
					encontrarposicaoVeiculo(listaveiculo1, codigoVeiculo, &apontadorVeiculo);
					
					apontadorCliente=listacliente1->prox;
					if(apontadorCliente->Codigo!=codigoCliente){
						while(apontadorCliente->Codigo != codigoCliente)
							apontadorCliente=apontadorCliente->prox;
					}	
					apontadorCliente->totalLocacao++;
					
					switch(apontadorVeiculo->tipoVeiculo){
						case CARRO:{
							apontadorVeiculo->uVeiculos.Carros.historicoLocacoes[apontadorVeiculo->uVeiculos.Carros.contadorLocacoes].dataLocacao=dataatual();
							apontadorVeiculo->uVeiculos.Carros.historicoLocacoes[apontadorVeiculo->uVeiculos.Carros.contadorLocacoes].dataDevolucao.dia=NULL; //Marcador que nao deixar� essa loca��o entrar no relat�rio at� ser devolvida
							strcpy(apontadorVeiculo->uVeiculos.Carros.historicoLocacoes[apontadorVeiculo->uVeiculos.Carros.contadorLocacoes].nomeCliente, apontadorCliente->Nome);
							apontadorVeiculo->uVeiculos.Carros.contadorLocacoes++;
							break;
						}
						case MOTO:{
							apontadorVeiculo->uVeiculos.Motos.historicoLocacoes[apontadorVeiculo->uVeiculos.Motos.contadorLocacoes].dataLocacao=dataatual();
							apontadorVeiculo->uVeiculos.Motos.historicoLocacoes[apontadorVeiculo->uVeiculos.Motos.contadorLocacoes].dataDevolucao.dia=NULL;
							strcpy(apontadorVeiculo->uVeiculos.Motos.historicoLocacoes[apontadorVeiculo->uVeiculos.Motos.contadorLocacoes].nomeCliente, apontadorCliente->Nome);
							apontadorVeiculo->uVeiculos.Motos.contadorLocacoes++;
							break;
						}
					}
					system("cls");
					puts("LOCACAO DE VEICULOS");
					puts("\nLOCACAO EFETUADA COM SUCESSO");
				}
			}
			
			else puts("\nO CARRO NAO ESTA DISPONIVEL PARA LOCACAO\n");
			fflush(stdin);
		}
		puts("\nDeseja realizar um novo emprestimo [s/n]? ");
		scanf("%c", &novoEmprestimo);
	}while(novoEmprestimo=='s');
}

int checardisponibilidade(movimentacao *listamovimentacao1, int codigo){
	
	movimentacao *apontador=listamovimentacao1->prox;
	if(!vaziamovimentacao(listamovimentacao1)){
		while(apontador!=NULL){
			if(codigo==apontador->codigoVeiculo)
				return 0;
			else apontador=apontador->prox;
		}
		if(apontador==NULL) return 1;
	}
	return 1;
}

void devolucaoveiculo(movimentacao *listamovimentacao1, veiculo *listaveiculo1, cliente *listacliente1){
	
	char novaDevolucao, confirm;
	int i, j; //Variaveis auxiliares
	int codigoVeiculo, codigoCliente;
	
	do{
		j=0;
		system("cls");
		puts("DEVOLUCAO DE VEICULOS");
		do{
			i=0;
			puts("\nDigite o codigo do veiculo: ");
			scanf("%d", &codigoVeiculo);
			i=consultarcodV(listaveiculo1, codigoVeiculo);
			if(i==0)
				printf("\nCODIGO NAO EXISTENTE, DIGITE NOVAMENTE\n");
		}while(i==0);
		
		if(i==1){
			if(!checardisponibilidade(listamovimentacao1, codigoVeiculo)){
				puts("\n\nVEICULO");
				printarveiculo(listaveiculo1, codigoVeiculo);
				puts("\n");
				movimentacao *apontadorMovimentacao=listamovimentacao1->prox;
				encontrarposicaoMovimentacao(listamovimentacao1, codigoVeiculo, &apontadorMovimentacao);
				codigoCliente=apontadorMovimentacao->codigoCliente;
				printarcliente(listacliente1, codigoCliente);
				printf("\n");
				confirm=confirmaropcao();
				
				if(confirm=='s'){
					
					movimentacao *aux=listamovimentacao1;
					while(aux->prox!=apontadorMovimentacao && aux!=apontadorMovimentacao)
						aux=aux->prox;
						
					aux->prox=apontadorMovimentacao->prox;
					free(apontadorMovimentacao);
					
					veiculo *apontadorVeiculo=listaveiculo1->prox;
					encontrarposicaoVeiculo(listaveiculo1, codigoVeiculo, &apontadorVeiculo);
					switch(apontadorVeiculo->tipoVeiculo){
						case CARRO:{
							apontadorVeiculo->uVeiculos.Carros.historicoLocacoes[apontadorVeiculo->uVeiculos.Carros.contadorLocacoes-1].dataDevolucao=dataatual();
							break;
						}
						case MOTO:{
							apontadorVeiculo->uVeiculos.Motos.historicoLocacoes[apontadorVeiculo->uVeiculos.Motos.contadorLocacoes-1].dataDevolucao=dataatual();
							break;
						}
					}
					reWriteArq(listamovimentacao1, arqM);
					system("cls");
					puts("DEVOLUCAO DE VEICULOS");
					puts("\nDEVOLUCAO EFETUADA COM SUCESSO");
				}
			}
			else puts("\nO CARRO NAO ESTA LOCADO");
			fflush(stdin);
		}
		puts("\nDeseja realizar uma nova devolucao [s/n]? ");
		scanf("%c", &novaDevolucao);
	}while(novaDevolucao=='s');
}

int vaziamovimentacao(movimentacao *listamovimentacao1){
	
	if(listamovimentacao1->prox==NULL)
		return 1;
	else 
		return 0;
}

void encontrarposicaoMovimentacao(movimentacao *listamovimentacao1, int codigoVeiculo, movimentacao **aux){
	
	int i=0;
	
	movimentacao *apontador=*aux;
	if(!vaziamovimentacao(listamovimentacao1)){
		while(i==0){
			if(apontador->codigoVeiculo==codigoVeiculo)
				i=1;
			else if(apontador->prox!=NULL)
				apontador=apontador->prox;
		}
	}
	*aux=apontador;
}

void menurelatorio(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1){
	
	int resposta;
	
	do{
		system("cls");
		puts("RELATORIOS\n");
		puts("1 - BALANCO DO ACERVO");
		puts("2 - BALANCO DE MOVIMENTACOES");
		puts("0 - RETORNAR");
		scanf("%d", &resposta);
		
		switch(resposta){
			case 1: balancodoacervo(listaveiculo1);
				break;	
			case 2: balancolocacoes(listacliente1, listaveiculo1, listamovimentacao1);
				break;
		}
	}while(resposta!=0);
}

void balancodoacervo(veiculo *listaveiculo1){ 
	
	system("cls");
	Data data=dataatual();
	printf("%d/%d/%d", data.dia, data.mes, data.ano);
	puts("   BALANCO DE ACERVO\n\n");
	puts("VEICULO             TIPO               QUANTIDADE");
	contadorbalanco(listaveiculo1);
}

void contadorbalanco(veiculo *listaveiculo1){ 
	
	int quantidade, i=0, j, k;
	veiculo *apontador=listaveiculo1->prox;
	
	int nVeiculos=totalveiculos(listaveiculo1);
	char modelo[nVeiculos][20], tipo[20];	
	apontador=listaveiculo1->prox;
	
	while(apontador!=NULL){ 								//Coleta e armazena todos os modelos numa matriz de char
		switch(apontador->tipoVeiculo){
			case CARRO: strcpy(modelo[i], strupr(apontador->uVeiculos.Carros.modelo));
			break;
			case MOTO: strcpy(modelo[i], strupr(apontador->uVeiculos.Motos.modelo));
			break;
		}
		i++;
		apontador=apontador->prox;
	}

	for(j=0; j<i; j++){										//Retira os modelos repetidos para compara��o
		if(modelo[j][0]!='\0'){
			for(k=j+1; k<i; k++){
				if(strcmp(modelo[j], modelo[k])==0)
					modelo[k][0]=='\0';
			}
		}
	}
	for(j=0; j<i; j++){										//Compara e printa o balan�o
		quantidade=0;
		apontador=listaveiculo1->prox;
		if(modelo[j][0]!='\0'){
			while(apontador!=NULL){
				switch(apontador->tipoVeiculo){
					case CARRO:{
						if(strcmp(modelo[j], apontador->uVeiculos.Carros.modelo)==0){
							switch(apontador->uVeiculos.Carros.tipoCarro){
								case CARROPOPULAR: strcpy(tipo, "CARRO POPULAR      ");
								break;
								case CARROLUXO: strcpy(tipo, "CARRO DE LUXO      ");
								break;
								case VAN: strcpy(tipo, "VAN                ");
								break;
							}
							quantidade++;
						}
						apontador=apontador->prox;
						break;
					}
					case MOTO:{
						if(strcmp(modelo[j], apontador->uVeiculos.Motos.modelo)==0){
							strcpy(tipo, "MOTO               ");
							quantidade++;
						}
						apontador=apontador->prox;
						break;
					}
				}
			}
			printf("%s", modelo[j]);
			for(int a=0; a<20-strlen(modelo[j]); a++)
				printf(" ");
			printf("%s%d\n", tipo, quantidade);
		}
	}
	contadortiposveiculos(listaveiculo1);
	puts("\nPRESSIONE QUALQUER TECLA PARA VOLTAR");
	getch();	
	
}

int totalveiculos(veiculo *listaveiculo1){
	int nVeiculos=0;
	if(!vaziaveiculo(listaveiculo1)){
		veiculo *apontador=listaveiculo1->prox;
		nVeiculos=1;
		while(apontador->prox!=NULL){
			apontador=apontador->prox;
			nVeiculos++;
		}
	}
	return nVeiculos;
}

int totalclientes(cliente *listacliente1){
	int nClientes=0;
	if(!vaziacliente(listacliente1)){
		cliente *apontador=listacliente1->prox;
		nClientes=1;
		while(apontador->prox!=NULL){
			apontador=apontador->prox;
			nClientes++;
		}
	}
	return nClientes;
}

int totallocacoes(cliente *listacliente1){
	int nLocacoes=0;
	if(!vaziacliente(listacliente1)){
		cliente *apontador=listacliente1->prox;
		while(apontador!=NULL){
			nLocacoes+=apontador->totalLocacao;
			apontador=apontador->prox;
		}
	}
	return nLocacoes;
}

void contadortiposveiculos(veiculo *listaveiculo1){ 
	
	int quantidade[4]={0}; 									//Armazenara em cada posicao a quantidade de cada tipo de veiculo
	veiculo *apontador=listaveiculo1->prox;
	while(apontador!=NULL){
		switch(apontador->tipoVeiculo){
			case CARRO:{
				if(apontador->uVeiculos.Carros.tipoCarro==1)
					quantidade[0]++; //carro popular
				else if(apontador->uVeiculos.Carros.tipoCarro==2)
					quantidade[1]++; //carro luxo
				else if(apontador->uVeiculos.Carros.tipoCarro==3)
					quantidade[2]++; //van
				break;
			}
			case MOTO:{
				quantidade[3]++; //moto
				break;
			}
		}
		apontador=apontador->prox;
	}
	puts("\nTOTAL POR TIPO:");
	printf("	CARROS POPULARES: %d\n", quantidade[0]);
	printf("	CARROS LUXO: %d\n", quantidade[1]);
	printf("	VANS: %d\n", quantidade[2]);
	printf("	MOTOS: %d\n\n", quantidade[3]);
}

void balancolocacoes(cliente *listacliente1, veiculo *listaveiculo1, movimentacao *listamovimentacao1){ 
	
	int tempodevolucao, nClientes=0, nLocacoes=0;
	nClientes=totalclientes(listacliente1);
	nLocacoes=totallocacoes(listacliente1);
	Data data=dataatual();
	
	system("cls");
	printf("%d/%d/%d", data.dia, data.mes, data.ano);
	puts("	BALANCO DE LOCACOES\n");
	printf("TOTAL DE CLIENTES: %d\n", nClientes);
	printf("TOTAL DE VEICULOS NO ACERVO: %d\n", totalveiculos(listaveiculo1));
	contadortiposveiculos(listaveiculo1);
	locacoesmes(listaveiculo1);
	
	if(nClientes>0) printf("MEDIA DE LOCACOES POR USUARIO: %.2f\n", ((float)nLocacoes/nClientes)); //Caso o numero de clientes seja 0 e n�o travar
	else printf("MEDIA DE LOCACOES POR USUARIO: 0.00\n");
	
	tempodevolucao=atrasodevolucao(listaveiculo1);
	
	if(nLocacoes>0) printf("MEDIA DE TEMPO DE ATRASO NA DEVOLUCAO (em dias): %d", (tempodevolucao/nLocacoes)); //Caso o numero de loca��es seja 0 e n�o travar
	else printf("MEDIA DE TEMPO DE ATRASO NA DEVOLUCAO (em dias): 0");
	
	puts("\n\n10 VEICULOS MAIS LOCADOS:\n");
	puts("	MODELO              TIPO                     No. LOCACOES");
	if(!vaziaveiculo(listaveiculo1))rankingveiculos(listaveiculo1);
	
	puts("\n\n10 MELHORES CLIENTES:\n");
	puts("	CODIGO			NOME		No. LOCACOES");
	if(!vaziacliente(listacliente1))rankingclientes(listacliente1);
	getch();
}

void locacoesmes(veiculo *listaveiculo1){ 
	
	int quantidade[4]={0}, j;
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int mes=tm.tm_mon + 1;
	veiculo *apontador=listaveiculo1->prox;
	
	while(apontador!=NULL){
		switch(apontador->tipoVeiculo){
			case CARRO:{
				for(j=0; j<apontador->uVeiculos.Carros.contadorLocacoes; j++){
					if(apontador->uVeiculos.Carros.historicoLocacoes[j].dataLocacao.mes==mes){
						if(apontador->uVeiculos.Carros.tipoCarro==1)
							quantidade[0]++;
						else if(apontador->uVeiculos.Carros.tipoCarro==2)
							quantidade[1]++;
						else if(apontador->uVeiculos.Carros.tipoCarro==3)
							quantidade[2]++;
					}
				}
				break;
			}
			case MOTO:{
				for(j=0; j<apontador->uVeiculos.Motos.contadorLocacoes; j++){
					if(apontador->uVeiculos.Motos.historicoLocacoes[j].dataLocacao.mes==mes)
						quantidade[3]++;
				}
				break;
			}
		}
		apontador=apontador->prox;
	}
	
	puts("TOTAL DE LOCACOES NO MES:");
	printf("	CARROS POPULARES: %d\n", quantidade[0]);
	printf("	CARROS LUXO: %d\n", quantidade[1]);
	printf("	VANS: %d\n", quantidade[2]);
	printf("	MOTOS: %d\n", quantidade[3]);
	printf("	TOTAL: %d\n\n", (quantidade[0]+quantidade[1]+quantidade[2]+quantidade[3]));
}

int atrasodevolucao(veiculo *listaveiculo1){ 
	
	int contadorAtrasoDia=0, contadorAtrasoMes=0, contadorAtrasoAno=0, contadorAtrasoTotal=0;
	int j;
	veiculo *apontador=listaveiculo1->prox;
	while(apontador!=NULL){
		switch(apontador->tipoVeiculo){
			case CARRO:{
				if(apontador->uVeiculos.Carros.contadorLocacoes>0){
					for(j=0; j<apontador->uVeiculos.Carros.contadorLocacoes; j++){
						if(apontador->uVeiculos.Carros.historicoLocacoes[j].dataDevolucao.dia!=NULL){ //Usado para n�o deixar hist�ricos incompletos entrar no relat�rio
						contadorAtrasoDia+=(apontador->uVeiculos.Carros.historicoLocacoes[j].dataDevolucao.dia - apontador->uVeiculos.Carros.historicoLocacoes[j].dataLocacao.dia)*1;
						contadorAtrasoMes+=(apontador->uVeiculos.Carros.historicoLocacoes[j].dataDevolucao.mes - apontador->uVeiculos.Carros.historicoLocacoes[j].dataLocacao.mes)*30;
						contadorAtrasoAno+=(apontador->uVeiculos.Carros.historicoLocacoes[j].dataDevolucao.ano - apontador->uVeiculos.Carros.historicoLocacoes[j].dataLocacao.ano)*365;
						}
					}
				}
				break;
			}
			case MOTO:{
				if(apontador->uVeiculos.Motos.contadorLocacoes>0){
					for(j=0; j<apontador->uVeiculos.Motos.contadorLocacoes; j++){
						if(apontador->uVeiculos.Motos.historicoLocacoes[j].dataDevolucao.dia!=NULL){
						contadorAtrasoDia=(apontador->uVeiculos.Motos.historicoLocacoes[j].dataDevolucao.dia - apontador->uVeiculos.Motos.historicoLocacoes[j].dataLocacao.dia)*1;
						contadorAtrasoMes=(apontador->uVeiculos.Motos.historicoLocacoes[j].dataDevolucao.mes - apontador->uVeiculos.Motos.historicoLocacoes[j].dataLocacao.mes)*30;
						contadorAtrasoAno=(apontador->uVeiculos.Motos.historicoLocacoes[j].dataDevolucao.ano - apontador->uVeiculos.Motos.historicoLocacoes[j].dataLocacao.ano)*365;
						}
					}
				}
				break;
			}
		}
		apontador=apontador->prox;
	}
	contadorAtrasoTotal=contadorAtrasoDia+contadorAtrasoMes+contadorAtrasoAno;
	return contadorAtrasoTotal;
}

void rankingveiculos(veiculo *listaveiculo1){ 
	const int tVeiculo=totalveiculos(listaveiculo1); 	//Constante com o total de ve�culos cadastrados
	int matrizRanking[tVeiculo][tVeiculo]; 				//Matriz que armazena o codigo e o numero de locacoes
	int i=0, j=0, aux;
	int a;
	veiculo *apontador=listaveiculo1->prox;
	
	while(apontador!=NULL){
		switch(apontador->tipoVeiculo){
			case CARRO:{
				matrizRanking[i][0]=apontador->uVeiculos.Carros.codigo;
				matrizRanking[i][1]=apontador->uVeiculos.Carros.contadorLocacoes;
				break;
			}
			case MOTO:{
				matrizRanking[i][0]=apontador->uVeiculos.Motos.codigo;
				matrizRanking[i][1]=apontador->uVeiculos.Motos.contadorLocacoes;
				break;
			}
		}
		apontador=apontador->prox;
		i++;
	}
	for(i=0; (i<10 && i<tVeiculo); i++){ 				//Ordena em ordem crescente
		
		for(j=i; j<tVeiculo; j++){
			
			if(matrizRanking[j][1] > matrizRanking[i][1]){
				aux=matrizRanking[i][0];
				matrizRanking[i][0]=matrizRanking[j][0];
				matrizRanking[j][0]=aux;
				aux=matrizRanking[i][1];
				matrizRanking[i][1]=matrizRanking[j][1];
				matrizRanking[j][1]=aux;
			}
		}
		apontador=listaveiculo1->prox;
		encontrarposicaoVeiculo(listaveiculo1, matrizRanking[i][0], &apontador);
		printf("%d", i+1);
		
		switch(apontador->tipoVeiculo){
			case CARRO:{
				printf("	%s", apontador->uVeiculos.Carros.modelo);
				for(a=0; a<20-strlen(apontador->uVeiculos.Carros.modelo); a++)
					printf(" ");
				switch(apontador->uVeiculos.Carros.tipoCarro){
					case CARROPOPULAR:{
						printf("CARRO POPULAR       ");
						break;
					}
					case CARROLUXO:{
						printf("CARRO LUXO          ");
						break;
					}
					case VAN:{
						printf("VAN                 ");
						break;
					}
				}
				break;
			}
			case MOTO:{
				printf("	%s", apontador->uVeiculos.Motos.modelo);
				for(a=0; a<20-strlen(apontador->uVeiculos.Motos.modelo); a++)
					printf(" ");
				printf("MOTO                ");
				break;
			}
		}
		printf("     %d", matrizRanking[i][1]);
		printf("\n");
	}
}

void rankingclientes(cliente *listacliente1){ 
	
	const int tCliente=totalclientes(listacliente1);
	int i=0, j=0, aux;
	int matrizRanking[tCliente][tCliente];
	
	cliente *apontador=listacliente1->prox;
	while(apontador!=NULL){
		matrizRanking[i][0]=apontador->Codigo;
		matrizRanking[i][1]=apontador->totalLocacao;
		i++;
		apontador=apontador->prox;
	}
	for(i=0; (i<10 && i<tCliente); i++){
		
		for(j=i; j<tCliente; j++){
			if(matrizRanking[j][1] > matrizRanking[i][1]){
				aux=matrizRanking[i][0];
				matrizRanking[i][0]=matrizRanking[j][0];
				matrizRanking[j][0]=aux;
				aux=matrizRanking[i][1];
				matrizRanking[i][1]=matrizRanking[j][1];
				matrizRanking[j][1]=aux;
			}
		}
		apontador=listacliente1->prox;
		if(apontador->Codigo!=matrizRanking[i][0]){
			while(apontador->Codigo != matrizRanking[i][0])
				apontador=apontador->prox;
		}
		printf("%d	%d			%s		%d", i+1, matrizRanking[i][0], apontador->Nome, matrizRanking[i][1]);
		printf("\n");
	}
}

template <typename X>
void criarelemento(X *lista, X aux){
	
	X *proxNo=(X *)malloc(sizeof(X));
	*proxNo=aux;
	
 	if(lista->prox==NULL)
 		lista->prox=proxNo;
 	else{
 		X *ultimoNo=lista;
 		while(ultimoNo->prox!=NULL)
 			ultimoNo=ultimoNo->prox;
 			
 		ultimoNo->prox=proxNo;
	 }
	 proxNo->prox=NULL;
}

template <typename X> 
void importarArq(X *lista, const char *nomeArq){
	FILE *arq;
	arq=fopen(nomeArq, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
		
	else{
		X aux;
		while(fread(&aux, sizeof(X), 1, arq)==1)
			criarelemento(lista, aux);
	}
	fclose(arq);
}

template <typename X> 
void writeArq(X *item, const char *nomeArq){
	
	FILE *arq;
	arq=fopen(nomeArq, "a");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
		
	else{
		fwrite(item, sizeof(X), 1, arq);
		fclose(arq);
	}
}

template <typename X>
void reWriteArq(X *lista, const char *nomeArq){
	FILE *arq;
	arq=fopen(nomeArq, "w");
	if(arq==NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		X *aux = lista->prox;
		while(aux!=NULL){
			fwrite(aux, sizeof(X), 1, arq);
			aux=aux->prox;
		}
		fclose(arq);
	}
}
