with open("./arquivos/IrisDataset.csv","r") as fi:
    
    lines = []
    for line in fi:
        lines.append(line)
    
    # remove primeira linha
    lines = lines[1:]

    # remove ultima coluna
    lines_aux = []
    for line in lines:
        line = line.split(",")
        line = line[0:4]
        lines_aux.append(",".join(line))

    # escreve no arquivo
    with open("./arquivos/dataset","w") as fo:
        for line in lines_aux:
            fo.write(line.replace(","," ")+"\n")