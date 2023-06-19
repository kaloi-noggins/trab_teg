class Grafo:

    def __init__(self,direcionado:bool):
        self.grafo = dict()
        self.direcionado = bool
        self.n_vertices = 0
        self.n_arestas = 0

    # Cria um vértice se ele não exsitir
    def adciona_vertice(self, vertice):
        if vertice not in self.grafo:
            self.grafo.update({vertice:[]})

    # Adciona uma aresta entre v1 ev2. Se o grafo for direcionado, 
    # a aresta é entre v1 e v2, e uma aresta entre v2 e v1 deverá ser adcionada
    # posteriormente. Senão existirá uma aresta entre v1 e v2 e v2 e v1
    def adciona_aresta(self, v1, v2):
        if v1 or v2 in self.grafo:
            # checa se a aresta v1--v2 já não existe
            if v2 not in self.grafo[v1]:
                self.grafo[v1].append(v2)
            # adciona aresta v2--v1 se nao direcionado
            if not self.direcionado:
                if v1 not in self.grafo[v2]:
                    self.grafo[v2].append(v1)
    
    def mostra_grafo(self):
        grafo_ordenado = dict(sorted(self.grafo.items()))

        for key in grafo_ordenado:
            print("{}:{}".format(key,self.grafo[key]))

    def grau_vertice(self, i):
        grau = len(self.grafo[i])
        return grau