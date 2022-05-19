IPMT

Autores:
João Pedro Henrique Santos Duarte (jphsd@cin.ufpe.br)
José Antônio Alves Maciel (jaam@cin.ufpe.br)

Instruções para compilação:
Para compilar a ferramenta, é suficiente executar o comando `make` dentro do diretório `src` do projeto. O programa compilado estará disponível em `./bin/ipmt`

Instruções para invocar a ferramenta:
    ./bin/ipmt modo [opções...] [padrão] arquivo [arquivos...]

Modos da ferramenta:

    index <file>:
        Cria um arquivo indexado a partir do textfile
        EX: ./bin/ipmt index texto.txt
    search [opções...] <pattern> <indexfile>:
        Faz a busca do pattern dentro do indexfile
        EX: ./bin/ipmt search -c 'dilma' texto.idx
    zip <textfile>:
        Cria um arquivo comprimido de textfile
        EX: ./bin/ipmt zip texto.txt
    unzip <textfile.myz>:
        Descompacta o arquivo comprimido textfile.myz
        EX: ./bin/ipmt unzip texto.myz

Opções da linha de comando:

    --help: 
        Mostrará na linha de comando as possíveis opções de argumento da ferramenta.
    -p, --pattern <patternfile>:
        Realizará a busca de todos os padrões indicados no patternfile. Dentro de um mesmo arquivo será possível indicar múltiplos padrões, cujo o delimitador será a quebra de linha
        e portanto, os padrões serão considerados diferentes caso estejam em linhas diferentes. Caso separados por espaço, vírgula ou algum outro delimitador, todas as palavras da linha
        serão consideradas como parte do mesmo padrão.
    -c, --count:
        Ao invés de imprimir as linhas em que há ocorrências dos padrões, será impresso a soma de todas as ocorrências dos padrões indicados.