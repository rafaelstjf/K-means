#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;
ifstream BD;
ofstream BDSAIDA;
ofstream BDCENTROS;
ofstream BDCENTROSI;
void AbrirBD(char nomeBD[100])
{
    BD.open(nomeBD,ios::out);//chama função membro open
}
void VoltaInicioBD()
{
    BD.clear(); //volta o estado para good
    BD.seekg(0, ios::beg); //volta ao inicio
}
int CalcNumLinhas()//calcula o num de linhas
{
    string str;
    int numlinhas = 0;
    if (BD.is_open() && BD.good())
    {
        VoltaInicioBD();
        getline(BD, str);
        while(!BD.fail())
        {
            getline(BD, str);
            numlinhas++;
        }
    }
    return numlinhas;
}
int CalcNumColunas()//calcula o num de colunas
{
    string str;
    int numcolunas = 0;
    VoltaInicioBD();
    if (BD.is_open()&& BD.good())
    {
        getline(BD, str);
        for(int i = 0; str[i]!=NULL; i++)
            if(str[i]==',')
                numcolunas++;
    }
    return numcolunas+1;
}
void CriaMatriz(int numlinhas, int numcolunas, double* Matriz) //cria a matriz
{
    string str;
    char temporaria[20];
    int linhaAtual = 0, colunaAtual = 0, i = 0, j = 0;
    double elemento;
    VoltaInicioBD();
    if (BD.is_open() && BD.good())
    {
        while (!BD.fail())
        {
            getline(BD, str);
            while (str[i] != '\0')
            {
                if (str[i] == ',' || str[i + 1] == '\0')
                {
                    //limpa os numeros apos a parada
                    if( str[i + 1] == '\0')
                    {
                        temporaria[j] = str[i];
                        j++;
                    }
                    for (int limp = j; temporaria[limp] != '\0'; limp++)
                        temporaria[limp] = NULL;
                    //converte o vetor char em double e passa pra matriz(vetor)
                    elemento = atof(temporaria);
                    Matriz[linhaAtual*numcolunas+colunaAtual] = elemento;//coloca o elemento no vetor
                    // cout << "PROGRESSO " << elemento << "\t" << linhaAtual << colunaAtual << endl;//imprime o progresso (elemento adicionado) usado apenas nos testes
                    colunaAtual++;//vai para a proxima coluna
                    elemento =0;
                    j = 0;//volta o indice do vetor do tipo char para 0
                }
                else if (str[i] != ',' || str[i + 1] != '\0')//for para passar os valores da string para o vetor char
                {
                    temporaria[j] = str[i];
                    j++;
                }
                i++;//aumenta o indice da string
            }
            if (str[i] == NULL&&BD.good())//verifica se a string chegou ao final da linha
            {
                linhaAtual++;
                colunaAtual = 0;
                i = 0;
                j = 0;
            }
        }
    }
}
void CalculaDistCentros (int qntcentros, double *Matriz, int numlinhas, int numcolunas, double *centros, double *Distancias)
{
    double diferencaAtributos[numcolunas];
    double soma=0;
    for(int c=0; c<qntcentros; c++)
    {
        for(int i=0; i<numlinhas; i++)
        {
            for(int j=0; j<numcolunas; j++)
            {
                diferencaAtributos[j]=((centros[(c*numcolunas)+j]-Matriz[(i*numcolunas)+j])*(centros[(c*numcolunas)+j]-Matriz[(i*numcolunas)+j]));
                soma+=diferencaAtributos[j];
            }
            Distancias[i*qntcentros+c]=sqrt(soma);
            soma=0;
        }
    }
}
bool verificaseexisteC(int qntcentros, int numarmazenados[], int numAtual)
{
    for(int i=0; i<qntcentros; i++)
    {
        if(numarmazenados[i]==numAtual)
            return true;
    }
    return false;
}
void EscolheCentro(int qntcentros, int numlinhas, int numcolunas, double* Matriz, double *centros)
{
    int c[qntcentros], centroValido=0;
    int verifcent[qntcentros]; //Recebe o centro atual para comparar com os outros centros
    srand(time(NULL)); // Randomiza o primeiro valor de rand(), que antes sempre começava com o mesmo numero int.

    ///cout << "Grupos: \t" ;
    for(int i=0; i<qntcentros; i++)
    {
        c[i]=rand() % (numlinhas-1);
        while(verificaseexisteC(qntcentros,verifcent,c[i]))
            c[i]=rand() % (numlinhas-1);
        verifcent[i]=c[i];
        for(int j=0; j<numcolunas; j++)
        {
            centros[(i*numcolunas)+j]=Matriz[(c[i]*numcolunas)+j];
        }
        cout << c[i] << "\t";
    }
}
void Agrupamento (int qntcentros, int numlinhas, int numcolunas, double* Matriz, double *centros, double *Distancias, int *grupos)
{
    double menor= 0;
    for(int i=0; i<numlinhas; i++)
    {
        menor=Distancias[i*qntcentros];
        grupos[i]=0;
        for(int c=0; c<qntcentros; c++)
        {
            if(menor>Distancias[i*qntcentros+c])
            {
                menor=Distancias[i*qntcentros+c];
                grupos[i]=c;
            }
        }
    }
}
void CalculaNovosCentros (int qntcentros, double *Matriz, int numlinhas, int numcolunas, double *centros, double *Distancias, int *grupos, bool *prosseguir)
{
    double soma [qntcentros][numcolunas]; ///Faz a soma dos atributos de cada grupo
    int stop=0;
    double antigocentro[qntcentros*numcolunas];
    int contGrupo[qntcentros]; /// Faz a contagem do total de amostras que contém em cada grupo
    //int centroIgual=0; /// Contagem para saber se os novos centros serão iguais aos anteriormente obtidos.
    //int AmostrasPorGrupo=0; /// Contagem para saber se não existe amostras em um determinado grupo e não mudar seu centro para 0
    ///string trash; /// Usado pra teste, para tirar o loop infinito.

    for(int g=0; g<numcolunas; g++)//Inicializa a matriz com 0
    {
        contGrupo[g]=0;
        for(int h=0; h<numcolunas; h++)
        {
            soma[g][h]=0;
        }
    }
    for(int c=0; c<qntcentros; c++) //percorre os centros
        for(int j=0; j<numcolunas; j++) //percorre as colunas
            for(int i =0; i<numlinhas; i++)//percorre todas as linhas
            {
                if(grupos[i]==c)
                {
                    soma[c][j]+=Matriz[i*numcolunas+j];
                    contGrupo[c]++;
                }
            }

    for(int c=0; c<qntcentros; c++) //percorre os centros
        for(int j=0; j<numcolunas; j++)
        {
            antigocentro[c*numcolunas+j]  = centros[c*numcolunas+j];
            if(contGrupo[c]!=0)
                centros[c*numcolunas+j] = (soma[c][j]/contGrupo[c]);
        }

    for(int c=0; c<qntcentros; c++) //percorre os centros
        for(int j=0; j<numcolunas; j++)
        {
            if( antigocentro[c*numcolunas+j] == centros[c*numcolunas+j])
                stop++;
        }
        if(stop==qntcentros-1)
                {
                *prosseguir=true;
                }
            else
            {
                *prosseguir=false;
            }
}
void ImprimeMatrizFinal (int numlinhas, int numcolunas, double *Matriz, int *Grupos)
{
    cout << endl << endl;
    for (int i=0; i<numlinhas; i++)
    {
        /*for (int j=0; j<numcolunas; j++)
        {
            cout << Matriz[(i*numcolunas)+j] << "\t";
        }

        cout << endl;
        */
        cout <<"Amostra- "<<i+1<<"->Grupo: " << Grupos[i] << "\t " ;/** ao inves de imprimir aquele monte de colunas imprime so (amostra- i   grupo)  **/
    }
}

void ImprimeMatrizFinalArquivo (int numlinhas, int numcolunas, double *Matriz, int *Grupos, char nomeBD[], int qntcentros, double* centros)
{
    BDSAIDA.open("RESULTADO.txt"); // o arquivo que será criado;
    for (int i=0; i<numlinhas; i++)
    {
        for (int j=0; j<numcolunas; j++)
        {
            BDSAIDA << Matriz[(i*numcolunas)+j]<< ",";/**VAI FICAR NO FORMATO DE UMA BSE DE DADOS EM TXT  DPS SO COMVENTER PRA CSV->EXEL POR ISSO TIREI O TITULO**/
        }


        BDSAIDA <<Grupos[i] << endl;
    }
    /**CRIA OUTRO TXT APENAS COM OS CENTROS**/
    BDCENTROS.open("CENTROS.txt");
    BDCENTROS << nomeBD <<" - CENTROS "<< endl;
    for (int i=0; i<qntcentros; i++)
    {
        BDCENTROS <<"Grupo["<<i<<"] possui como centro: [\t";
        for (int j=0; j<numcolunas; j++)
        {
            if(j<numcolunas)
                BDCENTROS << centros[(i*numcolunas)+j] << "\t";
        }
        BDCENTROS << "]";
        BDCENTROS << endl;
    }
    BDCENTROS.close();
}
void imprimecentroinicial(int numlinhas, int numcolunas, double *Matriz, int *Grupos, char nomeBD[], int qntcentros, double* centros)
{
    //CRIA OUTRO TXT APENAS COM OS CENTROS INICIAIS
    BDCENTROSI.open("CENTROS-INICIAIS.txt");
    BDCENTROSI << nomeBD <<" - CENTROS INICIAIS "<< endl;
    for (int i=0; i<qntcentros; i++)
    {
        BDCENTROSI <<"Grupo["<<i<<"] possui como centro: [\t";
        for (int j=0; j<numcolunas; j++)
        {
            if(j<numcolunas)
                BDCENTROSI << centros[(i*numcolunas)+j] << "\t";
        }
        BDCENTROSI << "]";
        BDCENTROSI << endl;
    }
    BDCENTROSI.close();
}
void NormalizaLinearmente(double *Matriz,int numcolunas,int numlinhas)
{
    int i=0;
    int j=0;//linha e coluna atual
    double MAX;
    double MIN;//maximo e minimo valor de x do atributo
    for(j=0; j<numcolunas; j++)
    {
        MAX=Matriz[j];
        MIN=Matriz[j];
        for(i=0; i<numlinhas; i++)
        {
            if(Matriz[(i*numcolunas)+j]>MAX)
                MAX=Matriz[(i*numcolunas)+j];
            if(Matriz[(i*numcolunas)+j]<MIN)
                MIN=Matriz[(i*numcolunas)+j];
        }
        for(i=0; i<numlinhas; i++)
        {
            Matriz[(i*numcolunas)+j]=(Matriz[(i*numcolunas)+j]-MIN)/(MAX-MIN);
        }

    }

    /**FAZ UM TXT COM A BASE DE DADOS NORMALIZADA**/
    BDSAIDA.open("Normalizada.txt"); // o arquivo que será criado;
    for (int i=0; i<numlinhas; i++)
    {
        for (int j=0; j<numcolunas; j++)
        {
            BDSAIDA << Matriz[(i*numcolunas)+j] <<",";// ,  por que vai salvar a base de dados do jeito que tava pronto pra converter pra csv dps pra tabela
        }

        BDSAIDA << endl;
    }
    BDSAIDA.close();

}
int main()
{
    int k,l, numerocolunas, numerolinhas, nmaxinteracoes=0;//variaveis gerais, k = numero de grupos, l usado apenas para fazer a impressao de teste da matriz
    bool prosseguir = false;
    char option;
    char nomeBD[100];//nome e extensao do arquivo
    clock_t tempo;//variavel para calcular o tempo de execucao do algoritmo
    cout << "-------------------------------------------------------------" << endl;
    cout << "\t\t\tClassificador K-means" << endl;
    cout << "Alunos: Rafael de Souza Terra\n\tLuis Felipe de Almeida Nascimento\n\tMateus Nery Franco"<<endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "Digite o nome do arquivo da base de dados" << endl;
    cin >>nomeBD;
    AbrirBD(nomeBD);//abre o arquivo
    while(!BD.is_open())
    {
        cout << "ERRO! ARQUIVO NAO ENCONTRADO!" << endl;
        cout << "Digite novamente o nome do arquivo da base de dados" << endl;
        cin >>nomeBD;
        AbrirBD(nomeBD);//abre o arquivo
    }
    cout<<endl;

    numerolinhas = CalcNumLinhas();//atribui os valores
    numerocolunas = CalcNumColunas();//atribui os valores
    double *Matriz = new double[numerolinhas*(numerocolunas)];//aloca dinamicamente um vetor para armazenar a matriz
    cout << numerolinhas <<" linhas" <<endl;
    cout << numerocolunas <<" colunas" <<endl<<endl;
    cout << "Digite o numero de grupos" << endl;
    cin >> k;
    while(k>numerolinhas)
    {
        cout << "ERRO! NUMERO DE GRUPOS MAIOR QUE NUMERO DE AMOSTRAS" <<endl;
        cout << "Digite novamente o numero de grupos" << endl;
        cin >> k;
    }
    tempo=clock();//inicia a contagem do tempo de compilacao do algoritmo k-means
    double *Distancias = new double [numerolinhas*k]; //Aloca dinamicamente uma matriz contendo a distancia em relação a cada k
    double *centros= new double[k*numerocolunas];//cria vetor unidimensional para salvar os centros
    int *Grupos = new int [numerolinhas];
    CriaMatriz(numerolinhas, numerocolunas, Matriz);
    // NormalizaLinearmente(Matriz,numerocolunas,numerolinhas);//<-------------------------------------------------------------
    EscolheCentro(k, numerolinhas, numerocolunas ,Matriz, centros);
    imprimecentroinicial(numerolinhas,numerocolunas,Matriz,Grupos,nomeBD, k, centros);
    CalculaDistCentros(k, Matriz, numerolinhas, numerocolunas, centros, Distancias);
    Agrupamento(k, numerolinhas, numerocolunas, Matriz, centros, Distancias, Grupos);
    while(!prosseguir && nmaxinteracoes < 500)
    {
        CalculaNovosCentros(k,Matriz,numerolinhas,numerocolunas,centros,Distancias,Grupos, &prosseguir);
        CalculaDistCentros(k, Matriz, numerolinhas, numerocolunas, centros, Distancias);
        Agrupamento(k, numerolinhas, numerocolunas, Matriz, centros, Distancias, Grupos);
        nmaxinteracoes++;
    }

    cout<<endl;
    tempo=clock()-tempo;//calcula o tempo de compilacao do algoritmo k-means
    cout <<"Tempo de compilacao: " << ((float)tempo/CLOCKS_PER_SEC) << " segundos" << endl;
    cout << endl << "Deseja imprimir a matriz? (y/n)" << endl;
    cin >> option;
    ImprimeMatrizFinalArquivo(numerolinhas,numerocolunas,Matriz,Grupos,nomeBD, k, centros);
    if (option == 'y' || option == 'Y')
        ImprimeMatrizFinal(numerolinhas,numerocolunas,Matriz,Grupos);
    cout << "Arquivo RESULTADO.txt salvo com sucesso!" << endl;
    delete[] Matriz;
    delete[] Distancias;
    delete[] centros;//desaloca o centro
    return 0;
}
