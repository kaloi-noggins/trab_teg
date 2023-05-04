adjacencias = []

with open("./arquivos/lista_adjacencias.csv","r") as f:
    for line in f:
        line = line.strip()
        line = line.replace(",","--")
        adjacencias.append(line)

str_adj = " "
for i in range(len(adjacencias)-1):
    str_adj += adjacencias[i] + "; "

str_adj+=adjacencias[-1]+" "

with open("./arquivos/grafo.dot", "w") as f:
    f.write("graph G {")
    f.write(str_adj)
    f.write("} ")