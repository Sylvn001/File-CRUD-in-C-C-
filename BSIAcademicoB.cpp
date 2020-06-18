#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

#define KEY_UP 72
#define KEY_DOWN 80



struct TpAluno
{
	char AluRA[13], AluNome[30];
};

struct TpDisc
{
	int DiscCod;
	char DiscNome[40];
};

struct TpNota
{
	char NotaRA[13];
	int NotaDiscCod;
	float Nota;
};

//Declaração de Protótipos de Funções
int BuscaAluno(FILE *PtrAlu, char RA[13]);
int BuscaBinariaAluno(FILE *PtrAlu, char Nome[30]);
void OrdenaAlunos(char NomeArq[]);
int BuscaCod(FILE *Ptr , int cod);
int BuscaNotasDis(FILE *ptr , int cod, char ra[31]);
int BuscaRa(FILE *ptr, char ra[31]); //busca se aluno esta cadastrado em notas para que impossibilite  a exclusao
int BuscaCodNotas(FILE *ptr, int cod); //busca se a disciplina selecionada esta cadastrada em disciplinas para que possa impossibilitar a exclusao

void CriaArquivos(void)
{
   FILE *PtrArquivo = fopen("Alunos.dat","ab");
   fclose(PtrArquivo);
   PtrArquivo = fopen("Disciplinas.dat","ab");
   fclose(PtrArquivo);
   PtrArquivo = fopen("Notas.dat","ab");
   fclose(PtrArquivo);
}

void LimpaSeta(int x,int y)
{
    gotoxy(x,y);
    printf("   ");
}

void Limpa(int x, int y, int num)
{
	gotoxy(x,y);while(num--)printf(" ");
}

char Setinha(int opcoes,int x,int y)
{
    opcoes--;
    char hit;
    int pos = 0;
    while(hit !=13)
    {
        gotoxy(x,pos+y);
        printf(" ->");
        gotoxy(x,pos+y);
        hit = getch();
        LimpaSeta(x,pos+y);
        if(hit == 72)
        {
            if(pos == 0)
            {
                pos=opcoes;
            }
            else 
                pos--;
        }
        else if(hit == 80)
        {
            if(pos == opcoes)
                pos = 0;
            else
                pos++;
        }
    }
    return pos+1;
}

void LimpaTela(int x, int y, int num)
{
	gotoxy(x,y);while(num--)printf(" ");
}

void Moldura(int CI, int LI, int CF, int LF, int cor)
{
	int i;
	
	textcolor(cor);

	gotoxy(CI,LI); printf("#");
	gotoxy(CF,LF); printf("#");
	gotoxy(CF,LI); printf("#");
	gotoxy(CI,LF); printf("#");
	
	//Coluna ate Coluna F
	for(i=CI+1; i<CF; i++)
	{
		gotoxy(i,LI);printf("-");
		gotoxy(i,LF);printf("-");
	}
	//Linha ate Linha F
	for(i=LI+1; i<LF; i++)
	{
		gotoxy(CI,i);printf(".");	
		gotoxy(CF,i);printf(".");
	}

}


char MenuP()
{
	clrscr();
	Moldura(1,1,80,25,12);
	gotoxy(15,8);printf("\\ Selecione a opcao que deseja! \\");
	gotoxy(15,10);printf("   Alunos");
	gotoxy(15,11);printf("   Disciplinas");
	gotoxy(15,12);printf("   Notas");
	gotoxy(15,13);printf("   Relatorios Especificos");
	textcolor(WHITE);gotoxy(15,14);printf("	 Encerrar");
	textcolor(12);
	return Setinha( 5, 15, 10);
}
char MenuA()
{
	clrscr();
	Moldura(1,1,80,25,13);
	
	textcolor(14);gotoxy(15,8);printf("\\* * *  ALUNOS  * * *//");
	textcolor(13);
	gotoxy(15,11);printf("   Inserir Alunos");
	gotoxy(15,12);printf("   Relatorio de Alunos");
	gotoxy(15,13);printf("   Consultar Alunos pelo R.A.");
	gotoxy(15,14);printf("   Alterar dados de Alunos");
	gotoxy(15,15);printf("   Excluir Alunos");
	gotoxy(15,16);printf("   Ordenar Alunos pelo Nome");
	gotoxy(15,17);printf("   Consultar Alunos pelo Nome");
	textcolor(WHITE);gotoxy(15,18);printf("   Voltar");
	textcolor(13);
	return Setinha( 8, 15, 11);
	
}
char MenuD()
{
	clrscr();
	Moldura(1,1,80,25,11);
	
	textcolor(14);gotoxy(15,8);printf("\\* * *  DISCIPLINAS  * * *//");
	textcolor(11);
	gotoxy(15,11);printf("   Inserir Disciplinas pelo Codigo");
	gotoxy(15,12);printf("   Relatorio de Disciplinas");
	gotoxy(15,13);printf("   Consultar Disciplinas pelo Codigo");
	gotoxy(15,14);printf("   Alterar dados de Disciplinas");
	gotoxy(15,15);printf("   Excluir Disciplinas");
	gotoxy(15,16);printf("   Ordenar Disciplinas pelo Nome");
	gotoxy(15,17);printf("   Consultar Disciplinas pelo Nome");
	textcolor(WHITE);gotoxy(15,18);printf("   Voltar");
	textcolor(11);
	
	return Setinha( 8, 15, 11);
}
char MenuN(){
	
	clrscr();
	Moldura(1,1,80,25,10);
	textcolor(14);gotoxy(15,8);printf("\\* * *  NOTAS  * * *//");
	textcolor(10);

	gotoxy(15,11);printf("   Inserir Notas");
	gotoxy(15,12);printf("   Relatorio de Notas");
	gotoxy(15,13);printf("   Consultar Notas");
	gotoxy(15,14);printf("   Alterar Notas");
	gotoxy(15,15);printf("   Excluir Notas");
	textcolor(WHITE);gotoxy(15,16);printf("   Voltar");
	textcolor(10);

	return Setinha( 6, 15, 11);		
}
char MenuR()
{
	clrscr();
	Moldura(1,1,80,25,9);
	textcolor(9);gotoxy(15,8);printf("\\* * *  RELATORIOS ESPECIFICOS  * * *//");
	
	gotoxy(15,10);printf("   Alunos reprovados em duas ou mais Disciplinas");
	gotoxy(15,11);printf("   Alunos que iniciam com uma determinada letra");
	gotoxy(15,12);printf("   Disciplinas com Média Geral abaixo de 6.0");
	gotoxy(15,13);printf("   Geral: Alunos e suas Disciplinas");
	gotoxy(15,14);printf("   Geral Ordenado: Disciplinas e seus Alunos");
	textcolor(WHITE);gotoxy(15,15);printf("   Voltar");
	textcolor(9);
	
	return Setinha(6,15,10);
	
}

void CadAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	int pos;
	FILE *PtrAluno = fopen(NomeArq,"ab+");
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(15,8);printf("# # #   INSERCAO DE NOVOS ALUNOS   # # #");
	gotoxy(15,10);printf("R.A. do Aluno: ");
	fflush(stdin);
	gets(RegAlu.AluRA);
	while(strcmp(RegAlu.AluRA,"\0")!=0)
	{
		pos = BuscaAluno(PtrAluno,RegAlu.AluRA);
		if (pos>-1)
		{
			gotoxy(15,20);textcolor(11);printf("Aluno encontrado! Nao pode cadastrar RA's repetidos!\n");textcolor(13);
			getch();
			LimpaTela(15,10,25);
			LimpaTela(15,20,52);
		}
		else
		{
			gotoxy(15,11);printf("Nome do Aluno: ");
			fflush(stdin);
			gets(RegAlu.AluNome);
			//Gravação em Arquivo == Alunos.dat
			fwrite(&RegAlu,sizeof(TpAluno),1,PtrAluno);
			gotoxy(15,15);textcolor(WHITE);printf("Aluno INSERIDO corretamente!\n");textcolor(13);
			getch();
			LimpaTela(15,11,52);
			LimpaTela(15,15,28);
			LimpaTela(15,10,32);
			
		}
		
		gotoxy(15,10);printf("R.A. do Aluno: ");
		fflush(stdin);
		gets(RegAlu.AluRA);
	}
	fclose(PtrAluno);
}

int BuscaBinariaAluno(FILE *PtrAlu, char Nome[30])
{
	TpAluno RegM;
	int Inicio=0, Meio, Fim;
	fseek(PtrAlu,0,2);
	Fim = ftell(PtrAlu)/sizeof(TpAluno) - 1;  // Fim = QtdedeRegistros - 1
	Meio = Fim/2;
	fseek(PtrAlu,Meio*sizeof(TpAluno),0);
	fread(&RegM,sizeof(TpAluno),1,PtrAlu);
	while (Inicio<Fim && stricmp(Nome,RegM.AluNome)!=0)
	{
		if (stricmp(Nome,RegM.AluNome)>0)
			Inicio = Meio + 1;
		else
			Fim = Meio - 1;
		
		Meio = (Inicio+Fim)/2;
		fseek(PtrAlu,Meio*sizeof(TpAluno),0);
		fread(&RegM,sizeof(TpAluno),1,PtrAlu);
	}
	
	if (stricmp(Nome,RegM.AluNome)==0)
  	    return Meio*sizeof(TpAluno);
    else
		return -1;
}

int BuscaAluno(FILE *PtrAlu, char RA[13])   //Busca Exaustiva no Arquivo de Alunos
{
	TpAluno R;
	rewind(PtrAlu);
	fread(&R,sizeof(TpAluno),1,PtrAlu);
	while (!feof(PtrAlu) && stricmp(R.AluRA,RA)!=0)
		   fread(&R,sizeof(TpAluno),1,PtrAlu);
	
	if (feof(PtrAlu))
		return -1;
	else 
		return ftell(PtrAlu) - sizeof(TpAluno);
}

void ConsAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	int pos;
	TpAluno aluno;
	FILE *PtrAluno = fopen(NomeArq,"rb");
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(15,8);printf("# # #   CONSULTA DE ALUNOS PELO R.A.  # # #");
	fread(&aluno, sizeof(TpAluno),1,PtrAluno);
	if(feof(PtrAluno))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(13);
		getch();
	}
	else
	{
		gotoxy(15,10);printf("Digite o R.A. do Aluno: ");
		fflush(stdin);
		gets(RegAlu.AluRA);
		while(strcmp(RegAlu.AluRA,"\0")!=0)
		{
			pos = BuscaAluno(PtrAluno,RegAlu.AluRA);
			if (pos>-1)
			{
				textcolor(WHITE);gotoxy(15,11);printf("Dados encontrados!");textcolor(13);
				fseek(PtrAluno,pos,0);
				fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
				gotoxy(15,15);textcolor(11);printf("R.A.:%s Nome do Aluno: %s ",RegAlu.AluRA,RegAlu.AluNome);textcolor(13);
			}
			else
			{
				gotoxy(15,15);textcolor(GREEN);printf("Aluno NAO Cadastrado!");textcolor(13);
			}
			getch();
			LimpaTela(25,10,32);
			LimpaTela(15,11,22);
			LimpaTela(15,15,52);
		
			gotoxy(15,10);printf("Digite o R.A. do Aluno: ");
			fflush(stdin);
			gets(RegAlu.AluRA);
		}
	}
	
	
	fclose(PtrAluno);
}

void ConsNomeAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	int pos;
	FILE *PtrAluno = fopen(NomeArq,"rb");
	clrscr();
	Moldura(1,1,80,25,13);
	TpAluno aluno;
	gotoxy(15,8);printf("# # #   CONSULTA DE ALUNOS PELO NOME  # # #");
	fread(&aluno, sizeof(TpAluno),1,PtrAluno);
	if(feof(PtrAluno))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum registro cadastrado!");textcolor(11);
		getch();
	}
	else
	{
		gotoxy(15,10);printf("Digite o NOME do Aluno: ");
		fflush(stdin);
		gets(RegAlu.AluNome);
		while(strcmp(RegAlu.AluNome,"\0")!=0)
		{
			pos = BuscaBinariaAluno(PtrAluno,RegAlu.AluNome);
			if (pos>-1)
			{
				textcolor(WHITE);gotoxy(15,11);printf("***Dados encontrados:\n");textcolor(13);
				fseek(PtrAluno,pos,0);
				fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
				textcolor(11);gotoxy(15,15);printf("R.A.:%s  Nome do Aluno: %s",RegAlu.AluRA,RegAlu.AluNome);textcolor(13);
			}
			else
			{
				gotoxy(15,15);textcolor(11);printf("Aluno NAO Cadastrado!");textcolor(13);
			}
			getch();
			LimpaTela(25,10,32);
			LimpaTela(15,11,22);
			LimpaTela(15,15,52);
			
			gotoxy(15,10);printf("Digite o NOME do Aluno: ");
			fflush(stdin);
			gets(RegAlu.AluNome);
		}
	}
	
	fclose(PtrAluno);
}

void AltAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	int pos;
	TpAluno aluno;
	FILE *PtrAluno = fopen(NomeArq,"rb+");
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(15,8);printf("# # #   ALTERACAO DE ALUNOS   # # #");
	fread(&aluno, sizeof(TpAluno),1,PtrAluno);
	if(feof(PtrAluno))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(13);
		getch();
	}
	else
	{
		gotoxy(15,10);printf("Digite o R.A. do Aluno: ");
		fflush(stdin);
		gets(RegAlu.AluRA);
		while(strcmp(RegAlu.AluRA,"\0")!=0)
		{
			pos = BuscaAluno(PtrAluno,RegAlu.AluRA);
			if (pos>-1)
			{
				gotoxy(15,12);textcolor(WHITE);printf("Dados encontrados!");textcolor(13);
				fseek(PtrAluno,pos,0);
				fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
				textcolor(11);gotoxy(15,14);printf("R.A.: %s  Nome do Aluno: %s",RegAlu.AluRA,RegAlu.AluNome);
				gotoxy(15,15);printf("Deseja Alterar? (S/N)  ");
				if (toupper(getche())=='S')
				{
					textcolor(13);
					gotoxy(15,16);printf("Digite o Novo Nome: ");
					fflush(stdin);
					gets(RegAlu.AluNome);
					fseek(PtrAluno,pos,0);
					fwrite(&RegAlu,sizeof(TpAluno),1,PtrAluno);
					textcolor(11);gotoxy(15,18);printf("Registro atualizado corretamente!");textcolor(13);
				}
				else
				{
					textcolor(11);gotoxy(15,18);printf("Alteracao Cancelada!");textcolor(13);
				}
			
			}
			else
			{
				textcolor(WHITE);gotoxy(15,15);printf("Aluno NAO Cadastrado!");textcolor(13);
			}
			getch();
			
			LimpaTela(15,10,40);
			LimpaTela(15,12,40);
			LimpaTela(15,14,50);
			LimpaTela(15,15,40);
			LimpaTela(15,16,50);
			LimpaTela(15,18,40);
			
			gotoxy(15,10);printf("Digite o R.A. do Aluno: ");
			fflush(stdin);
			gets(RegAlu.AluRA);
		}
	}
	fclose(PtrAluno);
	
}

void ExcFisAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	char AuxRA[13];
	int pos;
	FILE *PtrAluno = fopen(NomeArq,"rb");
	FILE *ptrNota = fopen("Notas.dat" ,"rb");
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(15,8);printf("# # #   EXCLUSAO FISICA DE ALUNOS   # # #");
	fread(&RegAlu, sizeof(RegAlu),1,PtrAluno);
	if(feof(PtrAluno))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(11);
		getch();
	}
	else
	{
		
		gotoxy(15,10);printf("Digite o R.A. do Aluno: ");
		fflush(stdin);
		gets(AuxRA);
		if(BuscaRa(ptrNota, AuxRA) >-1 ) // Remove apenas alunos sem notas cadastradas 
		{
			textcolor(11);gotoxy(15,11);printf("Erro! Aluno cadastrado em notas!! ");
			gotoxy(15,12);printf("Por favor, remova suas notas do sistema antes de apaga-lo");
			textcolor(13);
			getch();
		}
		//Para testar a exclusao, tente um aluno nao cadastrado em notas
		else
		{
			if(strcmp(AuxRA,"\0")!=0)
			{
				pos = BuscaAluno(PtrAluno,AuxRA);
				if (pos>-1)
				{
					textcolor(WHITE);gotoxy(15,11);printf("Dados encontrados:\n");textcolor(13);
					fseek(PtrAluno,pos,0);
					fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
					gotoxy(15,12);printf("R.A.:%s Nome do Aluno: %s",RegAlu.AluRA,RegAlu.AluNome);
					gotoxy(15,14);textcolor(11);printf("Confirma Exclusao ? (S/N)  ");textcolor(13);
					if (toupper(getche())=='S')
					{
						FILE *PtrTemp = fopen("Temp.dat","wb");
						rewind(PtrAluno);    //fseek(PtrAluno,0,0);
						fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
						while (!feof(PtrAluno))
						{
							if (strcmp(AuxRA,RegAlu.AluRA)!=0)
								fwrite(&RegAlu,sizeof(TpAluno),1,PtrTemp);
							
							fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
						}
						fclose(PtrAluno);
						fclose(PtrTemp);
						remove(NomeArq);
						rename("Temp.dat",NomeArq);
						gotoxy(15,15);textcolor(WHITE);printf("Registro EXCLUIDO corretamente!");textcolor(13);
					}
					else{
						textcolor(WHITE);gotoxy(15,15);printf("Exclusao cancelada!");textcolor(13);
					 	fclose(PtrAluno);
					}
				}
				else
				   {
					  textcolor(11);gotoxy(15,15);printf("Aluno NAO Cadastrado!");textcolor(13);
					  fclose(PtrAluno);
				   }
				getch();
			}
			else fclose(PtrAluno);
		}
	}
	
	fclose(ptrNota);
}

void RelAlunos(char NomeArq[])
{
	TpAluno RegAlu;
	FILE *PtrAluno = fopen(NomeArq,"rb");
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(15,1);textcolor(WHITE);printf("# # #   CONTEUDO DO ARQUIVO DE ALUNOS   # # #");textcolor(13);
	fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
	if (feof(PtrAluno))
	{
		gotoxy(15,15);textcolor(WHITE);printf(" Nao ha Alunos Cadastrados! ");textcolor(13);
	
	}
	else
	   {
	   		textcolor(15);gotoxy(15,3);printf("RA: ");gotoxy(34,3);printf("Nome: ");textcolor(13);
	   	
			int i=5;
			while (!feof(PtrAluno))
			{
				//gotoxy(15,i)printf("%s \t\t %s",RegAlu.AluRA,RegAlu.AluNome);
				gotoxy(15,i);printf("%s",RegAlu.AluRA);gotoxy(34,i);printf("%s",RegAlu.AluNome);
				fread(&RegAlu,sizeof(TpAluno),1,PtrAluno);
				Sleep(30);
				i++;
			}
			gotoxy(15,i+1);textcolor(11);printf("# # # FIM DO RELATORIO DE ALUNOS # # #");textcolor(13);
	   }
	   
	getch();
	fclose(PtrAluno);
}
		  
void OrdenaAlunos(char NomeArq[])
{
	int i, j, QtdeRegistros;
	TpAluno RegI, RegJ;
	FILE *PtrAluno = fopen(NomeArq,"rb+");
	fseek(PtrAluno,0,2);     
	QtdeRegistros = ftell(PtrAluno) / sizeof(TpAluno);
	for(i=0; i<QtdeRegistros-1; i++)
		for(j=i+1; j<QtdeRegistros; j++)
		{
			fseek(PtrAluno,i*sizeof(TpAluno),0);
			fread(&RegI,sizeof(TpAluno),1,PtrAluno);
			fseek(PtrAluno,j*sizeof(TpAluno),0);
			fread(&RegJ,sizeof(TpAluno),1,PtrAluno);
			if(strcmp(RegI.AluNome,RegJ.AluNome)>0)
			{
				fseek(PtrAluno,j*sizeof(TpAluno),0);
				fwrite(&RegI,sizeof(TpAluno),1,PtrAluno);
				fseek(PtrAluno,i*sizeof(TpAluno),0);
				fwrite(&RegJ,sizeof(TpAluno),1,PtrAluno);
			}
		}
	fclose(PtrAluno);
}

void l(void)
{
	//Mo preguiça de limpar buffer, tem que digitar muito
	fflush(stdin);
}

void CadastraDis(char nomeArq[31]){
	FILE *PtrCod = fopen(nomeArq , "ab+");
	clrscr();
	TpDisc tpDis;
	Moldura(1,1,80,25,11);
	int cod, pos=-1;
	gotoxy(15,8);printf("# # # Cadastro Disciplinas # # # ");
	
	gotoxy(15,10);printf("Digite o codigo: ");
	scanf("%d", &cod);
	while(cod != 0 )
	{
	
		pos = BuscaCod(PtrCod, cod);
		
		if(pos > -1)
		{
			gotoxy(15,11);textcolor(WHITE);printf("Disciplina ja cadastrada!\n");textcolor(11);
			 
		}
		else{
			tpDis.DiscCod = cod;
			gotoxy(15,11);printf("Digite o nome da disciplina: ");
			l();gets(tpDis.DiscNome);
			fwrite(&tpDis, sizeof(TpDisc) , 1 , PtrCod);	
			textcolor(WHITE);gotoxy(15,16);printf("Cadastrado com sucesso!!");textcolor(11);	
		}	
		getch();
		LimpaTela(15,16,40);
		LimpaTela(15,11,40);
		LimpaTela(15,10,40);
		
		gotoxy(15,10);printf("Digite o codigo: ");
		scanf("%d", &cod);
	}
	fclose(PtrCod);
}

void listaDis(char nomeArq[31])
{
	clrscr();
	FILE *PtrArq = fopen(nomeArq, "rb");
	Moldura(1,1,80,25,11);
	TpDisc dis;
	textcolor(WHITE);gotoxy(15,1);printf("* * * * Lista de disciplinas * * * *");textcolor(11);
	fread(&dis, sizeof(TpDisc) , 1 , PtrArq);
	if(feof(PtrArq))
	{
		gotoxy(15,15);textcolor(WHITE);printf("* * * NENHUMA DISCIPLINA CADASTRADA!! * * *");textcolor(11);
	}
	else{	
		//printf("\t Cod \t\t Disciplina\n");
		textcolor(WHITE);gotoxy(15,4);printf("Cod "); printf("Disciplina ");textcolor(11);
		int i=6;
		while(!feof(PtrArq))
		{
			
			gotoxy(15,i);printf("%d",dis.DiscCod);gotoxy(25,i);printf("%s",dis.DiscNome);
			//printf("\t %d \t\t %s\n",dis.DiscCod, dis.DiscNome);	
			i++;
			Sleep(30);
			fread(&dis, sizeof(TpDisc) , 1 , PtrArq);
		}
	}

	fclose(PtrArq);
	getch();
}

int BuscaCod(FILE *Ptr , int cod)
{
	rewind(Ptr);
	TpDisc dis;
	fread(&dis, sizeof(TpDisc) , 1 , Ptr);
	while(!feof(Ptr) && cod != dis.DiscCod)
	{
		fread(&dis, sizeof(TpDisc) , 1 , Ptr);	
			
	}
	
	if(feof(Ptr))
		return -1;
	else
		return ftell(Ptr) - sizeof(TpDisc);
}

void consCod(char nomeArq[31])
{
	FILE *Ptr = fopen(nomeArq, "rb+");
	int  cod, pos;
	TpDisc dis;
	clrscr();
	Moldura(1,1,80,25,11);
	gotoxy(15,8);printf("# # #Consultar disciplina atraves do codigo # # # ");
	
	fread(&dis, sizeof(TpDisc), 1, Ptr);
	if(feof(Ptr))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum registro cadastrado!");
	}
	else
	{
		
		gotoxy(15,10);printf("Digite o codigo da discinplina: ");
		scanf("%d", &cod);
		while(cod != 0)
		{
			pos = BuscaCod(Ptr, cod);
			if(pos > -1)
			{
				
				textcolor(WHITE);gotoxy(15,12);printf("* DISCIPLINA ENCONTRADA!! *");textcolor(11);
				fseek(Ptr, pos, 0);
				fread(&dis , sizeof(TpDisc) , 1 ,Ptr);
				
				gotoxy(15,14);printf("* Codigo: %d  Disciplina: %s ",dis.DiscCod, dis.DiscNome);	
			
			}
			else{
				gotoxy(15,14);textcolor(WHITE);printf("Disciplina nao encontrada!\n");textcolor(11);
			}
			getch();
			LimpaTela(15,12,40);
			LimpaTela(15,14,40);
			LimpaTela(15,10,55);
			gotoxy(15,10);printf("Digite o codigo da discinplina: ");
			scanf("%d", &cod);	
		}
	}

	fclose(Ptr);	
	getch();
}

void alteraDis(char nomeArq[31])
{
	FILE *Ptr = fopen(nomeArq, "rb+");
	
	int  cod, pos;
	TpDisc dis;
	clrscr();
	Moldura(1,1,80,25,11);
	gotoxy(15,8);printf("# # #Consultar disciplina atraves do codigo # # # ");
	fread(&dis, sizeof(TpDisc), 1, Ptr);
	if(feof(Ptr))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum Registro Cadastrado!");textcolor(11);
	}
	
	else
	{
		gotoxy(15,11);printf("Digite o codigo da discinplina: ");
		scanf("%d", &cod);
		while(cod != 0)
		{
			pos = BuscaCod(Ptr, cod);
			if(pos > -1)
			{	
				fseek(Ptr, pos, 0);
				
				gotoxy(15,12);textcolor(WHITE);printf("* DISCIPLINA ENCONTRADA!! *");textcolor(11);
				
				fread(&dis , sizeof(TpDisc) , 1 ,Ptr);
			
				gotoxy(15,14);printf("* Codigo: %d  Disciplina: %s ",dis.DiscCod, dis.DiscNome);	
				
				gotoxy(15,15);textcolor(WHITE);printf("Deseja alterar? (S/N) ");textcolor(11);
				if(toupper(getche()) == 'S')
				{
					fseek(Ptr, pos, 0);
					gotoxy(15,16);printf("Digite o novo nome da disciplina: ");
					l();gets(dis.DiscNome);
					if(stricmp(dis.DiscNome , "\0"))
					{
						fwrite(&dis, sizeof(TpDisc) , 1, Ptr);
						gotoxy(15,18);textcolor(WHITE);printf("Alterado com sucesso!");textcolor(11);
					}
					else{
						gotoxy(15,18);textcolor(WHITE);printf("Erro ao atualizar! Nome Invalido\n");textcolor(11);
					}
				}
				else
				{
					textcolor(WHITE);gotoxy(15,18);printf("Alteracao de disciplina cancelada!\n");textcolor(11);
					
				}
			}
			else{
				textcolor(WHITE);gotoxy(15,15);printf("Disciplina nao encontrada!\n");textcolor(11);
			}
			getch();
			LimpaTela(15,10,50);
			LimpaTela(15,11,40);
			LimpaTela(15,12,52);
			LimpaTela(15,14,40);
			LimpaTela(15,15,40);
			LimpaTela(15,16,52);
			LimpaTela(15,18,40);
		
			gotoxy(15,11);printf("Digite o codigo da discinplina: ");
			scanf("%d", &cod);
		}
	}
	
	fclose(Ptr);
	getch();	
}	
void ExcluirDis(char nomeArq[31])
{
	FILE *ptrArq = fopen(nomeArq, "rb");
	FILE *ptrNotas = fopen("Notas.dat" , "rb");
	int cod, pos;
	TpDisc dis;
	clrscr();
	Moldura(1,1,80,25,11);
	gotoxy(15,8);printf("* * * Excluir Disciplina * * *");
	fread(&dis, sizeof(TpDisc), 1, ptrArq);
	if(feof(ptrArq))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum registro cadastrado!");textcolor(11);
	}
	else
	{
		gotoxy(15,10);printf("Digite o cod da disciplina:  ");
		scanf("%d" , &cod);
		if(BuscaCodNotas(ptrNotas, cod) > -1 ) //apenas permite a exclusao se deletar os alunos cadastrado em notas
		{
			gotoxy(15,15);textcolor(WHITE);printf("ERRO * * * Disciplina em uso!!");
			gotoxy(15,16);printf("Remova os alunos cadastrado antes de apagar!");textcolor(11);
		}
		else
		{
			pos = BuscaCod(ptrArq , cod);
			if(pos > -1)
			{
				gotoxy(15,12);textcolor(WHITE);printf("Disciplina encontrada!!");textcolor(11);
				fseek(ptrArq,pos, 0);
				fread(&dis, sizeof(TpDisc) , 1, ptrArq);
				gotoxy(15,14);printf("* Codigo: %d  Disciplina: %s ",dis.DiscCod, dis.DiscNome);
			
				
				gotoxy(15,15);printf("Deseja Excluir a disciplina? (S/N) ");
				if(toupper(getche()) == 'S')
				{
					rewind(ptrArq);
					FILE *newArq = fopen("temp.dat", "wb");
					
					fread(&dis , sizeof(TpDisc) , 1 , ptrArq); 
					while(!feof(ptrArq))
					{
						if(cod != dis.DiscCod)
							fwrite(&dis , sizeof(TpDisc) , 1 , newArq);
						fread(&dis , sizeof(TpDisc) , 1 , ptrArq);
					}
					fclose(ptrArq);
					fclose(newArq);
					remove(nomeArq);
					rename("temp.dat" , nomeArq);
					gotoxy(15,16);textcolor(WHITE);printf("Excluido com sucesso!");textcolor(11);
				}
				else{
					gotoxy(15,16);textcolor(WHITE);printf("Exclusao cancelada!!");textcolor(11);
					fclose(ptrArq);
				}
			}
			else
			{
				gotoxy(15,15);textcolor(WHITE);printf("Erro! Codigo nao encontrado! ");textcolor(11);
				fclose(ptrArq);
			}
		}
	}
	
	fclose(ptrNotas);
	getch();
}

void BubbleSortDis(char nomeArq[31])
{
	//boing boing boing 
	int i, TL;
	TpDisc tpI, tpAux;
	FILE *Ptr = fopen(nomeArq , "rb+");
	clrscr();
	fseek(Ptr, 0, 2);
	TL = ftell(Ptr) / sizeof(TpDisc);
	
	while(TL > 0){
	
		for(i=0; i<TL-1; i++)
		{
			fseek(Ptr, i*sizeof(TpDisc), 0);
			fread(&tpI, sizeof(TpDisc) , 1, Ptr);	
			fseek(Ptr, (i+1)*sizeof(TpDisc), 0);
			fread(&tpAux, sizeof(TpDisc) , 1, Ptr);
			if(stricmp(tpI.DiscNome , tpAux.DiscNome) > 0)
			{
				fseek(Ptr, i*sizeof(TpDisc), 0);
				fwrite(&tpAux, sizeof(TpDisc) , 1, Ptr);	
				fseek(Ptr, (i+1)*sizeof(TpDisc), 0);
				fwrite(&tpI, sizeof(TpDisc) , 1, Ptr);
			}
		}
		TL--;
	}
	fclose(Ptr);

	Sleep(200);
	                                                                                                                          
}
int BuscaBinariaDis(FILE *Ptr, char nome[31])
{
	TpDisc regM;
	int inicio = 0, meio, fim;
	fseek(Ptr,0, 2 );
	fim = ftell(Ptr) / sizeof(TpDisc) -1;
	meio = fim/2;
	
	fseek(Ptr, meio * sizeof(TpDisc), 0);
	fread(&regM, sizeof(TpDisc),1,Ptr);
	while(inicio < fim && stricmp(nome, regM.DiscNome) !=0)
	{
		if(stricmp(nome, regM.DiscNome) > 0)
			inicio = meio+1;
		else
			fim = meio-1;
		meio = (inicio + fim) / 2;
		fseek(Ptr, meio * sizeof(TpDisc), 0);
		fread(&regM, sizeof(TpDisc),1,Ptr);
	}
	if(stricmp(nome, regM.DiscNome) == 0)
		return meio*sizeof(TpDisc);
	else
		return -1;	
		
}
void consDisNome(char nomeArq[31])
{
	FILE *ptr = fopen(nomeArq, "rb");
	char cons[31];
	int pos;
	TpDisc dis;
	clrscr();
	Moldura(1,1,80,25,11);
	gotoxy(15,8);printf("* * * Consultar Disciplinas pelo nome * * *");
	fread(&dis, sizeof(TpDisc), 1, ptr);
	if(feof(ptr))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum registro cadastrado!");textcolor(11);
		getch();
	}
	else
	{
		gotoxy(15,10);printf("Digite o nome da disciplina: ");
		l();gets(cons);
		fread(&dis, sizeof(TpDisc), 1, ptr);
		while(stricmp(cons,"\0") != 0)
		{
			pos = BuscaBinariaDis(ptr, cons);
			if(pos > -1)
			{
				gotoxy(15,12);textcolor(WHITE);printf("Disciplina encontrada!");textcolor(11);
				fseek(ptr, pos, 0);
				fread(&dis , sizeof(TpDisc) , 1 , ptr);
				gotoxy(15,14);printf("*Codigo: %d  Disciplina: %s ",dis.DiscCod, dis.DiscNome);	
			}
			else
			{
				textcolor(WHITE);gotoxy(15,15);printf("Erro, disciplina nao existe!");textcolor(11);
			}
			
			LimpaTela(15,12,40);
			LimpaTela(15,14,40);
			LimpaTela(15,15,40);
			getch();
		}
	}
	fclose(ptr);
}

void InsDireta(FILE *ptr)
{
	TpNota aux, cad; 
	fseek(ptr,0,2);
	int TL = (ftell(ptr) / sizeof(TpNota) )-1 ;
	
	fseek(ptr, (TL-1)*sizeof(TpNota), 0);
	fread(&aux, sizeof(TpNota), 1, ptr);
	fread(&cad, sizeof(TpNota), 1, ptr);
	while(TL > 0 && stricmp(aux.NotaRA , cad.NotaRA ) > 0 )
	{
		fseek(ptr, (TL-1)*sizeof(TpNota), 0);
		fwrite(&cad, sizeof(TpNota), 1, ptr);
		fwrite(&aux, sizeof(TpNota),1,ptr);
		
		TL--;
		
		fseek(ptr, (TL-1) * sizeof(TpNota), 0);
		fread(&aux, sizeof(TpNota), 1, ptr);
	}	
}

void cadastraNotas(char nomeArq[31])
{
	FILE *ptrArqNot = fopen(nomeArq, "rb+");
	FILE * PtrArqAlun = fopen("Alunos.dat","rb");
	FILE * ptrArqDis = fopen("Disciplinas.dat","rb");
	clrscr();
	Moldura(1,1,80,25,10);
	TpNota cad;
	gotoxy(15,7);printf("# # # Cadastro De Notas # # # ");
	gotoxy(15,9);printf("Digite o ra do aluno: ");
	l();gets(cad.NotaRA);
	while(stricmp(cad.NotaRA, "\0") != 0)
	{
		if(BuscaAluno(PtrArqAlun, cad.NotaRA) > -1)
		{
			gotoxy(15,10);printf("Digite o codigo da disciplina: ");
			scanf("%d" ,&cad.NotaDiscCod);
		
			if(BuscaCod(ptrArqDis, cad.NotaDiscCod) > -1)
			{	
				if(BuscaNotasDis(ptrArqNot, cad.NotaDiscCod, cad.NotaRA) > -1)
				{
					textcolor(WHITE);gotoxy(15,15);printf("Nota ja cadastrada!");textcolor(10);
				}
			
				else{
					gotoxy(15,11);printf("Nota do aluno: ");
					scanf("%f" ,&cad.Nota);
					while( !(cad.Nota >=0 && cad.Nota <=10) )
					{
						gotoxy(15,12);printf("Digite uma nota maior igual a 0 ou menor igual a 10!!!");
						gotoxy(15,11);scanf("%f" ,&cad.Nota);
					}
					LimpaTela(15,12,50);
					fseek(ptrArqNot,0,2);
					fwrite(&cad, sizeof(TpNota),1,ptrArqNot);
					InsDireta(ptrArqNot);
					gotoxy(15,15);printf("Cadastrado com sucesso");
				}
			
			}
			else{
				gotoxy(15,15);textcolor(WHITE);printf("Erro, disciplina nao encontrada!");textcolor(10);
			}
		}
		else{
			gotoxy(15,15);textcolor(WHITE);printf("Erro! Aluno nao encontrado.");textcolor(10);
		}
		getch();
		LimpaTela(15,9,62);
		LimpaTela(15,10,60);
		LimpaTela(15,11,50);
		LimpaTela(15,15,32);
		
		gotoxy(15,9);printf("Digite o ra do aluno: ");
		l();gets(cad.NotaRA);
	}
	textcolor(WHITE);gotoxy(15,18);printf("-----Fim do cadastro------");textcolor(10);
	getch();
	fclose(ptrArqNot);
	fclose(PtrArqAlun);
	fclose(ptrArqDis);
}

int BuscaNotasDis(FILE *ptr , int cod, char ra[31])
{
	TpNota nota;
	rewind(ptr);

	fread(&nota, sizeof(TpNota), 1, ptr);

	while(!feof(ptr) && (cod != nota.NotaDiscCod || stricmp(ra,nota.NotaRA) !=0 ) )
	{
		fread(&nota, sizeof(TpNota), 1, ptr);
	}
	
	if(feof(ptr))
	{
		return -1;
	}
	else
		return ftell(ptr) - sizeof(TpNota);
	
}

void listaNotas(char nomeArq[31])
{
	TpNota notas;
	TpDisc dis;
	TpAluno alun;
	int pos1, pos2;
	FILE *ptr =fopen(nomeArq, "rb");
	FILE *ptrDis = fopen("Disciplinas.dat", "rb");
	FILE *ptrAlun = fopen("Alunos.dat","rb");
	clrscr();
	Moldura(1,1,80,25,10);
	fread(&notas, sizeof(TpNota) , 1, ptr);
	if(feof(ptr))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Erro! Nenhum dado de nota cadastrado!");textcolor(10);
	}
	else
	{
		gotoxy(25,1);textcolor(WHITE);printf("# # # Lista de Notas # # #");textcolor(10);
		//printf("\nRA* \t\t Nome \t\t Cod* -  Nome Dis \t\t Notas");
		textcolor(WHITE);
		gotoxy(5,4);printf("RA*");gotoxy(20,4);printf("Nome ");gotoxy(35,4);printf("COD");gotoxy(40,4);printf("Nome Dis");gotoxy(60,4);printf("Nota");
		textcolor(10);
		int i=6;
		while(!feof(ptr))
		{
			pos1=BuscaAluno(ptrAlun, notas.NotaRA);
			pos2= BuscaCod(ptrDis , notas.NotaDiscCod);
			
			fseek(ptrAlun,pos1,0);
			fseek(ptrDis,pos2,0);
			
			fread(&dis,sizeof(TpDisc),1,ptrDis);
			fread(&alun , sizeof(TpAluno),1,ptrAlun);
			
			//printf("%s - %s \t %d   -   %s \t\t %.2f\n",notas.NotaRA, alun.AluNome,  notas.NotaDiscCod, dis.DiscNome  , notas.Nota );
			//Exibicao
			
			gotoxy(5,i);printf("%s ",notas.NotaRA);gotoxy(20,i);printf("%s",alun.AluNome);gotoxy(35,i);printf("%d",dis.DiscCod);
			gotoxy(40,i);printf("%s",dis.DiscNome);gotoxy(60,i);printf("%.2f",notas.Nota);
			i++;
			Sleep(30);
			fread(&notas, sizeof(TpNota) , 1, ptr);
		}
			
	}	
	fclose(ptr);
	fclose(ptrAlun);
	fclose(ptrDis);
	getch();
}

int BuscaRa(FILE *ptr, char ra[31])
{
	TpNota nota;
	rewind(ptr);
	fread(&nota , sizeof(TpNota) , 1, ptr);
	while(!feof(ptr) && stricmp(ra, nota.NotaRA) !=0 )
	{
		fread(&nota , sizeof(TpNota) , 1, ptr);
	}
	if(feof(ptr))
		return -1;
	else 
		return ftell(ptr) - sizeof(TpNota);
}

int BuscaCodNotas(FILE *ptr, int cod)
{
	TpNota nota;
	rewind(ptr);
	fread(&nota , sizeof(TpNota) , 1, ptr);
	while(!feof(ptr) && cod != nota.NotaDiscCod )
	{
		fread(&nota , sizeof(TpNota) , 1, ptr);
	}
	if(feof(ptr))
		return -1;
	else 
		return ftell(ptr) - sizeof(TpNota);
}

void ConsNotasAluno(char nome[31])
{
	FILE *ptrNotas = fopen(nome, "rb");  
	FILE *ptrDis = fopen("Disciplinas.dat", "rb");
	FILE *ptrAlun = fopen("Alunos.dat" , "rb"); 
	
	clrscr();
	Moldura(1,1,80,25,10);
	char raAlun[31];
	TpAluno alunos;
	TpDisc disc;
	TpNota notas;
	
	int pos, pos1, pos2;
	
	gotoxy(15,8);printf("* * * Consultar Notas do aluno! * * *");
	fread(&notas, sizeof(TpNota),1,ptrNotas);
	if(feof(ptrNotas))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhuma nota cadastrada!");textcolor(19);
	}
	else
	{	
		gotoxy(15,10);printf("Digite o RA do aluno: ");
		l();gets(notas.NotaRA);	
		
		gotoxy(15,11);printf("Digite o codigo da disciplina do aluno: ");
		scanf("%d", &notas.NotaDiscCod);
		
		while(stricmp(notas.NotaRA, "\0") !=0 && notas.NotaDiscCod != 0 )
		{
			pos = BuscaNotasDis(ptrNotas,notas.NotaDiscCod, notas.NotaRA);
			
			if(pos > -1)
			{
				textcolor(WHITE);gotoxy(15,13);printf("Aluno encontrado!");textcolor(10);
				
				fseek(ptrNotas, pos, 0);
				fread(&notas, sizeof(TpNota) , 1, ptrNotas);
				
				pos1=BuscaAluno(ptrAlun, notas.NotaRA);
				pos2= BuscaCod(ptrDis , notas.NotaDiscCod);
					
				fseek(ptrAlun,pos1,0);
				fseek(ptrDis,pos2,0);
				
				fread(&disc,sizeof(TpDisc),1,ptrDis);
				fread(&alunos , sizeof(TpAluno),1,ptrAlun);
						
				gotoxy(10,15);printf("%s ",notas.NotaRA);gotoxy(25,15);printf("%s",alunos.AluNome);gotoxy(40,15);printf("%d",disc.DiscCod);
				gotoxy(45,15);printf("%s",disc.DiscNome);gotoxy(60,15);printf("%.2f",notas.Nota);
				getch();
				LimpaTela(10,15,60);
				LimpaTela(15,10,50);
				LimpaTela(15,11,50);
				LimpaTela(15,13,30);
			}
			else{
				textcolor(WHITE);gotoxy(15,15);printf("Nota nao encontrada!");
				getch();
				LimpaTela(15,15,30);
			}
		
		
			
			gotoxy(15,10);printf("# # # Digite o RA do aluno: ");
			l();gets(notas.NotaRA);	
			
			gotoxy(15,11);printf("Digite o codigo da disciplina do aluno: ");
			scanf("%d", &notas.NotaDiscCod);
		}
		
	}
	getch();
	fclose(ptrDis);
	fclose(ptrNotas);
	fclose(ptrAlun);
	
}

void alteraNotAluno(char nome[31])
{
	FILE *PtrAlun = fopen("Alunos.dat", "rb");
	FILE *PtrDis = fopen("Disciplinas.dat", "rb");
	FILE *PtrNotas = fopen(nome, "rb+");
	clrscr();
	Moldura(1,1,80,25,10);
	
	TpDisc disc;
	TpNota notas;
	TpAluno alunos;
	
	int posN, posA, posD;
	
	gotoxy(15,8);printf("* * *Alterar Notas de alunos!! * * *");
	fread(&notas, sizeof(TpNota),1,PtrNotas);
	if(feof(PtrNotas))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhuma nota cadastrada!");
	}
	else
	{
		gotoxy(15,10);printf("# # # Digite o RA do aluno: ");
		l();gets(notas.NotaRA);	
				
		gotoxy(15,11);printf("Digite o codigo da disciplina do aluno: ");
		scanf("%d", &notas.NotaDiscCod);
		
		while(stricmp(notas.NotaRA, "\0") !=0 && notas.NotaDiscCod != 0)
		{
			posN = BuscaNotasDis(PtrNotas, notas.NotaDiscCod , notas.NotaRA);
			if( posN > -1)
			{
				gotoxy(15,12);textcolor(WHITE);printf(" - - - - - Nota do aluno encontrada! - - - - - -");textcolor(10);
				
				fseek(PtrNotas, posN, 0);
				fread(&notas, sizeof(TpNota),1,PtrNotas);
				
				posA = BuscaAluno(PtrAlun, notas.NotaRA);
				posD = BuscaCod(PtrDis, notas.NotaDiscCod);
				
				fseek(PtrAlun,posA,0);
				fseek(PtrDis,posD,0);
				
				fread(&alunos, sizeof(TpAluno),1,PtrAlun);
				fread(&disc, sizeof(TpDisc),1,PtrDis);
				
				gotoxy(10,14);printf("%s ",notas.NotaRA);gotoxy(25,14);printf("%s",alunos.AluNome);gotoxy(40,14);printf("%d",disc.DiscCod);
				gotoxy(45,14);printf("%s",disc.DiscNome);gotoxy(60,14);printf("%.2f",notas.Nota);
				
				
				gotoxy(15,15);printf("Deseja alterar? S/N");

				if(toupper(getche()) == 'S')
				{
					gotoxy(15,16);printf("Digite a nova nota do aluno: ");
					fseek(PtrNotas,posN,0);
					scanf("%f", &notas.Nota);
					while( !(notas.Nota >= 0 && notas.Nota <= 10 ) )
					{
						textcolor(WHITE);gotoxy(15,17);printf("Nota invalida! Digite entre 0 a 10: ");
						scanf("%f", &notas.Nota);
					}
					LimpaTela(15,17,60);
					
					fwrite(&notas, sizeof(TpNota),1,PtrNotas);
					textcolor(WHITE);gotoxy(15,18);printf("Alterado com sucesso!!");textcolor(10);
					
				}
				
				else{
					textcolor(WHITE);gotoxy(15,15);printf("Alteracao Cancelada!");textcolor(10);
				getch();
				LimpaTela(15,15,30);
				}
			}
			else{
				textcolor(WHITE);gotoxy(15,15);printf("Nota nao encontrada!");textcolor(10);
				getch();
				LimpaTela(15,15,30);
			}
			getch();
			LimpaTela(10,15,60);
			LimpaTela(15,10,50);
			LimpaTela(15,11,50);
			LimpaTela(15,12,50);
			LimpaTela(15,13,30);
			LimpaTela(10,14,60);
			LimpaTela(15,15,50);
			LimpaTela(15,16,50);
			LimpaTela(15,18,30);
				
			
			gotoxy(15,10);printf("# # # Digite o RA do aluno: ");
			l();gets(notas.NotaRA);	
			
			gotoxy(15,11);printf("Digite o codigo da disciplina do aluno: ");
			scanf("%d", &notas.NotaDiscCod);		
		}
	}

	
	getch();
	fclose(PtrAlun);
	fclose(PtrDis);
	fclose(PtrNotas);
}
void ExcluirNotas(char nome[31])
{
	FILE *PtrAlun = fopen("Alunos.dat", "rb");
	FILE *PtrDis = fopen("Disciplinas.dat", "rb");
	FILE *PtrNotas = fopen(nome, "rb+");
	clrscr();
	Moldura(1,1,80,25,10);
	TpDisc disc;
	TpNota notas;
	TpAluno alunos;
	
	int posN, posA, posD, CodAux;
	char raAux[31];
	
	gotoxy(15,8);printf("* * *Alterar Notas de alunos!! * * *");
	fread(&notas, sizeof(TpNota),1,PtrNotas);
	if(feof(PtrNotas))
	{
		gotoxy(15,10);printf("Nenhuma nota cadastrada!");
		fclose(PtrAlun);
		fclose(PtrDis);
		fclose(PtrNotas);
	}
	else{
		gotoxy(15,11);printf("Digite o RA do aluno: ");
		gets(notas.NotaRA);
		
		gotoxy(15,12);printf("Digite o codigo da disciplina: ");
		scanf("%d",&notas.NotaDiscCod);
		
		posN = BuscaNotasDis(PtrNotas, notas.NotaDiscCod, notas.NotaRA);
		if(posN >-1)
		{
			textcolor(WHITE);gotoxy(15,13);printf("Aluno encontrado!");textcolor(10);
			fseek(PtrNotas, posN, 0);
			fread(&notas, sizeof(TpNota),1,PtrNotas);
			
			posA = BuscaAluno(PtrAlun, notas.NotaRA);
			posD = BuscaCod(PtrDis, notas.NotaDiscCod);
			
			fseek(PtrAlun,posA,0);
			fseek(PtrDis,posD,0);
			
			fread(&alunos, sizeof(TpAluno),1,PtrAlun);
			fread(&disc, sizeof(TpDisc),1,PtrDis);
			
			gotoxy(10,15);printf("%s ",notas.NotaRA);gotoxy(25,15);printf("%s",alunos.AluNome);gotoxy(40,15);printf("%d",disc.DiscCod);
			gotoxy(45,15);printf("%s",disc.DiscNome);gotoxy(60,15);printf("%.2f",notas.Nota);
			
			textcolor(WHITE);gotoxy(15,17);printf("Deseja Excluir? (S/N) ");
			if(toupper(getche()) == 'S')
			{
				CodAux = notas.NotaDiscCod;
				strcpy(raAux, notas.NotaRA);
				rewind(PtrNotas);
				FILE *ptrAux = fopen("temp.dat" , "wb");
				fread(&notas, sizeof(TpNota),1,PtrNotas);
				while(!feof(PtrNotas))
				{
					if(stricmp(notas.NotaRA, raAux) !=0 || notas.NotaDiscCod != CodAux  )
						fwrite(&notas, sizeof(TpNota),1,ptrAux);
						
					fread(&notas, sizeof(TpNota),1,PtrNotas);
				}
				fclose(PtrAlun);
				fclose(PtrDis);
				fclose(PtrNotas);
				fclose(ptrAux);
				remove(nome);
				rename("temp.dat", nome);
				gotoxy(15,18);printf("Excluido Com Sucesso!!");textcolor(10);
			}
			else
			{
				textcolor(WHITE);gotoxy(15,18);printf("Exclusao Cancelada....");textcolor(10);
				fclose(PtrAlun);
				fclose(PtrDis);
				fclose(PtrNotas);
			}
		}
		else
		{
			gotoxy(15,14);printf("Aluno nao encontrado!");
			Sleep(400);
			textcolor(WHITE);gotoxy(15,18);printf("Exclusao Cancelada.....");textcolor(10);
			fclose(PtrAlun);
			fclose(PtrDis);
			fclose(PtrNotas);
		}
	}
	getch();
}

int BuscaVet(char vetRA[][13], int TL, char NotaRA[])
{
	int j=0;
	while(j < TL && stricmp(NotaRA, vetRA[j]) != 0)
		j++;
	
	if(j <TL)
		return j;
	else return -1;
	
}


void AlunosRep2(char nome[])
{	
	FILE *PtrNotas = fopen(nome , "rb");
	FILE *ptrAlun = fopen("Alunos.dat" , "rb");
	
	fseek(PtrNotas,0,2);
	
	int tam = ftell(PtrNotas) / sizeof(TpNota);
	char vetNome[tam][13], TL=0;
	int vetCont[tam], j, i=0, pos, vetCod[tam], posA;
	
	TpNota notas;
	TpAluno alun;
	TpDisc disc;
	
	rewind(PtrNotas);
	clrscr();
	Moldura(1,1,80,25,9);
	
	fread(&notas, sizeof(TpNota), 1, PtrNotas);
	if(feof(PtrNotas))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Nenhum aluno cadastrado!!");textcolor(9);
	}
	else
	{
		while(!feof(PtrNotas))
		{
			if(notas.Nota < 6)
			{
				pos = BuscaVet(vetNome,TL, notas.NotaRA);  
				if( pos == -1)
				{
					strcpy(vetNome[TL] , notas.NotaRA); 
					vetCont[TL] = 1;	
					TL++;
				}
				else vetCont[pos]++;		
					
			} 
		
			fread(&notas, sizeof(TpNota), 1, PtrNotas);
		}
		gotoxy(15,8);printf("Alunos reprovados em duas ou mais disciplinas");
		int j=10;
		textcolor(WHITE);
		while(i < TL)
		{
			if(vetCont[i] >= 2)
			{
				posA = BuscaAluno(ptrAlun, vetNome[i]);
				fseek(ptrAlun, posA, 0);
				fread(&alun, sizeof(TpAluno) ,1, ptrAlun);
				gotoxy(10,j);printf("Nome: %-15s *RA: %-8s Reprovou em %-2d  Materias", alun.AluNome , vetNome[i] , vetCont[i]);
				j++;
			}
			i++;
		}
		textcolor(9);
		
	}
	fclose(PtrNotas);
	fclose(ptrAlun);
	
	getch();
}

void AlunLetra(char nomeArq[31])
{
	FILE *ptrAlun = fopen(nomeArq, "rb");
	TpAluno aluno;
	char letra;
	clrscr();
	Moldura(1,1,80,25,9);
	textcolor(WHITE);gotoxy(15,8);printf("RELATORIOS: Alunos que iniciam com uma determinada letra!");textcolor(9);
	fread(&aluno, sizeof(TpAluno) , 1, ptrAlun);
	if(feof(ptrAlun))
	{
		textcolor(WHITE);gotoxy(15,15);printf("Arquivo vazio!! nenhum dado cadastrado!");textcolor(9);
	}
	else
	{
		rewind(ptrAlun);
		gotoxy(15,10);printf("Digite a letra inicial do aluno: ");
		l();scanf("%c", &letra);
		letra = toupper(letra);
		fread(&aluno, sizeof(TpAluno),1, ptrAlun);
		int j=12;
		while(!feof(ptrAlun))
		{
			if(toupper(aluno.AluNome[0] ) == letra)
			{
				gotoxy(15,j);printf("Aluno: %-15s RA: %-3s",aluno.AluNome, aluno.AluRA);
				j++;
			}
			fread(&aluno, sizeof(TpAluno) , 1, ptrAlun);
		}
	}
	fclose(ptrAlun);
	getch();
}
int BuscaVetCod(int vet[] , int TL, int cod)
{
	int i=0;
	while(i<TL && cod != vet[i])
		i++;
	if(i < TL)
		return i;
	else
		return -1;
}

void mediaGeralAlunos(char ArqNotas[31], char ArqDis[31])
{
	//abrindo arquivos
	FILE *ptrNotas = fopen(ArqNotas, "rb");
	FILE *ptrDis = fopen(ArqDis, "rb");
	//estruturas para leitura
	TpNota nota;
	TpDisc disc;
	//alocando valor para variavel tam
	fseek(ptrNotas, 0, 2);
	ftell(ptrNotas);
	int tam = ftell(ptrNotas) / sizeof(TpNota);
	//retornando o ponteiro para que possa ser feita a varredura
	rewind(ptrNotas);
	//declaração das variaveis e limpeza de tela
	int i,posD, vetCont[tam], TL=0, pos, vetCodDis[tam];
	float mediaG=0, vetMediaNotas[tam];	
	clrscr();
	Moldura(1,1,80,25,9);
	gotoxy(15,1);printf("Disciplinas com media geral inferior");
	fread(&nota, sizeof(TpNota), 1, ptrNotas);
	if(feof(ptrNotas))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(9);
	}
	else
	{	
		rewind(ptrNotas);
		fread(&nota, sizeof(TpNota), 1, ptrNotas);
		//Realizando o calculo da media geral.
		while(!feof(ptrNotas))
		{
			//armazenando valores para media das disciplinas 
			pos = BuscaVetCod(vetCodDis,TL, nota.NotaDiscCod);
		
			if(pos > -1)
			{
				vetCont[pos]++;
				vetMediaNotas[pos] += nota.Nota;
			}
			else{
				vetCodDis[TL] = nota.NotaDiscCod;
				vetCont[TL] = 1;
				vetMediaNotas[TL] = nota.Nota;
				TL++; 
			}
			//media geral
			mediaG += nota.Nota;	
			fread(&nota, sizeof(TpNota), 1, ptrNotas);
		}
		
		mediaG = mediaG / tam;
	
		gotoxy(15,4);textcolor(WHITE);printf(" * * * Media Geral: %.2f * * * ",mediaG);textcolor(9);
		
		
		gotoxy(15,6);printf("Disciplinas com media inferior a Media Geral::: ");
	
		i=0;
		int j =8;
		while(i < TL)
		{
			vetMediaNotas[i] /= vetCont[i];
			
			if(vetMediaNotas[i] < mediaG )
			{
				posD = BuscaCod(ptrDis, vetCodDis[i]);
				fseek(ptrDis, posD, 0);
				fread(&disc, sizeof(TpDisc), 1, ptrDis);
				gotoxy(15,j);printf("*Cod: %-5d %-15s MEDIA: %-4.2f ", disc.DiscCod, disc.DiscNome, vetMediaNotas[i]);
				j++;
			}
			
			i++;
		}
		
		gotoxy(15,j+2);printf("Media Individual de todas Disciplinas!");textcolor(WHITE);
		j= j+4;	
		i=0;
		while(i<TL)
		{
			posD = BuscaCod(ptrDis, vetCodDis[i]);
			fseek(ptrDis, posD, 0);
			fread(&disc, sizeof(TpDisc), 1, ptrDis);
			gotoxy(15,j);printf("*Cod: %-5d %-15s MEDIA: %-4.2f", disc.DiscCod, disc.DiscNome, vetMediaNotas[i]);
			i++;
			j++;
		}
	
	}

	fclose(ptrNotas);
	fclose(ptrDis);
	getch(); 
}

void geralAlunDis(char ArqAlun[31], char ArqDis[31], char ArqNotas[31])
{
	//Ordenado, antes de chamar essa funçao eu ordeno todos os arquivos
	//o arquivo notas está ordenado de forma inserida pelo RA
	 
	
	
	FILE *ptrAlun = fopen(ArqAlun, "rb"); 
	FILE *ptrDis = fopen(ArqDis, "rb"); 
	FILE *ptrNotas = fopen(ArqNotas, "rb");
	
	int posD;
	char sit[15];
	
	TpNota nota;
	TpDisc dis;
	TpAluno aluno;
	clrscr();
	system("mode 90,30");
	Moldura(1,1,90,30,9);
	textcolor(WHITE);gotoxy(25,1);printf("RELATORIO GERAL: Alunos e suas disciplinas!");textcolor(9);
	fread(&aluno, sizeof(TpAluno),1,ptrAlun);
	if(feof(ptrAlun))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(9);
	}
	else
	{
		int j =3;
		while(!feof(ptrAlun))
		{
			//Só vai exibir alunos que possuem notas cadastradas
			if(BuscaRa(ptrNotas,aluno.AluRA) > -1)
			{
				gotoxy(5,j);textcolor(WHITE);printf("Aluno: %-15s RA: %s ", aluno.AluNome, aluno.AluRA);textcolor(9);
				j++;
				fseek(ptrNotas,0,0);
				fread(&nota, sizeof(TpNota),1,ptrNotas);
				while(!feof(ptrNotas))
				{
					if(stricmp(nota.NotaRA, aluno.AluRA ) == 0)
					{
						if(nota.Nota >= 6.0)
							strcpy(sit, "Aprovado");
						else strcpy(sit, "Reprovado");
						
						posD = BuscaCod(ptrDis,nota.NotaDiscCod);
						fseek(ptrDis, posD, 0);
						fread(&dis, sizeof(TpDisc), 1, ptrDis);
						gotoxy(5,j);printf("Disciplina: %-10d %-20s Nota: %-10.2f Situacao: ",nota.NotaDiscCod, dis.DiscNome, nota.Nota);
						textcolor(WHITE); printf("%s",sit);textcolor(9);
						j++;	
					}	
					fread(&nota, sizeof(TpNota),1,ptrNotas);	
				}
				j++;
			}	

			fread(&aluno, sizeof(TpAluno),1,ptrAlun);	
		}
	}
	
	getch();
	fclose(ptrAlun);
	fclose(ptrNotas);
	fclose(ptrDis);
}

void geralDisAlun(char ArqAlun[31], char ArqDis[31], char ArqNotas[31])
{
	FILE *ptrAlun = fopen(ArqAlun, "rb"); 
	FILE *ptrDis = fopen(ArqDis, "rb"); 
	FILE *ptrNotas = fopen(ArqNotas, "rb");
	
	int posA;
	char sit[15];
	
	TpNota nota;
	TpDisc dis;
	TpAluno aluno;
	clrscr();
	system("mode 90,30");
	Moldura(1,1,90,30,9);
	
	textcolor(WHITE);gotoxy(25,1);printf("RELATORIO GERAL: Disciplinas e seus alunos");textcolor(9);
	fread(&dis, sizeof(TpDisc),1,ptrDis);
	if(feof(ptrDis))
	{
		gotoxy(15,15);textcolor(WHITE);printf("Nenhum registro cadastrado!");textcolor(9);
	}
	else
	{
		int j =3;
		while(!feof(ptrDis))
		{
			//Só vai exibir disciplinas que possuem alubos cadastrados
			if(BuscaCodNotas(ptrNotas,dis.DiscCod) > -1)
			{
				gotoxy(5,j);textcolor(WHITE);printf("Disciplina: %-7s COD: %d ", dis.DiscNome, dis.DiscCod);textcolor(9);
				j++;
				fseek(ptrNotas,0,0);
				fread(&nota, sizeof(TpNota),1,ptrNotas);
				while(!feof(ptrNotas))
				{
					if(nota.NotaDiscCod == dis.DiscCod)
					{
						if(nota.Nota >= 6.0)
							strcpy(sit, "Aprovado");
						else strcpy(sit, "Reprovado");
						
						posA = BuscaAluno(ptrAlun,nota.NotaRA);
						fseek(ptrAlun, posA, 0);
						fread(&aluno, sizeof(TpAluno), 1, ptrAlun);
						gotoxy(5,j);printf("RA: %-15s %-20s NOTA: %-5.2f Situacao: ",aluno.AluRA, aluno.AluNome, nota.Nota);
						textcolor(WHITE); printf("%s",sit);textcolor(9);
						j++;	
					}	
					fread(&nota, sizeof(TpNota),1,ptrNotas);	
				}
				j++;
			}	

			fread(&dis, sizeof(TpDisc),1,ptrDis);	
		}
	}
	
	getch();
	fclose(ptrAlun);
	fclose(ptrNotas);
	fclose(ptrDis);
}


int main(void)
{
	char opcao, aluno[31] , dis[31], notas[31], op2;
	system("mode 80,25");
	//Moldura(1,1,80,25,9)
	strcpy(aluno, "Alunos.dat");
	strcpy(dis, "Disciplinas.dat");
	strcpy(notas, "Notas.dat");
	CriaArquivos();
	do
	{
		//opcao = Menu();
		system("mode 80,25");
		opcao = MenuP();
		switch(opcao)
		{
			case 1:
				op2 = MenuA();
				switch(op2)
				{
					case 1:
						CadAlunos(aluno);
						break;
					case 2:
						RelAlunos(aluno);
						break;
					case 3:
						ConsAlunos(aluno);
						break;
					case 4:
						AltAlunos(aluno);
						break;
					case 5:
						ExcFisAlunos(aluno);
						break;
					case 6:
						clrscr();
						OrdenaAlunos(aluno);
						Moldura(1,1,80,25,13);
						gotoxy(25,10);printf("Ordenado com Sucesso!!");
						getch();
						break;
					case 7:
						OrdenaAlunos(aluno);
						ConsNomeAlunos(aluno);
						break;
				}
				break;
			case 2:
				op2 = MenuD();
				
				switch(op2){
						case 1:
						CadastraDis(dis);
						break;
						case 2:
							listaDis(dis);
							break;
						case 3:
							consCod(dis);
							break;
						case 4:
							alteraDis(dis);
							break;
						case 5:
							ExcluirDis(dis);
							break;
						case 6:
							clrscr();
							BubbleSortDis(dis);
							Moldura(1,1,80,25,11);
							gotoxy(25,10);printf("Ordenado com Sucesso!!");
							getch();
							break;
						case  7: 
							BubbleSortDis(dis);
							consDisNome(dis);
							break;	
				}
				break;
			case 3:
				op2 = MenuN();
				switch(op2)
				{
					case 1:
						cadastraNotas(notas);
						break;
					case 2:
						listaNotas(notas);
						break;
					case 3:
						ConsNotasAluno(notas);
						break;
					case 4:
						alteraNotAluno(notas);
						break;
					case 5:
						ExcluirNotas(notas);
						break;
				}
				break;
			case 4:
				op2 = MenuR();
				switch(op2)
				{
					case 1:
						AlunosRep2(notas);
						break;
					case 2:
						AlunLetra(aluno);
						break;
					case 3:
						mediaGeralAlunos(notas,dis);
						break;
					case 4:
						geralAlunDis(aluno,dis,notas);
						break;	
					case 5:
						OrdenaAlunos(aluno);
						BubbleSortDis(dis);
						geralDisAlun(aluno,dis,notas);
					break;	
				}
			
		}
		
	}while(opcao!=5);
	clrscr();
	Moldura(1,1,80,25,13);
	gotoxy(22,12);
	textcolor(5);
	printf("# # #  F I M   # # #");
	gotoxy(22,15);
	Sleep(1000);
	
	return 1;
}
