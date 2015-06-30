
#ifndef DEF_ROBOT_HPP
#define DEF_ROBOT_HPP

class Robot {
    public:
        Robot();
        ~Robot();

        /**** Déplacements ****/
        void forward();
        void backward();
        /* Retourne la distance parcourue depuis l'appel
         * à forward/backward
         */
        float stop();
        void turn(float degres);

        /**** Communication ****/
        /* Renvoie vrai si un autre robot est détecté
         * Initialise la connection dans ce cas
         */
        bool discover();
        /* Appel bloquant */
        void send(char *buffer, int size);
        /* Appel bloquant */
        int receive(char *buffer, int size);
        /* Termine la connection */
        void close();

        /**** Détection ****/
        /* Retourne la distance de l'obstacle présent
         * devant
         */
        float scan();

    private:
};

#endif//DEF_ROBOT_HPP

