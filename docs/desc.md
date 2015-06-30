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

## Mutation
TODO

## Protocole de communication
TODO

## Algorithme de controle
TODO

