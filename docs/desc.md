# Description
Ce documents présente les différentes techniques utilisées par le robot.

## Idée générale
Chaque robot se déplace, suivant un jeu d'instructions simples (par exemple
tourner à droite après un obstacle ...). Lorsque deux robots se rencontrent,
il partagent leurs génome, leur auto-évaluation et le temps depuis leur
précédente évolution. Chacun va ensuite adapter son génome en fonction
puis continuer son chemin, jusqu'à sa rencontre suivante avec un robot.

## Génome
Le génome est une suite de 0 et de 1 qui décrit le comportement du robot en
fonction de sa situation. On considère 3 situations possibles :
 - obstacle immédiat
 - obstacle proche
 - obstacle loin

Pour chaque situation, il y a 3 comportements possibles :
 - avancer tout droit (sauf dans le cas d'un obstacle immédiat)
 - tourner à droite
 - tourner à gauche

Le génome est de la forme : `iippll` où chaque lettre représente un bit. Chaque
couple de bit représente la réaction à avoir face à chacune des trois
situations. `01` indique qu'il faut tourner à droite, `10` à gauche et `11`
qu'il faut continuer tout droit.

## Évaluation
L'évaluation consiste à accorder un score aux performances du robot afin de
comparer la pertinence de leur génomes respectifs. Le critère prédominant est
la distance parcourue sans renctrer d'obstacles. Le score précédent est aussi
pris en compte dans la mesure de son importance lors de la précédente
évolution.
On note `a` le coefficient indiquant l'importance du génome précédent, `E` la
note d'auto-évaluation, `E_` la note précédente et `D` la distance maximale
parcourue sans rencontrer d'obstacles. On a `a` € [0,1]. On a alors :
    ``` E = a * E_ + (1 - a) * D ```

## Mutation
La mutation est le processus de mélange des gènes du robot avec ceux reçus
depuis un autre robot. Si la durée d'expérience d'un des deux robots est
trop faible, la mutation est annulée. Dans l'autre cas, un pourcentage est
attribué à chaque génome en fonction de leur notation. Le pourcentage du génome
courant est le `a` de la partie précédente.
Pour chaque génome, on choisit aléatoirement une à trois parties en fonction du
pourcentage. Si un partie est choisie dans chacun des deux génomes, elles sont
mélangées en prenant un bit de l'une et un bit de l'autre. Si un partie n'est
choisit que dans un génome, on la conserve telle quelle. Enfin, si elle n'est
choisit dans aucun génome, elle est générée aléatoirement.

## Protocole de communication
TODO

## Algorithme de controle
TODO

