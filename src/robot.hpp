
#ifndef DEF_ROBOT_HPP
#define DEF_ROBOT_HPP

class Robot {
    public:
        Robot();
        ~Robot();

        /**** Déplacements ****/
        void forward();
        void backward();
        /* Retourne la distance parcourue depuis l'appel à forward/backward
         */
        float stop();
        void turn(float rad);

        /**** Communication ****/
        /* Renvoie vrai si un autre robot est détecté
         * Initialise la connection dans ce cas
         */
        bool discover();
        /* Appel non bloquant */
        void send(char *buffer, int size);
        /* Appel bloquant */
        int receive(char *buffer, int size);
        /* Termine la connection */
        void close();

        /**** Détection ****/
        /* Retourne la distance de l'obstacle présent devant
         */
        float scan();

        /**** Divers ****/
        /* Retourne un nombre aléatoire (suivant une densité uniforme) dans
         * l'intervalle [0; 1]
         */
        float rand();
        /* Alloue un bloc de mémoire */
        void* alloc(unsigned long size);
        /* Libère un bloc de mémoire */
        void free(void* mem);
        /* Redimensionne un bloc de mémoire alloué */
        void* realloc(void* p, unsigned long nsize);
        /* Copie un bloc de mémoire */
        void memcpy(void* dest, void* src, unsigned long size);

    private:
};

#endif//DEF_ROBOT_HPP

