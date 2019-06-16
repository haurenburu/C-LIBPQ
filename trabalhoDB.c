#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>
// declaração das funções para usar depois
void print_tuplas(PGresult* res);
void cadastrarQuestoes();
void buscarQuestao();
void listarQuestao();
void menu();
// Variavel global "command" para não precisar passar por função/return
char command[500];
// main é o main lol
int main () {
	FILE* file = fopen("config.conf", "r"); /* should check the result */
    char line[256];
	char linha[4][200];
	
	int k = 0,t=0;
    for(k = 0; k < 4; k++){
    	strcpy(linha[k], fgets(line, sizeof(line), file));
    	t = strlen(linha[k]);
    	if(k <3){
    	linha[k][t-1] = '\0';
    	}
	}
	sprintf(line,"host=%s dbname=%s user=%s password=%s", linha[0],linha[1],linha[2],linha[3]);
	// -----------------------------------
	const char *conninfo; // ponteiro das informacoes da conecção
    PGconn     *conn; // pega as informações da conecção vai que da algo errado né...
    PGresult   *res; // resultado da query meio que caso volte algo ele vai printar, se não... sei la!
    int         nFields; // eu acho que é usado pra printar as tuplas
    int         i, // algum for certeza...
                j; // matriz kek
                
	//sprintf(conninfo, "host=%s dbname=%s user=%s password=%s", linha[0],linha[1],linha[2],linha[3]);
	conninfo = line;
	//conninfo = "host=localhost dbname=postgres user=postgres password=1998"; // variavel que recebe a string de conecção
	
	conn = PQconnectdb(conninfo); // chama a funcao de conecção e returna o valor para a variavel conn

	if(PQstatus(conn) != CONNECTION_OK) {
		printf("Falhou!");
		return 0;
	} else {
		printf("Conectado!\n");
	}
	
	system("pause"); // rapido demais pra ler?
	menu(); // chama o menu para criação da query
	//printf("%s", command); //mostra o com mando por enquanto TODO: remover isso...
	res = PQexec(conn, command); // executa a query recebido pelo menu() na conecção especificada...
	// oque aconteceu com a query? vai mostrar ai em baixo
	switch (PQresultStatus(res)) { 
		case PGRES_EMPTY_QUERY: // query vazia :<
		case PGRES_COMMAND_OK: printf("Done.\n"); break; // query formosa (não vai voltar nada)
		case PGRES_TUPLES_OK: print_tuplas(res); break; // a query deu certo porém precisa mostrar algo, ta tudo certo?
		case PGRES_BAD_RESPONSE: printf("[ERROR] Bad Response"); break; // algo de errado não está certo...
		case PGRES_NONFATAL_ERROR: // deu tudo certo :D
		case PGRES_FATAL_ERROR: printf(PQresultErrorMessage(res)); break; // azedou, mostre porque deu errado pf
		default:
			printf("Unexpected Response"); // chegou aqui tem algo bem errado
	}
	// limpa o ponteiro né, nada de momery leak aqui!!!
    PQclear(res);
    // fecha a conecção com o db
    PQfinish(conn);
    // o nome ja diz tudo
	exit(1);
	return 0;
}
// struct com os items do db
struct quest {
	short id;
	char dominio[100];
	char tema[100];
	short dificuldade;
	char texto[250];
	char resposta[500];
};
void print_tuplas(PGresult* res) {
	int nFields = PQnfields(res);
	int i, j;
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n");
    /* next, print out the rows */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }
}
void cadastrarQuestoes() {
	char query[150]; // acho que 150 é pouco TODO: aumentar pra uns 500 kek
	struct quest questao; // instancia o struct com os items, so pra ficar mais organizado mesmo...
	system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
	printf("Cadastrar!\n"); // mostra a seção do menu no caso, cadastro TODO: mudar esse nome
	// recebe o dominio da questao
	printf("Digite o dominio da questao: ");
	setbuf(stdin, NULL); // nada de /n aqui!
	gets(questao.dominio); // grava na variavel
	// recebe o tema da questao
	printf("Digite o tema da questao: ");
	setbuf(stdin, NULL); // nada de /n aqui!
	gets(questao.tema); // grava na variavel
	// recebe a dificuldade da questao
	printf("Digite a dificuldade da questao (0-10): ");
	scanf("%d", &questao.dificuldade);
	// recebe o enunciado da questao
	printf("Digite o texto da questao: ");
	setbuf(stdin, NULL); // nada de /n aqui!
	gets(questao.texto); // grava na variavel
	// recebe a resposta da questao
	printf("Digite a resposta esperada: ");
	setbuf(stdin, NULL); // nada de /n aqui!
	gets(questao.resposta); // grava na variavel
	// monstra a query com as coisas que o usuario digitou
	//sprintf(command, "INSERT INTO public.questoes(id, dominio, tema, dificuldade, texto, resposta) VALUES (%d, '{%s}', '{%s}', %d, '{%s}', '{%s}');", questao.id, questao.dominio, questao.tema, questao.dificuldade, questao.texto, questao.resposta);
	sprintf(command, "INSERT INTO public.questoes(id, dominio, tema, dificuldade, texto, resposta) VALUES (2, '%s', '%s', %d, '%s', '%s');", questao.dominio, questao.tema, questao.dificuldade, questao.texto, questao.resposta);
}
void buscarQuestao() {
	system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
	printf("Buscar!\n"); // mostra a seção do menu no caso, buscar TODO: mudar esse nome
	char textoQuery[150]; // também acho que é pouco texto.... TODO: mudar o valor pra uns 300
	char opcao = 0; // variavel controladora (sentinela que fala né?)
	do { // loop basico do menu
		printf("1-Tema\n2-Dominio\n3-Dificuldade\n4-Enunciado\n5-Sair\n>> ");
		scanf("%d", &opcao); // recebe a entrada do usuario
		// eae oque vai fazer? vamo saber agora...
		switch(opcao) {
			case 1:
				opcao = 5; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				printf("Tema!\n>>"); // so pra mostrar onde o usuario está
				scanf("%s", textoQuery);
				// monta a query e "exporta" para variavel global
				sprintf(command, "SELECT * FROM public.questoes WHERE tema = '%s';\n", textoQuery );
				break;
			case 2: 
				opcao = 5; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				printf("Dominio!\n>>"); // so pra mostrar onde o usuario está
				scanf("%s", textoQuery);
				// monta a query e "exporta" para variavel global
				sprintf(command, "SELECT * FROM public.questoes WHERE dominio = '%s';\n", textoQuery );
				break;
			case 3: 
				opcao = 5; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				printf("Dificuldade!\n>>"); // so pra mostrar onde o usuario está
				scanf("%s", textoQuery);
				// monta a query e "exporta" para variavel global
				sprintf(command, "SELECT * FROM public.questoes WHERE dificuldade = '%s';\n", textoQuery );
				break;
			case 4:
				opcao = 5; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				printf("Enunciado!\n>>"); // so pra mostrar onde o usuario está
				scanf("%s", textoQuery);
				// monta a query e "exporta" para variavel global
				sprintf(command, "SELECT * FROM public.questoes WHERE texto LIKE '%%%s%%';\n ", textoQuery );
				break;
			case 5: 
				break;
			default:
				printf("Opcao invalida"); // o usuario sempre quer te ferrar
		}
	} while(opcao != 5); // saiu do loop? espero que sim...
	
}
void listarQuestao() {
	system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
	printf("Buscar!\n"); // mostra a seção do menu no caso, buscar TODO: mudar esse nome
	char textoQuery[150]; // também acho que é pouco texto.... TODO: mudar o valor pra uns 300
	char opcao = 0; // variavel controladora (sentinela que fala né?)
	do { // loop basico do menu
		printf("1-Temas\n2-Dominios\n3-Dificuldades\n4-Sair\n>> ");
		scanf("%d", &opcao); // recebe a entrada do usuario
		// eae oque vai fazer? vamo saber agora...
		switch(opcao) {
			case 1:
				opcao = 4; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				strcpy(command, "SELECT tema FROM public.questoes;");
				break;
			case 2:
				opcao = 4; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				strcpy(command, "SELECT dominio FROM public.questoes;");
				break;
			case 3:
				opcao = 4; // garante que o loop so executa uma vez
				system("cls || clean"); // limpa a tela vale lembrar que essa parte vai funcionar no windows/linux
				strcpy(command, "SELECT dificuldade FROM public.questoes;");
				break;
			case 4: 
				break;
			default:
				printf("Opcao invalida"); // o usuario sempre quer te ferrar
		}
		
	} while(opcao != 4); // saiu do loop? espero que sim...
}

void menu() {
	system("cls || clear");
	short opcao = 0;
	do {
		printf("1-Cadastrar questao\n2-Buscar questao\n3-Listar\n4-Sair\n>> "); // menuzera
		scanf("%d", &opcao);
		switch(opcao){
			case 1: 
				opcao = 4; // garante que so execute uma vez
				cadastrarQuestoes(); // executa a rotina de cadastrar questao
				break; // pula fora
			case 2:
				opcao = 4; // garante que so execute uma vez
				buscarQuestao(); // executa a rotina de buscar por uma questao
				break; // pula fora
			case 3:
				opcao = 4;
				listarQuestao();
			case 4:
				break; // pula fora
			default:
				printf("Opcao invalida\n"); // que isso usuario....
		}
	} while(opcao != 4); // sai do loop
}
