# resource-simulator
brincadeira em c++.
No inicio da brincadeira, há um no root e uma struct de recursos.
A cada round um nó é adicionado á arvore e um valor, que é o custo de criação do nó, é subtraido da struct de recursos.
Nos proximos rounds, além do custo de adicionar um nó na arvore, ha tambem o custo para manter os nós ja existentes.
Cada nó ja adicionado, no round seguinte, rende um valor para a struct de recursos, ou seja, gera um valor que é adicionado de volta á struct de recursos.
Cada nó possui tambem uma lista de strikes. Quando a lista de recursos chega a zero, alguns nós podem deixar de receber recursos, nessas situações, o nó recebe um strike.
Caso o nó alcance tres strikes, o nó é removido da arvore. Todos os nós subjacentes serao removidos juntos.

O objetivo é ver a arvore crecendo de forma sustentavel de acordo com os recursos disponiveis.
