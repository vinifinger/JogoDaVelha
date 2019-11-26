#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>

/*####################################################*/	
/*##			JOGO DA VELHA PCxHUMAN		        ##*/
/*## DESENVOLVEDORES: Jos� Luis, Thierry Scotto,    ##*/
/*## Vinicius Finger e Juliano Soares.				##*/
/*## DISC: Estrutura de Dados 						##*/
/*## PROFESSORA: Daniela Duarte	   					##*/
/*## ATUALIZADO: 18/11/2019							##*/
/*####################################################*/

/***********************************************/
/* Defini��o dos Registros                     */
/***********************************************/
typedef struct {         // registro
       int id;			 // id para jogadas
} INFORMACAO;

typedef struct arv {
       INFORMACAO info;   // dados do registro
       struct arv* subd;  // ponteiro para o nodo da direita
       struct arv* sube;  // ponteiro para o nodo da esquerda
} ARVORE;

#define LINHA 3				// Matriz / tabuleiro
#define COLUNA 3			// Matriz / tabuleiro

/***********************************************/
/* Defini��o das Fun��es                       */
/***********************************************/
ARVORE* criarNo();
void criarJogo ( ARVORE **r, int tabuleiro[][COLUNA] );			// Inicio do jogo
void jogar ( ARVORE **r, int, int, int tabuleiro[][COLUNA]);	// Define uma jogada na �rvore de acordo com os id's								
void analisarJogo ( ARVORE **, int, int tabuleiro[][COLUNA]);	// Fun��o para analisar as jogadas de acordo com o id, retorna um idAux para pr�xima jogada se necess�rio
void tabuleiroAtual (int tabuleiro[][COLUNA]);					// Mostrar tabuleiro; Andamento do tabuleiro; Estado atual
void jogarHuman (int tabuleiro[][COLUNA]);						// Recebe, trata e adiciona a jogada da pessoa
void jogarAleatorio (int tabuleiro[][COLUNA]);					// Jogada aleat�ria do computador, se n�o houver jogadas significantes
void jogarDiagonal ( int tabuleiro[][COLUNA] );					// Usada pelo computador para fazer uma jogada na diagonal dispon�vel
void padraoWinLoseDraw ( int tabuleiro[][COLUNA] );				// Padr�o de jogadas, usados em Folhas, busca vit�ria, busca derrota e busca empate pro computador
int  procurarPerca (int tabuleiro[][COLUNA], int, int);			// Procura pr�xima prosi��o de derrota, codVencedor e codPerdedor auxiliam na busca ( 1: Pessoa 2: Computador)
int  progressoTabuleiro (int tabuleiro[][COLUNA]);				// Verifica se o jogo terminou
void imprime_ARVORE(ARVORE*);
ARVORE *excluir_arvore ( ARVORE *r );


/***********************************************/
/* Programa Principal                          */
/***********************************************/
int main(){

	setlocale (LC_ALL,"");					// defini��o dos caracteres
	int menu = 0;							// Switch case para menu
	ARVORE *r = NULL;								// Defini��o inicial da �rvore bin�ria em NULL

  do{
  		system ("color F0");
  		puts("\n [1] Novo jogo");			// Op��o 1
  		puts(" [2] Exibir �rvore");			// Op��o 2
  		puts(" [3] Excluir �rvore");		// Op��o 3
  		puts(" [4] Sair");					// Op��o 4
  		printf("\n [>] ");		
  		scanf("%d",&menu);
  		
  		switch ( menu ){
  			default :						// Default, para comandos n�o existentes no menu
  				system("cls");
  				printf(" [Comando inv�lido]\n");
  			break;
  			
  			case 4 :
  				printf("\n [Fim]");			// Menu: terminar programa
  			break;
  			
  			case 3 :
  				system("cls");
  				if ( r != NULL ){
  					r = excluir_arvore ( r );	// Excluir �rvore ( exclui da esquerda para a direita )
  					puts("\n [�rvore exclu�da]");
				  }else
  					puts("\n [�rvore vazia]");
  					
  				getch();
  				system("cls");
  			break;
  			
  			case 2 :						// Menu: Exibir a �rvore
  				system("cls");
  				imprime_ARVORE( r );
  				puts("");
  				getch();
  				system("cls");
  			break;
  			
  			case 1 :						// Menu: Iniciar um novo jogo
  				system("cls");
		
				int tabuleiro[LINHA][COLUNA] = {0,0};			// Matriz representa o tabuleiro
				criarJogo ( &r, tabuleiro );					// Cria um novo jogo
		  }
  } while(menu != 4);
  return 0;
} // Fim Main

/************************************************
 * Fun��es                                      *
 ************************************************/

//Fun��o cria n�s
ARVORE* criarNo(){
	ARVORE *novoNo = NULL;

	if((novoNo = ( ARVORE * ) malloc ( sizeof( ARVORE ))) != NULL) {
    novoNo->subd = NULL;                                 					// inicializa sub�rvore da direita
    novoNo->sube = NULL;								 					// inicializa sub�rvore da esquerda
		return novoNo;
	} else {
		printf("Erro na aloca��o de mem�ria");
		return NULL;
	}
}//Fim criarNo

//cria o n� raiz e atribui um id
void criarJogo ( ARVORE **r, int tabuleiro[3][3] ) {
	int idAux3;
	
	if ( *r == NULL ){
		ARVORE *no = criarNo();
  		*r = no;    
		no->info.id = 20;                                  						// Anterior aponta para novo registro
		idAux3 = no->info.id;
	}else
		idAux3 = 20;
	
  	analisarJogo( r ,idAux3 , tabuleiro );								// Envia o id
}//fim criar jogo

//come�a toda a l�gica por tras de cada jogada
void jogar ( ARVORE **r, int id, int idAux, int tabuleiro[3][3]){
	
	
 	ARVORE *aux = *r;														// Ponteiro auxiliar para percorrer �rvore
 	int idAux2;

 	while( aux->info.id != id ){											// Vai em busca de uma id para tratar o estado do jogo
 		if(aux->info.id > id)
 			aux = aux->sube;
 		else
 			aux = aux->subd;
	}
	
	idAux2 = idAux;
	
	if ( aux->subd == NULL && idAux > aux->info.id){
		ARVORE *no = criarNo();													// recebe um endere�o de um novo n�
 		no->info.id = idAux;

   		aux->subd = no;
   			
	} else if ( aux->sube == NULL && idAux < aux->info.id ) {
		ARVORE *no = criarNo();													// recebe um endere�o de um novo n�
 		no->info.id = idAux;
 		aux->sube = no;
	}

   	analisarJogo( r ,idAux2 , tabuleiro );								// Envia o id
 }//FIM JOGAR

// ANALISE DAS POSSIVES JOGADAS
// ############################
void analisarJogo ( ARVORE **r, int id, int tabuleiro[3][3] ){ // Analisar jogada posicionada na �rvore

	int idAux;						// Vari�vel para pr�ximo no
 	int codVencedor;				// Vari�vel para procurar pontos de vit�rias ou derrotas CPU/HUMANO
 	int codPerdedor;				// Vari�vel para procurar pontos de vit�rias ou derrotas CPU/HUMANO
 	int condicional; 				// Return da fun��o para procurar perdedor ou ganhador (procurarPerca)
 	int terminoJogo = 0;			// Verifica se o jogo terminou
 	int cont = 0;

 	switch( id ){ 					// Possibilidade de jogadas de acordo com o id da �rvore

 		// #### Jogadas padr�es ( buscar vit�rio, buscar derrota, buscar empate ) ####
		case 5 :
		case 15:
		case 35:
		case 55:
		case 85:	padraoWinLoseDraw ( tabuleiro );	break;

 		case 10:
 			system("cls"); 										// Limpar tela
 			tabuleiro [0][2] = 2; 								// PC joga

 			tabuleiroAtual(tabuleiro);  						// Mostrar tabuleiro
			jogarHuman(tabuleiro); 								// Vez da pessoa jogar
 			tabuleiroAtual(tabuleiro); 							// Mostrar tabuleiro

 			if ( tabuleiro[0][0] == 1 || tabuleiro[0][2] == 1  || tabuleiro[2][0] == 1  || tabuleiro[2][2] == 1 ) // Condicional para definir pr�ximo passo na �rvore
 				idAux = 5;
 			else
 				idAux = 15;

 			jogar( r, id, idAux, tabuleiro); 				// Entra na �rvore para procurar pr�ximo passo
 		break;

 		case 20:
			tabuleiroAtual(tabuleiro);						// Mostrar tabuleiro
			jogarHuman(tabuleiro);							// Vez da pessoa jogar
 			tabuleiroAtual(tabuleiro);						// Mostrar tabuleiro

 			if ( tabuleiro[1][1] != 0 )						// Raiz, determina caminho da �rvore ( se jogou meio ou n�o )
 				idAux = 10;									// SIM, jogou meio
 			else
 				idAux = 80;									// N�O jogou meio

 			jogar( r, id, idAux, tabuleiro);				// Entra na �rvore para procurar pr�ximo passo

 		break;

 		case 80 :
 			system("cls");
 			tabuleiro [1][1] = 2; 							// PC joga
 			tabuleiroAtual(tabuleiro);						// Mostrar tabuleiro

 			if ( tabuleiro[0][0] == 1 || tabuleiro[0][2] == 1  || tabuleiro[2][0] == 1  || tabuleiro[2][2] == 1 ) // Condicional para definir pr�ximo passo na �rvore
 				idAux = 50;									// SIM, jogou diagonal
 			else
 				idAux = 90;									// N�O jogou diagonal

 			jogar( r, id, idAux, tabuleiro);				// Entra na �rvore para procurar pr�ximo passo

 		break;

 		case 90 :
 			system("cls");
 			tabuleiroAtual(tabuleiro);						// Mostrar tabuleiro
 			jogarHuman(tabuleiro);							// Pessoa joga
 			tabuleiroAtual(tabuleiro);						// Mostrar tabuleiro

 			codVencedor = 1;
 			codPerdedor = 0;
 			condicional = procurarPerca (tabuleiro, codVencedor, codPerdedor);		// Perde pr�ximo lance?

 			if ( condicional == 0 )							// SIM, perde
 				idAux = 85;
 			else											// N�O perde
 				idAux = 95;

 			jogar( r, id, idAux, tabuleiro);				// Entra na �rvore para procurar pr�ximo passo

 		break;

 		case 95:	// FOLHA
 			system("cls");

 			jogarDiagonal( tabuleiro );						// Jogada de defesa do PC
 			tabuleiroAtual( tabuleiro );					// Mostrar tabuleiro
 			jogarHuman( tabuleiro );						// Vez da pessoa jogar
 			system("cls");									// Limpar tela
 			tabuleiroAtual( tabuleiro );					// Mostrar tabuleiro
 			padraoWinLoseDraw ( tabuleiro );				// Jogadas padr�es ( buscar vit�ria, buscar derrota, buscar empate )
 		break;

 		case 50:
 			cont = 0;
 			system("cls");
 			tabuleiroAtual ( tabuleiro );
 			jogarHuman ( tabuleiro );
 			system("cls");
 			tabuleiroAtual ( tabuleiro );

			if ( tabuleiro[0][0] == 1 )						// Verificar se jogou diagonal novamente
				cont++;
			if ( tabuleiro[0][2] == 1 )
				cont++;
			if ( tabuleiro[2][0] == 1 )
				cont++;
			if ( tabuleiro[2][2] == 1 )
				cont++;

 			if ( cont >= 2 )							// SIM, jogou diagonal
 				idAux = 40;
 			else										// N�O jogou diagonal
 				idAux = 60;

 			jogar( r, id, idAux, tabuleiro);			// Entra na �rvore para procurar pr�ximo passo
 		break;

 		case 40:
 			codVencedor = 1;
 			codPerdedor = 0;
 			condicional = procurarPerca (tabuleiro, codVencedor, codPerdedor);		// Perde pr�ximo lance?

 			if ( condicional == 0 )					// SIM, perde
 				idAux = 35;
 			else									// N�O perde
 				idAux = 45;

 			jogar( r, id, idAux, tabuleiro);		// Entra na �rvore para procurar pr�ximo passo
 		break;

 		case 45:										// FOLHA
 			system("cls");								// Limpar tela
 			tabuleiro[1][2] = 2;						// Vez do PC jogar
 			tabuleiroAtual ( tabuleiro );				// Mostrar tabuleiro
 			jogarHuman ( tabuleiro );					// Vez da pessoa jogar
 			padraoWinLoseDraw ( tabuleiro );			// Jogadas padr�es ( buscar vit�rio, buscar derrota, buscar empate )
 		break;

 		case 60 :
 			codVencedor = 1;
 			codPerdedor = 0;
 			condicional = procurarPerca (tabuleiro, codVencedor, codPerdedor);		// Perde pr�ximo lance?

 			if ( condicional == 0 )			// SIM, perde
 				idAux = 55;
 			else							// N�O perde
 				idAux = 65;

 			jogar( r, id, idAux, tabuleiro);		// Entra na �rvore para procurar pr�ximo passo
 		break;

 		case 65 :	// FOLHA
 			system("cls");

 			if ( tabuleiro[0][0] == 1 )			// Computador joga contra a diagonal ( nessa etapa, precisa jogar para evitar derrota )
 				tabuleiro[2][2] = 2;
 			else if ( tabuleiro[0][2] == 1 )
 				tabuleiro[2][0] = 2;
 			else if ( tabuleiro[2][0] == 1 )
 				tabuleiro[0][2] = 2;
 			else if ( tabuleiro[2][2] == 1 )
 				tabuleiro[0][0] = 2;

 			tabuleiroAtual ( tabuleiro );					// Mostrar tabuleiro
 			jogarHuman ( tabuleiro );						// Vez da pessoa jogar
 			system("cls");									// Limpar tela
 			tabuleiroAtual ( tabuleiro );					// Mostrar tabuleiro
 			padraoWinLoseDraw ( tabuleiro );				// Jogadas padr�es ( buscar vit�rio, buscar derrota, buscar empate )
 		break;
	 }
 }

//exibe o estado do babuleiro
void tabuleiroAtual( int tabuleiro[3][3] ){		// Mostrar o andamento do tabuleiro

	int count = 1;								//usado para exibir os valores das casas

	//logica para exibir os valores de X e O para o usu�rio
	puts("");
	for(int i = 0; i < LINHA; i++) {
		printf("    ");
		for(int j = 0; j < COLUNA; j++) {
			if(tabuleiro[i][j] == 1)
				printf(" X ");
			else if(tabuleiro[i][j] == 2)
				printf(" O ");
			else
				printf(" %d ", count);

      if(j != 2)
        printf("|");
      count++;
		}
    puts("\n");                         	//quebra de linha apos o fim de uma linha
 	}
}//fim tabuleiroAtual

 void jogarHuman(int tabuleiro[3][3]){ 		// Fun��o para receber jogada da pessoa via input

 	int autenticacao = 0;					// Controle do do while
 	int jogada = 0;							// recebe posi��o do tabuleiro, digitado pela pessoa

 	do{
 		puts("     [SUA VEZ]");
 		scanf("%d", &jogada);				// recebe posi��o do tabuleiro, digitado pela pessoa
 		autenticacao = 0;					// Controle do while

 		switch(jogada){ 					// Restringir jogada, dependendo da condi��o da casa escolhida (livre ou ocupada)
			case 1:
				if (tabuleiro[0][0] == 0 )		// Condi��o para verificar ocupa��o do tabuleiro ( 0 = livre )
					tabuleiro [0][0] = 1;		// Matriz: Linha 1 Coluna 1
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 2:
				if (tabuleiro[0][1] == 0 )
					tabuleiro [0][1] = 1;		// Matriz: Linha 1 Coluna 2
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 3:
				if (tabuleiro[0][2] == 0 )
					tabuleiro [0][2] = 1;		// Matriz: Linha 1 Coluna 3
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 4:
				if (tabuleiro[1][0] == 0 )
					tabuleiro [1][0] = 1;		// Matriz: Linha 2 Coluna 1
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 5:
				if (tabuleiro[1][1] == 0 )
					tabuleiro [1][1] = 1;		// Matriz: Linha 2 Coluna 2
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 6:
				if (tabuleiro[1][2] == 0 )
					tabuleiro [1][2] = 1;		// Matriz: Linha 2 Coluna 3
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 7:
				if (tabuleiro[2][0] == 0 )
					tabuleiro [2][0] = 1;		// Matriz: Linha 3 Coluna 1
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 8:
				if (tabuleiro[2][1] == 0 )
					tabuleiro [2][1] = 1;		// Matriz: Linha 3 Coluna 2
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			case 9:
				if (tabuleiro[2][2] == 0 )
					tabuleiro [2][2] = 1;		// Matriz: Linha 3 Coluna 3
				else{
					puts("\n [Posi��o ocupada]\n");
					autenticacao = 1;
				}
			break;

			default:
				printf ("\n[Est� posi��o n�o existe]\n\n");		// Caso a posi��o digitada n�o exista ( matriz )
				autenticacao = 1;
			break;
		}
	} while ( autenticacao != 0 );
 }
 

//fun��o trata todas as jogadas sempre visando sua vitoria e impossibilitando o usu�rio de vencer
 int procurarPerca (int tabuleiro[3][3], int codVencedor, int codPerdedor){
 	
	int contVencedor = 0;		// Contar as posi��es ocupadas para verificar se ir� ganhar
 	int contZero = 0;			// Contar as posi��es nulas
 	int saveLine = 0;			// Salvar linha, para marcar com "codPerdedor" 
 	int saveCol = 0;			// Salvar coluna, para marcar com "codPerdedor"
 	
	// ########## VERIFICAR HORIZONTAL ##########
 	for ( int line = 0; line < 3; line++ ) {			// Faz andar pelas linhas da matriz
 		
 		contVencedor = 0;
 		contZero = 0;
 		
 		for ( int col = 0; col < 3; col++ ) {			// Andar pelas colunas da matriz
 			if ( tabuleiro[line][col] == codVencedor) 
 				contVencedor++;							// Caso ache posi��o marcada com codVencedor

			 if ( tabuleiro[line][col] == 0 ) {			// Caso ache uma posi��o nula
			 	contZero++;
			 	saveLine = line;						// Salvar linha
			 	saveCol = col;							// Salvar coluna
			 }
			 
		 }
		 
		 if ( contVencedor == 2 && contZero == 1 ) {	// Se houver 2 posi��es marcados por X ou 0 e uma posi��o nula
		 	tabuleiro[saveLine][saveCol] = codPerdedor;
		 	return 0;									// Retorna 0 para dizer que a posi��o foi encontrada
		 }
		 
	 }
	 
	// ########## VERIFICAR VERTICAL ##########
	 for ( int col = 0; col < 3; col++ ) {				// Faz andar pelas colunas da matriz
 		
 		contVencedor = 0;
 		contZero = 0;
 		
 		for ( int line = 0; line < 3; line++ ) {		// Faz andar pelas linhas da matriz
 			if ( tabuleiro[line][col] == codVencedor) 
 				contVencedor++;							// Caso ache posi��o marcada com codVencedor

			 if ( tabuleiro[line][col] == 0 ) {			// Caso ache uma posi��o nula
			 	contZero++;
			 	saveLine = line;
			 	saveCol = col;
			 }
			 
		 }
		 
		 if ( contVencedor == 2 && contZero == 1 ) {	// Se houver 2 posi��es marcados por X ou 0 e uma posi��o nula
		 	tabuleiro[saveLine][saveCol] = codPerdedor;
		 	return 0;									// Retorna 0 para dizer que a posi��o foi encontrada	
		 }
		 
	 }
	 
	// ########## VERIFICAR DIAGONAL 1 ##########
	contVencedor = 0;
	contZero = 0;
	
 	for ( int line = 0; line < 3; line++ ) {			// Anda pelas linhas e colunas da matriz, Diagonal: coluna � igual a linha
 		int col = line;
 		
 		if ( tabuleiro[line][col] == codVencedor) 		// Caso ache posi��o marcada com X ou O ( dependendo do codVencedor recebido )
 			contVencedor++;

		if ( tabuleiro[line][col] == 0 ) {				// Caso ache posi��o nula
			contZero++;
			saveLine = line;
			saveCol = col;
		 }
		 
		 if ( contVencedor == 2 && contZero == 1 ) {	// Se houver 2 posi��es marcados por X ou 0 e uma posi��o nula
		 	tabuleiro[saveLine][saveCol] = codPerdedor;	// Marca posi��o nula com 2(PC) ou 0(manter nula)
		 	return 0;									// Retorna dizendo que a posi��o foi encontrada
		 }
			 
	}
	
	// ########## VERIFICAR DIAGONAL 2 ##########
	contVencedor = 0;
	contZero = 0;
	int col = 2;										// Defini��o inicial da coluna em 2, para realizar a procura da diagonal 2
	
	for ( int line = 0; line < 3; line++ ) {			// Anda pelas linhas da matriz
 		
 		if ( tabuleiro[line][col] == codVencedor) 		// Caso ache posi��o marcada com X ou O ( dependendo do codVencedor recebido )
 			contVencedor++;

		if ( tabuleiro[line][col] == 0 ) {				// Caso ache posi��o nula
			contZero++;
			saveLine = line;							// | Salvar linha e coluna
			saveCol = col;								// |
		 }
		 
		 if ( contVencedor == 2 && contZero == 1 ) {	// Se houver 2 posi��es marcados por X ou 0 e uma posi��o nula
		 	tabuleiro[saveLine][saveCol] = codPerdedor;	// Marca posi��o nula com 2(PC) ou 0(manter nula)
		 	return 0;									// Retorna dizendo que a posi��o foi encontrada
		 }
		 
		 col--;
			 
	}
	
	return 1;											// Retorna dizendo que nenhuma posi��o de vit�ria ou derrota foi encontrada

 }

 void jogarAleatorio (int tabuleiro[3][3]){			// Realizar uma jogada aleat�ria

 	for( int i = 0; i < 3; i++ ){ 					// Buscar posi��o livre
		for( int x = 0; x < 3; x++ ){
			if( tabuleiro[x][i] == 0 ){
				tabuleiro[x][i] = 2;
				return;								//retorna o fluxo para o chamador
			}
		}
 	}
 }

 int progressoTabuleiro (int tabuleiro[3][3]){		// Verifica��o para saber se o jogo j� terminou
 	for( int i = 0; i < 3; i++ ){ 					// Caso acho um espa�o vazio, o jogo continua
		for( int x = 0; x < 3; x++ ){
			if( tabuleiro[x][i] == 0 )
				return 0;
		}
 	}
 	return 1;										// Caso n�o ache espa�o vazio, o jogo termina
 }

// fun��o realiza uma jogada na diagonal, de acordo com a jogada da pessoa
 void jogarDiagonal ( int tabuleiro[3][3] ) {
 	if ( tabuleiro[0][1] == 1 ){
 		if ( tabuleiro[0][0] == 0 )
 			tabuleiro[0][0] = 2;
 		else
 			tabuleiro[0][2] = 2;

	 } else if ( tabuleiro[1][0] == 1 ) {
	 	if( tabuleiro[0][0] == 0 )
	 		tabuleiro[0][0] = 2;
	 	else
	 		tabuleiro[2][0] = 2;
	 } else if ( tabuleiro[1][2] == 1 ) {
	 	if( tabuleiro[0][2] == 0 )
	 		tabuleiro[0][2] = 2;
	 	else
	 		tabuleiro[2][2] = 2;
	 } else if ( tabuleiro[2][1]) {
	 	if ( tabuleiro[2][0] == 0)
	 		tabuleiro[2][0] = 2;
	 	else
	 		tabuleiro[2][2] = 2;
	 }
 }

// Jogadas comuns nas folhas, Procura posi��o de v�toria; derrota e empate
void padraoWinLoseDraw ( int tabuleiro[3][3] ) {

 	while(1){
	 	system("cls");

	 	int codVencedor = 2;
	 	int codPerdedor = 2;
	 	int condicional = procurarPerca(tabuleiro, codVencedor, codPerdedor);	// Procurar vit�ria da CPU (retorna false se achar ou true se n�o achar)

	 	if(condicional == 0){
	 		tabuleiroAtual(tabuleiro);
	 		puts("    [VOC� PERDEU]");
	 		break;
		} else {
	 		codVencedor = 1; 					// define a procura de quem ir� perder CPU ou PESSOA
	 		codPerdedor = 2;
	 		condicional = procurarPerca(tabuleiro, codVencedor, codPerdedor); // Procurar ponto de derrota da CPU (retorna false se achar ou true se n�o achar)
		}

		// Procurar diagonal que ganhe o jogo PROX
		if(condicional){			// Jogar aleat�rio (falta de jogadas)
			jogarAleatorio(tabuleiro);
		}

		tabuleiroAtual(tabuleiro);		// Mostrar tabuleiro
		jogarHuman(tabuleiro);			// Pessoa joga

		int terminoJogo = progressoTabuleiro( tabuleiro );		// Guardar resposta do poss�vel t�rmino do jogo

		if(terminoJogo){ 	// Verificar se o jogo terminou com empate
			system("cls");
			tabuleiroAtual(tabuleiro);	// mostrar tabuleiro
			puts("     [EMPATOU]");
	 		break;
		}
 	}
}

//exibe a arvore
void imprime_ARVORE( ARVORE* r ){						// Fun��o para imprimir a �rvore
	
	if( r != NULL ){              // verifica se a raiz � diferente de vazio
        printf( "\n C�digo.....: %d", r->info.id );  
		getch();  
        printf( "\n >> sube" );
        imprime_ARVORE( r->sube ); // recursivo, segue pelo caminho da esquerda
        printf( "\n >> subd" );
        imprime_ARVORE( r->subd ); // recursivo, segue pelo caminho da direita
    }
    else
       printf("\n �rvore vazia!");			
}

ARVORE *excluir_arvore ( ARVORE *r ) {	// Exclui �rvore recursivamente, da esquerda pra direita
	
	if ( r == NULL )					// Para definir um n� tratado como NULL
		return NULL;
	else{
		if ( r->sube != NULL )
			r->sube = excluir_arvore ( r->sube );		// Chama recursividade, andando para esquerda
		
		else if ( r->subd != NULL )
			r->subd = excluir_arvore ( r->subd );		// Chama recursividade, andando para direita

		else( r->sube == NULL && r->subd == NULL ){
			free( r );									// Desaloca espa�o na mem�ria
			r = NULL;									
		}
	}
	
	return r;
		
}
