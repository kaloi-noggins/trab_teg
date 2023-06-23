import json
with open("./arquivos/clusters.json", "r") as f:
    clusters = json.load(f)

for cluster in clusters:
    flor  = []
    for item in clusters[cluster]:
        if item in range(0,49):
            flor.append("Setosa")
        elif item in range(50,100):
            flor.append("Versicolor")
        else:
            flor.append("Virginica")
    
    clusters[cluster] = tuple(zip(clusters[cluster],flor))
    print(clusters[cluster],"\n")  
